#include <glad/glad.h>

#include <stb_image.h>

#include "camera.h"
#include "manager.h"
#include "skybox.h"
#include "shader_c.h"

unsigned int skybox_textureID;
unsigned int skybox_VAO;
unsigned int skybox_VBO;

Shader *shader_skybox;

float skyboxVertices[] = {
  -1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  -1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f
};

void make_skybox() {
  // Model stuff
  glGenVertexArrays(1, &skybox_VAO);
  glGenBuffers(1, &skybox_VBO);
  glBindVertexArray(skybox_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // Texture stuff
  glGenTextures(1, &skybox_textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_textureID);

  int width, height, nrChannels;
  unsigned char *data;

  char *faces[6] = {
    "assets/skybox_space/space_right.jpg",
    "assets/skybox_space/space_left.jpg",
    "assets/skybox_space/space_up.jpg",
    "assets/skybox_space/space_down.jpg",
    "assets/skybox_space/space_front.jpg",
    "assets/skybox_space/space_back.jpg"
  };

  stbi_set_flip_vertically_on_load(0);

  for(GLuint i = 0; i < 6; i++) {
    data = stbi_load(faces[i], &width, &height, &nrChannels, 0);

    if (data) {
      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
      );
    } else {
      printf("Error loading %s\n", faces[i]);
    }

    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  shader_skybox = newShader("shaders/skybox/shader.vert", "shaders/skybox/shader.frag");

  Shader_use(shader_skybox);
  Shader_set_int(shader_skybox, "skybox", 0);
}

void render_skybox() {
  Shader_reload_changes(shader_skybox);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glDepthFunc(GL_LEQUAL);

  Shader_use(shader_skybox);

  update_camera_projection_matrix(manager->active_camera, shader_skybox);
  update_camera_view_matrix(manager->active_camera, shader_skybox);

  {
    mat3 view3 = GLM_MAT3_ZERO_INIT;
    mat4 view = GLM_MAT4_ZERO_INIT;
    glm_mat4_pick3(manager->active_camera->view, view3);
    glm_mat4_ins3(view3, view);
    Shader_set_matrix4(shader_skybox, "view", (float*)view);
  }

  glBindVertexArray(skybox_VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_textureID);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);

  glDepthFunc(GL_LESS);
}
