#include "ModelLoader.hpp"
#include "3DEngine/Components/MaterialComponent.hpp"
#include "3DEngine/Components/BoundingComponent.hpp"
#include "3DEngine/Components/MeshFilterComponent.hpp"
#include "3DEngine/Components/TransformComponent.hpp"
#include "3DEngine/Texture.hpp"

namespace KikooRenderer {
namespace Util {
namespace FileIO {

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
