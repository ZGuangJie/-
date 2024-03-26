#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "fun.h"

extern int main();

//行，对应着纵坐标
#define ROW 29 
//列，对应着横坐标
#define COL 20

#define DOWN 80 //方向键：下
#define LEFT 75 //方向键：左
#define RIGHT 77 //方向键：右

#define SPACE 32 //空格键
#define ESC 27 //Esc键

/*一定要注意，face的ROW是行索引，对应着窗口的横坐标，下面方块的存储，行列对应到
 * 窗口的 y, x
 * 
 * */ 

struct Face {
    int data[ROW][COL+10];
    int color[ROW][COL+10];
} face;

// 结构体内的shape二维数组是用来存储形状信息，该结构体的二维数组用来
// 用来存 七 个基本形状以及 4 种变换形态。

struct Block {
    int shape[4][4];
}block[7][4];

int grade = 0;
int max = 0;

//隐藏光标
void HideCursor()
{
	CONSOLE_CURSOR_INFO curInfo; //定义光标信息的结构体变量
	curInfo.dwSize = 1;  //如果没赋值的话，隐藏光标无效
	curInfo.bVisible = FALSE; //将光标设置为不可见
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
	SetConsoleCursorInfo(handle, &curInfo); //设置光标信息
}

//光标跳转
void CursorJump(int x, int y)
{
	COORD pos; //定义光标位置的结构体变量
	pos.X = x; //横坐标设置
	pos.Y = y; //纵坐标设置
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
	SetConsoleCursorPosition(handle, pos); //设置光标位置
}

void InitInterface()
{
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL+10; j++){
            if (j == 0 || j == 20 || j == 29) {
                face.data[i][j] = 1;//标记有墙
                CursorJump(2*j, i);
                printf("■");
            }
            else if (i == ROW - 1) {
                face.data[i][j] = 1;//标记有墙
                CursorJump(2*j, i);
                printf("■");
            }
            else {
                face.data[i][j] = 0;
            }
        }
    }
    for (int j = COL+1; j < COL + 10; j++){
        face.data[5][j] = 1;
        CursorJump(2*j, 5);
        printf("■");
    }

    CursorJump(2*COL+3, 0);
    printf("下一次形状:");

    CursorJump(2*COL+6, ROW - 21);
    printf("左移: ←");

    CursorJump(2*COL+6, ROW - 19);
    printf("右移: →");

    CursorJump(2*COL+6, ROW - 17);
    printf("加速: ↓");

    CursorJump(2*COL+5, ROW - 15);
    printf("旋转：空格");

    CursorJump(2*COL+5, ROW - 13);
    printf("暂停: S");

    CursorJump(2*COL+5, ROW - 11);
    printf("退出: Esc");

    CursorJump(2*COL+4, ROW - 9);
    printf("重新开始: R");
    
    CursorJump(2*COL+4, ROW - 7);
    printf("最高记录:%d", grade);

    CursorJump(2*COL + 4, ROW - 5);
    printf("当前分数:%d", grade);
}

void InitBlockInfo()
{
    // 第一种形状 T 的第一种形式
    for (int i = 0; i < 3; i++){
        block[0][0].shape[1][i] = 1;
    }
    block[0][0].shape[2][1] = 1;

    // 第二种形状 L 的第一种形式
    for (int i = 1; i < 4; i++){
        block[1][0].shape[i][1] = 1;
    }
    block[1][0].shape[3][2] = 1;

    // 第三种形状 _| 的第一种形式
    for (int i = 1; i < 4; i++){
        block[2][0].shape[i][2] = 1;
    }
    block[2][0].shape[3][1] = 1;

    
    for (int i = 1; i < 3; i++){
        // 第四种形状 Z 的第一种形式
        block[3][0].shape[i][i-1] = 1;
        block[3][0].shape[i][i] = 1;

        // 第五种形状 _|- 的第一种形式
        block[4][0].shape[1][i] = 1;
        block[4][0].shape[2][i-1] = 1;

        // 第六种形状 田 的第一种形式
        block[5][0].shape[1][i] = 1;
        block[5][0].shape[2][i] = 1;
    }
    // 第七种形状 | 的第一种形式
    for (int i = 0; i < 4; i++) {
        block[6][0].shape[i][1] = 1;
    }
    
    // 形状变换，核心公式(i, j) -> (3-j, i)
    int temp[4][4]; //暂时存储当前形状信息
    
    for (int form = 0; form < 7; form++) {// 七种形状
        for (int shape = 0; shape < 3; shape++) {// 已经建立一种形式了，每个形状在添加三种形式
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    // 将上一个形状先复制到temp数组中
                    temp[i][j] = block[form][shape].shape[i][j];
                }
            }
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    // 将temp中的形状变换存到block数组中
                    block[form][shape+1].shape[i][j] = temp[3-j][i];
                }
            }
        }
    }
}

//颜色设置
void color(int c)
{
	switch (c)
	{
	case 0:
		c = 13; //“T”形方块设置为紫色
		break;
	case 1:
	case 2:
		c = 12; //“L”形和“J”形方块设置为红色
		break;
	case 3:
	case 4:
		c = 10; //“Z”形和“S”形方块设置为绿色
		break;
	case 5:
		c = 14; //“O”形方块设置为黄色
		break;
	case 6:
		c = 11; //“I”形方块设置为浅蓝色
		break;
	default:
		c = 7; //其他默认设置为白色
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //颜色设置
	//注：SetConsoleTextAttribute是一个API（应用程序编程接口）
}

void DrawBlock(int form, int shape, int x, int y)
{
    for (int i = 0; i<4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[form][shape].shape[i][j] == 1) {
                CursorJump(2*( j + x), i + y);
                printf("■");
            }
        }
    }
}

void DrawSpace(int form, int shape, int x, int y)
{
    for (int i = 0; i<4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[form][shape].shape[i][j] == 1) {
                CursorJump(2*( j + x), i + y);
                printf("  ");
            }
        }
    }
}

int IsLegal(int form, int shape, int x, int y)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if(block[form][shape].shape[i][j] && face.data[y + i][x + j])
                return 0;
        }
    }
    return 1;
} 

int JudeFunc()
{
    // 判断是否得分
    for (int i = ROW-2; i > 4; i--) {
        int sum = 0; //statistics block number
        for (int j = 1; j < COL - 1; j++) {
            sum += face.data[i][j];
        }
        if (sum == 0) {//上面没有方块，退出判断
            return 0;
        }
        else if (sum == COL-2) {//得分
            // 消除方块
            for (int j = 1; j < COL-1; j++) {
                face.data[i][j] = 0;
                CursorJump(2 * j, i);
                printf("  ");
            }
            grade += 10; 
            CursorJump(2*COL + 4, ROW - 5);
            printf("当前分数:%d", grade);
            // 将上面的行整体下移
            for (int m = i; m > 4; m--) {
                sum = 0;
                for (int j = 1; j < COL-1; j++) {
                    face.data[m][j] = face.data[m-1][j];
                    face.color[m][j] = face.data[m-1][j];
                    if (face.data[m-1][j] == 1) {
                        sum += face.data[m-1][j];
                        CursorJump(2*j, m);
                        color(face.color[m][j]);
                        printf("■");
                    }
                    else {
                        CursorJump(2*j, m);
                        printf("  ");
                    }
                }
                if (sum==0) {//上面一层已经没有方块
                    return 1;
                }
            }
        }
    }
    // 判断游戏是否结束
    for (int j = 1; j < COL-1; j++) {
        if (face.data[1][j] == 1) {
            //printf("grade = %d, max = %d", grade, max);
            Sleep(1000);
            system("cls");
            color(7);
            CursorJump(2 * (COL / 3), ROW / 2 -3);
            if (grade > max) {
                printf("恭喜你打破最高记录，最高记录更新为%d", grade);
                WriteGrade();
            }
            else if (grade == max) {
                printf("与最高分: %d 齐平, 继续努力, 再创佳绩!", grade);
            }
            else {
                printf("请继续加油，当前与最高记录相差%d", max - grade);
            }
            CursorJump(2 * (COL / 3), ROW / 2);
            printf("GAME OVER!");
            while (1) {
                CursorJump(2 * (COL / 3), ROW / 2 + 3);
                printf("再来一局? Y / N ");
                char ch;
                scanf("%s", &ch);
                if (ch=='y' || ch=='Y') {
                    system("cls");
                    main();
                }
                else if (ch=='n' || ch=='N') {
                    CursorJump(2 * (COL / 3), ROW / 2 + 5);
                    exit(0);
                }
                else {
                    CursorJump(2 * (COL / 3), ROW / 2 + 7);
                    printf("输入错误, 请重新输入...");
                }
            }
        }
    }
    return 0;
}

//游戏主体逻辑函数
void StartGame()
{
    // random init shape
    ReadGrade();
    int form = rand() % 7, shape = rand() % 4;
    while (1) 
    {
        int t = 0;
        int x = COL / 2 - 2, y = 0;// 定义初始下落位置
        int Nextform = rand() % 7, Nextshape = rand() % 4;
        color(Nextform);
        //CursorJump(2*COL+3, 2);
        DrawBlock(Nextform, Nextshape, COL+3, 1);

        while (1)
        {
            color(form);
            DrawBlock(form, shape, x, y);
            if (t==0)
            {
                t = 15000;
            }
            while (--t)
            {
                if(kbhit() != 0)//若键盘被敲击，则退出循环
                    break;
            }
            if (t == 0) //键盘没被敲击
            {
                if (IsLegal(form, shape, x, y + 1) == 0)// 到达底部
                {   
                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if (block[form][shape].shape[i][j] == 1)
                            {
                                // 记录方块的位置和颜色
                                face.data[y + i][x + j] = 1;
                                face.color[y + i][x + j] = form;
                            }
                        }
                    }
                    while (JudeFunc());
                    break;
                }
                else
                {
                    DrawSpace(form, shape, x, y);
                    y++;
                }
            }
            else //键盘被敲击
            {
                char ch = getch();
                switch (ch)
                {
                case DOWN:
                    if (IsLegal(form, shape, x, y + 1))
                    {
                        DrawSpace(form, shape, x, y);
                        y++;
                    }
                    break;
                case LEFT:
                    if (IsLegal(form, shape, x - 1, y))
                    {
                        DrawSpace(form, shape, x, y);
                        x--;
                    }
                    break;
                case RIGHT:
                    if (IsLegal(form, shape, x + 1, y))
                    {
                        DrawSpace(form, shape, x, y);
                        x++;
                    }
                    break;
                case SPACE:
                    if (IsLegal(form, (shape + 1) % 4, x, y))
                    {
                        DrawSpace(form, shape, x, y);
                        shape = (shape + 1) % 4;
                        y++;
                    }
                    break;
                case ESC: // 结束游戏
                    system("cls"); //清空屏幕
                    color(7);
                    CursorJump(COL, ROW / 2);
                    printf("  游戏结束  ");
                    CursorJump(COL, ROW / 2 + 2);
                    exit(0); //结束程序
                case 's':
                case 'S':
                    system("pause>nul"); //暂停（按任意键继续）
                    break;
                case 'r':
                case 'R':
                    system("cls"); // 清屏
                    main();
                }
            }
        }
        form = Nextform, shape = Nextshape;
        DrawSpace(Nextform, Nextshape, COL+3, 1);
    }
}
//从文件读取最高分
void ReadGrade()
{
	FILE* pf = fopen("HistoryMaxGrade.txt", "r"); //以只读方式打开文件
	if (pf == NULL) //打开文件失败
	{
		pf = fopen("HistoryMaxGrade.txt", "w"); //以只写方式打开文件（文件不存在可以自动创建该文件）
		fwrite(&grade, sizeof(int), 1, pf); //将max写入文件（此时max为0），即将最高历史得分初始化为0
	}
	fseek(pf, 0, SEEK_SET); //使文件指针pf指向文件开头
	fread(&max, sizeof(int), 1, pf); //读取文件中的最高历史得分到max当中
	fclose(pf); //关闭文件
	pf = NULL; //文件指针及时置空
}
//更新最高分到文件
void WriteGrade()
{
	FILE* pf = fopen("HistoryMaxGrade.txt", "w"); //以只写方式打开文件
	if (pf == NULL) //打开文件失败
	{
		printf("保存最高得分记录失败\n");
		exit(0);
	}
	fwrite(&grade, sizeof(int), 1, pf); //将本局游戏得分写入文件当中（更新最高历史得分）
	fclose(pf); //关闭文件
	pf = NULL; //文件指针及时置空
}