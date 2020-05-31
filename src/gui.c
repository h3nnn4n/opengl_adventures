#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "gui.h"
#include "input_handling.h"
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
  igBegin("Window", NULL, 0);

  float ms = delta_time();
  float fps = 1.0 / ms;

  sprintf(buffer, "FPS: %6.2f", fps);
  igText(buffer);

  sprintf(buffer, " ms: %6.4f", ms);
  igText(buffer);

  if (locked_cursor) {
    sprintf(buffer, "cursor: locked");
  } else {
    sprintf(buffer, "cursor: free");
  }
  igText(buffer);

  igEnd();
}

void gui_update_lights() {
  Light *light;
  _Bool show = 1;

  igBegin("Lights", NULL, 0);

  light = directional_light;
  if(light && igCollapsingHeaderBoolPtr("Directional", &show, 0)) {
    sprintf(buffer,
          "target: %4.2f %4.2f %4.2f",
          light->direction[0],
          light->direction[1],
          light->direction[2]);
    igText(buffer);

    igCheckbox("active", (_Bool*)&light->active);

    igText("Color");
    igColorEdit3("ambient", (float*)light->ambient, 0);
    igColorEdit3("diffuse", (float*)light->diffuse, 0);
    igColorEdit3("specular", (float*)light->specular, 0);
  }

  light = spot_light;
  if(light && igCollapsingHeaderBoolPtr("Spotlight", &show, 0)) {
    sprintf(buffer,
          "target: %4.2f %4.2f %4.2f",
          light->position[0],
          light->position[1],
          light->position[2]);
    igText(buffer);

    sprintf(buffer,
          "target: %4.2f %4.2f %4.2f",
          light->direction[0],
          light->direction[1],
          light->direction[2]);
    igText(buffer);

    igCheckbox("active", (_Bool*)&light->active);

    igText("Color");
    igColorEdit3("ambient", (float*)light->ambient, 0);
    igColorEdit3("diffuse", (float*)light->diffuse, 0);
    igColorEdit3("specular", (float*)light->specular, 0);

    igText("Cone Shape");
    igSliderFloat("cutoff", &light->cutOff, 0, 90, "%4.2f", 1);
    igSliderFloat("outter cutoff", &light->outterCutOff, 0, 90, "%4.2f", 1);

    igText("Attenuation");
    igSliderFloat("constant", &light->constant, 0, 1, "%4.2f", 1);
    igSliderFloat("linear", &light->linear, 0, 1, "%4.2f", 1);
    igSliderFloat("quadratic", &light->quadratic, 0, 1, "%4.2f", 1);
  }

  if (igCollapsingHeaderBoolPtr("Point lights", &show, 0)) {
    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
      light = point_lights[i];
      sprintf(buffer, "Point light %d", i);

      if(light && igTreeNodeStr(buffer)) {
        igPushIDInt(i);

        sprintf(buffer,
              "position: %4.2f %4.2f %4.2f",
              light->position[0],
              light->position[1],
              light->position[2]);
        igText(buffer);

        igCheckbox("active", (_Bool*)&light->active);

        igText("Color");
        igColorEdit3("ambient", (float*)light->ambient, 0);
        igColorEdit3("diffuse", (float*)light->diffuse, 0);
        igColorEdit3("specular", (float*)light->specular, 0);

        igText("Attenuation");
        igSliderFloat("constant", &light->constant, 0, 1, "%4.2f", 1);
        igSliderFloat("linear", &light->linear, 0, 1, "%4.2f", 1);
        igSliderFloat("quadratic", &light->quadratic, 0, 1, "%4.2f", 1);

        igPopID();
        igTreePop();
      }
    }
  }

  igEnd();
}
