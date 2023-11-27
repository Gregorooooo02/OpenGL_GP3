#ifndef GP_3_IMGUIINIT_H
#define GP_3_IMGUIINIT_H

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

void init_imgui(GLFWwindow* window) {
    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Change style of the ImGui
    ImGuiStyle& style = ImGui::GetStyle();

    // Change sliders and buttons appearance
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(.75f, .0f, 1.0f, .6f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(.75f, .0f, 1.0f, .6f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(.05f, .0f, .25f, .5f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(.4f, .0f, .8f, .6f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(.4f, .0f, .8f, 1.0f);

    // Change window appearance
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(.5f, .0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(.05f, .0f, .2f, .4f);
    style.Colors[ImGuiCol_Border] = ImVec4(.5f, .0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(.05f, .0f, .2f, .4f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(.8f, .0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(.8f, .0f, 1.0f, 1.0f);

    // Change resize grip appearance
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(.1f, .0f, .3f, .4f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(.4f, .0f, .65f, .7f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(.5f, .0f, 1.0f, 1.0f);

    //ImGui::StyleColorsDark();
}

#endif //GP_3_IMGUIINIT_H
