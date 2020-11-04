#pragma once

#ifdef _WIN32
#include <conio.h>
#define clrscr() system("cls")
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif

#define height  25
#define width  50

typedef enum
{
	UP,
	DOWN,
	LEFT,
	RIGHT
}Orientation;

typedef struct
{
	int x;
	int y;
}Vector2i;

struct Tail
{
	struct Tail* tail;
	Vector2i position;
	Vector2i dir;
};
typedef struct Tail Tail;

typedef struct
{
	Vector2i position;
	Tail* tail;
	Orientation facing;
}Player;

void ASCIISnake();