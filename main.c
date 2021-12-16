#define _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNONGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>  //���λ�ö�Ӧ���ͷ�ļ�
#include<conio.h>  //����������ͷ�ļ�
//1.���õ�ͼ�ķ�Χ���߽磩 �����
#define WIDE 60
#define HIGH 20


//֪�������ж��
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
//2.��ʼ����  ��ʼ��ʳ��
typedef struct _body
{
	int x;
	int y;

}BODY;
typedef struct snake
{
		BODY list[WIDE * HIGH];//����,�����ÿһ�����궼��BODY����
		int size;//�ߵ�����Ĵ�С
		BODY food;//ʳ�������
		COORD coord;//���ⷴ��������--���ǹ���λ����Ϣ
		int dx;      //��x���ƶ��ķ���
		int dy;      //��y���ƶ��ķ���    
		int score;      //�÷�
		BODY tail;
}SNAKE;
void init_food(SNAKE *snake)  //��ʼ��ʳ������
{
	srand(time(NULL));//�������������
	snake->food.x = rand() % WIDE;
	snake->food.y = rand() % HIGH;
}
void init_snake(SNAKE *snake)
{
	//��ʼ����ͷ����
	snake->list[0].x = WIDE / 2;
	snake->list[0].y = HIGH / 2;
	//��ʼ����β����
	snake->list[1].x = WIDE / 2-1;
	snake->list[1].y = HIGH / 2;

	//�ߵ�����Ĵ�С��ʼ��
	snake->size = 2;

	//��ʼ��ʳ�������
	init_food(snake);

	//��ʼ���ߵ��ƶ�����
	snake->dx = 1;
	snake->dy = 0;

	//��ʼ������
	snake->score = 0;
}
//3.���ߺ�ʳ����ʾ����Ļ��
void show_ui(SNAKE* snake)
{
	
	
	//��ʾ��  ע�⣺ÿ����ʾ�߻���ʳ���Ҫ������ʾ��λ�ã�����λ�ã�
	for (int i = 0; i < snake->size; i++)
	{
		snake->coord.X = snake->list[i].x;	 //����ͷ�����긳��coord.x
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//��ϵͳ�Դ��ĺ���,����̫�����--���ù���λ��
		if (0 == i) { printf("@"); }
	
		else { printf("*"); }
	}
	//��ʾʳ��
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("#");

	//��ԭ��β�͵�λ����ʾΪ�ո�
	snake->coord.X = snake->tail.x;
	snake->coord.Y = snake->tail.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf(" ");

}
//4.�ߵ��ƶ���ͨ��wsad���������ߵ��ƶ�����
/*
	a>������ǽ
	b>�������ߵ�����
	c>�������ϰ���
	d>������ʳ��ߵ���������һ�ڣ�ԭʳ����ʧ�������µ�ʳ����ӷ�����
*/
void move_snake(SNAKE *snake)     //--�����ߵ�������Ͻ������ǰһ��,���Դ��ߵ����һ�ؿ�ʼ
{
	//��¼β�͵�����
	snake->tail.x = snake->list[snake->size - 1].x;
	snake->tail.y = snake->list[snake->size - 1].y;
	//�����ߵ��������꣬����ͷ
	for (int i = snake->size - 1; i > 0; i--)       //����β�����һ�����꿪ʼ
	{
		snake->list[i] = snake->list[i - 1];//������ǰһ��Ԫ�ص��������һ��
	}
	//������ͷ
	snake->list[0].x += snake->dx;//1    ǰ���к����Ѿ���ʼ����
	snake->list[0].y += snake->dy;//0

}
//��Ϸ����
void game_end(SNAKE* snake)
{
	snake->coord.X = 25;
	snake->coord.Y = 25;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("��Ϸ�������÷�Ϊ%d\n", snake->score);//ʳ�������������л��ͳ������������Ҫ����������λ��Ĺ��λ��
	Sleep(3000);
	exit(0);            //�ó����˳�-�˳�����
}
void control_snake(SNAKE *snake)
{
	char key = 0;   //����ʼֵΪ0����������switch����ѭ��
	while (_kbhit())//�ж��Ƿ��°��������²�����0
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
//��ҧ���Լ�--ͷҪ���Լ�ÿһ����������(x,y)���бȽ�
 void snake_eat_body(SNAKE *snake)
 {
	//�����ͷ���ߵ��������һһ������������ͬ����Ե����Լ�������
	 for (int i = 1; i < snake->size; i++)
	 {
		 if (snake->list[0].x == snake->list[i].x && snake->list[0].y == snake->list[i].y)
		 {
			 game_end(snake);
		 }
	 }
 }
 //�߳Ե���ʳ��
 void snake_eat_food(SNAKE* snake)
 {
	 //�����ͷ��ʳ��������غϣ���Ե���ʳ��
	 if (snake->list[0].x == snake->food.x && snake->list[0].y == snake->food.y)
	 {
		 //ԭʳ����ʧ�����Բ�д��,�����µ�ʳ��,��ͷ��ʾ�����غ���ʳ��--���Ҳ�Ҫ��ʳ���������ߵ�����
		 init_food(snake);
		 //�ߵ���������һ��
		 snake->size++;//size+1�˶��ӡ��һ��*,�����һ�أ��Ѿ������ϵĺ�������ֵ��
		 //��������
		 
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
 //��ʼ��Ϸ
void start_game(SNAKE* snake)
{
	while(snake->list[0].x < 60 && snake->list[0].x >= 0 && snake->list[0].y >= 0 && snake->list[0].y < 20)  //�ж���ͷ�����Ƿ�����ƺõı߽緶Χ֮��--�Ƿ�����ǽ
	{
		//�����ߵķ���
		control_snake(snake);
		
		//��Ҫ�ƶ�����Ҫ***�����ߵ�����
		move_snake(snake);           
		//���ƶ�--��ʾ�ƶ�������

		/*system("cls");//����--����ȥ�������ԭ��λ�ã���Ӧ���µ�λ��
		//��Ϊ��ǽ�����ˣ����Բ�������*/

		show_ui(snake);
		//���Ƿ�����ǽ
		//���Ƿ������Լ�
		snake_eat_body(snake);
		 
		//���Ƿ�����ʳ��-������ʳ�ԭʳ����ʧ�������µ�ʳ�� ��
		snake_eat_food(snake);

		//�Ӹ��ӳ�
		Sleep(300);//�ӳ�0.5s,����һֱ���߸���
	}
	game_end(snake);

}
void hide_cur()
{
	//���ؿ���̨���
	CONSOLE_CURSOR_INFO  cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}
int main(void)
{
	hide_cur();//���ع��λ��--�ߵ�β��
	init_wall();        //��ʼ��ǽ
	//init_ui();��ʾҳ��
	SNAKE  *snake = (SNAKE *)malloc(sizeof(SNAKE));//�����ߵĽṹ���С�ռ�
	init_snake(snake);//ֻ���˵�ַ��û�а��Լ��������ݴ���ȥ--��ʼ���ߺ�����

	show_ui(snake);   //��ʾ������Ϣ
	start_game(snake);//��ʼ��Ϸ
	

	system("pause");
	return EXIT_SUCCESS;
}
