#ifndef _MODEL_H
#define _MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "mesh.hpp"
#include "shader.hpp"

using namespace std;

class Model
{
  public:
    vector<Texture> textures_loaded;
    string path;

    Model(char *path);
    void Draw(Shader *shader);

  private:
    string directory;
    vector<Mesh> meshes;

    void loadModel(string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

unsigned int TextureFromFile(const char *path, const string &directory, string typeName);

#endif
