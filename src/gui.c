#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gui.h"

struct ImGuiContext *ctx;
struct ImGuiIO *io;

void gui_init() {
  ctx = igCreateContext(NULL);
  io  = igGetIO();

  const char* glsl_version = "#version 330 core";
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  ImGui_ImplOpenGL3_Init(glsl_version);

  igStyleColorsDark(NULL);
}

void gui_terminate() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(ctx);
}

void gui_render() {
  igRender();
  ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void gui_update() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();

  igBegin("Hello Window", NULL, 0);
  igText("Hello World");
  igText("dear imgui says hello");
  igEnd();
}
