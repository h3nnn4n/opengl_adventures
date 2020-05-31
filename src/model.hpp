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
    // stores all the textures loaded so far, optimization to make sure
    // textures aren't loaded more than once.
    vector<Texture> textures_loaded;

    Model(char *path);
    void Draw(Shader *shader);
  private:
    // model data
    vector<Mesh> meshes;
    string directory;

    void loadModel(string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

#endif
