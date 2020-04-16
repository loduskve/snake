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

Node* head, * food, * index;            //ͷ��ʳ��α�
void snake_create(); 				//�� 
void wall_create();					//ǽ 
void Pos(int, int);					//���λ�� 
void game_description();			//��Ϸ˵������
void snake_move();					//�ƶ�����
void game_control();				//��Ϸ����
void start_game();					//��ʼ��Ϸ
void pause_game();					//��ͣ��Ϸ
void exit_game();					//�˳���Ϸ
void bitself();						//ҧ�Լ�
void bitwall();						//ײ��ǽ
void spawn_food();					//����ʳ��
int score = 0, add = 10, death_type, direction = R, speedtime = 200;

void Pos(int x, int y)		//���ù��λ�� 
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

void game_description()			//��Ϸ���� 
{
	Pos(40, 14);
	printf("��ӭ����̰������Ϸ��\n\n");
	system("pause");
	system("color de");
	system("cls");
	Pos(10, 14);
	printf("������ü����ϵģ��ϡ����¡���������ҡ���������С�߷���\n");
	Pos(10, 15);
	printf("�Ե�ʳ��������䳤��\n");
	Pos(10, 16);
	printf("������С������ǽ�ڻ��߳Ե��Լ�����Ϸ�ͻ����Ŷ��\n");
	system("pause");
	system("cls");
	wall_create();				//����ǽ�� 
	snake_create();				//����С�� 
	spawn_food();				//����ʳ�� 
}

void wall_create()			//����ǽ 
{
	int i;
	for (i = 0; i < 58; i += 2) //��ӡ���±߿� 
	{
		Pos(i, 0);
		printf("��");
		Pos(i, 26);
		printf("��");
	}
	for (i = 1; i < 26; i++)	//��ӡ���ұ߿� 
	{
		Pos(0, i);
		printf("��");
		Pos(56, i);
		printf("��");
		Pos(70, 10);
		printf("��ǰ����");
	}
}

void snake_create()			//������ 
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
	while (tail != NULL)			//��ͷ��β��������� 
	{
		Pos(tail->x, tail->y);
		printf("��");
		tail = tail->next;
	}
}

void spawn_food()			//����ʳ�� 
{
	if (food != NULL)		//����ʳ���ظ����� 
	{
		return;
	}
	Node* food_1 = (Node*)malloc(sizeof(Node));
	food_1->x = rand() % 52 + 2;
	food_1->y = rand() % 24 + 1;		//ȷ��ʳ����ǽ�ķ�Χ�� 
	while ((food_1->x) % 2 != 0)
	{
		food_1->x = rand() % 50 + 2;
	}
	index = head;
	while (index != NULL)
	{
		if (food_1->x == index->x && food_1->y == index->y)		//С�߳Ե�ʳ�� 
		{
			free(food_1);	//�Ե�ʳ����ͷ��ڴ棬ʳ����ʧ
			spawn_food();	//��������
			break;			//�˳��ж� 
		}
		index = index->next;
	}
	food = food_1;
	Pos(food->x, food->y);
	printf("��");
}

void snake_move() 		//�ߵ��ƶ�
{
	bitwall();
	Node* nexthead = (Node*)malloc(sizeof(Node));
	switch (direction)
	{
	case U:
		nexthead->x = head->x;
		nexthead->y = head->y - 1;			//�������ԭ�������������Ͻ� 
		break;
	case D:
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
		break;
	case L:
		nexthead->x = head->x - 2;			//����ʹ�õ��ַ���ռ�����ֽڵģ�������Ҫ���������ֿ������ʾ
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
			printf("��");
			index = index->next;
		}
		score += add;
		free(food);
		food = NULL;  //�Ե�ʳ��ͽ��丳��
		spawn_food();
	}
	else				//ǰ�� 
	{
		while (index->next->next != NULL)
		{
			Pos(index->x, index->y);
			printf("��");
			index = index->next;
		}
		Pos(index->next->x, index->next->y);
		printf(" ");
		free(index->next);
		index->next = NULL;
	}
}

void game_control()		//����С�� 
{
	Pos(64, 15);
	printf("��������ǽ������ҧ���Լ�\n");
	Pos(64, 16);
	printf("�á������������ߵķ���");
	Pos(64, 17);
	printf("F1Ϊ����,F2Ϊ����\n");
	Pos(64, 18);
	printf("ESC:�˳���Ϸ      space:��ͣ��Ϸ");
	direction = R;
	while (1)
	{
		Pos(64, 10);
		printf("�÷֣�%d ", score);
		Pos(64, 11);
		printf("��ǰʳ���ֵ��%d��", add);
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
		else if (GetAsyncKeyState(VK_F1))		//���� 
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
		else if (GetAsyncKeyState(VK_F2))		//���� 
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

void pause_game()			//��ͣ 
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

void bitwall()			//ײǽ 
{
	if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)
	{
		death_type = 1;
		exit_game();
	}
}

void bitself()			//ҧ�Լ� 
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

void exit_game()		//�˳���Ϸ 
{
	system("cls");
	Pos(24, 12);
	if (death_type == 1)
	{
		printf("С��ײ��ǽ��,��Ϸ������");
	}
	else if (death_type == 2)
	{
		printf("С��ҧ���Լ��ˣ���Ϸ������");
	}
	else if (death_type == 3)
	{
		printf("���������Ϸ��");
	}
	Pos(24, 13);
	printf("��ĵ÷���%d\n", score);
	getchar();
	exit(0);
}

int main()
{
	system("title ̰����");
	system("mode con cols= 100 lines = 30");
	game_description();
	game_control();
	system("pause");

}

