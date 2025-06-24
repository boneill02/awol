/**
 * @file graphics.h
 * @author Ben O'Neill <ben@oneill.sh>
 *
 * Graphics-related code.
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ecs.h"
#include "game.h"

void add_renderable_to_queue(Entity *);
TextureComponent *create_texture_component(SDL_Texture *, int, int);
void get_dest_rect(SDL_Rect *, TextureComponent *, PositionComponent *);
SDL_Texture *load_texture(const char *);
void tick(Game *, bool *);
void render(Game *);

#endif
