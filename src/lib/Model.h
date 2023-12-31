#ifndef GP_3_MODEL_H
#define GP_3_MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "Transform.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

class Model {
public:
    // Model data
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model(std::string const &path, bool gamma = false, const glm::mat4* instanceMatrices = nullptr, const unsigned int instanceCount = 1);
    void draw(Shader &shader);
    void drawInstanced(Shader &shader, const unsigned int instanceCount);
private:
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int textureFromFile(const char *path, const std::string &directory, bool gamma = false);
};

#endif //GP_3_MODEL_H