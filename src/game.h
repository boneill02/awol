/*
    AWOL: A 2D top down survival sandbox game
    Copyright (C) 2019 Benjamin O'Neill <benjaminmoneill@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

void add_renderable_to_queue(Entity *renderable);
void start_game(void);
Game *get_game(void);

#endif
