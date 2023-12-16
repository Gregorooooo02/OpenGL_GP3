#ifndef GP_3_TRANSFORM_H
#define GP_3_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/trigonometric.hpp>

#include <vector>

class Transform {
protected:
    // Instanced VBO
    unsigned int m_instanceVBO;

    // Local space information
    glm::vec3 pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 eulerRot = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    // Global space information concatenate in matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Dirty flag
    bool m_isDirty = true;

    // Scene graph
    Transform* parent = nullptr;
    std::vector<Transform*> children;

    glm::mat4 getLocalModelMatrix();
public:
    Transform();
    Transform(const glm::mat4& model);
    void update(bool parentDirty = false);

    void computeModelMatrix();
    void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix);

    void setParent(Transform* parent);
    void addChild(Transform* child);

    void setLocalPosition(const glm::vec3& newPosition);
    void setLocalRotation(const glm::vec3& newRotation);
    void setLocalRotationX(float newRotationX);
    void setLocalRotationY(float newRotationY);
    void setLocalRotationZ(float newRotationZ);
    void setLocalScale(const glm::vec3& newScale);
    void setModelMatrix(const glm::mat4& newModelMatrix);

    const glm::vec3& getLocalPosition() const;
    const glm::vec3& getLocalRotation() const;
    const glm::vec3& getLocalScale() const;
    const glm::mat4& getModelMatrix() const;

    bool isDirty() const;
};


#endif //GP_3_TRANSFORM_H
