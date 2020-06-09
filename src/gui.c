#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "clickcolor_rendering.h"
#include "editor.h"
#include "entity.h"
#include "gui.h"
#include "input_handling.h"
#include "light.h"
#include "manager.h"
#include "settings.h"

struct ImGuiContext *ctx;
struct ImGuiIO *io;

char *buffer;

#define FPS_BUFFER_SIZE 100
#define FPS_AVG_BUFFER_SIZE 10

float fps_buffer[FPS_BUFFER_SIZE];
float fps_index[FPS_BUFFER_SIZE];
float fps_avg_buffer[FPS_AVG_BUFFER_SIZE];
int fps_pivot = 0;
int fps_avg_pivot = 0;

void gui_init() {
  ctx = igCreateContext(NULL);
  io  = igGetIO();

  const char* glsl_version = "#version 330 core";
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  ImGui_ImplOpenGL3_Init(glsl_version);

  igStyleColorsDark(NULL);

  buffer = malloc(sizeof(char) * 1024);

  memset(fps_buffer, FPS_BUFFER_SIZE, sizeof(float));

  for (int i = 0; i < FPS_BUFFER_SIZE; ++i) {
    fps_index[i] = (float)i;
  }
}

void gui_terminate() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(ctx);

  free(buffer);
}

void gui_new_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();
}

void gui_render() {
  if (manager->game_mode != EDITOR) return;

  gui_new_frame();

  // Update gui
  Shader_use(manager->default_shader);

  gui_update_fps();
  gui_update_camera(manager->active_camera);
  gui_update_entity(selected_entity);
  gui_update_lights();
  gui_mouse();
  gui_fbo_clickcolor(texColorBuffer);

  igRender();
  ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
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

void update_rolling_fps_avg() {
  float ms = manager->delta_time;
  float fps = 1.0f / ms;

  if (fps_avg_pivot >= FPS_AVG_BUFFER_SIZE) {
    fps_avg_pivot = 0;
  }

  fps_avg_buffer[fps_avg_pivot] = fps;
  fps_avg_pivot++;

  float avg_fps = 0;

  for (int i = 0; i < FPS_AVG_BUFFER_SIZE; ++i) {
    avg_fps += fps_avg_buffer[i] / FPS_AVG_BUFFER_SIZE;
  }

  if (fps_pivot >= FPS_BUFFER_SIZE) {
    fps_pivot = 0;
  }

  fps_buffer[fps_pivot] = avg_fps;
  fps_pivot++;
}

void gui_update_fps() {
  igBegin("Window", NULL, 0);

  float ms = manager->delta_time;
  float fps = 1.0 / ms;

  update_rolling_fps_avg();

  sprintf(buffer, "FPS: %6.2f", fps);
  igText(buffer);

  sprintf(buffer, " ms: %6.4f", ms);
  igText(buffer);

  igText("Game mode: ");
  igSameLine(0, 0);
  switch (manager->game_mode) {
    case IN_GAME:
      igText("IN_GAME");
      break;

    case IN_MENU:
      igText("IN_MENU");
      break;

    case FREE_CAMERA:
      igText("FREE_CAMERA");
      break;

    case EDITOR:
      igText("EDITOR");
      break;
  }

  igCheckbox("wireframe", (_Bool*)&wireframe_mode);

  ImVec2 size = { 200, 100 };
  ImVec4 plot_color_line = { 1, 1, 0, 1    };
  ImVec4 plot_color_fill = { 1, 1, 0, 0.25 };

  ImPlot_SetNextPlotLimits(0, FPS_BUFFER_SIZE, 0, 80, 1);

  ImPlotAxisFlags axis_flags = ImPlotAxisFlags_GridLines | ImPlotAxisFlags_LockMin |  ImPlotAxisFlags_LockMax;
  ImPlot_BeginPlot("", NULL, NULL, size, ImPlotFlags_MousePos | ImPlotFlags_Crosshairs, 0, axis_flags, 0, 0);

  ImPlot_PushStyleColorVec4(ImPlotCol_Line, plot_color_line);
  ImPlot_PushStyleColorVec4(ImPlotCol_Line, plot_color_fill);
  ImPlot_PlotLineFloatPtrFloatPtr("", fps_index, fps_buffer, FPS_BUFFER_SIZE, 0, 4);
  ImPlot_PopStyleColor(2);

  ImPlot_EndPlot();

  igEnd();
}

void gui_update_entity(Entity *entity) {
  igBegin("Entity", NULL, 0);

  if (entity != NULL) {
    igSliderFloat3("position" , (float*)entity->position , -5 , 5   , "%4.2f" , 1);
    igSliderFloat3("rotation" , (float*)entity->rotation ,  0 , 180 , "%4.2f" , 1);
    igSliderFloat3("scale"    , (float*)entity->scale    ,  0 , 5   , "%4.2f" , 1);
  } else {
    igText("Nothing selected");
  }

  igEnd();
}

void gui_update_lights() {
  Light *light;
  _Bool show = 1;

  igBegin("Lights", NULL, 0);

  light = directional_light;
  if(light && igCollapsingHeaderBoolPtr("Directional", &show, 0)) {
    float range = 2;
    igSliderFloat3("direction", (float*)light->direction, -range, range, "%4.2f", 1);

    igCheckbox("active", (_Bool*)&light->active);

    igText("Color");
    igColorEdit3("ambient", (float*)light->ambient, 0);
    igColorEdit3("diffuse", (float*)light->diffuse, 0);
    igColorEdit3("specular", (float*)light->specular, 0);
  }

  light = spot_light;
  if(light && igCollapsingHeaderBoolPtr("Spotlight", &show, 0)) {
    float range = 10;
    igSliderFloat3("position", (float*)light->position, -range, range, "%4.2f", 1);
    igSliderFloat3("direction", (float*)light->direction, -range, range, "%4.2f", 1);

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

        float range = 10;
        igSliderFloat3("position", (float*)light->position, -range, range, "%4.2f", 1);

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

void gui_mouse() {
  igBegin("Mouse", NULL, 0);

  sprintf(buffer,
          "position: %8.2f %8.2f",
          lastX,
          lastY);
  igText(buffer);

  sprintf(buffer,
          "   world: %8.2f %8.2f %8.2f",
          mouse_world_position[0],
          mouse_world_position[1],
          mouse_world_position[2]);
  igText(buffer);

  sprintf(buffer, "left: %d", left_mouse_pressed);
  igText(buffer);

  sprintf(buffer, "right: %d", right_mouse_pressed);
  igText(buffer);

  igEnd();
}

void gui_fbo_clickcolor(unsigned int texColorBuffer) {
  igBegin("FBO Test", NULL, 0);

  ImVec2 size = {WINDOW_WIDTH / 3.0, WINDOW_HEIGHT / 3.0};
  ImVec2 uv0 = {0, 1};
  ImVec2 uv1 = {1, 0};
  ImVec4 tint_col = {1, 1, 1, 1};
  ImVec4 border_col = {1, 0, 0, 1};

  igImage(
    (void*)(intptr_t)texColorBuffer,
    size,
    uv0,
    uv1,
    tint_col,
    border_col
  );

  igEnd();
}
