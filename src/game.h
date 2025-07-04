/**
 * @file game.h
 * @author Ben O'Neill <ben@oneill.sh>
 *
 * Game logic.
 */

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "ecs.h"

#define STATE_EXIT 0
#define STATE_MENU 1
#define STATE_PLAY 2

#ifndef TEXTURE_SIZE
#define TEXTURE_SIZE  16
#endif

#ifndef TEXTURE_SCALE
#define TEXTURE_SCALE 4
#endif

#ifndef FPS
#define FPS 60
#endif

#ifndef MAX_RENDERABLES
#define MAX_RENDERABLES 2048
#endif
#define MAP_WIDTH 32
#define MAP_HEIGHT 32
#define PLAYER_SPEED 4

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
} Display;

typedef struct {
	int state;
	Display display;
} Game;

Game *get_game(void);
void start_game(void);
void update_keys(void);

#endif
