#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "camera.h"
#include "entity.h"
#include "gui.h"
#include "input_handling.h"
#include "light.h"
#include "manager.h"
#include "model_c.h"
#include "scene_loader.h"
#include "settings.h"
#include "shader_c.h"
#include "stb_image.h"
#include "timer.h"
#include "utils.h"

GLFWwindow *window;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  stbi_set_flip_vertically_on_load(1);
  gui_init();

  {
    manager = init_manager();

    Shader *shader = newShader("shaders/shader.vert", "shaders/phong_material.frag");
    Shader *shader_light = newShader("shaders/shader.vert", "shaders/light_obj.frag");

    manager->default_shader = shader;
    manager->default_shader_light = shader_light;
  }

  load_scene(manager);

  { // This will go away once we start to load scenes from disk
    int field_size = 5;

    for (int x = -field_size; x <= field_size; ++x) {
      for (int z = -field_size; z <= field_size; ++z) {
        Entity* cube = new_entity();
        load_model(cube, "assets/cube/cube.obj");
        cube->shader = manager->default_shader;
        cube->position[0] = x;
        cube->position[2] = z;

        Manager_add_entity(manager, cube);
      }
    }
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_FRAMEBUFFER_SRGB);
  glEnable(GL_MULTISAMPLE);

  ////////////////////////
  // Main loop
  //
  while(!glfwWindowShouldClose(window))
  {
    // Process input
    glfwPollEvents();
    processInput(window);

    /*Shader_reload_changes(manager->default_shader);*/

    // Clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (wireframe_mode) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Timer
    Manager_tick_timer(manager);

    // Camera
    Manager_update_active_camera_location(manager);

    // Lights
    refresh_lights();

    // Draws
    Manager_render_entities(manager);

    // Lights (render light positions)
    Shader_use(manager->default_shader_light);
    update_camera_projection_matrix(manager->active_camera, manager->default_shader_light);
    update_camera_view_matrix(manager->active_camera, manager->default_shader_light);
    draw_point_lights();

    // Update gui
    Shader_use(manager->default_shader);

    gui_new_frame();
    gui_update_fps();
    gui_update_camera(manager->active_camera);
    /*gui_update_entity(cube);*/
    gui_update_lights();
    gui_render();

    // Draw to screen
    glfwSwapBuffers(window);
  }

  gui_terminate();
  glfwTerminate();

  return 0;
}
