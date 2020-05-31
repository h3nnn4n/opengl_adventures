#ifndef _INPUT_HANDLING_H
#define _INPUT_HANDLING_H

#include <GLFW/glfw3.h>

#include "camera.h"

extern Camera *camera;
extern int firstMouse;
extern float lastX;
extern float lastY;
extern int locked_cursor;
extern int wireframe_mode;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif
