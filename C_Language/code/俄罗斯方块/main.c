#include <stdio.h>
#include <Windows.h>
#include "fun.c"
#include "fun.h"

int main()
{
    system("chcp 65001");
    system("title 我自己的方块"); //设置cmd窗口的名字
	system("mode con lines=29 cols=60"); //设置cmd窗口的大小
    HideCursor();
    InitInterface();
    InitBlockInfo();
    srand((unsigned int)time(NULL)); //设置随机数生成的起点
    StartGame();
    system("pause");
    return 0;
}

