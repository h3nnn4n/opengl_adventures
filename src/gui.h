#ifndef _GUI_H
#define _GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimplot.h>

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
void gui_new_frame();

void gui_update_camera(Camera *camera);
void gui_update_entity();
void gui_current_scene();
void gui_update_fps();
void gui_mouse();
void gui_fbo_clickcolor();
void gui_update_lights();
void gui_main_menu();

void save_scene_gui();
void load_scene_gui();

void toggle(bool *value);

#endif
