#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "stb_image.h"

#include "lib/ImGuiInit.h"
#include "lib/Shader.h"
#include "lib/Camera.h"
#include "lib/Model.h"
#include "lib/Entity.h"
#include "lib/Skybox.h"
#include "lib/Object.h"
#include "lib/InstancedObject.h"

#include <iostream>
#include <vector>

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(0.0f, 5.0f, 0.0f));
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
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
    glfwSetCursorPosCallback(window, mouseCallback); // Set callback function for mouse movement
    glfwSetScrollCallback(window, scrollCallback); // Set callback function for mouse scroll

    // Tell GLFW to capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // Initialize GLAD
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1; // Terminate program
    }

    // Shader setup
    Shader skyboxShader("res/shaders/skybox.vert", "res/shaders/skybox.frag");
    Shader lightShader("res/shaders/light.vert", "res/shaders/light.frag");
    Shader basicShader("res/shaders/basic.vert", "res/shaders/basic.frag");

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glEnable(GL_CULL_FACE); // Enable face culling
    glCullFace(GL_BACK); // Cull back faces

    // Skybox setup
    Skybox skybox;
    skybox.init();

    // Set skybox shader uniforms
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // Initialize ImGui
    init_imgui(window);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Per-frame time logic
        auto currentFrame = static_cast<float>(glfwGetTime()); // Get current frame time
        deltaTime = currentFrame - lastFrame; // Calculate time difference between current and last frame
        lastFrame = currentFrame; // Set last frame time to current frame time

        // Process GLFW events
        glfwPollEvents();
        // Input
        processInput(window);

        // Rendering commands here
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set clear color to blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color buffer

        // Draw skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        VP = glm::mat4(glm::mat3(camera.getViewMatrix()));
        skyboxShader.setMat4("view", VP);
        skyboxShader.setMat4("projection", projection);
        skybox.draw();

        // Swap buffers and poll events
        glfwMakeContextCurrent(window);

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }
    // Terminate GLFW
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    // Check if this is the first time the mouse is moved
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    // Calculate mouse offset
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement((float)xoffset, (float)yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window) {
    // Close window when escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.processKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.processKeyboard(DOWN, deltaTime);
    }

    // Camera speed modifiers
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.movementSpeed = 10.0f;
    } else {
        camera.movementSpeed = 5.0f;
    }
}