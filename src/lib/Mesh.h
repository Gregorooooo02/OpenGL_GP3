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

    Mesh() {}
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(Shader &shader);
    void setupMesh();
    void refreshMeshData();
private:
    unsigned int VBO, EBO;
};


#endif //GP_3_MESH_H
