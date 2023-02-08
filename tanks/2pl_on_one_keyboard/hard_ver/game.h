#ifndef __GAME_H__
#define __GAME_H__

// Player object
typedef struct Player
{
	//Coordinates of the tank center
	int x, y;

	//Coordinates of the tank gun
	int x_tower, y_tower;

	//Coordinates of the bullets
	int x_bul[4], y_bul[4];

	//Bullet direction and number of bullets in the arena
	int bul_tower[4], bul_count;

	//Player IP length and player ID(0 - first player, 1 - second player)
	int id;

	//The position of the tank and tank tower at a given time
	char new_tank[3][3], tower;

	//Vertical and horisontal position of the tank
	char hor_tank[3][3], vert_tank[3][3];

	//Bullet status
	bool bul_alive[4];

} Player;

typedef struct Window
{		
	int lines, cols, zero_x, zero_y;
} Window;
// Game object
typedef struct Game
{
	Player player[2];
} Game;

#endif
