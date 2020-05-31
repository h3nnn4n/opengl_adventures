#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "gui.h"
#include "light.h"
#include "timer.h"

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

void gui_new_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();
}

void gui_update_camera(Camera *camera) {
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

void gui_update_fps() {
  igBegin("FPS", NULL, 0);

  float ms = delta_time();
  float fps = 1.0 / ms;

  sprintf(buffer, "FPS: %6.2f", fps);
  igText(buffer);

  sprintf(buffer, " ms: %6.4f", ms);
  igText(buffer);

  igEnd();
}

void gui_update_lights() {
  if (directional_light == NULL) return;

  Light *light = directional_light;

  igBegin("Directional Light", NULL, 0);

  sprintf(buffer,
         "target: %4.2f %4.2f %4.2f",
         light->direction[0],
         light->direction[1],
         light->direction[2]);
  igText(buffer);

  sprintf(buffer,
         "ambient: %4.2f %4.2f %4.2f",
         light->ambient[0],
         light->ambient[1],
         light->ambient[2]);
  igText(buffer);

  sprintf(buffer,
         "specular: %4.2f %4.2f %4.2f",
         light->specular[0],
         light->specular[1],
         light->specular[2]);
  igText(buffer);

  sprintf(buffer,
         "diffuse: %4.2f %4.2f %4.2f",
         light->diffuse[0],
         light->diffuse[1],
         light->diffuse[2]);
  igText(buffer);

  igEnd();
}
