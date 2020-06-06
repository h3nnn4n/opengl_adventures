#ifndef _CAMERA_H
#define _CAMERA_H

#include <cglm/cglm.h>

#include "shader_c.h"
#include "utils.h"

typedef struct {
  vec3 camera_pos;
  vec3 camera_front;
  vec3 camera_target;
  vec3 camera_up;
  vec3 camera_right;

  mat4 projection;
  mat4 view;

  float pitch;
  float yaw;
  float zoom;

  Shader *shader;
} Camera;

Camera* make_camera();
void update_camera_target(Camera *camera, float xoffset, float yoffset);
void update_camera_fov(Camera *camera, float xoffset, float yoffset);
void update_camera_target(Camera *camera, float xoffset, float yoffset);
void update_camera_position(Camera *camera, Direction direction);

void update_camera_projection_matrix(Camera *camera, Shader *shader);
void update_camera_view_matrix(Camera *camera, Shader *shader);
void update_camera_position_matrix(Camera *camera);

#endif
