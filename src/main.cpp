#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "stb_image.h"

#include "lib/ImGuiInit.h"
#include "lib/Shader.h"
#include "lib/Camera.h"
#include "lib/Model.h"
#include "lib/Entity.h"
#include "lib/Skybox.h"
#include "lib/InstancedEntity.h"

#include <iostream>
#include <vector>

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(0.0f, 5.0f, 2.0f));
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cursorMode = false;

// Timing
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

int main(int, char**) {
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
    Shader basicShader("res/shaders/basic.vert", "res/shaders/basic.frag");
    Shader lightShader("res/shaders/light.vert", "res/shaders/light.frag");
    Shader textureShader("res/shaders/texture.vert", "res/shaders/light.frag");
    Shader skyboxShader("res/shaders/skybox.vert", "res/shaders/skybox.frag");

    // Material properties
    float shininess = 2.0f; // Shininess of the material
    bool isBlinn = false;
    float blinnExponent = 32.0f;

    // Light properties
    //DIR LIGHT PROPERTIES
	bool isDirLight = true;
	glm::vec3 direction(0, 0, 0);
	glm::vec3 ambient(.19f);
	glm::vec3 diffuse(0);
	glm::vec3 specular(0);

	//POINT LIGHT PROPERTIES
	bool isPointLight = false;
	glm::vec3 pointLightPosition(0.0f);
	glm::vec3 pointLightAmbient(1.0f);
	glm::vec3 pointLightDiffuse(1.0f);
	glm::vec3 pointLightSpecular(1.0f);
	float pointLightConstant = 1.0f;
	float pointLightLinear = .7f;
	float pointLightQuadratic = 1.8f;

	//SPOTLIGHT PROPERTIES
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

    // Instanced matrices setup
    int rows = 200, cols = 200;
    int amount = rows * cols;

    std::vector<Transform*> houseTransforms;
    std::vector<Transform*> roofTransforms;

    auto houseModel = new Model("res/models/cube/cube.obj");
    auto roofModel = new Model("res/models/pyramid/pyramid.obj");
    auto planeModel = new Model("res/models/plane/plane.obj");

    auto neighbourhood = new Entity(planeModel, &textureShader);

    auto neighbourhoodTransform = &neighbourhood -> transform;

    glm::mat4 tmp = glm::translate(glm::mat4(1.0f), {-400, 0, -400});
    for (auto i = 0; i < cols; i++) {
        glm::mat4 model(1.0f);
        for (auto j = 0; j < rows; j++) {
            auto houseTransform = new Transform();
            auto roofTransform = new Transform();

            tmp = glm::translate(tmp, glm::vec3(4.0f, 0.0f, 0.0f));
            model = glm::translate(tmp, glm::vec3(0.0f, 1.5f, 0.0f));

            houseTransform -> setModelMatrix(model);
            houseTransform -> setParent(neighbourhoodTransform);

            houseTransforms.emplace_back(houseTransform);

            roofTransform -> setLocalPosition(glm::vec3(0.0f, 2.0f, 0.0f));
            roofTransform -> setParent(houseTransforms.back());

            roofTransforms.emplace_back(roofTransform);
        }
        tmp = glm::translate(tmp, glm::vec3(-1.0f * static_cast<float>(rows) * 4.0f, 0.0f, 4.0f));
    }

    // Create instanced entities
    auto house = new InstancedEntity(houseModel, &lightShader, houseTransforms);
    auto roof = new InstancedEntity(roofModel, &lightShader, roofTransforms);

    // Lights
    Entity spotLightGizmo("res/models/cube/cube.obj", &basicShader);
    Entity spotLight1Gizmo("res/models/cube/cube.obj", &basicShader);
    Entity pointLight("res/models/pyramid/pyramid.obj", &basicShader);

    const auto gizmoScale = glm::vec3(0.2f);
    spotLightGizmo.transform.setLocalScale(gizmoScale);
    spotLight1Gizmo.transform.setLocalScale(gizmoScale);
    pointLight.transform.setLocalScale(gizmoScale);

    spotLightGizmo.transform.setParent(neighbourhoodTransform);
    spotLight1Gizmo.transform.setParent(neighbourhoodTransform);
    pointLight.transform.setParent(neighbourhoodTransform);

    neighbourhood -> update();

    int chosenBuilding = 0;

    glm::vec3 buildingLocalPos({0.0f, 0.0f, 0.0f});
    glm::vec3 prevBuildingLocalPos = buildingLocalPos;
    glm::vec3 housesLocalPos({0.0f, 0.0f, 0.0f});
    glm::vec3 prevHousesLocalPos = housesLocalPos;
    glm::vec3 neighbourhoodLocalPos({0.0f, 0.0f, 0.0f});

    // Skybox setup
    Skybox skybox;
    skybox.init();

    // Set skybox shader uniforms
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glEnable(GL_CULL_FACE); // Enable face culling
    glCullFace(GL_BACK); // Cull back faces

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

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Inspector");

            ImGui::InputInt("Chosen building", &chosenBuilding);
            ImGui::SliderFloat3("Building local pos", glm::value_ptr(buildingLocalPos), -10.0f, 10.0f);
            ImGui::InputFloat3("Plane local pos", glm::value_ptr(housesLocalPos));

            ImGui::InputFloat3("N loc", glm::value_ptr(neighbourhoodLocalPos));

            ImGui::Text("MATERIAL");
            ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);

            ImGui::Text("MISCELLANEOUS");
            ImGui::Checkbox("Blinn-Phong lighting", &isBlinn);
            ImGui::SliderFloat("Blinn-Phong exponent", &blinnExponent, 2.0f, 256.0f);

            ImGui::Checkbox("Directional light", &isDirLight);
            ImGui::SliderFloat3("Direction", glm::value_ptr(direction), -1.0f, 1.0f);
            ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
            ImGui::ColorEdit3("Specular", glm::value_ptr(specular));

            ImGui::Text("POINT LIGHT");
            ImGui::Checkbox("Point light", &isPointLight);

            ImGui::ColorEdit3("Point light ambient", glm::value_ptr(pointLightAmbient));
            ImGui::ColorEdit3("Point light diffuse", glm::value_ptr(pointLightDiffuse));
            ImGui::ColorEdit3("Point light specular", glm::value_ptr(pointLightSpecular));
            ImGui::InputFloat("Point light constant", &pointLightConstant);
            ImGui::InputFloat("Point light linear", &pointLightLinear);
            ImGui::InputFloat("Point light quadratic", &pointLightQuadratic);

            ImGui::Text("SPOT LIGHT");
            ImGui::Checkbox("Spot light", &isSpotActive);
            ImGui::DragFloat3("Spot light position", glm::value_ptr(spotLightPosition), .1f, -10.0f, 10.0f);
            ImGui::SliderFloat3("Spot light direction", glm::value_ptr(spotLightDirection), -1.0f, 1.0f);

            ImGui::ColorEdit3("Spot light ambient", glm::value_ptr(spotLightAmbient));
            ImGui::ColorEdit3("Spot light diffuse", glm::value_ptr(spotLightDiffuse));
            ImGui::ColorEdit3("Spot light specular", glm::value_ptr(spotLightSpecular));
            ImGui::InputFloat("Spot light constant", &spotLightConstant);
            ImGui::InputFloat("Spot light linear", &spotLightLinear);
            ImGui::InputFloat("Spot light quadratic", &spotLightQuadratic);
            ImGui::InputFloat("Spot light cut off", &spotLightCutOff);
            ImGui::InputFloat("Spot light outer cut off", &spotLightOuterCutOff);

            ImGui::Text("SPOT LIGHT 1");
            ImGui::Checkbox("Spot light 1", &isSpot1Active);
            ImGui::DragFloat3("Spot light 1 position", glm::value_ptr(spotLight1Position), .1f, -10.0f, 10.0f);
            ImGui::SliderFloat3("Spot light 1 direction", glm::value_ptr(spotLight1Direction), -1.0f, 1.0f);

            ImGui::ColorEdit3("Spot light 1 ambient", glm::value_ptr(spotLight1Ambient));
            ImGui::ColorEdit3("Spot light 1 diffuse", glm::value_ptr(spotLight1Diffuse));
            ImGui::ColorEdit3("Spot light 1 specular", glm::value_ptr(spotLight1Specular));
            ImGui::InputFloat("Spot light 1 constant", &spotLight1Constant);
            ImGui::InputFloat("Spot light 1 linear", &spotLight1Linear);
            ImGui::InputFloat("Spot light 1 quadratic", &spotLight1Quadratic);
            ImGui::InputFloat("Spot light 1 cut off", &spotLight1CutOff);
            ImGui::InputFloat("Spot light 1 outer cut off", &spotLight1OuterCutOff);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }

        pointLightPosition = pointLight.transform.getLocalPosition();

        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 VP = projection * camera.getViewMatrix();

        // Shader updates
        lightShader.use();
		lightShader.setMat4("VP", VP);
		lightShader.setVec3("viewPos", camera.position);

		lightShader.setFloat("shininess", shininess);
		lightShader.setVec3("offset", buildingLocalPos);
		lightShader.setInt("chosenInstance", chosenBuilding);

		lightShader.setBool("isBlinn", isBlinn);
		lightShader.setFloat("blinnExponent", blinnExponent);

		lightShader.setBool("dirLight.isActive", isDirLight);
		lightShader.setVec3("dirLight.direction", direction);
		lightShader.setVec3("dirLight.colors.ambient", ambient);
		lightShader.setVec3("dirLight.colors.diffuse", diffuse);
		lightShader.setVec3("dirLight.colors.specular", specular);

        // Point light
        lightShader.setBool("pointLights[0].isActive", isPointLight);

		lightShader.setVec3("pointLights[0].position", pointLightPosition);
		lightShader.setFloat("pointLights[0].att.constant", pointLightConstant);
		lightShader.setFloat("pointLights[0].att.linear", pointLightLinear);
		lightShader.setFloat("pointLights[0].att.quadratic", pointLightQuadratic);

		lightShader.setVec3("pointLights[0].colors.ambient", pointLightAmbient);
		lightShader.setVec3("pointLights[0].colors.diffuse", pointLightDiffuse);
		lightShader.setVec3("pointLights[0].colors.specular", pointLightSpecular);

        // Spotlight
        lightShader.setBool("spotLights[0].isActive", isSpotActive);

		lightShader.setVec3("spotLights[0].position", spotLightPosition);
		lightShader.setVec3("spotLights[0].direction", spotLightDirection);

		lightShader.setFloat("spotLights[0].att.constant", spotLightConstant);
		lightShader.setFloat("spotLights[0].att.linear", spotLightLinear);
		lightShader.setFloat("spotLights[0].att.quadratic", spotLightQuadratic);

		lightShader.setVec3("spotLights[0].colors.ambient", spotLightAmbient);
		lightShader.setVec3("spotLights[0].colors.diffuse", spotLightDiffuse);
		lightShader.setVec3("spotLights[0].colors.specular", spotLightSpecular);

		lightShader.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(spotLightCutOff)));
		lightShader.setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(spotLightOuterCutOff)));

        // Spotlight 1
        lightShader.setBool("spotLights[1].isActive", isSpot1Active);

		lightShader.setVec3("spotLights[1].position", spotLight1Position);
		lightShader.setVec3("spotLights[1].direction", spotLight1Direction);
		lightShader.setFloat("spotLights[1].att.constant", spotLight1Constant);
		lightShader.setFloat("spotLights[1].att.linear", spotLight1Linear);
		lightShader.setFloat("spotLights[1].att.quadratic", spotLight1Quadratic);

		lightShader.setVec3("spotLights[1].colors.ambient", spotLight1Ambient);
		lightShader.setVec3("spotLights[1].colors.diffuse", spotLight1Diffuse);
		lightShader.setVec3("spotLights[1].colors.specular", spotLight1Specular);

		lightShader.setFloat("spotLights[1].cutOff", glm::cos(glm::radians(spotLight1CutOff)));
		lightShader.setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(spotLight1OuterCutOff)));

        textureShader.use();
		textureShader.setMat4("VP", VP);
		textureShader.setVec3("viewPos", camera.position);

		textureShader.setFloat("shininess", shininess);

		textureShader.setBool("isBlinn", isBlinn);
		textureShader.setFloat("blinnExponent", blinnExponent);


		textureShader.setBool("dirLight.isActive", isDirLight);
		textureShader.setVec3("dirLight.direction", direction);
		textureShader.setVec3("dirLight.colors.ambient", ambient);
		textureShader.setVec3("dirLight.colors.diffuse", diffuse);
		textureShader.setVec3("dirLight.colors.specular", specular);

        // Point light
        textureShader.setBool("pointLights[0].isActive", isPointLight);

		textureShader.setVec3("pointLights[0].position", pointLightPosition);
		textureShader.setFloat("pointLights[0].att.constant", pointLightConstant);
		textureShader.setFloat("pointLights[0].att.linear", pointLightLinear);
		textureShader.setFloat("pointLights[0].att.quadratic", pointLightQuadratic);

		textureShader.setVec3("pointLights[0].colors.ambient", pointLightAmbient);
		textureShader.setVec3("pointLights[0].colors.diffuse", pointLightDiffuse);
		textureShader.setVec3("pointLights[0].colors.specular", pointLightSpecular);

        // Spotlight
        textureShader.setBool("spotLights[0].isActive", isSpotActive);

		textureShader.setVec3("spotLights[0].position", spotLightPosition);
		textureShader.setVec3("spotLights[0].direction", spotLightDirection);

		textureShader.setFloat("spotLights[0].att.constant", spotLightConstant);
		textureShader.setFloat("spotLights[0].att.linear", spotLightLinear);
		textureShader.setFloat("spotLights[0].att.quadratic", spotLightQuadratic);

		textureShader.setVec3("spotLights[0].colors.ambient", spotLightAmbient);
		textureShader.setVec3("spotLights[0].colors.diffuse", spotLightDiffuse);
		textureShader.setVec3("spotLights[0].colors.specular", spotLightSpecular);

		textureShader.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(spotLightCutOff)));
		textureShader.setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(spotLightOuterCutOff)));

        // Spotlight 1
        textureShader.setBool("spotLights[1].isActive", isSpot1Active);

        textureShader.setVec3("spotLights[1].position", spotLight1Position);
        textureShader.setVec3("spotLights[1].direction", spotLight1Direction);

		textureShader.setFloat("spotLights[1].att.constant", spotLight1Constant);
		textureShader.setFloat("spotLights[1].att.linear", spotLight1Linear);
		textureShader.setFloat("spotLights[1].att.quadratic", spotLight1Quadratic);

		textureShader.setVec3("spotLights[1].colors.ambient", spotLight1Ambient);
		textureShader.setVec3("spotLights[1].colors.diffuse", spotLight1Diffuse);
		textureShader.setVec3("spotLights[1].colors.specular", spotLight1Specular);

		textureShader.setFloat("spotLights[1].cutOff", glm::cos(glm::radians(spotLight1CutOff)));
		textureShader.setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(spotLight1OuterCutOff)));

        basicShader.use();
		basicShader.setMat4("VP", VP);
		basicShader.setVec3("diffuse", pointLightDiffuse * pointLightAmbient * pointLightSpecular);
        // End of shader updates

        spotLightGizmo.transform.setLocalPosition(spotLightPosition);
        spotLightGizmo.transform.setLocalRotation(spotLightDirection);

        spotLight1Gizmo.transform.setLocalPosition(spotLight1Position);
        spotLight1Gizmo.transform.setLocalRotation(spotLight1Direction);

        if (buildingLocalPos != prevBuildingLocalPos) {
            prevBuildingLocalPos = buildingLocalPos;
            house -> instanceTransforms[chosenBuilding] -> setLocalPosition(buildingLocalPos);
        }

        if (housesLocalPos != prevHousesLocalPos) {
            prevHousesLocalPos = housesLocalPos;
            neighbourhoodTransform -> setLocalPosition(housesLocalPos);
        }

        house -> transform.setLocalPosition(neighbourhoodLocalPos);
        house -> transform.update();

        const auto a = static_cast<float>(glfwGetTime());
        pointLight.transform.setLocalRotationX(15 * a);
        pointLight.transform.setLocalRotationY(15 * a);
        pointLight.transform.setLocalPosition({10 * glm::cos(a), 7.0f, 10 * glm::sin(a)});

        neighbourhoodTransform -> update();

        neighbourhood -> draw();
        house -> draw();
        roof -> draw();
        basicShader.setVec3("diffuse", pointLightDiffuse * pointLightAmbient * pointLightSpecular);

		pointLight.draw();
		basicShader.setVec3("diffuse", spotLightDiffuse * spotLightAmbient * spotLightSpecular);

		spotLightGizmo.draw();
		basicShader.setVec3("diffuse", spotLight1Diffuse * spotLight1Ambient * spotLight1Specular);

        spotLight1Gizmo.draw();

        // Draw skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        skybox.draw();

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glfwMakeContextCurrent(window);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll events
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete neighbourhood;

    // Terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!cursorMode) {
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
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (!cursorMode) {
        camera.processMouseScroll(static_cast<float>(yoffset));
    }
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

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.movementSpeed = 10.0f;
    } else {
        camera.movementSpeed = 5.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        if (cursorMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursorMode = false;
            firstMouse = true;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorMode = true;
            int w, h;
            glfwGetWindowSize(window, &w, &h);
            glfwSetCursorPos(window, w / 2, h / 2);
        }
    }
}