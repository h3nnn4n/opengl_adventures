#ifndef _CAMERA_H
#define _CAMERA_H

#include <cglm/cglm.h>

typedef struct {
  vec3 camera_pos;
  vec3 camera_front;
  vec3 camera_target;
  vec3 camera_up;
  vec3 camera_right;

  float delta_time, last_frame;

  int firstMouse;
  float lastX;
  float lastY;
  float pitch;
  float yaw;
  float zoom;
} Camera;

Camera* make_camera();

#endif
