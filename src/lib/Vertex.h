#ifndef GP_3_VERTEX_H
#define GP_3_VERTEX_H

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

#endif //GP_3_VERTEX_H
