#include<Windows.h>
#include "fun.h"

//行，对应着纵坐标
#define ROW 29 
//列，对应着横坐标
#define COL 20

#define DOWN 80 //方向键：下
#define LEFT 75 //方向键：左
#define RIGHT 77 //方向键：右

#define SPACE 32 //空格键
#define ESC 27 //Esc键

struct Face {
    int data[ROW][COL+10];
    int color[ROW][COL+10];
} face;

// 结构体内的shape二维数组是用来存储形状信息，该结构体的二维数组用来
// 用来存 七 个基本形状以及 4 种变换形态。

struct Block {
    int shape[4][4];
}block[7][4];

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
    int pos_y = 8;
    CursorJump(2*COL+3, 1);
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
    printf("最高记录:%d", 99);

    CursorJump(2*COL + 4, ROW - 5);
    printf("当前分数:%d", 99);
}