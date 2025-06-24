/**
 * @file game.c
 * @author Ben O'Neill <ben@oneill.sh>
 *
 * Game logic.
 */

#include "game.h"
#include "graphics.h"

#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_image.h>

Game *game;
bool keymap[4];
uuid_t *player_uuid;
SDL_Texture *spritesheet;

static void cleanup(void);
static void init(void);
static void init_map(void);
static void init_player(void);

Game *get_game(void) {
	return game;
}

void start_game(void) {
	/* TODO framerate check */
	init();

	while (game->state) {
		tick(game, keymap);
		render(game);
		SDL_Delay(1000 / FPS);
	}

	cleanup();
}

void update_keys(void) {
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

static void cleanup(void) {
	free(game);
	quit_ecs();
	SDL_Quit();
}

static void init(void) {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	init_ecs();

	game = (Game *) malloc(sizeof(Game));
	game->state = STATE_PLAY;
	game->display.window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 960, 640, SDL_WINDOW_SHOWN);
	game->display.renderer = SDL_CreateRenderer(game->display.window, 0, SDL_RENDERER_ACCELERATED);
	
	spritesheet = load_texture("assets/spritesheet.png");
	init_map();
	init_player();
}

static void init_map(void) {
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

static void init_player(void) {
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
