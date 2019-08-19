#include "BaseMeshes.hpp"

namespace KikooRenderer {
namespace CoreEngine {
    
    MeshFilterComponent* GetCubeMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object){
        std::vector<glm::dvec3> vertex;
        std::vector<glm::dvec3> normals;
        std::vector<glm::dvec2> uv;
        std::vector<glm::dvec4> colors;
        std::vector<int> triangles;

        //
        ///Vertices
        //

        //Front
        vertex.push_back(glm::dvec3(-0.5, -0.5, -0.5)); //bottom left
        vertex.push_back(glm::dvec3(-0.5, 0.5, -0.5)); // top left
        vertex.push_back(glm::dvec3(0.5, 0.5, -0.5)); //Top right
        vertex.push_back(glm::dvec3(0.5, -0.5, -0.5)); //Bottom right

        // Back
        vertex.push_back(glm::dvec3(0.5, -0.5, 0.5)); //Bottom Left
        vertex.push_back(glm::dvec3(0.5, 0.5, 0.5)); //Top left
        vertex.push_back(glm::dvec3(-0.5, 0.5, 0.5)); // top right
        vertex.push_back(glm::dvec3(-0.5, -0.5, 0.5)); //bottom right

        // Right
        vertex.push_back(glm::dvec3(0.5, -0.5, -0.5)); // Bottom left
        vertex.push_back(glm::dvec3(0.5, 0.5, -0.5)); //Top left
        vertex.push_back(glm::dvec3(0.5, 0.5, 0.5)); //Top right
        vertex.push_back(glm::dvec3(0.5, -0.5, 0.5)); //Bottom right

        // Left
        vertex.push_back(glm::dvec3(-0.5, -0.5, 0.5)); //Bottom Left
        vertex.push_back(glm::dvec3(-0.5, 0.5, 0.5)); //Top left
        vertex.push_back(glm::dvec3(-0.5, 0.5, -0.5)); //Top right
        vertex.push_back(glm::dvec3(-0.5, -0.5,-0.5)); // Bottom right

        // Top
        vertex.push_back(glm::dvec3(-0.5, 0.5, -0.5)); //Bottom Left
        vertex.push_back(glm::dvec3(-0.5, 0.5, 0.5)); //Top Left
        vertex.push_back(glm::dvec3(0.5, 0.5, 0.5)); // top right
        vertex.push_back(glm::dvec3(0.5, 0.5, -0.5)); //bottom right

        // Bottom
        vertex.push_back(glm::dvec3( -0.5, -0.5,  0.5)); //bottom left
        vertex.push_back(glm::dvec3( -0.5, -0.5, -0.5)); // top left
        vertex.push_back(glm::dvec3(0.5, -0.5, -0.5)); //Top right
        vertex.push_back(glm::dvec3(0.5, -0.5,  0.5)); //Bottom right


        //
        ///Normals
        //

        //Front
        normals.push_back(glm::dvec3(0, 0, -1));
        normals.push_back(glm::dvec3(0, 0, -1));
        normals.push_back(glm::dvec3(0, 0, -1));
        normals.push_back(glm::dvec3(0, 0, -1));
        
        //Back
        normals.push_back(glm::dvec3(0, 0, 1));
        normals.push_back(glm::dvec3(0, 0, 1));
        normals.push_back(glm::dvec3(0, 0, 1));
        normals.push_back(glm::dvec3(0, 0, 1));
        
        //Right
        normals.push_back(glm::dvec3(1, 0, 0));
        normals.push_back(glm::dvec3(1, 0, 0));
        normals.push_back(glm::dvec3(1, 0, 0));
        normals.push_back(glm::dvec3(1, 0, 0));

        //Left
        normals.push_back(glm::dvec3(-1, 0, 0));
        normals.push_back(glm::dvec3(-1, 0, 0));
        normals.push_back(glm::dvec3(-1, 0, 0));
        normals.push_back(glm::dvec3(-1, 0, 0));
        
        //Top
        normals.push_back(glm::dvec3(0, 1, 0));
        normals.push_back(glm::dvec3(0, 1, 0));
        normals.push_back(glm::dvec3(0, 1, 0));
        normals.push_back(glm::dvec3(0, 1, 0));

        //Bottom
        normals.push_back(glm::dvec3(0, -1, 0));
        normals.push_back(glm::dvec3(0, -1, 0));
        normals.push_back(glm::dvec3(0, -1, 0));
        normals.push_back(glm::dvec3(0, -1, 0));


        //
        ////UV
        //

        //Front
        uv.push_back(glm::dvec2(0, 1));
        uv.push_back(glm::dvec2(0, 0));
        uv.push_back(glm::dvec2(1, 0));
        uv.push_back(glm::dvec2(1, 1));
        
        //Back
        uv.push_back(glm::dvec2(0, 1));
        uv.push_back(glm::dvec2(0, 0));
        uv.push_back(glm::dvec2(1, 0));
        uv.push_back(glm::dvec2(1, 1));
        
        //Right
        uv.push_back(glm::dvec2(0, 1));
        uv.push_back(glm::dvec2(0, 0));
        uv.push_back(glm::dvec2(1, 0));
        uv.push_back(glm::dvec2(1, 1));
        
        //Left
        uv.push_back(glm::dvec2(0, 1));
        uv.push_back(glm::dvec2(0, 0));
        uv.push_back(glm::dvec2(1, 0));
        uv.push_back(glm::dvec2(1, 1));
        
        //Top
        uv.push_back(glm::dvec2(0, 1));
        uv.push_back(glm::dvec2(0, 0));
        uv.push_back(glm::dvec2(1, 0));
        uv.push_back(glm::dvec2(1, 1));
        
        //Bottom
        uv.push_back(glm::dvec2(0, 1));
        uv.push_back(glm::dvec2(0, 0));
        uv.push_back(glm::dvec2(1, 0));
        uv.push_back(glm::dvec2(1, 1));


        //
        ////Colors
        //

        //Front
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        colors.push_back(glm::dvec4(0, 255, 0, 255));
        colors.push_back(glm::dvec4(0, 0, 255, 255));
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        
        //Back
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        colors.push_back(glm::dvec4(0, 255, 0, 255));
        colors.push_back(glm::dvec4(0, 0, 255, 255));
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        
        //Right
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        colors.push_back(glm::dvec4(0, 255, 0, 255));
        colors.push_back(glm::dvec4(0, 0, 255, 255));
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        
        //Left
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        colors.push_back(glm::dvec4(0, 255, 0, 255));
        colors.push_back(glm::dvec4(0, 0, 255, 255));
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        
        //Top
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        colors.push_back(glm::dvec4(0, 255, 0, 255));
        colors.push_back(glm::dvec4(0, 0, 255, 255));
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        
        //Bottom
        colors.push_back(glm::dvec4(255, 0, 0, 255));
        colors.push_back(glm::dvec4(0, 255, 0, 255));
        colors.push_back(glm::dvec4(0, 0, 255, 255));
        colors.push_back(glm::dvec4(255, 0, 0, 255));


        //
        ////Triangles
        //

        //Front
        int index = 0;
        triangles.push_back(index + 0);
        triangles.push_back(index + 2);
        triangles.push_back(index + 1);
        triangles.push_back(index + 3);
        triangles.push_back(index + 2);
        triangles.push_back(index + 0);
        
        index = 4;
        triangles.push_back(index + 0);
        triangles.push_back(index + 2);
        triangles.push_back(index + 1);
        triangles.push_back(index + 3);
        triangles.push_back(index + 2);
        triangles.push_back(index + 0);

        index = 8;
        triangles.push_back(index + 0);
        triangles.push_back(index + 2);
        triangles.push_back(index + 1);
        triangles.push_back(index + 3);
        triangles.push_back(index + 2);
        triangles.push_back(index + 0);

        index = 12;
        triangles.push_back(index + 0);
        triangles.push_back(index + 2);
        triangles.push_back(index + 1);
        triangles.push_back(index + 3);
        triangles.push_back(index + 2);
        triangles.push_back(index + 0);

        index = 16;
        triangles.push_back(index + 0);
        triangles.push_back(index + 2);
        triangles.push_back(index + 1);
        triangles.push_back(index + 3);
        triangles.push_back(index + 2);
        triangles.push_back(index + 0);
        
        index = 20;
        triangles.push_back(index + 0);
        triangles.push_back(index + 2);
        triangles.push_back(index + 1);
        triangles.push_back(index + 3);
        triangles.push_back(index + 2);
        triangles.push_back(index + 0);

        //Setup mesh
        MeshFilterComponent* mesh = new MeshFilterComponent(object);
        mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, true);

        QJsonObject json;
        json["Type"] = QString("Primitive");
        json["Primitive"] = QString("Cube");
        mesh->jsonObj = json;
        
        return mesh;
    }

    MeshFilterComponent* GetSphereMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object){
        std::vector<glm::dvec3> vertex;
        std::vector<glm::dvec3> normals;
        std::vector<glm::dvec2> uv;
        std::vector<glm::dvec4> colors;
        std::vector<int> triangles;


        uint32_t numSlices =64;
        float radius = 1.0;

        for(int x=0, inx = 0; x<=numSlices; x++) {
            for(int y=0; y<=numSlices; y++, inx++) {
                float xAngle = ((float)x / (float)numSlices) * PI;
                float yAngle = ((float)y / (float)numSlices) * TWO_PI;
                
                float posx = radius * std::sin(xAngle) * std::cos(yAngle);
                float posz = radius * std::sin(xAngle) * std::sin(yAngle);
                float posy = radius * std::cos(xAngle);

                vertex.push_back(glm::dvec3(posx, posy, posz));
                normals.push_back(glm::dvec3(posx, posy, posz));
                uv.push_back(glm::dvec2(0, 0));
                colors.push_back(glm::dvec4(255, 255, 255, 255));

                if(y < numSlices) {
                    triangles.push_back(inx + 1);
                    triangles.push_back(inx);
                    triangles.push_back(inx + numSlices+1);

                    triangles.push_back(inx + numSlices + 1);
                    triangles.push_back(inx);
                    triangles.push_back(inx + numSlices);
                } else { // If last of the row
                    triangles.push_back(inx);
                    triangles.push_back(inx - numSlices);
                    triangles.push_back(inx + 1);

                    triangles.push_back(inx);
                    triangles.push_back(inx + 1);
                    triangles.push_back(inx + numSlices);
                }

            }        
        }

        //Setup mesh
        MeshFilterComponent* mesh = new MeshFilterComponent(object);
        mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, false);
        
        QJsonObject json;
        json["Type"] = QString("Primitive");
        json["Primitive"] = QString("Sphere");
        mesh->jsonObj = json;
        return mesh;
    }
    
    MeshFilterComponent* GetCircleMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object){
        std::vector<glm::dvec3> vertex;
        std::vector<glm::dvec3> normals;
        std::vector<glm::dvec2> uv;
        std::vector<glm::dvec4> colors;
        std::vector<int> triangles;


        uint32_t numSlices = 16;

        vertex.push_back(glm::dvec3(0, 0, 0));
        normals.push_back(glm::dvec3(0, 0, -1));
        uv.push_back(glm::dvec2(0, 0));
        colors.push_back(glm::dvec4(255, 255, 255, 255));

        for(uint32_t i=0; i<=numSlices; i++) {
            float inx = ((float)i / (float)numSlices) * TWO_PI;
            float x = std::cos(inx);
            float y = std::sin(inx);

            vertex.push_back(glm::dvec3(x, y, 0));
            normals.push_back(glm::dvec3(0, 0, -1));
            uv.push_back(glm::dvec2(0, 0));
            colors.push_back(glm::dvec4(255, 255, 255, 255));
            
            triangles.push_back(i);
            triangles.push_back(i-1);
            triangles.push_back(0);
        }
        
        triangles.push_back(numSlices);
        triangles.push_back(1);
        triangles.push_back(0);
        //Setup mesh
        MeshFilterComponent* mesh = new MeshFilterComponent(object);
        mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, false);


        QJsonObject json;
        json["Type"] = QString("Primitive");
        json["Primitive"] = QString("Circle");
        mesh->jsonObj = json;        
        return mesh;

    }
    
    MeshFilterComponent* GetConeMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object){
        std::vector<glm::dvec3> vertex;
        std::vector<glm::dvec3> normals;
        std::vector<glm::dvec2> uv;
        std::vector<glm::dvec4> colors;
        std::vector<int> triangles;


        uint32_t numSlices = 16;

        //Circle center
        vertex.push_back(glm::dvec3(0, 0, 0));
        normals.push_back(glm::dvec3(0, 0, -1));
        uv.push_back(glm::dvec2(0, 0));
        colors.push_back(glm::dvec4(255, 255, 255, 255));

        //Peak of the cone
        vertex.push_back(glm::dvec3(0, 0, 1));
        normals.push_back(glm::dvec3(0, 0, -1));
        uv.push_back(glm::dvec2(0, 0));
        colors.push_back(glm::dvec4(255, 255, 255, 255));


        float radius = 0.25 ;
        for(uint32_t i=0; i<=numSlices; i++) {
            float inx = ((float)i / (float)numSlices) * TWO_PI;
            float x = radius * std::cos(inx);
            float y = radius * std::sin(inx);

            vertex.push_back(glm::dvec3(x, y, 0));
            normals.push_back(glm::dvec3(0, 0, -1));
            uv.push_back(glm::dvec2(0, 0));
            colors.push_back(glm::dvec4(255, 255, 255, 255));
            
            triangles.push_back(i);
            triangles.push_back(i-1);
            triangles.push_back(0);
            
            triangles.push_back(i);
            triangles.push_back(i-1);
            triangles.push_back(1);
        }
        
        triangles.push_back(numSlices);
        triangles.push_back(2);
        triangles.push_back(0);

        //Setup mesh
        MeshFilterComponent* mesh = new MeshFilterComponent(object);
        mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);

        QJsonObject json;
        json["Type"] = QString("Primitive");
        json["Primitive"] = QString("Cone");
        mesh->jsonObj = json;                
        return mesh;

    }
    
    MeshFilterComponent* GetQuadMesh(glm::dvec3 size, glm::dvec4 color, Object3D* object){
        std::vector<glm::dvec3> vertex;
        std::vector<glm::dvec3> normals;
        std::vector<glm::dvec2> uv;
        std::vector<glm::dvec4> colors;
        std::vector<int> triangles;


        //
        ///Vertices
        //

        //Front
        vertex.push_back(glm::dvec3(-1.0, -1.0, 0)); //bottom left
        vertex.push_back(glm::dvec3(-1.0, 1.0, 0)); // top left
        vertex.push_back(glm::dvec3(1.0, 1.0, 0)); //Top right
        vertex.push_back(glm::dvec3(1.0, -1.0, 0)); //Bottom right

        //
        ///Normals
        //

        //Front
        normals.push_back(glm::dvec3(0, 0, -1));
        normals.push_back(glm::dvec3(0, 0, -1));
        normals.push_back(glm::dvec3(0, 0, -1));
        normals.push_back(glm::dvec3(0, 0, -1));


        //
        ////UV
        //

        //Front
        uv.push_back(glm::dvec2(0, 1));
        uv.push_back(glm::dvec2(0, 0));
        uv.push_back(glm::dvec2(1, 0));
        uv.push_back(glm::dvec2(1, 1));


        //
        ////Colors
        //

        //Front
        colors.push_back(glm::dvec4(255, 255, 255, 255));
        colors.push_back(glm::dvec4(255, 255, 255, 255));
        colors.push_back(glm::dvec4(255, 255, 255, 255));
        colors.push_back(glm::dvec4(255, 255, 255, 255));

        //
        ////Triangles
        //

        //Front
        int index = 0;
        triangles.push_back(index + 0);
        triangles.push_back(index + 2);
        triangles.push_back(index + 1);
        triangles.push_back(index + 3);
        triangles.push_back(index + 2);
        triangles.push_back(index + 0);


        //Setup mesh
        MeshFilterComponent* mesh = new MeshFilterComponent(object);
        mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, true);


        QJsonObject json;
        json["Type"] = QString("Primitive");
        json["Primitive"] = QString("Quad");
        mesh->jsonObj = json;         
        return mesh;
    }

    MeshFilterComponent* GetById(PRIMITIVE_MESH meshId, Object3D* object){
        enum PRIMITIVE_MESH {CUBE_MESH, SPHERE_MESH, CIRCLE_MESH, CONE_MESH, QUAD_MESH};
        
        switch (meshId)
        {
            case CUBE_MESH:
                return GetCubeMesh(glm::dvec3(1), glm::dvec4(1), object);
                break;
            case SPHERE_MESH:
                return GetSphereMesh(glm::dvec3(1), glm::dvec4(1), object);
                break;
            case CIRCLE_MESH:
                return GetCircleMesh(glm::dvec3(1), glm::dvec4(1), object);
                break;  
            case CONE_MESH:
                return GetConeMesh(glm::dvec3(1), glm::dvec4(1), object);
                break;  
            case QUAD_MESH:
                return GetQuadMesh(glm::dvec3(1), glm::dvec4(1), object);
                break;              
            default:
                break;
        }

    }
}
}