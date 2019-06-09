#include "BaseObjects.hpp"
#include "Components/MaterialComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {


Object3D* GetCube(Scene* scene) {
     //Start each Object3D in scene
    Object3D* newObject = new Object3D("Cube", scene);
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
    vertex.push_back(glm::dvec3(0.5, -0.5, 0.5)); // Bottom left
    vertex.push_back(glm::dvec3(0.5, 0.5, 0.5)); //Top left
    vertex.push_back(glm::dvec3(0.5, 0.5, -0.5)); //Top right
    vertex.push_back(glm::dvec3(0.5, -0.5, -0.5)); //Bottom right

    // Left
    vertex.push_back(glm::dvec3(-0.5, -0.5, -0.5)); //Bottom Left
    vertex.push_back(glm::dvec3(-0.5, 0.5, -0.5)); //Top left
    vertex.push_back(glm::dvec3(-0.5, 0.5, 0.5)); //Top right
    vertex.push_back(glm::dvec3(-0.5, -0.5, 0.5)); // Bottom right

    // Top
    vertex.push_back(glm::dvec3(-0.5, 0.5, 0.5)); //Bottom Left
    vertex.push_back(glm::dvec3(-0.5, 0.5, -0.5)); //Top Left
    vertex.push_back(glm::dvec3(0.5, 0.5, -0.5)); // top right
    vertex.push_back(glm::dvec3(0.5, 0.5, 0.5)); //bottom right

    // Bottom ?????
    vertex.push_back(glm::dvec3( 0.5, -0.5,  0.5)); //bottom left
    vertex.push_back(glm::dvec3( 0.5, -0.5, -0.5)); // top left
    vertex.push_back(glm::dvec3(-0.5, -0.5, -0.5)); //Top right
    vertex.push_back(glm::dvec3(-0.5, -0.5,  0.5)); //Bottom right


    //
    ///Normals
    //

    //Front
    normals.push_back(glm::dvec3(0, 0, 1));
    normals.push_back(glm::dvec3(0, 0, 1));
    normals.push_back(glm::dvec3(0, 0, 1));
    normals.push_back(glm::dvec3(0, 0, 1));
    
    //Back
    normals.push_back(glm::dvec3(0, 0, -1));
    normals.push_back(glm::dvec3(0, 0, -1));
    normals.push_back(glm::dvec3(0, 0, -1));
    normals.push_back(glm::dvec3(0, 0, -1));
    
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
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    
    //Back
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    
    //Right
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    
    //Left
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    
    //Top
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    
    //Bottom
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));


    //
    ////Colors
    //

    //Front
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    colors.push_back(glm::dvec4(0.0, 255, 0, 255));
    colors.push_back(glm::dvec4(0.0, 0, 255, 255));
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    
    //Back
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    colors.push_back(glm::dvec4(0.0, 255, 0, 255));
    colors.push_back(glm::dvec4(0.0, 0, 255, 255));
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    
    //Right
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    colors.push_back(glm::dvec4(0.0, 255, 0, 255));
    colors.push_back(glm::dvec4(0.0, 0, 255, 255));
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    
    //Left
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    colors.push_back(glm::dvec4(0.0, 255, 0, 255));
    colors.push_back(glm::dvec4(0.0, 0, 255, 255));
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    
    //Top
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    colors.push_back(glm::dvec4(0.0, 255, 0, 255));
    colors.push_back(glm::dvec4(0.0, 0, 255, 255));
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    
    //Bottom
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));
    colors.push_back(glm::dvec4(0.0, 255, 0, 255));
    colors.push_back(glm::dvec4(0.0, 0, 255, 255));
    colors.push_back(glm::dvec4(255.0, 0, 0, 255));


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
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
    newObject->AddComponent(transform);

    return newObject;
}


}
}