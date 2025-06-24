/**
 * @file esc.h
 * @author Ben O'Neill <ben@oneill.sh>
 *
 * Entity component system.
 */

#ifndef AWOL_ECS_H
#define AWOL_ECS_H

#include <stdbool.h>
#include <uuid/uuid.h>

#include <SDL2/SDL.h>

#define MAX_ENTITIES    65536
#define MAX_COMPONENTS  256

typedef enum {
	COMPONENT_POSITION,
	COMPONENT_TEXTURE,
	COMPONENT_DIRECTION,
} ComponentType;

typedef enum {
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
} Direction;

typedef struct {
	ComponentType type;
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
	Direction direction;
	TextureComponent *textures[5];
} DirectionComponent;

typedef struct {
	uuid_t uuid;
	Component *components[MAX_COMPONENTS];
} Entity;

Entity *add_entity(void);
bool attach_component(Entity *, Component *);
Component *create_component(ComponentType);
Component *get_component_by_type(Entity *, ComponentType);
Component *get_component_by_uuid(Entity *, uuid_t);
Entity *get_entity(uuid_t);
void init_ecs(void);
void quit_ecs(void);
bool remove_component(Entity *, uuid_t);
bool remove_entity(uuid_t);

#endif
