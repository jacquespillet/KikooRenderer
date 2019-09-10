#include "ModelLoader.hpp"
#include "3DEngine/Components/MaterialComponent.hpp"
#include "3DEngine/Components/BoundingComponent.hpp"
#include "3DEngine/Components/MeshFilterComponent.hpp"
#include "3DEngine/Components/TransformComponent.hpp"
#include "3DEngine/Texture.hpp"

namespace KikooRenderer {
namespace Util {
namespace FileIO {

KikooRenderer::CoreEngine::Object3D* ObjectFromOBJ(std::string fileName, KikooRenderer::CoreEngine::Scene* scene) {
    FILE * file = fopen(fileName.c_str(), "r");
    if( file == NULL ){
        std::cout << "Cannot open OBJ file" << std::endl;
        return nullptr;
    }

    KikooRenderer::CoreEngine::Object3D* newObject = new KikooRenderer::CoreEngine::Object3D("New Game object", scene);
    std::vector<glm::dvec3> tmpVertex;
    std::vector<glm::dvec3> tmpNormals;
    std::vector<glm::dvec2> tmpUv;
    std::vector<glm::dvec4> tmpColors;
    std::vector<int> vertexIndices;
    std::vector<int> uvIndices;
    std::vector<int> normalIndices;

    std::vector<glm::dvec3> outVertex;
    std::vector<glm::dvec3> outNormals;
    std::vector<glm::dvec2> outUv;
    std::vector<glm::dvec4> outColors;

    std::vector<int> triangles;

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
                std::cout << "Cannot read OBJ : Try exporting with other options " <<std::endl;
                return nullptr;
            }
            vertexIndices.push_back(vertexIndex[0] - 1);
            vertexIndices.push_back(vertexIndex[1] - 1);
            vertexIndices.push_back(vertexIndex[2] - 1);
            uvIndices    .push_back(uvIndex[0] - 1);
            uvIndices    .push_back(uvIndex[1] - 1);
            uvIndices    .push_back(uvIndex[2] - 1);
            normalIndices.push_back(normalIndex[0] - 1);
            normalIndices.push_back(normalIndex[1] - 1);
            normalIndices.push_back(normalIndex[2] - 1);
        }     
    }
    outVertex = tmpVertex;
    outNormals.resize(vertexIndices.size(), glm::dvec3(1, 0, 0));
    outUv.resize(vertexIndices.size(), glm::dvec2(0, 0));
    outColors.resize(vertexIndices.size(), glm::dvec4(1, 1, 1, 1));

    //Use Max of tmp arrays in case they are empty
    for(int i=0; i<vertexIndices.size(); i++) {
        outNormals[vertexIndices[i]] = tmpNormals[normalIndices[i]];
        outUv[vertexIndices[i]] = tmpUv[uvIndices[i]];
    }

    KikooRenderer::CoreEngine::MeshFilterComponent* mesh = new KikooRenderer::CoreEngine::MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( outVertex, outNormals, outUv, outColors, vertexIndices, true);

    //Setup transform
    KikooRenderer::CoreEngine::TransformComponent* transform = new  KikooRenderer::CoreEngine::TransformComponent(newObject );
    
    //Setup material
    KikooRenderer::CoreEngine::MaterialComponent* material = new  KikooRenderer::CoreEngine::MaterialComponent(newObject);
    material->SetShader(&scene->standardShaders.blinnPhongShader);

    // material->albedoTex = KikooRenderer::CoreEngine::Texture("C:/Users/Jacques/Documents/Boulot/2019/3D Models/E-45-Aircraft/textures/E-45 _col.jpg", GL_TEXTURE0);
    // // material->specularTex = KikooRenderer::CoreEngine::Texture("C:/Users/Jacques/Documents/Boulot/2019/3D Models/Cobblestones/Textures/BrickRound0105_5_SPEC.png", GL_TEXTURE1);
    // material->normalTex = KikooRenderer::CoreEngine::Texture("C:/Users/Jacques/Documents/Boulot/2019/3D Models/E-45-Aircraft/textures/E-45-nor_1.jpg", GL_TEXTURE2);

    KikooRenderer::CoreEngine::BoundingBoxComponent* boundingBox = new  KikooRenderer::CoreEngine::BoundingBoxComponent(newObject);


    newObject->AddComponent(material);
    newObject->AddComponent(transform);
    newObject->AddComponent(mesh);
    newObject->AddComponent(boundingBox);

    return newObject;
}

void GetModelBuffers(std::string filename, std::vector<glm::dvec3>* vertex, std::vector<glm::dvec3>* normals, std::vector<glm::dvec2>* uv, std::vector<glm::dvec4>* colors, std::vector<int>* triangles) {
    FILE * file = fopen(filename.c_str(), "r");
    if( file == NULL ){
        std::cout << "Cannot open OBJ file" << std::endl;
        return;
    }

    std::vector<glm::dvec3> tmpVertex;
    std::vector<glm::dvec3> tmpNormals;
    std::vector<glm::dvec2> tmpUv;
    std::vector<glm::dvec4> tmpColors;
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
                std::cout << "Cannot read OBJ : Try exporting with other options " <<std::endl;
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
    (*normals).resize(triangles->size(), glm::dvec3(1, 0, 0));
    (*uv).resize(triangles->size(), glm::dvec2(0, 0));
    (*colors).resize(triangles->size(), glm::dvec4(1, 1, 1, 1));

    for(int i=0; i<triangles->size(); i++) {
        (*normals)[(*triangles)[i]] = tmpNormals[normalIndices[i]];
        (*uv)[(*triangles)[i]] = tmpUv[uvIndices[i]];
    }  
}

}
}
}
