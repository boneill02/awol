#include "graphics.h"

#include "game.h"

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

SDL_Rect *get_dest_rect(TextureComponent *tex, PositionComponent *pos)
{
	SDL_Rect *r = malloc(sizeof(SDL_Rect));
	r->x = pos->x;
	r->y = pos->y;
	r->w = tex->src.w * TEXTURE_SCALE;
	r->h = tex->src.h * TEXTURE_SCALE;
	return r;
}
