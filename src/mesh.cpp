#include <string>
#include <vector>

#include "mesh.hpp"

using namespace std;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setupMesh();
}

void Mesh::setupMesh()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}

void Mesh::Draw(Shader *shader) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;

  for(unsigned int i = 0; i < textures.size(); i++) {
    // activate proper texture unit before binding
    glActiveTexture(GL_TEXTURE0 + i);

    // retrieve texture number (the N in diffuse_textureN)
    string number;
    string name = textures[i].type;

    if(name == "texture_diffuse") {
      if (diffuseNr > 1) cout << "Warning, ignoring " << name;
      number = std::to_string(diffuseNr++);
    } else if(name == "texture_specular") {
      if (specularNr > 1) cout << "Warning, ignoring " << name;
      number = std::to_string(specularNr++);
    }

    //string uniform_name = "material." + name + number;
    string uniform_name = "material." + name;

    shader->setInt(uniform_name.c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  glActiveTexture(GL_TEXTURE0);

  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
