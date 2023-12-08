#include "InstancedObject.h"

void InstancedObject::prepareInstanceMatricesBuffer() {
    glGenBuffers(1, &instanceMatrixBuffer);

    std::vector<glm::mat4> matrices;
    for (const auto& entity : instanceEntities) {
        matrices.push_back(entity -> getModelMatrix());
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
    glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), instanceEntities.data(), GL_DYNAMIC_DRAW);

    for (const auto& mesh : model -> meshes) {
        glBindVertexArray(mesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

void InstancedObject::updateInstanceMatricesBuffer() {
    shader -> setMat4("model", entity.getModelMatrix());

    std::vector<glm::mat4> matrices;
    for (const auto& entity : instanceEntities) {
        matrices.push_back(entity -> getModelMatrix());
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, matrices.size() * sizeof(glm::mat4), matrices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

InstancedObject::InstancedObject() : Object() {
    prepareInstanceMatricesBuffer();
}

InstancedObject::InstancedObject(Model *model, Shader *shader, std::vector<Entity *> entities)
    : Object(model, shader), instanceEntities(std::move(entities)) {
    prepareInstanceMatricesBuffer();
}

void InstancedObject::update() {
    Object::update();
}

void InstancedObject::draw() {
    updateInstanceMatricesBuffer();

    if (model != nullptr) {
        shader -> use();
        model -> drawInstanced(*shader, instanceEntities.size());
    }
}
