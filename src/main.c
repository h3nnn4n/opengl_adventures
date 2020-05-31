#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "camera.h"
#include "gui.h"
#include "input_handling.h"
#include "light.h"
#include "model_c.h"
#include "settings.h"
#include "shader_c.h"
#include "stb_image.h"
#include "timer.h"
#include "utils.h"

GLFWwindow *window;
Camera *camera;

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

  camera = make_camera();

  Shader *shader = newShader("shaders/shader.vert", "shaders/shader_obj_color.frag");
  Shader *light_shader = newShader("shaders/shader.vert", "shaders/light_obj.frag");
  Shader_use(shader);

  ///////////////////////////////
  // Light
  //
  directional_light = make_light(DIRECTIONAL);
  spot_light = make_light(SPOTLIGHT);
  point_lights[0] = make_light(POINT);
  point_lights[1] = make_light(POINT);
  point_lights[2] = make_light(POINT);
  point_lights[3] = make_light(POINT);

  directional_light->shader = shader;
  spot_light->shader = shader;

  directional_light->active = 1;
  spot_light->active = 1;

  vec3 light_direction = { -0.2f, -1.0f, -0.3f };
  set_direction(directional_light, light_direction);

  vec3 light_positions[] = {
    { 0.7f,  0.2f,  2.0f },
    { 2.3f, -3.3f, -4.0f },
    {-4.0f,  2.0f, -12.0f},
    { 0.0f, -2.0f, -3.0f }
  };

  for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
    set_position(point_lights[i], (float*)light_positions[i]);
    point_lights[i]->shader = shader;
    point_lights[i]->active = 1;
  }

  /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
  /*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

  glEnable(GL_DEPTH_TEST);

  Model* model = newModel("assets/backpack/backpack.obj");

  ////////////////////////
  // Main loop
  //
  while(!glfwWindowShouldClose(window))
  {
    // Process input
    glfwPollEvents();
    processInput(window);

    // Clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader_use(shader);

    // Timer
    float timer = glfwGetTime();
    update_delta(timer);
    Shader_set_float(shader, "time", timer);

    update_camera(camera, shader);

    set_position(spot_light, camera->camera_pos);
    set_direction(spot_light, camera->camera_front);

    refresh_lights();

    // Draws
    Model_draw(model, shader);

    // Update gui
    gui_new_frame();
    gui_update_fps();
    gui_update_camera(camera);
    gui_update_lights();
    gui_render();

    // Draw to screen
    glfwSwapBuffers(window);
  }

  gui_terminate();
  glfwTerminate();

  return 0;
}
