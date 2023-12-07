#ifndef GP_3_SKYBOX_H
#define GP_3_SKYBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <string>

class Skybox {
private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    std::vector<std::string> faces;
    static unsigned int loadCubemap(std::vector<std::string> faces);
public:
    Skybox();
    ~Skybox();
    void init();
    void draw() const;
};


#endif //GP_3_SKYBOX_H
