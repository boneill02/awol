#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ecs.h"

SDL_Texture *load_texture(const char *path);
SDL_Rect    *get_dest_rect(TextureComponent *tex, PositionComponent *pos);

#endif
