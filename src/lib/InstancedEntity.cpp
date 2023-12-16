#include "InstancedEntity.h"

void InstancedEntity::prepareInstanceMatrixBuffer() {
	glGenBuffers(1, &instanceMatrixBuffer);

	std::vector<glm::mat4> instanceMatrices;
	for (const auto& transform : instanceTransforms)
		instanceMatrices.emplace_back(transform -> getModelMatrix());

	glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int>(instanceMatrices.size()) * sizeof(glm::mat4), instanceMatrices.data(), GL_DYNAMIC_DRAW);


	for (const auto& mesh : model -> meshes)
	{
		constexpr std::size_t vec4Size = sizeof(glm::vec4);

		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);

		// vertex attributes
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);

	}
}

void InstancedEntity::updateInstanceMatrixBuffer() {
	shader -> setMat4("mainObjectModel", transform.getModelMatrix());

	std::vector<glm::mat4> instanceMatrices;
	for (const auto& transform : instanceTransforms)
		instanceMatrices.emplace_back(transform -> getModelMatrix());

	glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<int>(instanceMatrices.size()) * sizeof(glm::mat4), instanceMatrices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

InstancedEntity::InstancedEntity() {
    prepareInstanceMatrixBuffer();
}

InstancedEntity::InstancedEntity(Model *model, Shader *shader, std::vector<Transform *> transforms) :
    Entity(model, shader), instanceTransforms(std::move(transforms)) {
    prepareInstanceMatrixBuffer();
}

void InstancedEntity::draw() {
    updateInstanceMatrixBuffer();

    if (model != nullptr) {
        shader -> use();
        model -> drawInstanced(*shader, instanceTransforms.size());
    }
}

void InstancedEntity::update() {
    Entity::update();
}
