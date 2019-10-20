#include "ModelLoader.hpp"
#include "3DEngine/Components/MaterialComponent.hpp"
#include "3DEngine/Components/BoundingComponent.hpp"
#include "3DEngine/Components/MeshFilterComponent.hpp"
#include "3DEngine/Components/TransformComponent.hpp"
#include "3DEngine/Texture.hpp"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace KikooRenderer {
namespace Util {
namespace FileIO {

void GetModelBuffers(std::string filename, std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles) {
    FILE * file = fopen(filename.c_str(), "r");
    if( file == NULL ){
        std::cout << "ModelLoader:GetModelBuffers: Cannot open OBJ file" << std::endl;
        return;
    }

    std::vector<glm::vec3> tmpVertex;
    std::vector<glm::vec3> tmpNormals;
    std::vector<glm::vec2> tmpUv;
    std::vector<glm::vec4> tmpColors;
    std::vector<int> uvIndices;
    std::vector<int> normalIndices;

    while(true){

        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            tmpVertex.push_back(vertex);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            tmpUv.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            tmpNormals.push_back(normal);
        } else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                std::cout << "ModelLoader:GetModelBuffers: Cannot read OBJ : Try exporting with other options " <<std::endl;
                return;
            }
            triangles->push_back(vertexIndex[0] - 1);
            triangles->push_back(vertexIndex[1] - 1);
            triangles->push_back(vertexIndex[2] - 1);
            uvIndices    .push_back(uvIndex[0] - 1);
            uvIndices    .push_back(uvIndex[1] - 1);
            uvIndices    .push_back(uvIndex[2] - 1);
            normalIndices.push_back(normalIndex[0] - 1);
            normalIndices.push_back(normalIndex[1] - 1);
            normalIndices.push_back(normalIndex[2] - 1);
        }     
    }
    (*vertex) = tmpVertex;
    (*normals).resize(triangles->size(), glm::vec3(1, 0, 0));
    (*uv).resize(triangles->size(), glm::vec2(0, 0));
    (*colors).resize(triangles->size(), glm::vec4(1, 1, 1, 1));

    for(int i=0; i<triangles->size(); i++) {
        (*normals)[(*triangles)[i]] = tmpNormals[normalIndices[i]];
        (*uv)[(*triangles)[i]] = tmpUv[uvIndices[i]];
    }  
}

void processNode(aiNode *node, const aiScene *scene, std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles) {
    // process all the node's meshes (if any)
    std::cout << "ModelLoader:processNode: Processing node " <<node->mNumMeshes <<  std::endl;
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            glm::vec3 pos; 
            pos.x = mesh->mVertices[i].x;
            pos.y = mesh->mVertices[i].y;
            pos.z = mesh->mVertices[i].z; 
            vertex->push_back(pos);

            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            normals->push_back(normal);

            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                uv->push_back(vec);
            }
            else
                uv->push_back(glm::vec2(0.0f, 0.0f));
        }

        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                triangles->push_back(face.mIndices[j]);
        } 
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, vertex, normals, uv, colors, triangles);
    }
}

void LoadModel(std::string filename, std::vector<glm::vec3>* vertex, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uv, std::vector<glm::vec4>* colors, std::vector<int>* triangles) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FlipWindingOrder); 

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ModelLoader:LoaderModel: ERROR::" << import.GetErrorString() << std::endl;
        return;
    }
    std::string directory = filename.substr(0, filename.find_last_of('/'));

    for(unsigned int j = 0; j < scene->mNumMeshes; j++) {
        aiMesh *mesh = scene->mMeshes[j]; 
        std::cout << "NUM VERTICES " << mesh->mNumVertices << std::endl;
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            glm::vec3 pos; 
            pos.x = mesh->mVertices[i].x;
            pos.y = mesh->mVertices[i].y;
            pos.z = mesh->mVertices[i].z; 
            vertex->push_back(pos);

            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            normals->push_back(normal);

            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                uv->push_back(vec);
            }
            else
                uv->push_back(glm::vec2(0.0f, 0.0f));
        }

        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                triangles->push_back(face.mIndices[j]);
        }
    }
    // processNode(scene->mRootNode, scene, vertex, normals, uv, colors, triangles);
    colors->resize(vertex->size());
}

}
}
}
