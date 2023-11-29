#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "lib/ImGuiInit.h"
#include "lib/Shader.h"

#include <iostream>

const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1000;

float skyboxVertices[] =
{
        -1.0f, -1.0f,  1.0f,    // 0
         1.0f, -1.0f,  1.0f,    // 1
         1.0f, -1.0f, -1.0f,    // 2
        -1.0f, -1.0f, -1.0f,    // 3
        -1.0f,  1.0f,  1.0f,    // 4
         1.0f,  1.0f,  1.0f,    // 5
         1.0f,  1.0f, -1.0f,    // 6
        -1.0f,  1.0f, -1.0f     // 7
};

unsigned int skyboxIndices[] =
{
        0, 1, 2, // Bottom
        0, 2, 3,
        4, 5, 6, // Top
        4, 6, 7,
        0, 1, 5, // Front
        0, 5, 4,
        2, 3, 7, // Back
        2, 7, 6,
        1, 2, 6, // Right
        1, 6, 5,
        0, 3, 7, // Left
        0, 7, 4
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set OpenGL profile to core
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For macOS
#endif
    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GP_3", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // Terminate GLFW
        return -1; // Terminate program
    }
    glfwMakeContextCurrent(window); // Make window current context
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // Set callback function for window resizing
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // Initialize GLAD
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1; // Terminate program
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Set viewport size (in pixels)
    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // Shaders
    Shader shaderProgram();

    // Initialize ImGui
    init_imgui(window);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Rendering commands here
        glClearColor(0.5f, 1.0f, 1.0, 1.0f); // Set clear color to blue
        glClear(GL_COLOR_BUFFER_BIT); // Clear color buffer

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    // Close window when escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}