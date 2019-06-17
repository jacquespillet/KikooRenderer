#include "BaseObjects.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/BoundingComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {


Object3D* GetCube(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
     //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
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
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
    newObject->AddComponent(transform);
    newObject->AddComponent(mesh);

    return newObject;
}

Object3D* GetWireFrameBox(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
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
    vertex.push_back(glm::dvec3(-0.5, -0.5, 0.5)); //bottom right
    vertex.push_back(glm::dvec3(-0.5, 0.5, 0.5)); // top right
    vertex.push_back(glm::dvec3(0.5, 0.5, 0.5)); //Top left
    vertex.push_back(glm::dvec3(0.5, -0.5, 0.5)); //Bottom Left

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


    //
    ////Triangles
    //

    //Front
    int index = 0;
    triangles.push_back(index + 0);
    triangles.push_back(index + 1);

    triangles.push_back(index + 1);
    triangles.push_back(index + 2);

    triangles.push_back(index + 2);
    triangles.push_back(index + 3);

    triangles.push_back(index + 3);
    triangles.push_back(index + 0);


    index = 4;
    triangles.push_back(index + 0);
    triangles.push_back(index + 1);

    triangles.push_back(index + 1);
    triangles.push_back(index + 2);

    triangles.push_back(index + 2);
    triangles.push_back(index + 3);

    triangles.push_back(index + 3);
    triangles.push_back(index + 0);

    triangles.push_back( 0);
    triangles.push_back( 4);

    triangles.push_back( 1);
    triangles.push_back( 5);

    triangles.push_back( 2);
    triangles.push_back( 6);

    triangles.push_back( 3);
    triangles.push_back( 7);

    //Setup mesh
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);
    mesh->drawingMode = GL_LINES;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
    newObject->AddComponent(transform);
    newObject->AddComponent(mesh);

    return newObject;
}

Object3D* GetQuad(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
     //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
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
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));


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
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
    newObject->AddComponent(transform);

    return newObject;
}

Object3D* GetCircle(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
     //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
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
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
    newObject->AddComponent(transform);

    return newObject;
}

Object3D* GetSphere(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;


    uint32_t numSlices = 32;
    float radius = 1.0;

    for(int x=0, inx = 0; x<numSlices; x++) {
        for(int y=0; y<numSlices * 2; y++, inx++) {
            float yAngle = ((float)y / (float)numSlices * 2) * TWO_PI;
            float xAngle = ((float)x / (float)numSlices) * PI;
            
            float posx = radius * std::sin(xAngle) * std::cos(yAngle);
            float posy = radius * std::sin(xAngle) * std::sin(yAngle);
            float posz = radius * std::cos(xAngle);

            vertex.push_back(glm::dvec3(posx, posy, posz));
            normals.push_back(glm::dvec3(posx, posy, posz));
            uv.push_back(glm::dvec2(0, 0));
            colors.push_back(glm::dvec4(255, 255, 255, 255));
            
            triangles.push_back(inx);
            triangles.push_back(inx + numSlices * 2);
            triangles.push_back(inx + (numSlices * 2 + 1));
        }        
    }

    //Setup mesh
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);
    mesh->drawingMode = GL_TRIANGLE_FAN;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
    newObject->AddComponent(transform);
    newObject->AddComponent(boundingBox);

    return newObject;
}

Object3D* GetGrid(Scene* scene, std::string name) {
   //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;

    for(int i=0, z=-5; z<=5; i++, z++) {
        if(z != 0) {
            vertex.push_back(glm::dvec3( -5, 0,  z)); 
            vertex.push_back(glm::dvec3(  5, 0,  z)); 

            normals.push_back(glm::dvec3(0, 0, 1));
            normals.push_back(glm::dvec3(0, 0, 1));

            uv.push_back(glm::dvec2(0, 0));
            uv.push_back(glm::dvec2(0, 0));

            colors.push_back(glm::dvec4(255, 255, 255, 255));
            colors.push_back(glm::dvec4(255, 255, 255, 255));

            triangles.push_back(vertex.size()-1);
            triangles.push_back(vertex.size()-2);        
        }
    }

   for(int i=0, x=-5; x<=5; i++, x++) {
       if(x != 0) {
            vertex.push_back(glm::dvec3( x, 0,  -5)); //bottom left
            vertex.push_back(glm::dvec3(  x, 0,  5)); // top left

            normals.push_back(glm::dvec3(0, 0, 1));
            normals.push_back(glm::dvec3(0, 0, 1));

            uv.push_back(glm::dvec2(0, 0));
            uv.push_back(glm::dvec2(0, 0));

            colors.push_back(glm::dvec4(255, 0, 255, 255));
            colors.push_back(glm::dvec4(255, 0, 255, 255));

            triangles.push_back(vertex.size()-1);
            triangles.push_back(vertex.size()-2);
       }
    }

    //Setup mesh
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);
    mesh->drawingMode = GL_LINES;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo =  glm::vec4(0.6, 0.6, 0.6, 0.6);
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
    newObject->AddComponent(transform);
    newObject->AddComponent(mesh);

    return newObject;
}


Object3D* GetAxes(Scene* scene, std::string name) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;


    //X axis
    vertex.push_back(glm::dvec3( -100, 0,  0)); 
    vertex.push_back(glm::dvec3(  100, 0,  0)); 

    normals.push_back(glm::dvec3(0, 0, 1));
    normals.push_back(glm::dvec3(0, 0, 1));

    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));

    colors.push_back(glm::dvec4(255, 0, 0, 255));
    colors.push_back(glm::dvec4(255, 0, 0, 255));

    triangles.push_back(vertex.size()-1);
    triangles.push_back(vertex.size()-2);     

    //Y axis
    vertex.push_back(glm::dvec3( 0, -100,  0)); 
    vertex.push_back(glm::dvec3( 0,  100,  0)); 

    normals.push_back(glm::dvec3(0, 0, 1));
    normals.push_back(glm::dvec3(0, 0, 1));

    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));

    colors.push_back(glm::dvec4(0, 255, 0, 255));
    colors.push_back(glm::dvec4(0, 255, 0, 255));

    triangles.push_back(vertex.size()-1);
    triangles.push_back(vertex.size()-2);
    
    //Z axis
    vertex.push_back(glm::dvec3( 0,  0, -100)); 
    vertex.push_back(glm::dvec3( 0,  0,  100)); 

    normals.push_back(glm::dvec3(0, 0, 1));
    normals.push_back(glm::dvec3(0, 0, 1));

    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));

    colors.push_back(glm::dvec4(0, 0, 255, 255));
    colors.push_back(glm::dvec4(0, 0, 255, 255));

    triangles.push_back(vertex.size()-1);
    triangles.push_back(vertex.size()-2);

    
    //Setup mesh
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);
    mesh->drawingMode = GL_LINES;
    mesh->primitiveSize = 10;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo =glm::vec4(0.6, 0.6, 0.6, 0.6);
    material->influence = 0;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
    newObject->AddComponent(transform);
    newObject->AddComponent(mesh);

    return newObject;
}


Object3D* GetCone(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
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
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject);
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
    newObject->AddComponent(transform);

    return newObject;
}


Object3D* GetLine(Scene* scene, std::string name, glm::dvec3 position1, glm::dvec3 position2, glm::dvec4 color) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;


    //X axis
    vertex.push_back(position1); 
    vertex.push_back(position2); 

    normals.push_back(glm::dvec3(0, 0, 1));
    normals.push_back(glm::dvec3(0, 0, 1));

    uv.push_back(glm::dvec2(0, 0));
    uv.push_back(glm::dvec2(0, 0));

    colors.push_back(color);
    colors.push_back(color);

    triangles.push_back(0);
    triangles.push_back(1);     
    
    //Setup mesh
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);
    mesh->drawingMode = GL_LINES;
    mesh->primitiveSize = 10;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo =color;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
    newObject->AddComponent(transform);
    newObject->AddComponent(mesh);

    return newObject;
}

Object3D* GetTranslateWidget(Scene* scene, std::string name,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale) {
    Object3D* widget = new Object3D(name, scene);

    Object3D* xcone = GetCone(scene,"coneX", glm::dvec3(1, 0, 0), glm::dvec3(0,  90, 0), glm::dvec3(1), glm::dvec4(1.0, 0.0, 0.0, 1.0));
    Object3D* xline = GetLine(scene,"lineX", glm::dvec3(0, 0, 0), glm::dvec3(1, 0, 0), glm::dvec4(1.0, 0.0, 0.0, 1.0));   

    widget->AddObject(xcone);
    widget->AddObject(xline);

    Object3D* ycone = GetCone(scene,"coneY", glm::dvec3(0, 1, 0), glm::dvec3(- 90,  0, 0), glm::dvec3(1), glm::dvec4(0.0, 1.0, 0.0, 1.0));
    Object3D* yline = GetLine(scene,"lineY", glm::dvec3(0, 0, 0), glm::dvec3(0, 1, 0), glm::dvec4(0.0, 1.0, 0.0, 1.0));

    widget->AddObject(ycone);
    widget->AddObject(yline);

    Object3D* zcone = GetCone(scene,"coneZ", glm::dvec3(0, 0, 1), glm::dvec3(0, 0, 0), glm::dvec3(1), glm::dvec4(0.0, 0.0, 1.0, 1.0));
    Object3D* zline = GetLine(scene,"lineZ", glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 1), glm::dvec4(0.0, 0.0, 1.0, 1.0));

    widget->AddObject(zcone);
    widget->AddObject(zline);

    TransformComponent* transform =  (TransformComponent*)widget->GetComponent("Transform");
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;

    return widget;
}

Object3D* GetScaleWidget(Scene* scene, std::string name,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale) {
    Object3D* widget = new Object3D(name, scene);

    Object3D* xcube = GetCube(scene, "cubeX", glm::dvec3(1, 0, 0), glm::dvec3(0,  0, 0), glm::dvec3(0.5), glm::dvec4(1.0, 0.0, 0.0, 1.0));
    Object3D* xline = GetLine(scene, "lineX", glm::dvec3(0, 0, 0), glm::dvec3(1, 0, 0), glm::dvec4(1.0, 0.0, 0.0, 1.0));   

    widget->AddObject(xcube);
    widget->AddObject(xline);

    Object3D* ycube = GetCube(scene, "cubeY", glm::dvec3(0, 1, 0), glm::dvec3(0,  0, 0), glm::dvec3(0.5), glm::dvec4(0.0, 1.0, 0.0, 1.0));
    Object3D* yline = GetLine(scene, "lineY", glm::dvec3(0, 0, 0), glm::dvec3(0, 1, 0), glm::dvec4(0.0, 1.0, 0.0, 1.0));

    widget->AddObject(ycube);
    widget->AddObject(yline);

    Object3D* zcube = GetCube(scene, "cubeZ", glm::dvec3(0, 0, 1), glm::dvec3(0, 0, 0), glm::dvec3(0.5), glm::dvec4(0.0, 0.0, 1.0, 1.0));
    Object3D* zline = GetLine(scene, "lineZ", glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 1), glm::dvec4(0.0, 0.0, 1.0, 1.0));

    widget->AddObject(zcube);
    widget->AddObject(zline);

    TransformComponent* transform =  (TransformComponent*)widget->GetComponent("Transform");
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;

    return widget;
}

}
}