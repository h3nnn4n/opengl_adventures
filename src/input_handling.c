#include <GLFW/glfw3.h>

#include "input_handling.h"
#include "manager.h"

int firstMouse;
int left_mouse_pressed;
int right_mouse_pressed;

float lastX;
float lastY;

int wireframe_mode = 0;

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
    glfwSetWindowShouldClose(window, 1);
  }

  if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
    manager->game_mode = IN_GAME;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
    manager->game_mode = IN_MENU;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  if(glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
    manager->game_mode = FREE_CAMERA;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  if(glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
    manager->game_mode = EDITOR;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  if (manager->game_mode == FREE_CAMERA || manager->game_mode == EDITOR) {
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
}
