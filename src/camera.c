#include <stdlib.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "camera.h"

Camera* make_camera() {
  Camera *camera = malloc(sizeof(Camera));

  vec3 camera_pos = { 0, 0, -3 };
  vec3 camera_front = { 0, 0, 1 };
  vec3 camera_target = { 0, 0, 0 };
  vec3 camera_up = { 0, 1, 0 };
  vec3 camera_right = { 0, 0, 0 };

  glm_vec3_copy(camera_pos   , camera->camera_pos);
  glm_vec3_copy(camera_front , camera->camera_front);
  glm_vec3_copy(camera_target, camera->camera_target);
  glm_vec3_copy(camera_up    , camera->camera_up);
  glm_vec3_copy(camera_right , camera->camera_right);

  camera->delta_time = 0;
  camera->last_frame = 0;

  camera->firstMouse = 1;
  camera->lastX = 400;
  camera->lastY = 300;
  camera->pitch = 0;
  camera->yaw = 0;
  camera->zoom = 45;

  return camera;
}
