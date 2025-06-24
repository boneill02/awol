/*
    AWOL: A 2D top down survival sandbox game
    Copyright (C) 2019 Ben O'Neill <ben@oneill.sh>
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ecs.h"

SDL_Texture *load_texture(const char *path);
SDL_Rect *get_dest_rect(TextureComponent *tex, PositionComponent *pos);
TextureComponent *create_texture_component(SDL_Texture *sheet, int x, int y);

#endif
