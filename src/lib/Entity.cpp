#include "Entity.h"

Entity::Entity(const std::string &modelPath, Shader *shader) : model(new Model(modelPath)), shader(shader) {}

Entity::Entity(Model *loadedModel, Shader *shader) : model(loadedModel), shader(shader) {}

Entity::Entity() {}

void Entity::setModel(Model *newModel) {
    model = newModel;
}

void Entity::setShader(Shader *newShader) {
    shader = newShader;
}

void Entity::draw() {
    if (model != nullptr) {
        shader -> use();
        shader -> setMat4("model", transform.getModelMatrix());
        model -> draw(*shader);
    }
}

void Entity::update() {}
