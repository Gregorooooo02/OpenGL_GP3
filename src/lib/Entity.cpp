#include "Entity.h"

Entity::Entity() = default;

Entity::Entity(const glm::mat4 &model) : modelMatrix(model) {
    glm::quat rotation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(model, scale, rotation, position, skew, perspective);
    eulerRotation = glm::eulerAngles(rotation);
}

void Entity::update(bool parentDirty) {
    parentDirty |= dirty;

    if (parentDirty) {
        computeModelMatrix();
    }

    for (auto child : children) {
        child->update(parentDirty);
    }
}

void Entity::computeModelMatrix() {
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), eulerRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), eulerRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), eulerRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    const glm::mat4 rotationMatrix = transformX * transformY * transformZ;

    modelMatrix = glm::translate(glm::mat4(1.0f), position) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);

    if (parent != nullptr) {
        modelMatrix = parent->getModelMatrix() * modelMatrix;
    }

    dirty = false;
}

void Entity::computeModelMatrix(const glm::mat4 &parentGlobalMatrix) {
    computeModelMatrix();
    modelMatrix = parentGlobalMatrix * modelMatrix;
}

void Entity::setParent(Entity *parent) {
    this -> parent = parent;
    parent -> addChild(this);
}

void Entity::addChild(Entity *child) {
    children.emplace_back(child);
}

void Entity::setLocalRotation(const glm::vec3 &rotation) {
    eulerRotation = rotation;
    dirty = true;
}

void Entity::setLocalRotationX(float rotation) {
    eulerRotation.x = rotation;
    dirty = true;
}

void Entity::setLocalRotationY(float rotation) {
    eulerRotation.y = rotation;
    dirty = true;
}

void Entity::setLocalRotationZ(float rotation) {
    eulerRotation.z = rotation;
    dirty = true;
}

void Entity::setLocalPosition(const glm::vec3 &position) {
    this -> position = position;
    dirty = true;
}

void Entity::setLocalScale(const glm::vec3 &scale) {
    this -> scale = scale;
    dirty = true;
}

void Entity::setModelMatrix(const glm::mat4 &model) {
    modelMatrix = model;
    glm::quat rotation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(model, scale, rotation, position, skew, perspective);
    eulerRotation = glm::eulerAngles(rotation);
    dirty = true;
}

const glm::vec3 &Entity::getLocalRotation() const {
    return eulerRotation;
}

const glm::vec3 &Entity::getLocalPosition() const {
    return position;
}

const glm::vec3 &Entity::getLocalScale() const {
    return scale;
}

const glm::mat4 &Entity::getModelMatrix() const {
    return modelMatrix;
}

bool Entity::isDirty() const {
    return dirty;
}
