#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "defines.h"
#include "player.h"
#pragma warning(disable:4996)

int gBoardHeight;
int gBoardWidth;


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

//PLAYER
Player p;
COORD StartPosition;
void InitPlayer();
void ControlCharacter(int key);
int DetectCollisionForPlayer(int x, int y);
int MovePlayer();


int main()
{
	srand(time(NULL));
	ControlConsoleWindow();


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
	//1 = 게임시작 / 0 = 게임종료

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
	printf("▶");

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
					printf("▶");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos(111, 44);
					printf("  ");
					SetCurrentCursorPos(108, 40);
					printf("▶");
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
				printf("─");
			}

		for (int x = 0; x < (gBoardWidth * 2) + 3; x += (gBoardWidth * 2) + 2)
			for (int y = 1; y < gBoardHeight + 1; y++)
			{
				SetCurrentCursorPos(x, y);
				printf("│");
			}

		for (int y = 1; y < gBoardHeight + 1; y++)
		{
			SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, y);
			printf("│");
		}
	}

	// 유저블록판그리기
	{
		int x, y;
		x = (gBoardWidth * 2) + 6, y = gBoardHeight - 1;
		for (int z = 1; z <= 3; z++)
		{
			for (int x = (gBoardWidth * 2) + 2; x <= (gBoardWidth * 2) + 30; x += 2)
			{
				SetCurrentCursorPos(x, y);
				if (x == (gBoardWidth * 2) + 2)
					printf("├");
				else if (x == (gBoardWidth * 2) + 30)
					printf("┤");
				else
					printf("─");
			}
			y -= 6;
		}
		for (int y = gBoardHeight - 1; y >= gBoardHeight - 13; y--)
		{
			x = (gBoardWidth * 2) + 16;
			SetCurrentCursorPos(x, y);
			if (y == gBoardHeight - 13)
				printf("┬");
			else if (y == gBoardHeight - 1)
				printf("┴");
			else if (y == gBoardHeight - 7)
				printf("┼");
			else
				printf("│");

		}
	}

	{
		SetCurrentCursorPos(0, 0);
		printf("┌");
		SetCurrentCursorPos((gBoardWidth * 2) + 2, gBoardHeight + 1);
		printf("┴");
		SetCurrentCursorPos((gBoardWidth * 2) + 2, 0);
		printf("┬");
		SetCurrentCursorPos(0, gBoardHeight + 1);
		printf("└");
		SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, 0);
		printf("┐");
		SetCurrentCursorPos((gBoardWidth * 2) + statusBoardWidth + 2, gBoardHeight + 1);
		printf("┘");
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
	//		printf("★");
	//}
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

	//if (p.isExtraLife)
	//{
	//	SetCurrentCursorPos((gBoardWidth * 2) + (statusBoardWidth / 2) + 2, 7);
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
	//	printf("♥ ");
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
	//0 = 메인 메뉴로 / 1 = 게임 재개

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
			printf("─");
		}
	}
	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y++)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x += 30)
		{
			SetCurrentCursorPos(x, y);
			printf("│");
		}
	}

	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 - 5);
	printf("┌");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 - 5);
	printf("┐");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 + 5);
	printf("┘");
	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 + 5);
	printf("└");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 10, gBoardHeight / 2 - 3);
	printf("G A M E   P A U S E D");

	SetCurrentCursorPos((gBoardWidth * 2) / 2 - 6, gBoardHeight / 2);
	printf("▶  RESUME");

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
					printf("▶");
				}
				break;
			case UARROW:
				if (select == 0)
				{
					select = 1;
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 13, gBoardHeight / 2 + 2);
					printf("  ");
					SetCurrentCursorPos((gBoardWidth * 2) / 2 - 6, gBoardHeight / 2);
					printf("▶");
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
			printf("─");
		}
	}
	for (int y = gBoardHeight / 2 - 5; y <= gBoardHeight / 2 + 5; y++)
	{
		for (int x = gBoardWidth - 15; x <= gBoardWidth + 15; x += 30)
		{
			SetCurrentCursorPos(x, y);
			printf("│");
		}
	}

	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 - 5);
	printf("┌");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 - 5);
	printf("┐");
	SetCurrentCursorPos(gBoardWidth + 15, gBoardHeight / 2 + 5);
	printf("┘");
	SetCurrentCursorPos(gBoardWidth - 15, gBoardHeight / 2 + 5);
	printf("└");

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
void ControlCharacter(int key)
{

	switch (key)
	{
	case LARROW:
	case RARROW:
	case UARROW:
	case DARROW:
		break;
	case BUILD:
		break;
	}


}
int DetectCollisionForPlayer(int x, int y)
{
	//플레이어가 움직이고싶은 좌표 x, y를 인자로 받음
	//만약 닿았을때 죽는 오브젝트와 충돌하면 -1를 리턴함
	//닿아도 움직임에 영향이 없는(통과하거나 먹는 오브젝트면) 0을 리턴함
	//닿아도 죽지는 않지만 움직임이 막히면 1을 리턴함

	
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
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 왼쪽으로 1칸 이동
				p.x -= 2;
			}
			else if (rtn == -1)
				DiePlayer();

		}
		//오른쪽키 누를때
		//조건:캐릭터위치가 맵 안쪽에있고 &&
		//게임보드인포상 오른쪽 값이 0일때
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 &&
			p.x < gBoardWidth * 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2 + 1, p.y);

			if (rtn == 0)
			{
				SetCurrentCursorPos(p.x, p.y);
				//캐릭터가 지나간자리를 공백으로만들기위해
				printf("  ");
				//캐릭터가 지나간자리 게임보드인포를 0으로 만들음
				gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;
				//캐릭터 오른쪽으로 1칸 이동
				p.x += 2;
			}
			else if (rtn == -1)
				DiePlayer();
		}

		//바닥 Y값(바닥까지 캐릭터가 떨어지게 하기위해 값 지정)
		int floor_Y = gBoardHeight + 1;
		if (GetAsyncKeyState(VK_UP) & 0x8000
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//캐릭터의 t_jump값을 0으로만들음
			if (rtn == 1)
				p.t_jump = 0;
			else if (rtn == -1)
				DiePlayer();
		}

		SetCurrentCursorPos(p.x, p.y);
		//캐릭터가 지나간자리를 공백으로만들기위해(점프했을때)
		printf("  ");
		//캐릭터가 지나간자리 게임보드인포를 0으로 만들음(점프했을때)
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 0;

		//점프
		//캐릭터가 맵 바닥보다 높이 있고 
		//&& 캐릭터 t_jump 가 2보다 크고
		//&& 캐릭터 아래 게임보드인포값이 0일때

		//아래
		if (p.y < floor_Y
			&& p.t_jump >= 2)
		{
			rtn = DetectCollisionForPlayer(p.x / 2, p.y + 1);

			//캐릭터 아래로 내리기
			if (rtn == 0)
				p.y += 1;
			else if (rtn == -1)
				DiePlayer();
		}

		//캐릭터 t_jump 가 2보다작을때(up을 눌러서 t_jump가 0이됐을때)
		if (p.t_jump < 2) {
			//조건:캐릭터위에 벽이있지않으면(위쪽 충돌처리)
			//&&캐릭터가 맵 위쪽 넘어가지않게
			if (p.y > 1)
			{
				rtn = DetectCollisionForPlayer(p.x / 2, p.y - 1);

				if (rtn == 0)
					p.y -= 1;
				//캐릭터 위로 올리기
				//floor(반올림)을 쓰면 캐릭터가 덜 점프하게 할 수있는데 반응속도가 느려짐
				//p.y -= floor(p.t_jump);
			}
			//점프를 그만하게하기위한 t_jump값 조정
			p.t_jump += 0.6;

		}
	}
	//중력무시아이템 먹었을때
	else if (p.defyingGravity == 1 && p.invincibility == 0) 
	{
		
	}
	//무적아이템 먹었을때
	else if (p.defyingGravity == 0 && p.invincibility == 1) 
	{
		
	}
	//중력무시아이템과 무적아이템 둘다 먹었을때
	else if (p.defyingGravity == 1 && p.invincibility == 1)
	{
		
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
		printf("●");
		gameBoardInfo[p.y - 1][p.x / 2 - 1] = 900;
	}

}