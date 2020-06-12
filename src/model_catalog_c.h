#ifndef _MODEL_CATALOG_C_H
#define _MODEL_CATALOG_C_H

#include "model_c.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct ModelCatalog ModelCatalog;

  extern ModelCatalog *modelCatalog;

  ModelCatalog* newModelCatalog(char* path);
  Model* ModelCatalog_get_model(ModelCatalog* modelCatalog, char* path);
  void ModelCatalog_destroy(ModelCatalog* model);

#ifdef __cplusplus
}
#endif

#endif
