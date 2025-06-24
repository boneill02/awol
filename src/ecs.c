/*
    AWOL: A 2D top down survival sandbox game
    Copyright (C) 2019 Ben O'Neill <ben@oneill.sh>
*/

#include "ecs.h"

typedef struct {
	Entity *entities[MAX_ENTITIES];
} ECS;

ECS *ecs;

/* Add a component to the specified entity */
void attach_component(Entity *entity, Component *component)
{
	for (int i = 0; i < MAX_COMPONENTS; i++) {
		if (entity->components[i] == NULL) {
			entity->components[i] = component;
			return;
		}
	}
	printf("Couldn't attach component to entity because it was full.");
}

/* Remove the specified component from its entity and free the memory */
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

/* Create a component of the specified type */
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
		case COMPONENT_DIRECTION:
			size = sizeof(DirectionComponent);
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

/* Create an entity and add it to the ECS. */
Entity *add_entity(void)
{
	int i;
	for (i = 0; i < MAX_ENTITIES; i++) {
		if (ecs->entities[i] == NULL) {
			ecs->entities[i] = malloc(sizeof(Entity));
			memset(ecs->entities[i], 0, sizeof(Entity));
			uuid_generate(ecs->entities[i]->uuid);

			return ecs->entities[i];
		}
	}

	printf("Couldn't create an entity. There are too many already.");

	return NULL;
}

/* Remove the entity from the ECS and free its memory. */
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

/* Get the entity with the specified UUID. */
Entity *get_entity(uuid_t uuid)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (!(uuid_compare(ecs->entities[i]->uuid, uuid))) {
			return ecs->entities[i];
		}
	}
	return NULL;
}

/* Get the first component owned by the specified entity that is of the
   specified type. */
Component *get_component_by_type(Entity *entity, int component_type)
{
	for (int i = 0; i < MAX_COMPONENTS; i++)
		if (entity->components[i] != NULL)
			if (entity->components[i]->type == component_type)
				return entity->components[i];

	return NULL;
}

/* Get the component owned by the specified entity that has the specified UUID. */
Component *get_component_by_uuid(Entity *entity, uuid_t uuid)
{
	for (int i = 0; i < MAX_COMPONENTS; i++) {
		if (!(uuid_compare(entity->components[i]->uuid, uuid))) {
			return entity->components[i];
		}
	}

	return NULL;
}

/* Initialize the ECS. */
void init_ecs(void)
{
	ecs = malloc(sizeof(ECS));
	memset(ecs, 0, sizeof(ECS));
}

/* Deinitialize the ECS. Delete all components and entities. */
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
