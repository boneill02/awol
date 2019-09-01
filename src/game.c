/*
    Project: AWOL
    File: game.c
    File Author(s): Benjamin O'Neill <benjaminmoneill@gmail.com>
    License: GNU GPL v2
*/

#include "game.h"
#include "graphics.h"

#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_image.h>

bool keymap[4];
Entity *render_queue[MAX_RENDERABLES];
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

	if (keymap[0])
		pos->y--;
	if (keymap[1])
		pos->x--;
	if (keymap[2])
		pos->y++;
	if (keymap[3])
		pos->x++;
}

void init_player(void)
{
	Entity *player = add_entity();
	player_uuid = &(player->uuid);

	TextureComponent *texture_component = (TextureComponent *) create_component(COMPONENT_TEXTURE);
	PositionComponent *position_component = (PositionComponent *) create_component(COMPONENT_POSITION);

	position_component->x = 300;
	position_component->y = 100;

	texture_component->texture = load_texture("assets/player.png");
	texture_component->src.w = 16;
	texture_component->src.h = 16;

	attach_component(player, (Component *) texture_component);
	attach_component(player, (Component *) position_component);
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
			SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	game->display.renderer = SDL_CreateRenderer(game->display.window, 0, SDL_RENDERER_ACCELERATED);

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
	TextureComponent *tex;
	PositionComponent *pos;
	SDL_Rect *dest;

	SDL_SetRenderDrawColor(game->display.renderer, 255, 255, 255, 0);
	SDL_RenderClear(game->display.renderer);

	for (int i = 0; i < render_queue_len; i++) {
		tex = (TextureComponent *) get_component_by_type(render_queue[i], COMPONENT_TEXTURE);
		pos = (PositionComponent *) get_component_by_type(render_queue[i], COMPONENT_POSITION);
		if (tex == NULL) {
			printf("Entity without texture tried to render\n");
		} else if (pos == NULL) {
			printf("Entity without position tried to render\n");
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
