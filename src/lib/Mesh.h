#ifndef GP_3_MESH_H
#define GP_3_MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"
#include "Entity.h"

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

    Mesh() = default;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, std::vector<Entity>* instanceOffsets = nullptr);

    void draw(Shader &shader);
    void updateInstanceMatrices();

    void setupMesh();
    void refreshMeshData();
private:
    std::vector<Entity>* instanceOffsets = nullptr;
    unsigned int VBO, EBO;
    unsigned int instanceVBO;
    int indexCount;
};


#endif //GP_3_MESH_H
