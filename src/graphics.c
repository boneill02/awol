/*
    AWOL: A 2D top down survival sandbox game
    Copyright (C) 2019 Ben O'Neill <ben@oneill.sh>
*/

#include "graphics.h"

#ifndef GAME_H
#include "game.h"
#endif

/* Load an image file as a texture. */
SDL_Texture *load_texture(const char *path)
{
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

/* Create a TextureComponent using a spritesheet. */
TextureComponent *create_texture_component(SDL_Texture *sheet, int x, int y)
{
	TextureComponent *res = (TextureComponent *) create_component(COMPONENT_TEXTURE);

	res->texture = sheet;
	res->src.x = x * TEXTURE_SIZE;
	res->src.y = y * TEXTURE_SIZE;
	res->src.w = TEXTURE_SIZE;
	res->src.h = TEXTURE_SIZE;

	return res;
}

/* Generate an output SDL_Rect for rendering using given Components. */
SDL_Rect *get_dest_rect(TextureComponent *tex, PositionComponent *pos)
{
	SDL_Rect *r = malloc(sizeof(SDL_Rect));
	r->x = pos->x;
	r->y = pos->y;
	r->w = tex->src.w * TEXTURE_SCALE;
	r->h = tex->src.h * TEXTURE_SCALE;
	return r;
}
