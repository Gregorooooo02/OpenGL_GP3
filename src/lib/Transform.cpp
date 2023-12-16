#include "Transform.h"

glm::mat4 Transform::getLocalModelMatrix() {
    return glm::mat4();
}

Transform::Transform() = default;

Transform::Transform(const glm::mat4 &model) : modelMatrix(model) {
    glm::quat rot;
    glm::vec4 perspective;
    glm::vec3 skew;
    glm::decompose(model, scale, rot, pos, skew, perspective);
    eulerRot = glm::eulerAngles(rot);
}

void Transform::update(bool parentDirty) {
    parentDirty |= m_isDirty;
    if (parentDirty) {
        computeModelMatrix();
    }

    for (auto child : children) {
        child->update(parentDirty);
    }
}

void Transform::computeModelMatrix() {
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
		glm::radians(eulerRot.x),
		glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
		glm::radians(eulerRot.y),
		glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
		glm::radians(eulerRot.z),
		glm::vec3(0.0f, 0.0f, 1.0f));

	const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    modelMatrix = glm::translate(glm::mat4(1.0f), pos) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);

	if(parent != nullptr)
        modelMatrix = parent -> getModelMatrix() * modelMatrix;

	m_isDirty = false;
}

void Transform::computeModelMatrix(const glm::mat4 &parentGlobalModelMatrix) {
    computeModelMatrix();
    modelMatrix = parentGlobalModelMatrix * modelMatrix;
}

void Transform::setParent(Transform *parent) {
    this -> parent = parent;
    parent -> addChild(this);
}

void Transform::addChild(Transform *child) {
    children.emplace_back(child);
}

void Transform::setLocalPosition(const glm::vec3 &newPosition) {
    pos = newPosition;
    m_isDirty = true;
}

void Transform::setLocalRotation(const glm::vec3 &newRotation) {
    eulerRot = newRotation;
    m_isDirty = true;
}

void Transform::setLocalRotationX(float newRotationX) {
    eulerRot.x = newRotationX;
    m_isDirty = true;
}

void Transform::setLocalRotationY(float newRotationY) {
    eulerRot.y = newRotationY;
    m_isDirty = true;
}

void Transform::setLocalRotationZ(float newRotationZ) {
    eulerRot.z = newRotationZ;
    m_isDirty = true;
}

void Transform::setLocalScale(const glm::vec3 &newScale) {
    scale = newScale;
    m_isDirty = true;
}

void Transform::setModelMatrix(const glm::mat4 &newModelMatrix) {
    modelMatrix = newModelMatrix;
    glm::quat rot;
    glm::vec4 perspective;
    glm::vec3 skew;
    glm::decompose(modelMatrix, scale, rot, pos, skew, perspective);
    eulerRot = glm::eulerAngles(rot);
    m_isDirty = true;
}

const glm::vec3 &Transform::getLocalPosition() const {
    return pos;
}

const glm::vec3 &Transform::getLocalRotation() const {
    return eulerRot;
}

const glm::vec3 &Transform::getLocalScale() const {
    return scale;
}

const glm::mat4 &Transform::getModelMatrix() const {
    return modelMatrix;
}

bool Transform::isDirty() const {
    return m_isDirty;
}
