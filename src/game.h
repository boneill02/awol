#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "ecs.h"

#define STATE_EXIT 0
#define STATE_MENU 1
#define STATE_PLAY 2

#define TEXTURE_SCALE 8
#define FPS 60
#define MAX_RENDERABLES 256

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
} Display;

typedef struct {
	int state;
	Display display;
} Game;

Game *game;
void add_renderable_to_queue(Entity *renderable);
void start_game(void);

#endif
