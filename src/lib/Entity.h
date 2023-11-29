#ifndef GP_3_ENTITY_H
#define GP_3_ENTITY_H

#include "Model.h"
#include "Transform.h"

#include <list>

class Entity {
public:
    // Scene graph
    std::list<Entity*> children;
    Entity* parent = nullptr;

    // Space information
    Shader *shader = nullptr;
    Model *model = nullptr;
    Transform transform;

    // Constructor
    Entity() {}
    Entity(Entity *parent);

    // Add a child to the scene graph
    void addChild(Entity *child);
    // Draw the entity
    void draw();
    // Update transform if it was changed
    void updateSelfAndChild();
    // Force update transform
    void forceUpdateSelfAndChild();
};


#endif //GP_3_ENTITY_H
