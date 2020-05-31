#include <GLFW/glfw3.h>

#include "input_handling.h"

int firstMouse;
float lastX;
float lastY;

int locked_cursor = 1;

extern Camera *camera;

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

  if (!locked_cursor) return;

  update_camera_target(camera, xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if (!locked_cursor) return;

  update_camera_fov(camera, xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
    locked_cursor = !locked_cursor;

    if (locked_cursor) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }

  if (!locked_cursor) return;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    update_camera_position(camera, FRONT);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    update_camera_position(camera, BACK);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    update_camera_position(camera, LEFT);
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    update_camera_position(camera, RIGHT);
  }

  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
    update_camera_position(camera, DOWN);
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    update_camera_position(camera, UP);
  }
}
