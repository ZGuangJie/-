#ifndef _FUNCTION_
#define _FUNCTION_
    //光标跳转
    void CursorJump(int x, int y);
    //初始化界面
    void InitInterface();
    //初始化方块信息
    void InitBlockInfo();
    //颜色设置
    void color(int num);
    //画出方块
    void DrawBlock(int form, int shape, int x, int y);
    //空格覆盖
    void DrawSpace(int form, int shape, int x, int y);
    //合法性判断
    int IsLegal(int form, int shape, int x, int y);
    //判断得分与结束
    int JudeFunc();
    //游戏主体逻辑函数
    void StartGame();
    //从文件读取最高分
    void ReadGrade();
    //更新最高分到文件
    void WriteGrade();
#endif