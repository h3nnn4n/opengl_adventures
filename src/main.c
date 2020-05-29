#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "shader_c.h"
#include "stb_image.h"
#include "utils.h"

vec3 camera_pos = { 0, 0, 3 };
vec3 camera_front = { 0, 0, 3 };
vec3 camera_target = { 0, 0, 0 };
vec3 camera_up = { 0, 1, 0 };
vec3 camera_right = { 0, 0, 0 };

float delta_time, last_frame;

int firstMouse = 1;
float lastX, lastY;
float pitch, yaw;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = 0;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw   += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f)
    pitch =  89.0f;
  if(pitch < -89.0f)
    pitch = -89.0f;

  camera_front[0] = cos(deg2rad(yaw)) * cos(deg2rad(pitch));
  camera_front[1] = sin(deg2rad(pitch));
  camera_front[2] = sin(deg2rad(yaw)) * cos(deg2rad(pitch));
  glm_vec3_normalize(camera_front);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  const float camera_speed = 2.5f * delta_time;

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    vec3 tmp;
    glm_vec3_scale(camera_front, camera_speed, tmp);
    glm_vec3_add(camera_pos, tmp, camera_pos);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    vec3 tmp;
    glm_vec3_scale(camera_front, camera_speed, tmp);
    glm_vec3_sub(camera_pos, tmp, camera_pos);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    vec3 tmp;
    glm_vec3_crossn(camera_front, camera_up, tmp);
    glm_vec3_scale(tmp, camera_speed, tmp);
    glm_vec3_sub(camera_pos, tmp, camera_pos);
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    vec3 tmp;
    glm_vec3_crossn(camera_front, camera_up, tmp);
    glm_vec3_scale(tmp, camera_speed, tmp);
    glm_vec3_add(camera_pos, tmp, camera_pos);
  }
}

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  Shader *shader = newShader("shaders/shader.vert", "shaders/shader.frag");
  Shader_use(shader);
  Shader_set_float(shader, "colorOverrideIntensity", 0.5);

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
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  unsigned int VBO, VAO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);*/
  /*glEnableVertexAttribArray(0);*/

  /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));*/
  /*glEnableVertexAttribArray(1);*/

  /*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));*/
  /*glEnableVertexAttribArray(2);*/

  /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);*/
  /*glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

  glBindVertexArray(0);

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
    const char *tex_filename = "assets/container.jpg";
    unsigned char *data = stbi_load(tex_filename, &width, &height, &nrChannels, 0);

    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
    const char *tex_filename = "assets/awesomeface.png";
    unsigned char *data = stbi_load(tex_filename, &width, &height, &nrChannels, 0);

    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      fprintf(stderr, "Failed to load %s", tex_filename);
    }

    stbi_image_free(data);
  }

  Shader_set_int(shader, "texture1", 0);
  Shader_set_int(shader, "texture2", 1);

  /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
  /*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

  ////////////////////////
  // View projection

  mat4 m_projection = GLM_MAT4_IDENTITY_INIT;
  glm_perspective(45 * GLM_PI / 180.0, 800 / 600, 1, 100, m_projection);

  Shader_set_matrix4(shader, "projection", (float*)m_projection);

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
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float timer = glfwGetTime();

    delta_time = timer - last_frame;
    last_frame = timer;

    Shader_set_float(shader, "time", timer);

    {
      vec3 camera_direction;
      glm_vec3_sub(camera_pos, camera_target, camera_direction);
      glm_vec3_normalize(camera_direction);

      glm_vec3_cross(GLM_YUP, camera_direction, camera_right);
      glm_vec3_normalize(camera_right);

      mat4 m_view = GLM_MAT4_IDENTITY_INIT;
      glm_vec3_add(camera_pos, camera_front, camera_target);
      glm_lookat(camera_pos, camera_target, camera_up, m_view);
      Shader_set_matrix4(shader, "view", (float*)m_view);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    /*glBindTexture(GL_TEXTURE_2D, texture);*/

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);

    {
      for (int i = 0; i < 10; ++i) {
        vec3 v_scale = GLM_VEC3_ONE_INIT;
        /*glm_vec3_scale(v_scale, 0.5, v_scale);*/

        /*vec3 v_translate = GLM_VEC3_ONE_INIT;*/
        /*glm_vec3_scale(v_translate, 0.1, v_translate);*/
        /*v_translate[2] = 0;*/

        mat4 m_model = GLM_MAT4_IDENTITY_INIT;

        glm_translate(m_model, cube_positions[i]);
        glm_rotate(m_model, (25 * GLM_PI / 180.0) * timer * i / 3.0, GLM_XUP);
        glm_rotate(m_model, (50 * GLM_PI / 180.0) * timer * i / 2.0, GLM_YUP);
        glm_scale(m_model, v_scale);

        Shader_set_matrix4(shader, "model", (float*)m_model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

    /*glDrawArrays(GL_TRIANGLES, 0, 3);*/
    /*glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
    /*glDrawArrays(GL_TRIANGLES, 0, 36);*/

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
