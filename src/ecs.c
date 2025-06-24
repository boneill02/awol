/**
 * @file esc.c
 * @author Ben O'Neill <ben@oneill.sh>
 *
 * Entity component system.
 */

#include "ecs.h"

typedef struct {
	Entity *entities[MAX_ENTITIES];
} ECS;

ECS *ecs;

/**
 * @brief Create an entity and add it to the ECS.
 *
 * @return malloc'd entity
 */
Entity *add_entity(void) {
	int i;
	for (i = 0; i < MAX_ENTITIES; i++) {
		if (ecs->entities[i] == NULL) {
			ecs->entities[i] = (Entity *) malloc(sizeof(Entity));
			memset(ecs->entities[i], 0, sizeof(Entity));
			uuid_generate(ecs->entities[i]->uuid);

			return ecs->entities[i];
		}
	}

	fprintf(stderr, "Couldn't create an entity. There are too many already.");

	return NULL;
}

/**
 * @brief Add a component to the specified entity
 *
 * @param entity entity to add component to
 * @param component component to add entity to
 *
 * @return true if component was attached, false if entity is full
 */
bool attach_component(Entity *entity, Component *component) {
	for (int i = 0; i < MAX_COMPONENTS; i++) {
		if (entity->components[i] == NULL) {
			entity->components[i] = component;
			return true;
		}
	}
	fprintf(stderr, "Couldn't attach component to entity because it was full.");
	return false;
}

/**
 * @brief Create a component of the specified type
 *
 * @param type component type
 *
 * @return malloc'd component
 */
Component *create_component(ComponentType type) {
	Component *component;
	size_t size;
	switch (type) {
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

	component = (Component *) malloc(size);
	memset(component, 0, size);
	uuid_generate(component->uuid);
	component->type = type;
	return component;
}

/**
 * @brief Get the first component owned by the specified entity that is of the
 * specified type.
 * @param entity entity to search
 * @param c component type
 * @return First component to match type or NULL
 */
Component *get_component_by_type(Entity *entity, ComponentType c) {
	for (int i = 0; i < MAX_COMPONENTS; i++) {
		if (entity->components[i] != NULL) {
			if (entity->components[i]->type == c) {
				return entity->components[i];
			}
		}
	}

	return NULL;
}

/**
 * @brief Get component by UUID
 * @param entity entity to search
 * @param uuid uuid of Component
 * @return Component or NULL if not found
 */
Component *get_component_by_uuid(Entity *entity, uuid_t uuid) {
	for (int i = 0; i < MAX_COMPONENTS; i++) {
		if (!(uuid_compare(entity->components[i]->uuid, uuid))) {
			return entity->components[i];
		}
	}

	return NULL;
}

/**
 * @brief Get the entity with the specified UUID.
 * @param uuid uuid of the entity to get
 * @return pointer to Entity
 */
Entity *get_entity(uuid_t uuid) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (!(uuid_compare(ecs->entities[i]->uuid, uuid))) {
			return ecs->entities[i];
		}
	}
	return NULL;
}

/**
 * @brief Initialize the ECS.
 */
void init_ecs(void) {
	ecs = (ECS *) malloc(sizeof(ECS));
	memset(ecs, 0, sizeof(ECS));
}

/**
 * @brief Deinitialize the ECS.
 */
void quit_ecs(void) {
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
/**
 * @brief Remove the specified component from its parent entity
 *
 * @param entity Entity to remove component from
 * @param uuid uuid of component
 *
 * @return true if component was removed, false if component was not attached
 */
bool remove_component(Entity *entity, uuid_t uuid) {
	for (int i = 0; i < MAX_COMPONENTS; i++) {
		if (!(uuid_compare(entity->components[i]->uuid, uuid))) {
			free(entity->components[i]);
			entity->components[i] = NULL;
			return true;
		}
	}
	return false;
}

/**
 * @brief Remove the entity from the ECS
 *
 * @param uuid uuid of the entity to remove
 * @return true if success, false otherwise
 */
bool remove_entity(uuid_t uuid) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (!(uuid_compare(ecs->entities[i]->uuid, uuid))) {
			free(ecs->entities[i]);
			ecs->entities[i] = NULL;
			return true;
		}
	}
	return false;
}

