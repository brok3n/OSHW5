#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "defines.h"
#include "player.h"
#include "block.h"
#pragma warning(disable:4996)

int gBoardHeight;
int gBoardWidth;
clock_t start1, finish1, start2, finish2;
double duration1, duration2;


//SYSTEM
COORD GetCurrentCursorPos();
void SetCurrentCursorPos(int x, int y);

//DRAW
void ControlConsoleWindow();
int ShowMainMenu();
int ShowGame();
void ShowResult();
void DrawGameUI();
void UpdateTimeUI(int time);
void UpdateGameUI();
int GamePause();
void ShowGameOver();
void ShowStageClear(int time);
void ClearConsole();

//MAP
int gameBoardInfo[MAX_HEIGHT][MAX_WIDTH] = { 0 };
int SideQuest = 0;
int StageClear = 0;

//PLAYER
Player p;
COORD StartPosition;
void InitPlayer();
int DetectCollisionForPlayer(int x, int y);
int MovePlayer();

//BLOCKMANAGE
int UserBlockID[MAXUSERBLOCK] = { 0 };
int CurrentUserBlock = 0;
int page = 1;
int bX, bY;
int blockid = 0;
int MODE = 0, prevblockid = -1;
int prevbX = 0, prevbY = 0;
int collosion_redraw = 0;
void AddBlock(int blockid);
void BlockListUpdate(int UseBlock);
void BlockBuild(int key);
void UserBlockManage();
void BlockAllocator();
void ShowBlock(char blockinfo[4][4], int color);
void DeleteBlock(char blockinfo[4][4]);
void DeleteAllBlock();
void MakeBlock(char blockInfo[4][4]);
int DetectCollisionForBlock(int x, int y, char blockInfo[4][4]);


int main()
{
	srand(time(NULL));
	ControlConsoleWindow();

	ControlConsoleWindow();

	int control;

	while (1)
	{
		if (ShowMainMenu() == 0)
			break;
		BlockAllocator();
		while (1)
		{
			//���θ޴��� = 0 / �������� Ŭ���� = 1
			control = ShowGame();

			if (control == 0)
				break;
		}
	}

	return 0;
}



//SYSTEM
COORD GetCurrentCursorPos()
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}
void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


//DRAW
void ControlConsoleWindow()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

	system("mode con cols=236 lines=65 | title Escape From Sejong");
}
int ShowMainMenu()
{
	int select = 1;
	//1 = ���ӽ��� / 0 = ��������

	SetCurrentCursorPos(70, 10);
	printf(" #######   #####     ####     ##     ######   #######           #######  ######     ###    ##   ## ");
	SetCurrentCursorPos(70, 11);
	printf(" ##       ##   ##   ##  ##    ##     ##   ##  ##                ##       ##   ##   ## ##   ##   ## ");
	SetCurrentCursorPos(70, 12);
	printf(" ##       ##       ##        ####    ##   ##  ##                ##       ##   ##  ##   ##  ### ### ");
	SetCurrentCursorPos(70, 13);
	printf(" #####     #####   ##        ## #    ######   #####             #####    ######   ##   ##  ## # ## ");
	SetCurrentCursorPos(70, 14);
	printf(" ##            ##  ##       ######   ##       ##                ##       ## ##    ##   ##  ## # ## ");
	SetCurrentCursorPos(70, 15);
	printf(" ##       ##   ##   ##  ##  ##   #   ##       ##                ##       ##  ##    ## ##   ##   ## ");
	SetCurrentCursorPos(70, 16);
	printf(" #######   #####     ####  ###   ##  ##       #######           ##       ##   ##    ###    ##   ## ");

	SetCurrentCursorPos(91, 19);
	printf("  #####   #######    ####     ###    ##   ##    ####  ");
	SetCurrentCursorPos(91, 20);
	printf(" ##   ##  ##           ##    ## ##   ###  ##   ##  ## ");
	SetCurrentCursorPos(91, 21);
	printf(" ##       ##           ##   ##   ##  ###  ##  ##      ");
	SetCurrentCursorPos(91, 22);
	printf("  #####   #####        ##   ##   ##  ## # ##  ##      ");
	SetCurrentCursorPos(91, 23);
	printf("      ##  ##       ##  ##   ##   ##  ## # ##  ##  ### ");
	SetCurrentCursorPos(91, 24);
	printf(" ##   ##  ##       ##  ##    ## ##   ##  ###   ##  ## ");
	SetCurrentCursorPos(91, 25);
	printf("  #####   #######   ####      ###    ##   ##    ##### ");

	SetCurrentCursorPos(115, 40);
	printf("GAME START");

	SetCurrentCursorPos(118, 44);
	printf("EXIT");

	SetCurrentCursorPos(108, 40);
	printf("��");

	while (1)
	{
		if (_kbhit() != 0)
		{
			int key = _getch();
			//printf("%d", key);

			switch (key)
			{
			case DARROW:
				if (select == 1)
				{
					select = 0;
					SetCurrentCursorPos(108, 40);
					printf("  ");
					SetCurrentCursorPos(111, 44);
					printf("��");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos(111, 44);
					printf("  ");
					SetCurrentCursorPos(108, 40);
					printf("��");
				}
				break;
			case ENTER:
				ClearConsole();
				return select;
				break;
			}
		}
	}
}
int ShowGame()
{
	
}
void ShowResult()
{

}
void DrawGameUI()
{
	int statusBoardWidth = 28;
	int statusBoardHeight = gBoardHeight;

	{
		for (int y = 0; y < gBoardHeight + 2; y += gBoardHeight + 1)
			for (int x = 2; x < (gBoardWidth * 2) + statusBoardWidth + 2; x += 2)
			{
				SetCurrentCursorPos(x, y);
				printf("��");
			}

		for (int x = 0; x < (gBoardWidth * 2) + 3; x += (gBoardWidth * 2) + 2)
			for (int y = 1; y < gBoardHeight + 1; y++)
			{
				SetCurrentCursorPos(x, y);
				printf("��");
			}

		for (int y = 1; y < gBoardHeight + 1; y++)
		{
			SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, y);
			printf("��");
		}
	}

	// ��������Ǳ׸���
	{
		int x, y;
		x = (gBoardWidth * 2) + 6, y = gBoardHeight - 1;
		for (int z = 1; z <= 3; z++)
		{
			for (int x = (gBoardWidth * 2) + 2; x <= (gBoardWidth * 2) + 30; x += 2)
			{
				SetCurrentCursorPos(x, y);
				if (x == (gBoardWidth * 2) + 2)
					printf("��");
				else if (x == (gBoardWidth * 2) + 30)
					printf("��");
				else
					printf("��");
			}
			y -= 6;
		}
		for (int y = gBoardHeight - 1; y >= gBoardHeight - 13; y--)
		{
			x = (gBoardWidth * 2) + 16;
			SetCurrentCursorPos(x, y);
			if (y == gBoardHeight - 13)
				printf("��");
			else if (y == gBoardHeight - 1)
				printf("��");
			else if (y == gBoardHeight - 7)
				printf("��");
			else
				printf("��");

		}
	}

	{
		SetCurrentCursorPos(0, 0);
		printf("��");
		SetCurrentCursorPos((gBoardWidth * 2) + 2, gBoardHeight + 1);
		printf("��");
		SetCurrentCursorPos((gBoardWidth * 2) + 2, 0);
		printf("��");
		SetCurrentCursorPos(0, gBoardHeight + 1);
		printf("��");
		SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, 0);
		printf("��");
		SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, gBoardHeight + 1);
		printf("��");
	}

	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 2);
	printf("TIME");
	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 3);
	printf("%02d:%02d", 0, 0);
	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2), 5);
	//for (int i = 1; i <= 3; i++)
	//{
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	//	if (i <= SideQuest)
	//		printf("��");
	//}
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

	//if (p.isExtraLife)
	//{
	//	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) + 2, 7);
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
	//	printf("�� ");
	//}
}
void UpdateTimeUI(int time)
{
	
}
void UpdateGameUI()
{
	
}
int GamePause()
{
	//0 = ���� �޴��� / 1 = ���� �簳

	int select = 1;

	for (int y = gBoardHeight / 2 - 6; y <= gBoardHeight / 2 + 6; y++)
	{
		for (int x = (gBoardWidth * 2) / 2 - 15; x <= (gBoardWidth * 2) / 2 + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("  ");
		}
	}

	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y += 10)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("��");
		}
	}
	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y++)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x += 30)
		{
			SetCurrentCursorPos(x, y);
			printf("��");
		}
	}

	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 - 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 - 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 + 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 + 5);
	printf("��");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 10, gBoardHeight / 2 - 3);
	printf("G A M E   P A U S E D");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 6, gBoardHeight / 2);
	printf("��  RESUME");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 9, gBoardHeight / 2 + 2);
	printf("RETURN TO MAIN MENU");



	while (1)
	{
		if (_kbhit() != 0)
		{
			int key = _getch();

			switch (key)
			{
			case DARROW:
				if (select == 1)
				{
					select = 0;
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 6, gBoardHeight / 2);
					printf("  ");
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 13, gBoardHeight / 2 + 2);
					printf("��");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 13, gBoardHeight / 2 + 2);
					printf("  ");
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 6, gBoardHeight / 2);
					printf("��");
				}
				break;
			case ENTER:
				return select;
				break;
			}
		}
	}
}
void ShowGameOver()
{
	for (int y = gBoardHeight / 2 - 6; y <= gBoardHeight / 2 + 6; y++)
	{
		for (int x = (gBoardWidth * 2) / 2 - 15; x <= (gBoardWidth * 2) / 2 + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("  ");
		}
	}

	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y += 10)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("��");
		}
	}
	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y++)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x += 30)
		{
			SetCurrentCursorPos(x, y);
			printf("��");
		}
	}

	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 - 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 - 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 + 5);
	printf("��");
	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 + 5);
	printf("��");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 8, gBoardHeight / 2 - 3);
	printf("G A M E   O V E R !");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 8, gBoardHeight / 2 + 2);
	printf("PRESS ANY KEY TO");
	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 9, gBoardHeight / 2 + 3);
	printf("RETURN TO MAIN MENU");

	Sleep(1500);

	while (1)
	{
		if (_kbhit() != 0)
		{
			break;
		}
	}

	return 0;
}
void ShowStageClear(int time)
{
	
}
void ClearConsole()
{
	for (int y = 0; y < MAX_HEIGHT; y++)
	{
		for (int x = 0; x < MAX_WIDTH; x++)
		{
			SetCurrentCursorPos(x, y);
			printf("  ");
		}
	}
}	



//PLAYER
void InitPlayer()
{
	p.x = 4;
	p.y = 10;
	p.t_jump = 2;
	p.isDead = 0;
	p.isExtraLife = 0;
	p.defyingGravity = 0;
	p.invincibility = 0;
}
int DetectCollisionForPlayer(int x, int y)
{
	//�÷��̾ �����̰���� ��ǥ x, y�� ���ڷ� ����
	//���� ������� �״� ������Ʈ�� �浹�ϸ� -1�� ������
	//��Ƶ� �����ӿ� ������ ����(����ϰų� �Դ� ������Ʈ��) 0�� ������
	//��Ƶ� ������ ������ �������� ������ 1�� ������

	int colID = gameBoardInfo[y - 1][x - 1] / 100;

	if ((x < 1 || x >= gBoardWidth + 2) || (y < 1 || y >= gBoardHeight))
		return 1;
	if (colID == 1)
	{
		int type = gameBoardInfo[y - 1][x - 1] / 10 % 10;
		if (type == 2)
		{
			drawPurplePuzzle(1);
		}
	}
	if (colID == 3)
		return -1;
	else if (colID == 0 || colID == 7 || colID == 8)
	{
		int type = gameBoardInfo[y - 1][x - 1] / 10 % 10;

		//ITEM
		if (colID == 7)
		{
			switch (type)
			{
			case 1: //�߷¹���
				p.defyingGravity = 1;
				start1 = clock();
				break;
			case 2: //����
				p.invincibility = 1;
				start2 = clock();
				break;
			case 3:
				p.isExtraLife = 1;
				UpdateGameUI();
				break;
			}
		}
		//POINT
		else if (colID == 8)
		{
			switch (type)
			{
			case 1:
				SideQuest++;
				UpdateGameUI();
				break;
			case 3:
				StageClear = 1;
				break;
			}
		}

		return 0;
	}
	else
		return 1;
}
int MovePlayer()
{
	int rtn;
	if (p.defyingGravity == 0 && p.invincibility == 0) {
		UpdateGameUI();
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 &&
			p.x > 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 - 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� �������� 1ĭ �̵�
				p.x -= 2;
			}
			else if (rtn == -1)
				DiePlayer();

		}
		//������Ű ������
		//����:ĳ������ġ�� �� ���ʿ��ְ� &&
		//���Ӻ��������� ������ ���� 0�϶�
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.x += 2;
			}
			else if (rtn == -1)
				DiePlayer();
		}

		//�ٴ� Y��(�ٴڱ��� ĳ���Ͱ� �������� �ϱ����� �� ����)
		int floor_Y = gBoardHeight + 1;
		if (GetAsyncKeyState(VK_UP) & 0x8000
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//ĳ������ t_jump���� 0���θ�����
			if (rtn == 1)
				p.t_jump = 0;
			else if (rtn == -1)
				DiePlayer();
		}

		SetCurrentCursorPos(p.x, p.y);
		//ĳ���Ͱ� �������ڸ��� �������θ��������(����������)
		printf("  ");
		//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������(����������)
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;

		//����
		//ĳ���Ͱ� �� �ٴں��� ���� �ְ� 
		//&& ĳ���� t_jump �� 2���� ũ��
		//&& ĳ���� �Ʒ� ���Ӻ����������� 0�϶�

		//�Ʒ�
		if (p.y < floor_Y
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//ĳ���� �Ʒ��� ������
			if (rtn == 0)
				p.y += 1;
			else if (rtn == -1)
				DiePlayer();
		}

		//ĳ���� t_jump �� 2����������(up�� ������ t_jump�� 0�̵�����)
		if (p.t_jump < 2) {
			//����:ĳ�������� ��������������(���� �浹ó��)
			//&&ĳ���Ͱ� �� ���� �Ѿ���ʰ�
			if (p.y > 1)
			{
				rtn = DetectCollisionForPlayer(p.x / 2, p.y - 1);

				if (rtn == 0)
					p.y -= 1;
				//ĳ���� ���� �ø���
				//floor(�ݿø�)�� ���� ĳ���Ͱ� �� �����ϰ� �� ���ִµ� �����ӵ��� ������
				//p.y -= floor(p.t_jump);
			}
			//������ �׸��ϰ��ϱ����� t_jump�� ����
			p.t_jump += 0.6;

		}
	}
	//�߷¹��þ����� �Ծ�����
	else if (p.defyingGravity == 1 && p.invincibility == 0) {
		finish1 = clock();
		duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
		UpdateGameUI();
		if (duration1 > DEFYING_GRAVITY_TIME) {
			p.defyingGravity = 0;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 &&
			p.x > 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 - 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� �������� 1ĭ �̵�
				p.x -= 2;
			}
			else if (rtn == -1)
				DiePlayer();

		}
		//������Ű ������
		//����:ĳ������ġ�� �� ���ʿ��ְ� &&
		//���Ӻ��������� ������ ���� 0�϶�
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.x += 2;
			}
			else if (rtn == -1)
				DiePlayer();
		}

		//�ٴ� Y��(�ٴڱ��� ĳ���Ͱ� �������� �ϱ����� �� ����)
		int floor_Y = gBoardHeight + 1;
		//���� Ű ������
		//����:ĳ���� t_jump���� 2���� Ŭ�� &&
		//ĳ���� �Ʒ� ���Ӻ��� �������� 2 �϶�
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y - 1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.y -= 1;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.y += 1;
			}
		}
	}
	//���������� �Ծ�����
	else if (p.defyingGravity == 0 && p.invincibility == 1) {
		finish2 = clock();
		duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
		UpdateGameUI();
		if (duration2 > INVINCIBILITY_TIME) {
			p.invincibility = 0;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 &&
			p.x > 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 - 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� �������� 1ĭ �̵�
				p.x -= 2;
			}

		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.x += 2;
			}
		}

		//�ٴ� Y��(�ٴڱ��� ĳ���Ͱ� �������� �ϱ����� �� ����)
		int floor_Y = gBoardHeight + 1;
		if (GetAsyncKeyState(VK_UP) & 0x8000
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//ĳ������ t_jump���� 0���θ�����
			if (rtn == 1 || rtn == -1)
				p.t_jump = 0;
		}

		SetCurrentCursorPos(p.x, p.y);
		//ĳ���Ͱ� �������ڸ��� �������θ��������(����������)
		printf("  ");
		//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������(����������)
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;

		//����
		//ĳ���Ͱ� �� �ٴں��� ���� �ְ� 
		//&& ĳ���� t_jump �� 2���� ũ��
		//&& ĳ���� �Ʒ� ���Ӻ����������� 0�϶�

		//�Ʒ�
		if (p.y < floor_Y
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//ĳ���� �Ʒ��� ������
			if (rtn == 0)
				p.y += 1;
		}

		//ĳ���� t_jump �� 2����������(up�� ������ t_jump�� 0�̵�����)
		if (p.t_jump < 2) {
			if (p.y > 1)
			{
				rtn = DetectCollisionForPlayer(p.x / 2, p.y - 1);

				if (rtn == 0)
					p.y -= 1;

			}
			p.t_jump += 0.6;
		}
	}
	//�߷¹��þ����۰� ���������� �Ѵ� �Ծ�����
	else if (p.defyingGravity == 1 && p.invincibility == 1) {
		finish1 = clock();
		finish2 = clock();
		duration1 = (double)(finish1 - start1) / CLOCKS_PER_SEC;
		duration2 = (double)(finish2 - start2) / CLOCKS_PER_SEC;
		UpdateGameUI();
		if (duration1 > DEFYING_GRAVITY_TIME) {
			p.defyingGravity = 0;
		}
		if (duration2 > INVINCIBILITY_TIME) {
			p.invincibility = 0;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 &&
			p.x > 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 - 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� �������� 1ĭ �̵�
				p.x -= 2;
			}
		}
		//������Ű ������
		//����:ĳ������ġ�� �� ���ʿ��ְ� &&
		//���Ӻ��������� ������ ���� 0�϶�
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.x += 2;
			}
		}

		//�ٴ� Y��(�ٴڱ��� ĳ���Ͱ� �������� �ϱ����� �� ����)
		int floor_Y = gBoardHeight + 1;
		//���� Ű ������
		//����:ĳ���� t_jump���� 2���� Ŭ�� &&
		//ĳ���� �Ʒ� ���Ӻ��� �������� 2 �϶�
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y - 1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.y -= 1;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);
			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//ĳ���Ͱ� �������ڸ��� �������θ��������
				printf("  ");
				//ĳ���Ͱ� �������ڸ� ���Ӻ��������� 0���� ������
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//ĳ���� ���������� 1ĭ �̵�
				p.y += 1;
			}
		}
	}
}
void DiePlayer()
{
	if (!p.isExtraLife)
		p.isDead = 1;
	else
	{
		int revive = 0;

		p.isExtraLife = 0;
		UpdateGameUI();
		SetCurrentCursorPos(p.x, p.y);
		printf("  ");
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;

		p.x = StartPosition.X;
		p.y = StartPosition.Y;

		SetCurrentCursorPos(p.x, p.y);
		printf("��");
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 900;
	}

}



void AddBlock(int blockid)
{
	UserBlockID[CurrentUserBlock] = blockid;
	CurrentUserBlock++;
}
void BlockListUpdate(int UseBlock)
{
	for (int i = UseBlock; i < MAXUSERBLOCK - 1; i++)
	{
		UserBlockID[i] = UserBlockID[i + 1];
		if (UserBlockID[i + 1] == -1) {
			UserBlockID[i] = -1;
			break;
		}
	}
	CurrentUserBlock--;
}
void BlockBuild(int key)
{
	// �浹�� �߻��ϸ� ������ ��ġ�� ��������� �����ϼ� �ְ� �浹�� ����
	// �浹�� ��ġ �Ұ���
	int color = WHITE;
	int categori = 1;
	if (UserBlockID[blockid] == 28)
		categori = 2;
	else if (UserBlockID[blockid] == 29)
		categori = 3;
	else if (UserBlockID[blockid] == 30)
		categori = 4;
	else if (UserBlockID[blockid] == 31)
		categori = 5;
	else
		categori = 1;
	SetCurrentCursorPos(prevbX, prevbY);
	switch (key)
	{
	case 49:
		blockid = (page - 1) * 4;
		break;
	case 50:
		blockid = (page - 1) * 4 + 1;
		break;
	case 51:
		blockid = (page - 1) * 4 + 2;
		break;
	case 52:
		blockid = (page - 1) * 4 + 3;
		break;
	case LARROW:
		if (-bX + p.x <= 10 && bX > 2)
			bX = bX - 2;

		break;
	case RARROW:
		if (bX - p.x <= 10 && bX + 4 < 2 * gBoardWidth)
			bX = bX + 2;

		break;
	case UARROW:
		if (-bY + p.y <= 5 && bY > 1)
			bY--;

		break;
	case DARROW:
		if (bY - p.y <= 5 && bY + 2 < gBoardHeight)
			bY++;

		break;

	case SPACE:
		if (!(DetectCollisionForBlock(bX, bY, blockModel[prevblockid])))
		{
			UserBlockID[blockid] = -1;
			//BlockListUpdate(blockid);

			MakeBlock(blockModel[prevblockid], categori);
			UserBlockManage();
			prevblockid = -1;
			MODE = 0;
			DrawGameBoard();
		}
		break;
	case L_ROTATE:
		UserBlockID[blockid] = 4 * (UserBlockID[blockid] / 4) + (UserBlockID[blockid] + 3) % 4;

		break;
	case R_ROTATE:
		UserBlockID[blockid] = 4 * (UserBlockID[blockid] / 4) + (UserBlockID[blockid] + 1) % 4;

		break;
	case BUILD:

		break;
	}

	if (prevblockid != -1)
	{
		DeleteBlock(blockModel[prevblockid]);
		if (collosion_redraw == 1)
		{
			DrawGameBoardPart();
			//DrawGameBoard();
			collosion_redraw = 0;
		}
	}
	SetCurrentCursorPos(bX, bY);
	if (UserBlockID[blockid] >= 28)
		color = BLUE;
	if ((DetectCollisionForBlock(bX, bY, blockModel[UserBlockID[blockid]])))
	{
		ShowBlock(blockModel[UserBlockID[blockid]], LIGHTRED);
		collosion_redraw = 1;
	}
	else
		ShowBlock(blockModel[UserBlockID[blockid]], color);
	color = WHITE;
	if (UserBlockID[blockid] == -1) {
		BlockListUpdate(blockid);
		UserBlockManage();
	}
	prevbX = bX, prevbY = bY;
	prevblockid = UserBlockID[blockid];

}
void ShowBlock(char blockInfo[4][4], int color)
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
				printf("��");
			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	SetCurrentCursorPos(curPos.X, curPos.Y);
}

void DeleteBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("  ");
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}
void MakeBlock(char blockInfo[4][4], int blockCategori)
{
	//1 �⺻�� 2~5 �������� 6~ ?
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	curPos.X = curPos.X - 2;
	curPos.Y--;
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			if (blockInfo[y][x] == 1)
			{

				if (blockCategori == 2)
					gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 610;
				else if (blockCategori == 3)
					gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 620;
				else if (blockCategori == 4)
					gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 630;
				else if (blockCategori == 5)
					gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 640;
				else
					gameBoardInfo[y + curPos.Y][x + curPos.X / 2] = 100;

			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}
void DeleteAllBlock()
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			printf("  ");
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}
int DetectCollisionForBlock(int x, int y, char blockInfo[4][4])
{
	int x1, y1;
	COORD curPos = GetCurrentCursorPos();
	x = x - 2;
	y--;
	for (y1 = 0; y1 < 4; y1++) {
		for (x1 = 0; x1 < 4; x1++) {
			if (blockInfo[y1][x1] == 1 && gameBoardInfo[y1 + y][x1 + x / 2] != 000)
				return 1;

		}
	}
	return 0;
}