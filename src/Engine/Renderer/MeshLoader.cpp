#include "MeshLoader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Hollow {

MeshComponent MeshLoader::LoadFromFile(const std::string& path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices
    );

    MeshComponent mesh;

    if (!scene || !scene->mRootNode || scene->mNumMeshes == 0)
        return mesh;

    aiMesh* ai_mesh = scene->mMeshes[0];

    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
    {
        vertices.push_back(ai_mesh->mVertices[i].x);
        vertices.push_back(ai_mesh->mVertices[i].y);
        vertices.push_back(ai_mesh->mVertices[i].z);
    }

    for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
    {
        const aiFace& face = ai_mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    mesh.Setup(vertices, indices);

    return mesh;
}

}