/*
    AWOL: A 2D top down survival sandbox game
    Copyright (C) 2019 Ben O'Neill <ben@oneill.sh>
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
	COMPONENT_TEXTURE,
	COMPONENT_DIRECTION,
};

enum {
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
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
	Component parent;
	int direction;
	TextureComponent *textures[5];
} DirectionComponent;

typedef struct {
	uuid_t uuid;
	Component *components[MAX_COMPONENTS];
} Entity;

Entity *add_entity(void);
void attach_component(Entity *entity, Component *component);
Component *create_component(int component_type);
Component *get_component_by_type(Entity *entity, int component_type);
Component *get_component_by_uuid(Entity *entity, uuid_t uuid);
Entity *get_entity(uuid_t uuid);
void init_ecs(void);
void quit_ecs(void);
bool remove_component(Entity *entity, uuid_t uuid);
bool remove_entity(uuid_t uuid);

#endif
