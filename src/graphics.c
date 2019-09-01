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

#include "graphics.h"

#include "game.h"

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

	res = SDL_CreateTextureFromSurface(game->display.renderer, temp_surf);

	if (res == NULL) {
		printf("Warning: Couldn't create texture from surface\n");
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
