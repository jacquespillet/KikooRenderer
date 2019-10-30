#include "BaseObjects.hpp"
#include "Scene.hpp"
#include "Shaders/AxisHelperShader.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/BoundingComponent.hpp"
#include "Components/FluidComponent.hpp"
#include "Components/MeshFilterComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/LightComponent.hpp"
#include "Components/BaseMeshes.hpp"
#include "../Util/ModelLoader.hpp"
#include "ParticleSystem/ParticleSystem.hpp"
#include "WaterTiles/WaterTile_1/WaterTile_1.hpp"
#include "WaterTiles/WaterTile_2/WaterTile_2.hpp"
#include "WaterTiles/Wavetile/WaveTile.hpp"
#include "Curves/CatmutRollSpline.hpp"
#include "Curves/NURBS.hpp"
#include "Curves/NonUniformBSpline.hpp"

#include "PhysicsEngines/Physics.hpp"

namespace KikooRenderer {

namespace CoreEngine {


Object3D* GetCube(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, bool physics) {
     //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
   

    //Setup mesh
    MeshFilterComponent* mesh = GetCubeMesh(glm::vec3(1), _color, newObject);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;

    Shader shader = scene->standardShaders.blinnPhongShader;
    material->SetShader(shader);

    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);



    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);
    newObject->AddComponent(boundingBox);


    return newObject;
}

Object3D* GetWireFrameBox(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);

    MeshFilterComponent* mesh = GetCubeMesh(glm::vec3(1), _color, newObject);
    mesh->drawingMode = GL_LINES;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}

Object3D* GetQuad(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
 
    MeshFilterComponent* mesh = GetQuadMesh(glm::vec3(1), _color, newObject);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);
 
    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);


    return newObject;
}

Object3D* GetMiniQuad(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
     //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
 
    MeshFilterComponent* mesh = GetMiniQuadMesh(glm::vec3(1), _color, newObject);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);


    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);


    return newObject;
}

Object3D* GetCircle(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
     //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);

    MeshFilterComponent* mesh = GetCircleMesh(glm::vec3(1), _color, newObject);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);

    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);


    return newObject;
}


Object3D* GetWireCircle(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
	//Start each Object3D in scene
	Object3D* newObject = new Object3D(name, scene);

    MeshFilterComponent* mesh = GetWireCircleMesh(glm::vec3(1), _color, newObject);
	mesh->drawingMode = GL_LINES;

	//Setup transform
	TransformComponent* transform = new TransformComponent(newObject);
	transform->position = _position;
	transform->rotation = _rotation;
	transform->scale = _scale;


	//Setup material
	MaterialComponent* material = new MaterialComponent(newObject);
	material->albedo = _color;
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);

	BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

	newObject->AddComponent(material);
	newObject->AddComponent(mesh);
	newObject->transform = transform;
	newObject->AddComponent(boundingBox);


	return newObject;
}



Object3D* GetSphere(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    
    MeshFilterComponent* mesh = GetSphereMesh(glm::vec3(1), _color, newObject);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.blinnPhongShader;
    material->SetShader(shader);

    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);

    return newObject;
}

Object3D* GetCapsule(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, float height) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    
    MeshFilterComponent* mesh = GetCapsuleMesh(glm::vec3(1), _color, newObject);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.blinnPhongShader;
    material->SetShader(shader);

    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);

    return newObject;
}

Object3D* GetWireCapsule(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, float height) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    
    MeshFilterComponent* mesh = GetCapsuleMesh(glm::vec3(1), _color, newObject);
	mesh->drawingMode = GL_LINES;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.blinnPhongShader;
    material->SetShader(shader);

    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);

    return newObject;
}

Object3D* GetWireSphere(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    
    MeshFilterComponent* mesh = GetSphereMesh(glm::vec3(1), _color, newObject, 12);
	mesh->drawingMode = GL_LINES;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.blinnPhongShader;
    material->SetShader(shader);

    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);

    return newObject;
}

Object3D* GetGrid(Scene* scene, std::string name) {
   //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;

    for(int i=0, z=-5; z<=5; i++, z++) {
        if(z != 0) {
            vertex.push_back(glm::vec3( -5, 0,  z)); 
            vertex.push_back(glm::vec3(  5, 0,  z)); 

            normals.push_back(glm::vec3(0, 0, 1));
            normals.push_back(glm::vec3(0, 0, 1));

            uv.push_back(glm::vec2(0, 0));
            uv.push_back(glm::vec2(0, 0));

            colors.push_back(glm::vec4(255, 255, 255, 255));
            colors.push_back(glm::vec4(255, 255, 255, 255));

            triangles.push_back(vertex.size()-1);
            triangles.push_back(vertex.size()-2);        
        }
    }

   for(int i=0, x=-5; x<=5; i++, x++) {
       if(x != 0) {
            vertex.push_back(glm::vec3( x, 0,  -5)); //bottom left
            vertex.push_back(glm::vec3(  x, 0,  5)); // top left

            normals.push_back(glm::vec3(0, 0, 1));
            normals.push_back(glm::vec3(0, 0, 1));

            uv.push_back(glm::vec2(0, 0));
            uv.push_back(glm::vec2(0, 0));

            colors.push_back(glm::vec4(255, 255, 255, 255));
            colors.push_back(glm::vec4(255, 255, 255, 255));

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
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}


Object3D* GetAxes(Scene* scene, std::string name) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;


    //X axis
    vertex.push_back(glm::vec3( -100, 0,  0)); 
    vertex.push_back(glm::vec3(  100, 0,  0)); 

    normals.push_back(glm::vec3(0, 0, 1));
    normals.push_back(glm::vec3(0, 0, 1));

    uv.push_back(glm::vec2(0, 0));
    uv.push_back(glm::vec2(0, 0));

    colors.push_back(glm::vec4(255, 0, 0, 255));
    colors.push_back(glm::vec4(255, 0, 0, 255));

    triangles.push_back(vertex.size()-1);
    triangles.push_back(vertex.size()-2);     

    //Y axis
    vertex.push_back(glm::vec3( 0, -100,  0)); 
    vertex.push_back(glm::vec3( 0,  100,  0)); 

    normals.push_back(glm::vec3(0, 0, 1));
    normals.push_back(glm::vec3(0, 0, 1));

    uv.push_back(glm::vec2(0, 0));
    uv.push_back(glm::vec2(0, 0));

    colors.push_back(glm::vec4(0, 255, 0, 255));
    colors.push_back(glm::vec4(0, 255, 0, 255));

    triangles.push_back(vertex.size()-1);
    triangles.push_back(vertex.size()-2);
    
    //Z axis
    vertex.push_back(glm::vec3( 0,  0, -100)); 
    vertex.push_back(glm::vec3( 0,  0,  100)); 

    normals.push_back(glm::vec3(0, 0, 1));
    normals.push_back(glm::vec3(0, 0, 1));

    uv.push_back(glm::vec2(0, 0));
    uv.push_back(glm::vec2(0, 0));

    colors.push_back(glm::vec4(0, 0, 255, 255));
    colors.push_back(glm::vec4(0, 0, 255, 255));

    triangles.push_back(vertex.size()-1);
    triangles.push_back(vertex.size()-2);

    
    //Setup mesh
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, false);
    mesh->drawingMode = GL_LINES;
    mesh->primitiveSize = 10;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo =glm::vec4(0.6, 0.6, 0.6, 0.6);
    material->influence = 0.0;
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}

Object3D* GetAxesHelper(Scene* scene) {
    Object3D* newObject = new Object3D("Axis Helper", scene);
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;


    //X axis
    vertex.push_back(glm::vec3(  0, 0,  0.2)); 
    vertex.push_back(glm::vec3(  1, 0,  0.2)); 
    normals.push_back(glm::vec3(0, 0, 1));
    normals.push_back(glm::vec3(0, 0, 1));
    uv.push_back(glm::vec2(0, 0));
    uv.push_back(glm::vec2(0, 0));
    colors.push_back(glm::vec4(255, 0, 0, 255));
    colors.push_back(glm::vec4(255, 0, 0, 255));
    triangles.push_back(vertex.size()-1);
    triangles.push_back(vertex.size()-2);

    //Y axis
    vertex.push_back(glm::vec3( 0,  0,  0.2)); 
    vertex.push_back(glm::vec3( 0,  1,  0.2)); 
    normals.push_back(glm::vec3(0, 0, 1));
    normals.push_back(glm::vec3(0, 0, 1));
    uv.push_back(glm::vec2(0, 0));
    uv.push_back(glm::vec2(0, 0));
    colors.push_back(glm::vec4(0, 255, 0, 255));
    colors.push_back(glm::vec4(0, 255, 0, 255));
    triangles.push_back(vertex.size()-1);
    triangles.push_back(vertex.size()-2);
    
    //Z axis
    vertex.push_back(glm::vec3( 0,  0,  0.2));
    vertex.push_back(glm::vec3( 0,  0,  1.2));
    normals.push_back(glm::vec3(0, 0, 1));
    normals.push_back(glm::vec3(0, 0, 1));
    uv.push_back(glm::vec2(0, 0));
    uv.push_back(glm::vec2(0, 0));
    colors.push_back(glm::vec4(0, 0, 255, 255));
    colors.push_back(glm::vec4(0, 0, 255, 255));
    triangles.push_back(vertex.size()-1);
    triangles.push_back(vertex.size()-2);

    
    //Setup mesh
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, false);
    mesh->drawingMode = GL_LINES;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = glm::vec3(2, 2, 2);
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo =glm::vec4(0.6, 0.6, 0.6, 0.6);
    material->influence = 0.0;
    Shader shader = GetAxisHelperShader();
    material->SetShader(shader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;    
}


Object3D* GetCone(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, bool isScreenSize) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    
    MeshFilterComponent* mesh = GetConeMesh(glm::vec3(1), _color, newObject);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject);
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    transform->isScreenSize = isScreenSize;

    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.blinnPhongShader;
    material->SetShader(shader);

    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);

    return newObject;
}


Object3D* GetLine(Scene* scene, std::string name, glm::vec3 position1, glm::vec3 position2, glm::vec4 color, bool isScreenSize) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;


    //X axis
    vertex.push_back(position1); 
    vertex.push_back(position2); 

    normals.push_back(glm::vec3(0, 0, 1));
    normals.push_back(glm::vec3(0, 0, 1));

    uv.push_back(glm::vec2(0, 0));
    uv.push_back(glm::vec2(0, 0));

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
    transform->isScreenSize = isScreenSize;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo =color;
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}


Object3D* GetTerrain(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, float width, float height, int subdivisionsX, int subdivisionsY) {
 //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;

    float xOffset = width / (float)subdivisionsX;
    float yOffset = height / (float)subdivisionsY;

    //
    ///Vertices
    //
    int numAdded=0;
    for(float y=-width/2, yInx=0; yInx<subdivisionsY; y+=yOffset, yInx++) {
        for(float x=-height/2, xInx=0; xInx<subdivisionsX; x+= xOffset, xInx++) {
            vertex.push_back(glm::vec3(x, 0, y));
            normals.push_back(glm::vec3(0, 1, 0));
            uv.push_back(glm::vec2(0, 0));
            colors.push_back(glm::vec4(255, 255, 255, 255));  

            if(xInx < subdivisionsX-1 && yInx < subdivisionsY-1) {
                triangles.push_back(numAdded);
                triangles.push_back(numAdded + subdivisionsX + 1);
                triangles.push_back(numAdded + subdivisionsX);
                
                triangles.push_back(numAdded);
                triangles.push_back(numAdded + 1);
                triangles.push_back(numAdded + subdivisionsX + 1);
            }
            numAdded++;
        }
    }

    //Setup mesh
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    // mesh->drawingMode = GL_TRIANGLE_FAN;
    
    // mesh->drawingMode = GL_LINES;
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, true);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.blinnPhongShader;
    material->SetShader(shader);


    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);



    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);
    newObject->AddComponent(mesh);


    return newObject;    
}

Object3D* GetHermiteCurve(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, glm::vec3 point1, glm::vec3 point2, glm::vec3 tan1, glm::vec3 tan2) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;


    int inx =0;
    for(double t=0; t<1; t+=0.01) {
        inx++;
        if(inx < 1 / 0.01) {
            triangles.push_back(inx-1);
            triangles.push_back(inx);
        }

        glm::vec3 first  = (1 - 3 * t * t + 2 * t * t * t) * point1;
        glm::vec3 second = (t*t) * (3 - 2*t) * point2;
        glm::vec3 third  = t * ((t-1) * (t-1)) * tan1;
        glm::vec3 fourth = (t * t) * (t-1) * tan2;
        glm::vec3 H = first + second + third + fourth;
        
        vertex.push_back(H); 
        normals.push_back(glm::vec3(0, 0, 1));
        uv.push_back(glm::vec2(0, 0));
        colors.push_back(_color);
    }
    //Setup mesh
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);
    mesh->drawingMode = GL_LINES;
    mesh->primitiveSize = 10;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}

Object3D* GetBezierCurve(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;


    int inx =0;
    for(double t=0; t<1; t+=0.01) {
        inx++;
        if(inx < 1 / 0.01) {
            triangles.push_back(inx-1);
            triangles.push_back(inx);
        }

        glm::vec3 tan1 =  3.0 * (point1 - point0);
        glm::vec3 tan2 = -3.0 * (point2 - point3);

        glm::vec3 first  = (1 - 3 * t * t + 2 * t * t * t) * point0;
        glm::vec3 second = (t*t) * (3 - 2*t) * point3;
        glm::vec3 third  = t * ((t-1) * (t-1)) * tan1;
        glm::vec3 fourth = (t * t) * (t-1) * tan2;
        glm::vec3 H = first + second + third + fourth;
        
        vertex.push_back(H); 
        normals.push_back(glm::vec3(0, 0, 1));
        uv.push_back(glm::vec2(0, 0));
        colors.push_back(_color);
    }
    //Setup mesh
    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);
    mesh->drawingMode = GL_LINES;
    mesh->primitiveSize = 10;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}

Object3D* GetCatmutRollSpline(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    CatmutRollSpline* spline = new CatmutRollSpline(name, scene);
    return spline;
}

Object3D* GetNonUniformBSpline(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    NonUniformBSpline* spline = new NonUniformBSpline(name, scene);
    return spline;
}


Object3D* GetNURBS(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    NonUniformBSpline* NURBS = new NonUniformBSpline(name, scene);
    return NURBS;
}


Object3D* GetBezierSurface(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, std::vector<glm::vec4> controlPoints) {
    return nullptr;
}


Object3D* GetDirectionalLight(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    Object3D* dirLight = new Object3D(name, scene);
	dirLight->transform->position = _position;
	dirLight->transform->rotation = _rotation;
	dirLight->transform->scale = _scale;
	LightComponent* lightComponent = new LightComponent(dirLight, _color, glm::vec3(0.25, 0.05, 0.001), 0);
	dirLight->AddComponent(lightComponent);
	return dirLight;
}

Object3D* GetPointLight(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
	Object3D* pointLight = new Object3D(name, scene);
	pointLight->transform->position = _position;
	pointLight->transform->rotation = _rotation;
	pointLight->transform->scale = _scale;
	LightComponent* lightComponent = new LightComponent(pointLight, _color, glm::vec3(0.25, 0.05, 0.001), 1);
	pointLight->AddComponent(lightComponent);
	return pointLight;
}

Object3D* GetSpotLight(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, double FOV) {
	Object3D* spotLight = new Object3D(name, scene);
	spotLight->transform->position = _position;
	spotLight->transform->rotation = _rotation;
	spotLight->transform->scale = _scale;
	LightComponent* lightComponent = new LightComponent(spotLight, _color, glm::vec3(0.25, 0.05, 0.001), 2);
	lightComponent->fov = FOV;
	spotLight->AddComponent(lightComponent);
	return spotLight;
}


Object3D* ObjectFromModelFile(Scene* scene, std::string name, std::string filename) {
    Object3D* newObject = new Object3D(name, scene);

    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;

    Util::FileIO::LoadModel(filename, &vertex, &normals, &uv, &colors, &triangles);

    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, false);
    //Setup transform
    TransformComponent* transform = new  TransformComponent(newObject );
    
    //Setup material
    MaterialComponent* material = new  MaterialComponent(newObject);
    Shader shader = scene->standardShaders.blinnPhongShader;
    material->SetShader(shader);

    BoundingBoxComponent* boundingBox = new  BoundingBoxComponent(newObject);


    newObject->AddComponent(material);
    newObject->transform = transform;
    newObject->AddComponent(mesh);
    newObject->AddComponent(boundingBox);

    return newObject;
}

Object3D* GetParticleSystem(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    ParticleSystem* ps = new ParticleSystem(name, scene);
    return ps;
}

Object3D* GetWaterTile_1(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    WaterTile_1* wt1 = new WaterTile_1(name, scene);
    return wt1;
}

Object3D* GetWaterTile_2(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    WaterTile_2* wt2 = new WaterTile_2(name, scene);
    return wt2;
}

Object3D* GetWaterTile_3(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    WaterTile_2* wt3 = new WaterTile_2(name, scene, TILE_TYPE::WATER);
    return wt3;
} 

Object3D* GetWaterTile_4(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color) {
    WaveTile* wt = new WaveTile(name, scene);
    return wt;
} 
}
}