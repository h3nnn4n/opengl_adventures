#include "model.hpp"
#include "model_c.h"

Model* newModel(char* path) {
  return new Model(path);
}

void Model_draw(Model* model, Shader* shader) {
  model->Draw(shader);
}

void Model_destroy(Model* model) {
  delete model;
}
