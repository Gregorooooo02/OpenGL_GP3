#ifndef GP_3_ENTITY_H
#define GP_3_ENTITY_H

#include "glm/trigonometric.hpp"
#include "glm/gtc//matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "Model.h"
#include "Transform.h"

class Entity {
public:
    Entity();
    Entity(glm::mat4 parentTransform = glm::mat4(1.0f));

    void addChild(Entity* child);
    void recalculate(glm::mat4 parentTransform = glm::mat4(1.0f));

    void reset();
    void simulate();
    void move(glm::vec3 value);
    void rotate(glm::vec3 value);
    void scale(glm::vec3 value);

    glm::mat4 getWorldTransform();

    glm::mat4 localTransform;
    glm::mat4 worldTransform;
private:
    std::vector<Entity*> children;
};


#endif //GP_3_ENTITY_H
