#include <stdlib.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "camera.h"
#include "settings.h"
#include "manager.h"
#include "utils.h"

Camera* make_camera() {
  Camera *camera = malloc(sizeof(Camera));

  vec3 camera_pos = { 0, 0, -3 };
  vec3 camera_front = { 0, 0, 1 };
  vec3 camera_target = { 0, 0, 0 };
  vec3 camera_up = { 0, 1, 0 };
  vec3 camera_right = { 0, 0, 0 };
  mat4 projection = GLM_MAT4_IDENTITY_INIT;

  glm_vec3_copy(camera_pos   , camera->camera_pos);
  glm_vec3_copy(camera_front , camera->camera_front);
  glm_vec3_copy(camera_target, camera->camera_target);
  glm_vec3_copy(camera_up    , camera->camera_up);
  glm_vec3_copy(camera_right , camera->camera_right);

  glm_mat4_copy(projection   , camera->projection);

  camera->pitch = 0;
  camera->yaw = 0;
  camera->zoom = 45;

  return camera;
}

void update_camera_target(Camera *camera, float xoffset, float yoffset) {
  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  camera->yaw   += xoffset;
  camera->pitch += yoffset;

  if(camera->pitch > 89.0f)
    camera->pitch = 89.0f;

  if(camera->pitch < -89.0f)
    camera->pitch = -89.0f;

  camera->camera_front[0] = cos(deg2rad(camera->yaw)) * cos(deg2rad(camera->pitch));
  camera->camera_front[1] = sin(deg2rad(camera->pitch));
  camera->camera_front[2] = sin(deg2rad(camera->yaw)) * cos(deg2rad(camera->pitch));

  glm_vec3_normalize(camera->camera_front);
}

void update_camera_fov(Camera *camera, float xoffset, float yoffset) {
  camera->zoom -= yoffset;

  if (camera->zoom < 1.0f)
    camera->zoom = 1.0f;

  if (camera->zoom > 90.0f)
    camera->zoom = 90.0f;
}

void update_camera_position(Camera *camera, Direction direction) {
  const float camera_speed = 2.5f * manager->delta_time;

  vec3 tmp;

  switch (direction) {
    case UP:
      glm_vec3_scale(camera->camera_up, camera_speed, tmp);
      glm_vec3_add(camera->camera_pos, tmp, camera->camera_pos);
      break;

    case DOWN:
      glm_vec3_scale(camera->camera_up, camera_speed, tmp);
      glm_vec3_sub(camera->camera_pos, tmp, camera->camera_pos);
      break;

    case FRONT:
      glm_vec3_scale(camera->camera_front, camera_speed, tmp);
      glm_vec3_add(camera->camera_pos, tmp, camera->camera_pos);
      break;

    case BACK:
      glm_vec3_scale(camera->camera_front, camera_speed, tmp);
      glm_vec3_sub(camera->camera_pos, tmp, camera->camera_pos);
      break;

    case LEFT:
      glm_vec3_crossn(camera->camera_front, camera->camera_up, tmp);
      glm_vec3_scale(tmp, camera_speed, tmp);
      glm_vec3_sub(camera->camera_pos, tmp, camera->camera_pos);
      break;

    case RIGHT:
      glm_vec3_crossn(camera->camera_front, camera->camera_up, tmp);
      glm_vec3_scale(tmp, camera_speed, tmp);
      glm_vec3_add(camera->camera_pos, tmp, camera->camera_pos);
      break;
  }
}

void update_camera(Camera *camera, Shader *shader) {
  glm_perspective(
      deg2rad(camera->zoom),
      WINDOW_WIDTH / WINDOW_HEIGHT,
      1,
      100,
      camera->projection
      );

  Shader_set_matrix4(shader, "projection", (float*)camera->projection);

  vec3 camera_direction;

  glm_vec3_sub(camera->camera_pos, camera->camera_target, camera_direction);
  glm_vec3_normalize(camera_direction);

  glm_vec3_cross(GLM_YUP, camera_direction, camera->camera_right);
  glm_vec3_normalize(camera->camera_right);

  mat4 m_view = GLM_MAT4_IDENTITY_INIT;

  glm_vec3_add(camera->camera_pos, camera->camera_front, camera->camera_target);
  glm_lookat(camera->camera_pos, camera->camera_target, camera->camera_up, m_view);

  Shader_set_matrix4(shader, "view", (float*)m_view);

  Shader_set_vec3(shader, "viewPos", (float*)camera->camera_pos);
}
