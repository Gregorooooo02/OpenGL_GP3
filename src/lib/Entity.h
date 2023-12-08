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
    Entity(const glm::mat4& model);

    void update(bool parentDirty = false);
    void computeModelMatrix();
    void computeModelMatrix(const glm::mat4& parentGlobalMatrix);

    void setParent(Entity* parent);
    void addChild(Entity* child);

    void setLocalRotation(const glm::vec3& rotation);
    void setLocalRotationX(float rotation);
    void setLocalRotationY(float rotation);
    void setLocalRotationZ(float rotation);
    void setLocalPosition(const glm::vec3& position);
    void setLocalScale(const glm::vec3& scale);
    void setModelMatrix(const glm::mat4& model);

    [[nodiscard]] const glm::vec3& getLocalRotation() const;
    [[nodiscard]] const glm::vec3& getLocalPosition() const;
    [[nodiscard]] const glm::vec3& getLocalScale() const;
    [[nodiscard]] const glm::mat4& getModelMatrix() const;

    [[nodiscard]] bool isDirty() const;
private:
    unsigned int instanceVBO = 0;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 eulerRotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    bool dirty = true;
    Entity *parent = nullptr;
    std::vector<Entity*> children;
};


#endif //GP_3_ENTITY_H
