#include "time.h"

float currentFrame = 0;
float lastFrame = 0;
float deltaTime = 0;

void update_delta(float current_time) {
  lastFrame = currentFrame;
  currentFrame = current_time;
  deltaTime = currentFrame - lastFrame;
}

float delta_time() {
  return deltaTime;
}
