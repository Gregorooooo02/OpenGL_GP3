#include "Entity.h"

Entity::Entity() {
    this -> localTransform = glm::mat4(1.0f);
    this -> worldTransform = glm::mat4(1.0f);
}

Entity::Entity(glm::mat4 parentTransform) {
    this -> localTransform = parentTransform;
    this -> worldTransform = parentTransform;
}

void Entity::addChild(Entity *child) {
    children.push_back(child);
    child -> recalculate(worldTransform);
}

void Entity::recalculate(glm::mat4 parentTransform) {
    worldTransform = parentTransform * localTransform;
    for (auto &child : children) {
        child -> recalculate(worldTransform);
    }
}

void Entity::reset() {
    localTransform = glm::mat4(1.0f);
    recalculate();
}

void Entity::simulate() {

}

void Entity::move(glm::vec3 value) {
    localTransform = glm::translate(localTransform, value);
    recalculate();
}

void Entity::rotate(glm::vec3 value) {
    localTransform = glm::rotate(localTransform, glm::radians(value.x), glm::vec3(1.0f, 0.0f, 0.0f));
    localTransform = glm::rotate(localTransform, glm::radians(value.y), glm::vec3(0.0f, 1.0f, 0.0f));
    localTransform = glm::rotate(localTransform, glm::radians(value.z), glm::vec3(0.0f, 0.0f, 1.0f));
    recalculate();
}

void Entity::scale(glm::vec3 value) {
    localTransform = glm::scale(localTransform, value);
    recalculate();
}

glm::mat4 Entity::getWorldTransform() {
    return worldTransform;
}
