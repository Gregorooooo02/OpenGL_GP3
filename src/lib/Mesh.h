#ifndef GP_3_MESH_H
#define GP_3_MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"

#include <string>
#include <vector>

class Mesh {
public:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int VAO;
    unsigned int instanceMatricesBuffer = 0;

    // Constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, const glm::mat4* instanceMatrices = nullptr, unsigned int instanceCount = 1);
    // Render the mesh
    void draw(Shader& shader) const;
    // Render the mesh with instancing
    void drawInstanced(Shader& shader, unsigned int instanceCount) const;
private:
    // Render data
    unsigned int VBO, EBO;
    // Initializes all the buffer objects/arrays
    void setupMesh(const glm::mat4* instanceMatrices = nullptr, unsigned int instanceCount = 1);
    void setupInstanceMesh(const glm::mat4* instanceMatrices, unsigned int instanceCount);
};


#endif //GP_3_MESH_H