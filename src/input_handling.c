#include <GLFW/glfw3.h>

#include "input_handling.h"
#include "manager.h"

int firstMouse;
float lastX;
float lastY;

int wireframe_mode = 0;

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

  if (manager->game_mode == FREE_CAMERA || manager->game_mode == EDITOR) {
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
  if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
    wireframe_mode = !wireframe_mode;
  }

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }

  if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
    manager->game_mode = IN_GAME;
  }

  if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
    manager->game_mode = IN_MENU;
  }

  if(glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
    manager->game_mode = FREE_CAMERA;
  }

  if(glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
    manager->game_mode = EDITOR;
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
