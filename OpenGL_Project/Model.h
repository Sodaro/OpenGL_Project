#pragma once
#include <vector>
#include <string>
#include <assimp/scene.h>
#include "Mesh.h"

unsigned int texture_from_file(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
    Model(const char* path)
    {
        load_model(path);
    }
    void Draw(Shader& shader);
private:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    void load_model(std::string path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};