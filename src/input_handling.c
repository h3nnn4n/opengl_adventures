#include <GLFW/glfw3.h>

#include "input_handling.h"

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

  update_camera_target(camera, xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  update_camera_fov(camera, xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }

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
