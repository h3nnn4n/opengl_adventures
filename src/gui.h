#ifndef _GUI_H
#define _GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

extern GLFWwindow *window;
extern struct ImGuiContext* ctx;
extern struct ImGuiIO* io;

int ImGui_ImplOpenGL3_Init(const char* glsl_version);
int ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks);
void ImGui_ImplGlfw_Shutdown();
void ImGui_ImplOpenGL3_Shutdown();

void ImGui_ImplOpenGL3_RenderDrawData();
void ImGui_ImplOpenGL3_NewFrame();
void ImGui_ImplGlfw_NewFrame();

void gui_init();
void gui_terminate();
void gui_render();
void gui_update();

#endif
