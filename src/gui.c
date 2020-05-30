#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "gui.h"

struct ImGuiContext *ctx;
struct ImGuiIO *io;

char *buffer;

void gui_init() {
  ctx = igCreateContext(NULL);
  io  = igGetIO();

  const char* glsl_version = "#version 330 core";
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  ImGui_ImplOpenGL3_Init(glsl_version);

  igStyleColorsDark(NULL);

  buffer = malloc(sizeof(char) * 1024);
}

void gui_terminate() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(ctx);

  free(buffer);
}

void gui_render() {
  igRender();
  ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void gui_update_camera(Camera *camera) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();

  igBegin("Camera", NULL, 0);

  sprintf(buffer,
          "position: %4.2f %4.2f %4.2f",
          camera->camera_pos[0],
          camera->camera_pos[1],
          camera->camera_pos[2]);
  igText(buffer);

  sprintf(buffer,
          "target: %4.2f %4.2f %4.2f",
          camera->camera_target[0],
          camera->camera_target[1],
          camera->camera_target[2]);
  igText(buffer);

  sprintf(buffer,
          "yaw, pitch: %4.2f %4.2f",
          camera->yaw,
          camera->pitch);
  igText(buffer);

  sprintf(buffer,
          "fov: %4.2f",
          camera->zoom);
  igText(buffer);

  igEnd();
}
