#include "Entity.h"

Entity::Entity(Entity *parent) {
    parent->addChild(this);
}

void Entity::addChild(Entity *child) {
    children.emplace_back(child);
    children.back()->parent = this;
}

void Entity::draw() {
    if (model != nullptr) {
        model->draw(*shader, transform);
    }
}

void Entity::updateSelfAndChild() {
    if (transform.isDirty()) {
        forceUpdateSelfAndChild();
        return;
    }

    for (auto&& child : children) {
        child->updateSelfAndChild();
    }
}

void Entity::forceUpdateSelfAndChild() {
    if (parent) {
        transform.computeModelMatrix(parent->transform.getModelMatrix());
    } else {
        transform.computeModelMatrix();
    }

    for (auto&& child : children) {
        child->forceUpdateSelfAndChild();
    }
}