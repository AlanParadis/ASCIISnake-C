#include "ASCIISnake.h"

#include <stdio.h>
#include <stdlib.h>

/// @brief Function that move the player with the given offset
/// 
/// @param _tab Playground array
/// 
/// @param _player Player
/// 
/// @param _offset Offset wich the player will be moved by
void MovePlayer(int _tab[height][width], Player* _player, Vector2i _offset);

/// @brief Move the player taill acording to the player movement
/// 
/// @param _tab Playground array
///  
/// @param _player Player
void MoveTail(int _tab[height][width], Player* _player);

/// @brief Add lenght to the tail of the player
/// 
/// @param _tab Playground array
/// 
/// @param _player Player
void AddTail(int _tab[height][width], Player* _player);

/// @brief Generate a new bonus randomly on the playground
/// 
/// @param _tab Playground array
void CreateBonus(int _tab[height][width]);

/// @brief Load the game
void LoadGame();

typedef enum
{
	GAME,
	MENU
}Gamestate;

static int tab[height][width];
static Player player;
static double gameClock;
static double framerate;

Gamestate state;

void ASCIISnake()
{
	// set the framerate
	framerate = 1. / 5.;

	// set game state
	state = MENU;

	// setup game
	LoadGame();
	// game
	while (1)
	{
		if (state == GAME)
		{
			// update game clock
			gameClock += 0.00000001;
			if (gameClock > framerate)
			{
				// reset clock and clear screen
				gameClock = 0.;
				clrscr();
				// draw the game
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						if (tab[i][j] == 0)
						{
							printf(" ");
						}
						else if (tab[i][j] == 1)
						{
							printf("*");
						}
						else if (tab[i][j] == 2)
						{
							printf("@");
						}
						else if (tab[i][j] == 3)
						{
							printf("O");
						}
						else if (tab[i][j] == 4)
						{
							printf("#");
						}
					}
					printf("\n");
				}
				// get player input
				if (_kbhit())
				{
					char c = _getch();
					///////////////////////////////////
					//				↑ 72				//
					//		 75	←		 → 77		//
					//				↓ 80				//
					///////////////////////////////////
					// change player direction
					if (c == 72 && player.facing != DOWN)
					{
						
						player.facing = UP;
					}
					else if (c == 80 && player.facing != UP)
					{
						
						player.facing = DOWN;
					}
					else if (c == 77 && player.facing != LEFT)
					{
						
						player.facing = RIGHT;
					}
					else if (c == 75 && player.facing != RIGHT)
					{
						
						player.facing = LEFT;
					}
				}
				// move player according to it's direction
				if (player.facing == UP)
				{
					MovePlayer(tab, &player, (Vector2i) { 0, -1 });
				}
				else if (player.facing == DOWN)
				{
					MovePlayer(tab, &player, (Vector2i) { 0, 1 });
				}
				else if (player.facing == LEFT)
				{
					MovePlayer(tab, &player, (Vector2i) { -1, 0 });
				}
				else if (player.facing == RIGHT)
				{
					MovePlayer(tab, &player, (Vector2i) { 1, 0 });
				}
			}
		}
		else
		{
			// update clock
			gameClock += 0.00001;
			if (gameClock > framerate)
			{
				// reset clock and draw menu
				gameClock = 0;
				clrscr();
				printf("\n\n\n\t\t===>Press enter to play <===\t\t\n\n\n");
			}
			if (_kbhit())
			{
				// wait for input
				char c = _getch();
				if (c == 13)
				{
					state = GAME;
					LoadGame();
				}
			}
		}
		
	}
}

void MovePlayer(int _tab[height][width], Player* _player, Vector2i _offset)
{
	// prevent player from getting out of the grid
	if (_player->position.y != 0 && _player->position.x != 0)
	{
		_tab[_player->position.y][_player->position.x] = 0;
	}
	// update the tail of the player
	if (_player->tail != NULL)
	{
		_player->tail->dir.x = _player->position.x - _player->tail->position.x;
		_player->tail->dir.y = _player->position.y - _player->tail->position.y;
	}
	// move player
	_player->position.x += _offset.x;
	_player->position.y += _offset.y;
	// gather bonus if the player encounter one
	if (_tab[_player->position.y][_player->position.x] == 4)
	{
		AddTail(_tab, _player);
		CreateBonus(_tab);
	}
	// if the player hit his tail or a wall, game over
	else if (_tab[_player->position.y][_player->position.x] == 3 
		|| _tab[_player->position.y][_player->position.x] == 1)
	{
		state = MENU;
	}
	// update the playground 
	_tab[_player->position.y][_player->position.x] = 2;
	// move the player tail;
	MoveTail(tab, &player);
}

void MoveTail(int _tab[height][width], Player* _player)
{
	if (_player->tail != NULL)
	{
		Tail* head = _player->tail;
		Tail* tmp;
		while (head != NULL)
		{
			tmp = head;
			head = head->tail;
			// every tail get it's direction by looking at the tail before it
			if (head != NULL)
			{
				head->dir.x = tmp->position.x - head->position.x;
				head->dir.y = tmp->position.y - head->position.y;
			}

			//clean the tail before it
			_tab[tmp->position.y][tmp->position.x] = 0;

			//update position
			tmp->position = (Vector2i){
				tmp->position.x + tmp->dir.x,
				tmp->position.y + tmp->dir.y };

			// update the playground
			_tab[tmp->position.y][tmp->position.x] = 3;
		}
	}
}

void AddTail(int _tab[height][width], Player* _player)
{
	// check if this new tail is the first one
	char firstTail = 0;
	if (_player->tail == NULL)
	{
		_player->tail = malloc(sizeof(Tail));
		if (_player->tail != NULL)
		{
			_player->tail->position = (Vector2i){ 0,0 };
			_player->tail->tail = NULL;
			firstTail = 1;
		}
	}
	if (_player->tail != NULL)
	{
		Tail* head = _player->tail;
		Tail* tmp;

		// if the tail is new, add the tail according to the facinf of the player
		if (firstTail == 1)
		{
			if (_player->facing == UP)
			{
				head->position =
					(Vector2i){ _player->position.x, _player->position.y + 1 };
				_tab[head->position.y][head->position.x] = 3;
				head->dir = (Vector2i){ 0,-1 };
				return;
			}
			else if (_player->facing == DOWN)
			{
				head->position =
					(Vector2i){ _player->position.x, _player->position.y - 1 };
				_tab[head->position.y][head->position.x] = 3;
				head->dir = (Vector2i){ 0,1 };
				return;
			}
			else if (_player->facing == LEFT)
			{
				head->position =
					(Vector2i){ _player->position.x + 1, _player->position.y };
				_tab[head->position.y][head->position.x] = 3;
				head->dir = (Vector2i){ -1,0 };
				return;
			}
			else if (_player->facing == RIGHT)
			{
				head->position =
					(Vector2i){ _player->position.x - 1, _player->position.y };
				_tab[head->position.y][head->position.x] = 3;
				head->dir = (Vector2i){ 1,0 };
				return;
			}
			else
				_player->tail->tail = NULL;
		}

		// get to the last tail to place the new tail according to the direction 
		// of the last tail
		while (head != NULL)
		{
			tmp = head;
			head = head->tail;
			if (head == NULL)
			{
				head = malloc(sizeof(Tail));
				if (head != NULL)
				{
					head->position =
						(Vector2i){ tmp->position.x - tmp->dir.x, tmp->position.y - tmp->dir.y };
					_tab[head->position.y][head->position.x] = 3;
					head->tail = NULL;
					head->dir = tmp->dir;
					tmp->tail = head;
					return;
				}
			}
		}
	}
}

void CreateBonus(int _tab[height][width])
{
	int x;
	int y;
	do
	{
		x = 1 + (rand() % (width - 2));
		y = 1 + (rand() % (height - 2));
	}
	while (_tab[y][x] != 0);
	_tab[y][x] = 4;
}

void LoadGame()
{
	// setup game state
	//state = MENU;
	//setup clock 
	gameClock = 0;
	// setup player
	player.position = (Vector2i){ 0,0 };
	player.tail = NULL;
	player.facing = RIGHT;
	//	setup grid
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
			{
				tab[i][j] = 1;
			}
			else
			{
				tab[i][j] = 0;
			}
		}
	}
	// place player
	MovePlayer(tab, &player, (Vector2i) { 10, 12 });
	// generate a bonus
	CreateBonus(tab);
}