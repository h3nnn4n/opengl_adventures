#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "camera.h"
#include "gui.h"
#include "input_handling.h"
#include "settings.h"
#include "shader_c.h"
#include "stb_image.h"
#include "timer.h"
#include "utils.h"

int firstMouse;
float lastX;
float lastY;

GLFWwindow *window;
Camera *camera;

vec3 light_position = { 1.2f, 1.0f, 2.0f }; // Point light
vec3 light_direction = { -0.2f, -1.0f, -0.3f }; // Directional light

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

  gui_init();

  camera = make_camera();

  /*Shader *shader = newShader("shaders/shader.vert", "shaders/shader.frag");*/
  Shader *shader = newShader("shaders/shader.vert", "shaders/shader_obj_color.frag");
  Shader_use(shader);
  Shader_set_float(shader, "colorOverrideIntensity", 0.5);

  vec3 objectColor = {1.0f, 0.5f, 0.31f};
  vec3 lightColor = {1.0f, 1.0f, 1.0f};
  Shader_set_vec3(shader, "objectColor", objectColor);
  Shader_set_vec3(shader, "lightColor", lightColor);

  Shader *light_shader = newShader("shaders/shader.vert", "shaders/light_obj.frag");

  /*float vertices[] = {*/
    /*// positions         // colors*/
     /*0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right*/
     /*0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right*/
    /*-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left*/
    /*-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left*/
  /*};*/

  /*unsigned int indices[] = {  // note that we start from 0!*/
    /*0, 1, 3,   // first triangle*/
    /*1, 2, 3    // second triangle*/
  /*};*/

  float cube_vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
  };

  unsigned int VBO, VAO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);*/
  /*glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

  glBindVertexArray(0);

  ///////////////////////////////
  // Light
  //
  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  // we only need to bind to the VBO, the container's VBO's data already contains the data.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // set the vertex attributes (only position data for our lamp)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  ///////////////////////////////
  // Textures
  //
  unsigned int texture1, texture2;

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(1);

  {
    int width, height, nrChannels;
    const char *tex_filename = "assets/container2.png";
    unsigned char *data = stbi_load(tex_filename, &width, &height, &nrChannels, 0);

    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      fprintf(stderr, "Failed to load %s", tex_filename);
    }

    stbi_image_free(data);
  }

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  {
    int width, height, nrChannels;
    const char *tex_filename = "assets/container2_specular.png";
    unsigned char *data = stbi_load(tex_filename, &width, &height, &nrChannels, 0);

    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      fprintf(stderr, "Failed to load %s", tex_filename);
    }

    stbi_image_free(data);
  }

  Shader_set_int(shader, "material.diffuse", 0);
  Shader_set_int(shader, "material.specular", 1);

  /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
  /*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

  glEnable(GL_DEPTH_TEST);

  vec3 cube_positions[] = {
    { 0.0f,  0.0f,  0.0f },
    { 2.0f,  5.0f, -15.0f},
    {-1.5f, -2.2f, -2.5f },
    {-3.8f, -2.0f, -12.3f},
    { 2.4f, -0.4f, -3.5f },
    {-1.7f,  3.0f, -7.5f },
    { 1.3f, -2.0f, -2.5f },
    { 1.5f,  2.0f, -2.5f },
    { 1.5f,  0.2f, -1.5f },
    {-1.3f,  1.0f, -1.5f }
  };

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
    Shader_set_vec3(shader, "viewPos", (float*)camera->camera_pos);

    /*Shader_set_vec3(shader, "light.position", (float*)light_position);*/
    /*Shader_set_vec3(shader, "light.direction", (float*)light_direction);*/

    Shader_set_vec3(shader, "light.position", (float*)camera->camera_pos);
    Shader_set_vec3(shader, "light.direction", (float*)camera->camera_front);

    Shader_set_float(shader, "light.cutOff", cos(deg2rad(12.5f)));

    Shader_set_vec3f(shader, "light.ambient",  0.2f, 0.2f, 0.2f);
    Shader_set_vec3f(shader, "light.diffuse",  0.6f, 0.7f, 0.8f);
    Shader_set_vec3f(shader, "light.specular", 1.0f, 1.0f, 1.0f);

    Shader_set_float(shader, "light.constant",  1.0f);
    Shader_set_float(shader, "light.linear",    0.09f);
    Shader_set_float(shader, "light.quadratic", 0.032f);

    // Timer
    float timer = glfwGetTime();
    update_delta(timer);
    Shader_set_float(shader, "time", timer);

    update_camera(camera, shader);

    // Draw cubes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    /*glBindTexture(GL_TEXTURE_2D, texture);*/

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);

    {
      Shader_set_vec3f(shader, "material.specular", 0.5f, 0.5f, 0.5f);
      Shader_set_float(shader, "material.shininess", 32.0f);

      int i = 0;
      for (i = 0; i < 10; ++i) {
        vec3 v_scale = GLM_VEC3_ONE_INIT;
        /*glm_vec3_scale(v_scale, 0.5, v_scale);*/

        /*vec3 v_translate = GLM_VEC3_ONE_INIT;*/
        /*glm_vec3_scale(v_translate, 0.1, v_translate);*/
        /*v_translate[2] = 0;*/

        mat4 m_model = GLM_MAT4_IDENTITY_INIT;

        glm_translate(m_model, cube_positions[i]);
        /*glm_rotate(m_model, (25 * GLM_PI / 180.0) * timer * i / 3.0, GLM_XUP);*/
        /*glm_rotate(m_model, (50 * GLM_PI / 180.0) * timer * i / 2.0, GLM_YUP);*/
        glm_rotate(m_model, (25 * GLM_PI / 180.0) * i / 3.0, GLM_XUP);
        glm_rotate(m_model, (50 * GLM_PI / 180.0) * i / 2.0, GLM_YUP);
        glm_scale(m_model, v_scale);

        Shader_set_matrix4(shader, "model", (float*)m_model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

    /*glDrawArrays(GL_TRIANGLES, 0, 3);*/
    /*glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
    /*glDrawArrays(GL_TRIANGLES, 0, 36);*/

    glBindVertexArray(0);

    { // Draw light source
      Shader_use(light_shader);
      update_camera(camera, light_shader);

      mat4 m_model = GLM_MAT4_IDENTITY_INIT;

      glm_translate(m_model, light_position);
      glm_scale_uni(m_model, 0.2f);

      Shader_set_matrix4(light_shader, "model", (float*)m_model);
      Shader_set_vec3(light_shader, "light.position", (float*)light_position);
      /*Shader_set_vec3(light_shader, "light.direction", (float*)light_direction);*/
      Shader_set_vec3f(light_shader, "light.ambient",  0.2f, 0.2f, 0.2f);
      Shader_set_vec3f(light_shader, "light.diffuse",  0.6f, 0.7f, 0.8f);
      Shader_set_vec3f(light_shader, "light.specular", 1.0f, 1.0f, 1.0f);

      glBindVertexArray(lightVAO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      glBindVertexArray(0);
    }

    // Update gui
    gui_new_frame();
    gui_update_fps();
    gui_update_camera(camera);
    gui_render();

    // Draw to screen
    glfwSwapBuffers(window);
  }

  gui_terminate();
  glfwTerminate();

  return 0;
}
