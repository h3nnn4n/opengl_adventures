#include <iostream>

#include "model_catalog.hpp"

Model* ModelCatalog::get_model(char* &path) {
  for (unsigned int i = 0; i < model_catalog.size(); ++i) {
    if (std::strcmp(model_catalog[i]->path.data(), path) == 0) {
      return model_catalog[i];
    }
  }

  Model *model = new Model(path);

  model_catalog.push_back(model);

  return model;
}
