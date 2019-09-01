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

#ifndef AWOL_ECS_H
#define AWOL_ECS_H

#include <stdbool.h>
#include <uuid/uuid.h>

#include <SDL2/SDL.h>

#define MAX_ENTITIES    65536
#define MAX_COMPONENTS  256

enum {
	COMPONENT_POSITION,
	COMPONENT_TEXTURE
};

typedef struct {
	int type;
	uuid_t uuid;
} Component;

typedef struct {
	Component parent;
	long x, y;
} PositionComponent;

typedef struct {
	Component parent;
	SDL_Texture *texture;
	SDL_Rect src;
} TextureComponent;

typedef struct {
	uuid_t uuid;
	Component *components[MAX_COMPONENTS];
} Entity;

void        attach_component(Entity *entity, Component *component);
bool        remove_component(Entity *entity, uuid_t uuid);
Component   *create_component(int component_type);
Entity      *add_entity(void);
bool        remove_entity(uuid_t uuid);
Entity      *get_entity(uuid_t uuid);
Component   *get_component_by_type(Entity *entity, int component_type);
Component   *get_component_by_uuid(Entity *entity, uuid_t uuid);
void        init_ecs(void);
void        quit_ecs(void);

#endif
