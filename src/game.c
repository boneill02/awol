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

#include "game.h"
#include "graphics.h"

#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_image.h>

bool keymap[4];
Entity *render_queue[MAX_RENDERABLES];
SDL_Texture *spritesheet;
uuid_t *player_uuid;
int render_queue_len = 0;

void add_renderable_to_queue(Entity *renderable)
{
	render_queue[render_queue_len] = renderable;
	render_queue_len++;
}

void update_keys(void)
{
	Entity *player = get_entity(*player_uuid);
	PositionComponent *pos = (PositionComponent *) get_component_by_type(player, COMPONENT_POSITION);
	DirectionComponent *dir = (DirectionComponent *) get_component_by_type(player, COMPONENT_DIRECTION);

	if (keymap[0]) {
		pos->y -= PLAYER_SPEED;
		dir->direction = DIRECTION_UP;
	}
	if (keymap[2]) {
		dir->direction = DIRECTION_DOWN;
		pos->y += PLAYER_SPEED;
	}
	if (keymap[1]) {
		dir->direction = DIRECTION_LEFT;
		pos->x -= PLAYER_SPEED;
	}
	if (keymap[3]) {
		dir->direction = DIRECTION_RIGHT;
		pos->x += PLAYER_SPEED;
	}
}

void init_map(void)
{
	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {
			Entity *tile = add_entity();
			TextureComponent *tex;
			if (rand() % 2 == 1) {
				tex = create_texture_component(spritesheet, 4, 1);
			} else {
				tex = create_texture_component(spritesheet, 7, 1);
			}
			PositionComponent *pos = (PositionComponent *) create_component(COMPONENT_POSITION);
			pos->x = x * TEXTURE_SIZE * TEXTURE_SCALE;
			pos->y = y * TEXTURE_SIZE * TEXTURE_SCALE;
			attach_component(tile, (Component *) tex);
			attach_component(tile, (Component *) pos);
			add_renderable_to_queue(tile);
		}
	}
}

void init_player(void)
{
	Entity *player = add_entity();
	player_uuid = &(player->uuid);

	DirectionComponent *direction = (DirectionComponent *) create_component(COMPONENT_DIRECTION);
	PositionComponent *position = (PositionComponent *) create_component(COMPONENT_POSITION);

	position->x = 300;
	position->y = 100;

	direction->direction = DIRECTION_DOWN;

	direction->textures[DIRECTION_UP] = (TextureComponent *) create_texture_component(spritesheet, 0, 1);
	direction->textures[DIRECTION_DOWN] = (TextureComponent *) create_texture_component(spritesheet, 0, 2);
	direction->textures[DIRECTION_RIGHT] = (TextureComponent *) create_texture_component(spritesheet, 0, 3);
	direction->textures[DIRECTION_LEFT] = (TextureComponent *) create_texture_component(spritesheet, 0, 0);

	attach_component(player, (Component *) direction);
	attach_component(player, (Component *) position);
	add_renderable_to_queue(player);
}

void init(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	init_ecs();

	game = malloc(sizeof(Game));
	game->state = STATE_PLAY;
	game->display.window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 960, 640, SDL_WINDOW_SHOWN);
	game->display.renderer = SDL_CreateRenderer(game->display.window, 0, SDL_RENDERER_ACCELERATED);
	
	spritesheet = load_texture("assets/spritesheet.png");
	init_map();
	init_player();
}

void tick(void)
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT: game->state = STATE_EXIT; break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_w)
					keymap[0] = true;
				else if (e.key.keysym.sym == SDLK_a)
					keymap[1] = true;
				else if (e.key.keysym.sym == SDLK_s)
					keymap[2] = true;
				else if (e.key.keysym.sym == SDLK_d)
					keymap[3] = true;
				break;
			case SDL_KEYUP:
				if (e.key.keysym.sym == SDLK_w)
					keymap[0] = false;
				else if (e.key.keysym.sym == SDLK_a)
					keymap[1] = false;
				else if (e.key.keysym.sym == SDLK_s)
					keymap[2] = false;
				else if (e.key.keysym.sym == SDLK_d)
					keymap[3] = false;
				break;
			default: break;
		}
	}
	update_keys();
}

void render(void)
{
	DirectionComponent *dir;
	TextureComponent *tex;
	PositionComponent *pos;
	SDL_Rect *dest;

	SDL_SetRenderDrawColor(game->display.renderer, 255, 255, 255, 0);
	SDL_RenderClear(game->display.renderer);
	
	for (int i = 0; i < render_queue_len; i++) {
		tex = (TextureComponent *) get_component_by_type(render_queue[i], COMPONENT_TEXTURE);
		pos = (PositionComponent *) get_component_by_type(render_queue[i], COMPONENT_POSITION);

		if (tex == NULL) {
			/* If it has no default texture, must be direction based */
			dir = (DirectionComponent *) get_component_by_type(render_queue[i], COMPONENT_DIRECTION);
			if (dir == NULL) {
				printf("Entity without texture tried to render\n");
				continue;
			} else {
				tex = dir->textures[dir->direction];
				if (tex == NULL) {
					printf("Entity without texture tried to render\n");
					continue;
				}
			}
		}

		if (pos == NULL) {
			printf("Entity without position tried to render\n");
			continue;
		} else {
			dest = get_dest_rect(tex, pos);
			SDL_RenderCopy(game->display.renderer, tex->texture, &(tex->src), dest);
			free(dest);
		}
	}

	SDL_RenderPresent(game->display.renderer);
}

void cleanup(void)
{
	free(game);
	quit_ecs();
	SDL_Quit();
}

void start_game(void)
{
	/* TODO framerate check */
	init();

	while (game->state) {
		tick();
		render();
		SDL_Delay(1000 / FPS);
	}

	cleanup();
}
