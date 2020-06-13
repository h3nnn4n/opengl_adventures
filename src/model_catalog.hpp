#ifndef MODEL_CATALOG_H
#define MODEL_CATALOG_H

#include <vector>

#include "model.hpp"

class ModelCatalog
{
  public:
    Model* get_model(char* &path);
    unsigned int get_texture_id(const char* path, string typeName);

  private:
    std::vector<Model*> model_catalog;
    std::vector<string> texture_catalog;
    std::vector<unsigned int> texture_catalog_id;
};

unsigned int TextureFromFile(const char *path, string typeName);

#endif /* MODEL_CATALOG_H */
