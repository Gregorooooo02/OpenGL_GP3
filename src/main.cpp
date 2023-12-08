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

Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
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
    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // Shader setup
    Shader skyboxShader("res/shaders/skybox.vert", "res/shaders/skybox.frag");
    Shader lightShader("res/shaders/light.vert", "res/shaders/light.frag");
    Shader basicShader("res/shaders/basic.vert", "res/shaders/basic.frag");
    Shader textureShader("res/shaders/texture.vert", "res/shaders/light.frag");

    float shininess = 2.0f;
	bool isBlinn = false;
	float blinnExponent = 32.0f;

	// Directional light properties
	bool isDirLight = true;
	glm::vec3 direction(0, 0, 0);
	glm::vec3 ambient(.19f);
	glm::vec3 diffuse(0);
	glm::vec3 specular(0);

	//Point light properties
	bool isPointLight = false;
	glm::vec3 pointLightPosition(0.0f);
	glm::vec3 pointLightAmbient(1.0f);
	glm::vec3 pointLightDiffuse(1.0f);
	glm::vec3 pointLightSpecular(1.0f);
	float pointLightConstant = 1.0f;
	float pointLightLinear = .7f;
	float pointLightQuadratic = 1.8f;

	//Spotlight properties
	bool isSpotActive = false;
	glm::vec3 spotLightPosition(0.0f);
	glm::vec3 spotLightDirection(0.0f);
	glm::vec3 spotLightAmbient(1.0f);
	glm::vec3 spotLightDiffuse(1.0f);
	glm::vec3 spotLightSpecular(1.0f);
	float spotLightConstant = 1.0f;
	float spotLightLinear = .7f;
	float spotLightQuadratic = 1.8f;
	float spotLightCutOff = 12.5f;
	float spotLightOuterCutOff = 17.5f;

	bool isSpot1Active = false;
	glm::vec3 spotLight1Position(0.0f);
	glm::vec3 spotLight1Direction(0.0f);
	glm::vec3 spotLight1Ambient(1.0f);
	glm::vec3 spotLight1Diffuse(1.0f);
	glm::vec3 spotLight1Specular(1.0f);
	float spotLight1Constant = 1.0f;
	float spotLight1Linear = .7f;
	float spotLight1Quadratic = 1.8f;
	float spotLight1CutOff = 12.5f;
	float spotLight1OuterCutOff = 17.5f;

    // Model setup (grass)
    auto grassModel = new Model("res/models/grass/grass.obj");

    // Model setup (house);
    auto houseBodyModel = new Model("res/models/house_body/house_body.obj");
    auto houseRoofModel = new Model("res/models/house_roof/house_roof.obj");

    // Skybox setup
    Skybox skybox;
    skybox.init();

    // Set skybox shader uniforms
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // Instance rendering of houses
    auto neighbourhood = new Object(grassModel, &textureShader);
    auto neighEntity = &neighbourhood -> entity;

    std::vector<Entity*> houses;
    std::vector<Entity*> roofs;

    int rows = 200, cols = 200;
    int amount = rows * cols;

    glm::mat4 temp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -5.0f));
    for (auto i = 0; i < cols; i++) {
        glm::mat4 model(1.0f);
        for (auto j = 0; j < rows; j++) {
            auto houseEntity = new Entity();
            auto roofEntity = new Entity();

            temp = glm::translate(temp, glm::vec3(3.0f, 0.0f, 0.0f));
            model = glm::translate(temp, glm::vec3(0.0f, 1.5f, 0.0f));

            houseEntity -> setModelMatrix(model);
            houseEntity -> setParent(neighEntity);
            houses.emplace_back(houseEntity);

            roofEntity -> setLocalPosition(glm::vec3(0.0f, 1.5f, 0.0f));
            roofEntity -> setParent(houses.back());

            roofs.emplace_back(roofEntity);
        }
        temp = glm::translate(temp, glm::vec3(-1.0f * static_cast<float>(rows) * 3.0f, 0.0f, 3.0f));
    }
    auto house = new InstancedObject(houseBodyModel, &lightShader, houses);
    auto roof = new InstancedObject(houseRoofModel, &lightShader, roofs);

    Object spotLightGizmo("res/models/pyramid/pyramid.obj", &basicShader);
	Object spotLight1Gizmo("res/models/pyramid/pyramid.obj", &basicShader);
	Object pointLight("res/models/cube/cube.obj", &basicShader);

	const auto gizmoScale = glm::vec3(0.2f);
	spotLightGizmo.entity.setLocalScale(gizmoScale);
	spotLight1Gizmo.entity.setLocalScale(gizmoScale);
	pointLight.entity.setLocalScale(gizmoScale);

	spotLightGizmo.entity.setParent(neighEntity);
	spotLight1Gizmo.entity.setParent(neighEntity);
	pointLight.entity.setParent(neighEntity);

    neighbourhood -> update();

    int chosenBuilding = 0;

	glm::vec3 buildingLocalPos(0.0f);
	glm::vec3 prevBuildingLocalPos = buildingLocalPos;
	glm::vec3 housesLocalPos(0.0f); //house.transform->GetLocalPosition();
	glm::vec3 prevHousesLocalPos = housesLocalPos;
	glm::vec3 neigbourhoodLocalPos(0.0f);

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

        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.getViewMatrix();

        // Draw skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.getViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        skybox.draw();

        // Draw houses
        neighEntity -> update();

        neighbourhood -> draw();
        house -> draw();
        roof -> draw();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
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