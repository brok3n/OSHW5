#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#pragma warning(disable:4996)


//SYSTEM
COORD GetCurrentCursorPos();
void SetCurrentCursorPos(int x, int y);


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