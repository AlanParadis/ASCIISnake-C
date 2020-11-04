#ifndef ASCIISNAKE_H_
#define ASCIISNAKE_H_

#ifdef _WIN32
#include <conio.h>
#define clrscr() system("cls")
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif

#define height  25
#define width  50

// different orientation possible
typedef enum
{
	UP,
	DOWN,
	LEFT,
	RIGHT
}Orientation;

// simple vector struct
struct Vector2i
{
	int x;
	int y;
};
typedef struct Vector2i Vector2i;

// tail struct for the player
struct Tail
{
	struct Tail* tail;
	Vector2i position;
	Vector2i dir;
};
typedef struct Tail Tail;

// player struct
struct Player
{
	Vector2i position;
	Tail* tail;
	Orientation facing;
};
typedef struct Player Player;

/// @brief Game main function
void ASCIISnake();

#endif
