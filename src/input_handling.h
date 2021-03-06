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

extern vec3 mouse_world_position;

extern int left_mouse_pressed;
extern int right_mouse_pressed;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void handle_camera_movements(GLFWwindow* window);
void handle_player_movements(GLFWwindow* window);

#endif
