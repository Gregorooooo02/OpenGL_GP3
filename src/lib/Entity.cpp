#include "Entity.h"

Entity::Entity(Model &model, float startScale, int num, bool empty) {
    transform.setLocalScale(glm::vec3(startScale));

    if (empty)  {
        colorBasic = {1, 1, 1, 0};
        lighting = false;
    } else {
        colorBasic = {1, 1, 1, 1};
    }

    color = colorBasic;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorBuffer);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, idNumber * sizeof(glm::vec4), sizeof(glm::vec4), &color);
    idNumber = num;
}

void Entity::addChild(Model &model, float startScale, int num, bool empty) {
    this -> children.emplace_back(std::make_unique<Entity>(model, startScale, num, empty));
    this -> children.back() -> parent = this;
}

void Entity::draw(Shader &shader, int houses) {

}

void Entity::updateSelfAndChild() {
    if (transform.isDirty()) {
        forceUpdateSelfAndChild();
        return;
    }

    for (auto &child : children) {
        child->updateSelfAndChild();
    }
}

void Entity::forceUpdateSelfAndChild() {
    if (parent) {
        transform.computeModelMatrix(parent->transform.getModelMatrix());
    } else {
        transform.computeModelMatrix();
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, modelBuffer);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, idNumber * sizeof(glm::mat4), sizeof(glm::mat4), &transform.getModelMatrix());

    for (auto &child : children) {
        child->forceUpdateSelfAndChild();
    }
}

void Entity::swapColor() {

}

Entity *Entity::getNodeAtIndex(int index) {
    return nullptr;
}
