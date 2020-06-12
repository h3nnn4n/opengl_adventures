#include "model_catalog.hpp"

#include "model.hpp"

#include "model_catalog_c.h"

ModelCatalog *modelCatalog = new ModelCatalog();

ModelCatalog* newModelCatalog() {
  return new ModelCatalog();
}

Model* ModelCatalog_get_model(ModelCatalog* modelCatalog, char* path) {
  return modelCatalog->get_model(path);
}

void ModelCatalog_destroy(ModelCatalog *modelCatalog) {
  delete modelCatalog;
}
