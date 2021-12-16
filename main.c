#define _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNONGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>  //光标位置对应项的头文件
#include<conio.h>  //按键反馈的头文件
//1.设置地图的范围（边界） 宽与高
#define WIDE 60
#define HIGH 20


//知道界面有多大
void init_ui()
{
	for (int i = 0; i < HIGH; i++)
	{
		for (int j = 0; j < WIDE; j++)
		{
			printf("#");
		}
		printf("\n");
	}

}
//2.初始化蛇  初始化食物
typedef struct _body
{
	int x;
	int y;

}BODY;
typedef struct snake
{
		BODY list[WIDE * HIGH];//身体,身体的每一节坐标都是BODY类型
		int size;//蛇的身体的大小
		BODY food;//食物的坐标
		COORD coord;//避免反复定义光标--这是光标的位置信息
		int dx;      //蛇x轴移动的方向
		int dy;      //蛇y轴移动的方向    
		int score;      //得分
		BODY tail;
}SNAKE;
void init_food(SNAKE *snake)  //初始化食物坐标
{
	srand(time(NULL));//设置随机数种子
	snake->food.x = rand() % WIDE;
	snake->food.y = rand() % HIGH;
}
void init_snake(SNAKE *snake)
{
	//初始化蛇头坐标
	snake->list[0].x = WIDE / 2;
	snake->list[0].y = HIGH / 2;
	//初始化蛇尾坐标
	snake->list[1].x = WIDE / 2-1;
	snake->list[1].y = HIGH / 2;

	//蛇的身体的大小初始化
	snake->size = 2;

	//初始化食物的坐标
	init_food(snake);

	//初始化蛇的移动方向
	snake->dx = 1;
	snake->dy = 0;

	//初始化分数
	snake->score = 0;
}
//3.将蛇和食物显示在屏幕上
void show_ui(SNAKE* snake)
{
	
	
	//显示蛇  注意：每次显示蛇或者食物，都要设置显示的位置（光标的位置）
	for (int i = 0; i < snake->size; i++)
	{
		snake->coord.X = snake->list[i].x;	 //将蛇头的坐标赋给coord.x
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//用系统自带的函数,不用太过理解--设置光标的位置
		if (0 == i) { printf("@"); }
	
		else { printf("*"); }
	}
	//显示食物
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("#");

	//将原来尾巴的位置显示为空格
	snake->coord.X = snake->tail.x;
	snake->coord.Y = snake->tail.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf(" ");

}
//4.蛇的移动（通过wsad按键控制蛇的移动方向）
/*
	a>蛇碰到墙
	b>蛇碰到蛇的身体
	c>蛇碰到障碍物
	d>蛇碰到食物（蛇的身体增长一节，原食物消失，生成新的食物，增加分数）
*/
void move_snake(SNAKE *snake)     //--更新蛇的坐标把上截坐标给前一截,所以从蛇的最后一截开始
{
	//记录尾巴的坐标
	snake->tail.x = snake->list[snake->size - 1].x;
	snake->tail.y = snake->list[snake->size - 1].y;
	//更新蛇的身体坐标，除蛇头
	for (int i = snake->size - 1; i > 0; i--)       //从蛇尾的最后一截坐标开始
	{
		snake->list[i] = snake->list[i - 1];//将数组前一个元素的坐标给后一截
	}
	//更新蛇头
	snake->list[0].x += snake->dx;//1    前面有函数已经初始化了
	snake->list[0].y += snake->dy;//0

}
//游戏结束
void game_end(SNAKE* snake)
{
	snake->coord.X = 25;
	snake->coord.Y = 25;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("游戏结束，得分为%d\n", snake->score);//食物光标在哪里这行话就出现在哪里。所以要重新设置这段话的光标位置
	Sleep(3000);
	exit(0);            //让程序退出-退出进程
}
void control_snake(SNAKE *snake)
{
	char key = 0;   //赋初始值为0，方便下面switch语句的循环
	while (_kbhit())//判断是否按下按键，按下不等于0
	{
		key = _getch();
	}
	switch (key)
	{
	case 'a':
		snake->dx = -1;
		snake->dy = 0;
		break;
	case 'w':
		snake->dx = 0;
		snake->dy = -1;
		break;
	case 's':
		snake->dx = 0;
		snake->dy = 1;
		break;
	case 'd':
		snake->dx = 1;
		snake->dy = 0;
		break;
	}

}
//蛇咬到自己--头要与自己每一节身体坐标(x,y)进行比较
 void snake_eat_body(SNAKE *snake)
 {
	//如果蛇头和蛇的身体的任一一截身体坐标相同，则吃到了自己的身体
	 for (int i = 1; i < snake->size; i++)
	 {
		 if (snake->list[0].x == snake->list[i].x && snake->list[0].y == snake->list[i].y)
		 {
			 game_end(snake);
		 }
	 }
 }
 //蛇吃到了食物
 void snake_eat_food(SNAKE* snake)
 {
	 //如果蛇头和食物的坐标重合，则吃到了食物
	 if (snake->list[0].x == snake->food.x && snake->list[0].y == snake->food.y)
	 {
		 //原食物消失（可以不写）,生成新的食物,蛇头显示覆盖重合了食物--并且不要让食物生成在蛇的身上
		 init_food(snake);
		 //蛇的身体增长一截
		 snake->size++;//size+1了多打印了一个*,是最后一截，已经被以上的函数给赋值了
		 //分数增加
		 
		 snake->score += 10;

	 }

 }


 void init_wall()
 {
	 for (int i = 0; i <= HIGH; i++)
	 {
		 for (int j = 0; j <= WIDE; j++)
		 {
			 if (i == HIGH || j == WIDE)
			 {
				 printf("+");
			 }
			 else
			 {
				 printf(" ");
			 }
		 }
		 printf("\n");
	 }
 }
 //开始游戏
void start_game(SNAKE* snake)
{
	while(snake->list[0].x < 60 && snake->list[0].x >= 0 && snake->list[0].y >= 0 && snake->list[0].y < 20)  //判断蛇头坐标是否在设计好的边界范围之内--是否碰到墙
	{
		//控制蛇的方向
		control_snake(snake);
		
		//蛇要移动就是要***更新蛇的坐标
		move_snake(snake);           
		//蛇移动--显示移动的坐标

		/*system("cls");//清屏--》》去掉蛇身的原有位置，反应出新的位置
		//因为把墙给清了，所以不能用了*/

		show_ui(snake);
		//蛇是否碰到墙
		//蛇是否碰到自己
		snake_eat_body(snake);
		 
		//蛇是否碰到食物-（碰到食物，原食物消失，产生新的食物 ）
		snake_eat_food(snake);

		//加个延迟
		Sleep(300);//延迟0.5s,不能一直让蛇更新
	}
	game_end(snake);

}
void hide_cur()
{
	//隐藏控制台光标
	CONSOLE_CURSOR_INFO  cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}
int main(void)
{
	hide_cur();//隐藏光标位置--蛇的尾巴
	init_wall();        //初始化墙
	//init_ui();显示页面
	SNAKE  *snake = (SNAKE *)malloc(sizeof(SNAKE));//申请蛇的结构体大小空间
	init_snake(snake);//只传了地址，没有把自己三个内容传进去--初始化蛇和身体

	show_ui(snake);   //显示界面信息
	start_game(snake);//开始游戏
	

	system("pause");
	return EXIT_SUCCESS;
}
