/**
 * @file graphics.c
 * @author Ben O'Neill <ben@oneill.sh>
 *
 * Graphics code.
 */

#include "graphics.h"
#include "game.h"

Entity *renderQueue[MAX_RENDERABLES];
int renderQueueLen = 0;

/**
 * @brief Add a renderable Entity to the render queue.
 */
void add_renderable_to_queue(Entity *renderable) {
	renderQueue[renderQueueLen] = renderable;
	renderQueueLen++;
}

/**
 * @brief Create a TextureComponent using a spritesheet.
 */
TextureComponent *create_texture_component(SDL_Texture *sheet, int x, int y) {
	TextureComponent *res = (TextureComponent *) create_component(COMPONENT_TEXTURE);

	res->texture = sheet;
	res->src.x = x * TEXTURE_SIZE;
	res->src.y = y * TEXTURE_SIZE;
	res->src.w = TEXTURE_SIZE;
	res->src.h = TEXTURE_SIZE;

	return res;
}

/**
 * @brief Load an image file as a texture.
 */
SDL_Texture *load_texture(const char *path) {
	SDL_Surface *temp_surf;
	SDL_Texture *res;

	temp_surf = IMG_Load(path);

	if (temp_surf == NULL) {
		printf("Warning: Couldn't load surface\n");
		return NULL;
	}

	res = SDL_CreateTextureFromSurface(get_game()->display.renderer, temp_surf);

	if (res == NULL) {
		printf("Warning: Couldn't create texture from surface\n");
		return NULL;
	}

	SDL_FreeSurface(temp_surf);
	return res;
}

/**
 * Generate an output SDL_Rect for rendering using given Components.
 */
void get_dest_rect(SDL_Rect *r, TextureComponent *tex, PositionComponent *pos) {
	r->x = pos->x;
	r->y = pos->y;
	r->w = tex->src.w * TEXTURE_SCALE;
	r->h = tex->src.h * TEXTURE_SCALE;
}

void tick(Game *game, bool *keymap) {
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

void render(Game *game) {
	DirectionComponent *dir;
	TextureComponent *tex;
	PositionComponent *pos;
	SDL_Rect dest;

	SDL_SetRenderDrawColor(game->display.renderer, 255, 255, 255, 0);
	SDL_RenderClear(game->display.renderer);
	
	for (int i = 0; i < renderQueueLen; i++) {
		tex = (TextureComponent *) get_component_by_type(renderQueue[i], COMPONENT_TEXTURE);
		pos = (PositionComponent *) get_component_by_type(renderQueue[i], COMPONENT_POSITION);

		if (tex == NULL) {
			/* If it has no default texture, must be direction based */
			dir = (DirectionComponent *) get_component_by_type(renderQueue[i], COMPONENT_DIRECTION);
			if (dir == NULL) {
				fprintf(stderr, "Entity without texture tried to render\n");
				continue;
			} else {
				tex = dir->textures[dir->direction];
				if (tex == NULL) {
					fprintf(stderr, "Entity without texture tried to render\n");
					continue;
				}
			}
		}

		if (pos == NULL) {
			fprintf(stderr, "Entity without position tried to render\n");
			continue;
		} else {
			get_dest_rect(&dest, tex, pos);
			SDL_RenderCopy(game->display.renderer, tex->texture, &(tex->src), &dest);
		}
	}

	SDL_RenderPresent(game->display.renderer);
}
