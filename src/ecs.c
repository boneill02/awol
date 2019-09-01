/*
    Project: AWOL
    File: ecs.c
    File Author(s): Benjamin O'Neill <benjaminmoneill@gmail.com>
    License: GNU GPL v2
*/

#include "ecs.h"

typedef struct {
	Entity *entities[MAX_ENTITIES];
} ECS;

ECS *ecs;

void attach_component(Entity *entity, Component *component)
{
	/* TODO error checking */
	for (int i = 0; i < MAX_COMPONENTS; i++) {
		if (entity->components[i] == NULL) {
			entity->components[i] = component;
			return;
		}
	}
}

bool remove_component(Entity *entity, uuid_t uuid)
{
	for (int i = 0; i < MAX_COMPONENTS; i++) {
		if (!(uuid_compare(entity->components[i]->uuid, uuid))) {
			free(entity->components[i]);
			entity->components[i] = NULL;
			return true;
		}
	}
	return false;
}

Component *create_component(int component_type)
{
	Component *component;
	size_t size;
	switch (component_type) {
		case COMPONENT_POSITION:
			size = sizeof(PositionComponent);
			break;
		case COMPONENT_TEXTURE:
			size = sizeof(TextureComponent);
			break;
		default:
			return NULL;
	}

	component = malloc(size);
	memset(component, 0, size);
	uuid_generate(component->uuid);
	component->type = component_type;
	return component;
}

Entity *add_entity(void)
{
	/* TODO protect against too many entities */
	int i;
	for (i = 0; i < MAX_ENTITIES; i++) {
		if (ecs->entities[i] == NULL) {
			ecs->entities[i] = malloc(sizeof(Entity));
			memset(ecs->entities[i], 0, sizeof(Entity));
			uuid_generate(ecs->entities[i]->uuid);

			return ecs->entities[i];
		}
	}
	
	return NULL;
}

bool remove_entity(uuid_t uuid)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (!(uuid_compare(ecs->entities[i]->uuid, uuid))) {
			free(ecs->entities[i]);
			ecs->entities[i] = NULL;
			return true;
		}
	}
	return false;
}

Entity *get_entity(uuid_t uuid)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (!(uuid_compare(ecs->entities[i]->uuid, uuid))) {
			return ecs->entities[i];
		}
	}
	return NULL;
}

Component *get_component_by_type(Entity *entity, int component_type)
{
	for (int i = 0; i < MAX_COMPONENTS; i++)
		if (entity->components[i]->type == component_type)
			return entity->components[i];

	return NULL;
}

Component *get_component_by_uuid(Entity *entity, uuid_t uuid)
{
	for (int i = 0; i < MAX_COMPONENTS; i++) {
		if (!(uuid_compare(entity->components[i]->uuid, uuid))) {
			return entity->components[i];
		}
	}

	return NULL;
}

void init_ecs(void)
{
	ecs = malloc(sizeof(ECS));
	memset(ecs, 0, sizeof(ECS));
}

void quit_ecs(void)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (ecs->entities[i] != NULL) {
			for (int j = 0; j < MAX_COMPONENTS; j++)
				if (ecs->entities[i]->components[j] != NULL)
					free(ecs->entities[i]->components[j]);

			free(ecs->entities[i]);
		}
	}
	free(ecs);
}
