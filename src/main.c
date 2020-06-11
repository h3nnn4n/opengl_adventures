#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include <stb_image.h>

#include "camera.h"
#include "clickcolor_rendering.h"
#include "entity.h"
#include "gui.h"
#include "input_handling.h"
#include "light.h"
#include "main_render_pass.h"
#include "manager.h"
#include "model_c.h"
#include "player.h"
#include "scene_loader.h"
#include "scene_save.h"
#include "settings.h"
#include "shader_c.h"
#include "skybox.h"
#include "timer.h"
#include "utils.h"

GLFWwindow *window;

#define MSAA 4

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef MSAA
  glfwWindowHint(GLFW_SAMPLES, MSAA);
#endif

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetMouseButtonCallback(window, mouse_click_callback);

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

  load_scene(manager, "scenes/test_scene.json");

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_FRAMEBUFFER_SRGB);
#ifdef MSAA
  glEnable(GL_MULTISAMPLE);
#endif

  glCullFace(GL_BACK);

  build_clickcolor_framebuffer();
  make_skybox();

  ////////////////////////
  // Main loop
  //
  while(!glfwWindowShouldClose(window))
  {
    // Process input
    glfwPollEvents();
    processInput(window);

    Shader_reload_changes(manager->default_shader);
    Shader_reload_changes(manager->default_shader_light);

    // Timer
    Manager_tick_timer(manager);

    // Entity stuff
    Manager_update_entities(manager);

    // Camera
    Manager_update_active_camera_location(manager);

    // Render calls
    /*clickcolor_render_pass();*/
    /*main_render_pass();*/
    render_skybox();
    /*gui_render();*/

    // Draw to screen
    glfwSwapBuffers(window);
  }

  gui_terminate();
  glfwTerminate();

  return 0;
}
