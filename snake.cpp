#pragma comment(lib,"user32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<string.h>
#include<math.h>
#pragma comment(lib,"winmm.lib")

#define U 1
#define D 2
#define L 3
#define R 4

typedef struct Node
{
	int x;
	int y;
	struct Node* next;
} Node;

Node* head, * food, * index;            //头，食物，游标
void snake_create(); 				//蛇 
void wall_create();					//墙 
void Pos(int, int);					//光标位置 
void game_description();			//游戏说明界面
void snake_move();					//移动控制
void game_control();				//游戏控制
void start_game();					//开始游戏
void pause_game();					//暂停游戏
void exit_game();					//退出游戏
void bitself();						//咬自己
void bitwall();						//撞到墙
void spawn_food();					//出现食物
int score = 0, add = 10, death_type, direction = R, speedtime = 200;

void Pos(int x, int y)		//设置光标位置 
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

void game_description()			//游戏描述 
{
	Pos(40, 14);
	printf("欢迎进入贪吃蛇游戏！\n\n");
	system("pause");
	system("color de");
	system("cls");
	Pos(10, 14);
	printf("你可以用键盘上的：上↑，下↓，左←，右→。来控制小蛇方向\n");
	Pos(10, 15);
	printf("吃到食物后身体会变长！\n");
	Pos(10, 16);
	printf("如果你的小蛇碰到墙壁或者吃到自己，游戏就会结束哦！\n");
	system("pause");
	system("cls");
	wall_create();				//生成墙壁 
	snake_create();				//生成小蛇 
	spawn_food();				//生成食物 
}

void wall_create()			//生成墙 
{
	int i;
	for (i = 0; i < 58; i += 2) //打印上下边框 
	{
		Pos(i, 0);
		printf("■");
		Pos(i, 26);
		printf("■");
	}
	for (i = 1; i < 26; i++)	//打印左右边框 
	{
		Pos(0, i);
		printf("■");
		Pos(56, i);
		printf("■");
		Pos(70, 10);
		printf("当前分数");
	}
}

void snake_create()			//生成蛇 
{
	Node* tail;
	int i;
	tail = (Node*)malloc(sizeof(Node));
	tail->x = 24;
	tail->y = 5;
	tail->next = NULL;
	for (i = 1; i <= 4; i++)
	{
		head = (Node*)malloc(sizeof(Node));
		head->next = tail;
		head->x = 24 + 2 * i;
		head->y = 5;
		tail = head;
	}
	while (tail != NULL)			//由头到尾，输出蛇身 
	{
		Pos(tail->x, tail->y);
		printf("◆");
		tail = tail->next;
	}
}

void spawn_food()			//生成食物 
{
	if (food != NULL)		//避免食物重复生成 
	{
		return;
	}
	Node* food_1 = (Node*)malloc(sizeof(Node));
	food_1->x = rand() % 52 + 2;
	food_1->y = rand() % 24 + 1;		//确保食物在墙的范围内 
	while ((food_1->x) % 2 != 0)
	{
		food_1->x = rand() % 50 + 2;
	}
	index = head;
	while (index != NULL)
	{
		if (food_1->x == index->x && food_1->y == index->y)		//小蛇吃到食物 
		{
			free(food_1);	//吃掉食物后，释放内存，食物消失
			spawn_food();	//重新生成
			break;			//退出判断 
		}
		index = index->next;
	}
	food = food_1;
	Pos(food->x, food->y);
	printf("●");
}

void snake_move() 		//蛇的移动
{
	bitwall();
	Node* nexthead = (Node*)malloc(sizeof(Node));
	switch (direction)
	{
	case U:
		nexthead->x = head->x;
		nexthead->y = head->y - 1;			//坐标轴的原点在命令框的左上角 
		break;
	case D:
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
		break;
	case L:
		nexthead->x = head->x - 2;			//我们使用的字符是占三个字节的，所以需要至少两个字宽才能显示
		nexthead->y = head->y;
		break;
	case R:
		nexthead->x = head->x + 2;
		nexthead->y = head->y;
		break;
	}
	nexthead->next = head;
	head = nexthead;
	index = head;
	bitself();
	if (nexthead->x == food->x && nexthead->y == food->y)
	{
		while (index != NULL)
		{
			Pos(index->x, index->y);
			printf("◆");
			index = index->next;
		}
		score += add;
		free(food);
		food = NULL;  //吃到食物就将其赋空
		spawn_food();
	}
	else				//前进 
	{
		while (index->next->next != NULL)
		{
			Pos(index->x, index->y);
			printf("◆");
			index = index->next;
		}
		Pos(index->next->x, index->next->y);
		printf(" ");
		free(index->next);
		index->next = NULL;
	}
}

void game_control()		//控制小蛇 
{
	Pos(64, 15);
	printf("不能碰到墙，不能咬到自己\n");
	Pos(64, 16);
	printf("用↑↓←→控制蛇的方向");
	Pos(64, 17);
	printf("F1为加速,F2为减速\n");
	Pos(64, 18);
	printf("ESC:退出游戏      space:暂停游戏");
	direction = R;
	while (1)
	{
		Pos(64, 10);
		printf("得分：%d ", score);
		Pos(64, 11);
		printf("当前食物分值：%d分", add);
		if (GetAsyncKeyState(VK_UP) && direction != D)
		{
			direction = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && direction != U)
		{
			direction = D;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && direction != L)
		{
			direction = R;
		}
		else if (GetAsyncKeyState(VK_LEFT) && direction != R)
		{
			direction = L;
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			death_type = 3;
			break;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{
			pause_game();
		}
		else if (GetAsyncKeyState(VK_F1))		//加速 
		{
			if (speedtime > 50)
			{
				speedtime -= 30;
				add += 2;
				if (speedtime == 320)
				{
					add = 2;
				}
			}
		}
		else if (GetAsyncKeyState(VK_F2))		//减速 
		{
			speedtime += 30;
			add -= 2;
			if (speedtime == 350)
			{
				add = 1;
			}
		}
		Sleep(speedtime);
		snake_move();
	}
}

void pause_game()			//暂停 
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
		{
			break;
		}
	}
}

void bitwall()			//撞墙 
{
	if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)
	{
		death_type = 1;
		exit_game();
	}
}

void bitself()			//咬自己 
{
	Node* self = head->next;
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)
		{
			death_type = 2;
			exit_game();
		}
		self = self->next;
	}
}

void exit_game()		//退出游戏 
{
	system("cls");
	Pos(24, 12);
	if (death_type == 1)
	{
		printf("小蛇撞到墙了,游戏结束！");
	}
	else if (death_type == 2)
	{
		printf("小蛇咬到自己了，游戏结束！");
	}
	else if (death_type == 3)
	{
		printf("你结束了游戏！");
	}
	Pos(24, 13);
	printf("你的得分是%d\n", score);
	getchar();
	exit(0);
}

int main()
{
	system("title 贪吃蛇");
	system("mode con cols= 100 lines = 30");
	game_description();
	game_control();
	system("pause");

}

