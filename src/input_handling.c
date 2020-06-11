#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#include "camera.h"
#include "clickcolor.h"
#include "input_handling.h"
#include "manager.h"
#include "player.h"
#include "scene_loader.h"
#include "scene_save.h"
#include "settings.h"
#include "utils.h"

int firstMouse;
int left_mouse_pressed;
int right_mouse_pressed;

int ctrl_key_pressed;
int alt_key_pressed;

vec3 mouse_world_position;

float lastX;
float lastY;

int wireframe_mode = 0;

// Code from https://antongerdelan.net/opengl/raycasting.html
void update_mouse_world_position() {
  float x = (2.0f * lastX) / WINDOW_WIDTH - 1.0f;
  float y = 1.0f - (2.0f * lastY) / WINDOW_HEIGHT;
  float z = 1.0f;

  vec3 ray_nds = {x, y, z};
  vec4 ray_clip = {ray_nds[0], ray_nds[1], -1.0, 1.0};

  /*printf("\n");*/
  /*printf("rayclip: %5.2f %5.2f %5.2f %5.2f \n",*/
      /*ray_clip[0], ray_clip[1], ray_clip[2], ray_clip[3]);*/

  mat4 inverse_projection;
  glm_mat4_inv(manager->active_camera->projection, inverse_projection);

  vec4 ray_eye;
  glm_mat4_mulv(inverse_projection, ray_clip, ray_eye);
  ray_eye[2] = -1;
  ray_eye[3] =  0;
  /*printf(" rayeye: %5.2f %5.2f %5.2f %5.2f \n",*/
      /*ray_eye[0], ray_eye[1], ray_eye[2], ray_eye[3]);*/

  mat4 inverse_view;
  glm_mat4_inv(manager->active_camera->view, inverse_view);

  vec4 ray_world;
  glm_mat4_mulv(inverse_view, ray_eye, ray_world);
  glm_vec4_normalize(ray_world);

  for (int i = 0; i < 3; i++) {
    mouse_world_position[i] = ray_world[i];
  }
}

void mouse_click_callback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      right_mouse_pressed = 1;
    } else if (action == GLFW_RELEASE) {
      right_mouse_pressed = 0;
    }
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      left_mouse_pressed = 1;

      if (manager->game_mode == EDITOR) {
        if (ctrl_key_pressed || alt_key_pressed) {
          clickcolor_event();
        }

        if (alt_key_pressed) {
          add_new_block(manager);
        }
      }
    } else if (action == GLFW_RELEASE) {
      left_mouse_pressed = 0;
    }
  }
}

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

  if (manager->game_mode == FREE_CAMERA) {
    update_camera_target(manager->active_camera, xoffset, yoffset);
  }

  if (manager->game_mode == EDITOR && right_mouse_pressed) {
    update_camera_target(manager->active_camera, xoffset, yoffset);
  }

  update_mouse_world_position();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  if (manager->game_mode == FREE_CAMERA) {
    update_camera_fov(manager->active_camera, xoffset, yoffset);
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    /*glfwSetWindowShouldClose(window, 1);*/
  }

  if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
    manager->game_mode = IN_GAME;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
    /*manager->game_mode = IN_MENU;*/

    /*glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);*/
  }

  if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    ctrl_key_pressed = 1;
  } else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
    ctrl_key_pressed = 0;
  }

  if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
    alt_key_pressed = 1;
  } else if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE) {
    alt_key_pressed = 0;
  }

  if(glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
    manager->game_mode = FREE_CAMERA;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  if(glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
    manager->game_mode = EDITOR;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
    load_scene(manager, manager->current_scene_name);
  }

  if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS) {
    save_scene(manager, manager->current_scene_name);
  }

  if (manager->game_mode == FREE_CAMERA || manager->game_mode == EDITOR) {
    handle_camera_movements(window);
  } else if (manager->game_mode == IN_GAME) {
    handle_player_movements(window);
  }
}

void handle_player_movements(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    move_player(manager, FRONT);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    move_player(manager, BACK);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    move_player(manager, LEFT);
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    move_player(manager, RIGHT);
  }
}

void handle_camera_movements(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    update_camera_position(manager->active_camera, FRONT);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    update_camera_position(manager->active_camera, BACK);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    update_camera_position(manager->active_camera, LEFT);
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    update_camera_position(manager->active_camera, RIGHT);
  }

  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
    update_camera_position(manager->active_camera, DOWN);
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    update_camera_position(manager->active_camera, UP);
  }
}
