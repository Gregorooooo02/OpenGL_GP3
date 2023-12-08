#include "Object.h"

Object::Object(Model* model, Shader* shader) : model(model), shader(shader) {}

Object::Object(char* modelPath, Shader *shader) : model(new Model(modelPath)), shader(shader) {}

void Object::setModel(Model *model) {
    this -> model = model;
}

void Object::setShader(Shader *shader) {
    this -> shader = shader;
}

void Object::draw() {
    if (model != nullptr) {
        shader -> use();
        shader -> setMat4("model", entity.getModelMatrix());
        model -> draw(*shader);
    }
}

void Object::update() {}
