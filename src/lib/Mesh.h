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
    // Mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;
    unsigned int instanceMatricesBuffer = 0;

    Mesh() {}
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, const glm::mat4* instanceMatricesBuffer = nullptr, const unsigned instanceCount = 1);

    void draw(Shader &shader);
    void drawInstanced(Shader& shader, const unsigned int amount) const;

    void setupMesh(const glm::mat4* instanceMatrices = nullptr, const unsigned int instanceCount = 1);
    void setupInstancedMesh(const glm::mat4* instanceMatrices, const unsigned int instanceCount);
    void refreshMeshData();
private:
    unsigned int VBO, EBO;
};


#endif //GP_3_MESH_H
