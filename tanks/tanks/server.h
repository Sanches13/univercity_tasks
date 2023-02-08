#ifndef __SERVER_H__
#define __SERVER_H__

#include "game.h"

// Define in server header that we use extern object
extern int work_flag;
extern int win;

// Define extern game object
extern Game game;
extern void key_check(int ch, int id);

// Function prototype
void *udp_server(void *data);

#endif
