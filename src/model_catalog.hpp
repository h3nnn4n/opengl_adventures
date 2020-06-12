#ifndef MODEL_CATALOG_H
#define MODEL_CATALOG_H

#include <vector>

#include "model.hpp"

class ModelCatalog
{
  public:
    Model* get_model(char* &path);

  private:
    std::vector<Model*> model_catalog;
};

#endif /* MODEL_CATALOG_H */
