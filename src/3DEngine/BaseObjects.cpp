#include "BaseObjects.hpp"
#include "Scene.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/BoundingComponent.hpp"
#include "Components/FluidComponent.hpp"
#include "Components/MeshFilterComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/LightComponent.hpp"
#include "Components/BaseMeshes.hpp"
#include "../Util/ModelLoader.hpp"

namespace KikooRenderer {

namespace CoreEngine {


Object3D* GetCube(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
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
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);


    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);
    newObject->AddComponent(boundingBox);

    return newObject;
}

Object3D* GetWireFrameBox(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
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
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}

Object3D* GetQuad(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
 
    MeshFilterComponent* mesh = GetQuadMesh(glm::dvec3(1), _color, newObject);

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
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);


    return newObject;
}

Object3D* GetMiniQuad(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
     //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
 
    MeshFilterComponent* mesh = GetMiniQuadMesh(glm::dvec3(1), _color, newObject);

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
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);


    return newObject;
}

Object3D* GetCircle(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
     //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);

    MeshFilterComponent* mesh = GetCircleMesh(glm::dvec3(1), _color, newObject);

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
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);


    return newObject;
}


Object3D* GetWireCircle(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
	//Start each Object3D in scene
	Object3D* newObject = new Object3D(name, scene);

    MeshFilterComponent* mesh = GetQuadMesh(glm::dvec3(1), _color, newObject);
	mesh->drawingMode = GL_LINES;

	//Setup transform
	TransformComponent* transform = new TransformComponent(newObject);
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
	newObject->transform = transform;
	newObject->AddComponent(boundingBox);


	return newObject;
}


Object3D* GetSphere(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    
    MeshFilterComponent* mesh = GetSphereMesh(glm::dvec3(1), _color, newObject);

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
	newObject->transform = transform;
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

            colors.push_back(glm::dvec4(255, 255, 255, 255));
            colors.push_back(glm::dvec4(255, 255, 255, 255));

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
	newObject->transform = transform;
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
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, false);
    mesh->drawingMode = GL_LINES;
    mesh->primitiveSize = 10;

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject );
    
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo =glm::vec4(0.6, 0.6, 0.6, 0.6);
    material->influence = 0.0;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}


Object3D* GetCone(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color, bool isScreenSize) {
    //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    
    MeshFilterComponent* mesh = GetConeMesh(glm::dvec3(1), _color, newObject);

    //Setup transform
    TransformComponent* transform = new TransformComponent(newObject);
    transform->position = _position;
    transform->rotation = _rotation;
    transform->scale = _scale;
    transform->isScreenSize = isScreenSize;

    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo = _color;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);

    newObject->AddComponent(material);
    newObject->AddComponent(mesh);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);

    return newObject;
}


Object3D* GetLine(Scene* scene, std::string name, glm::dvec3 position1, glm::dvec3 position2, glm::dvec4 color, bool isScreenSize) {
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
    transform->isScreenSize = isScreenSize;
    
    //Setup material
    MaterialComponent* material = new MaterialComponent(newObject);
    material->albedo =color;
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}


Object3D* GetTerrain(Scene* scene, std::string name,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color, float width, float height, int subdivisionsX, int subdivisionsY) {
 //Start each Object3D in scene
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;

    float xOffset = width / (float)subdivisionsX;
    float yOffset = height / (float)subdivisionsY;

    //
    ///Vertices
    //
    int numAdded=0;
    for(float y=0, yInx=0; yInx<subdivisionsY; y+=yOffset, yInx++) {
        for(float x=0, xInx=0; xInx<subdivisionsX; x+= xOffset, xInx++) {
            vertex.push_back(glm::dvec3(x, 0, y));
            normals.push_back(glm::dvec3(0, 1, 0));
            uv.push_back(glm::dvec2(0, 0));
            colors.push_back(glm::dvec4(255, 255, 255, 255));  

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
    material->SetShader(&scene->standardShaders.blinnPhongShader);


    BoundingBoxComponent* boundingBox = new BoundingBoxComponent(newObject);


    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(boundingBox);
    newObject->AddComponent(mesh);


    return newObject;    
}

Object3D* GetHermiteCurve(Scene* scene, std::string name,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color, glm::dvec3 point1, glm::dvec3 point2, glm::dvec3 tan1, glm::dvec3 tan2) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;


    int inx =0;
    for(double t=0; t<1; t+=0.01) {
        inx++;
        if(inx < 1 / 0.01) {
            triangles.push_back(inx-1);
            triangles.push_back(inx);
        }

        glm::dvec3 first  = (1 - 3 * t * t + 2 * t * t * t) * point1;
        glm::dvec3 second = (t*t) * (3 - 2*t) * point2;
        glm::dvec3 third  = t * ((t-1) * (t-1)) * tan1;
        glm::dvec3 fourth = (t * t) * (t-1) * tan2;
        glm::dvec3 H = first + second + third + fourth;
        
        vertex.push_back(H); 
        normals.push_back(glm::dvec3(0, 0, 1));
        uv.push_back(glm::dvec2(0, 0));
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
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}

Object3D* GetBezierCurve(Scene* scene, std::string name,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color, glm::dvec3 point0, glm::dvec3 point1, glm::dvec3 point2, glm::dvec3 point3) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;


    int inx =0;
    for(double t=0; t<1; t+=0.01) {
        inx++;
        if(inx < 1 / 0.01) {
            triangles.push_back(inx-1);
            triangles.push_back(inx);
        }

        glm::dvec3 tan1 =  3.0 * (point1 - point0);
        glm::dvec3 tan2 = -3.0 * (point2 - point3);

        glm::dvec3 first  = (1 - 3 * t * t + 2 * t * t * t) * point0;
        glm::dvec3 second = (t*t) * (3 - 2*t) * point3;
        glm::dvec3 third  = t * ((t-1) * (t-1)) * tan1;
        glm::dvec3 fourth = (t * t) * (t-1) * tan2;
        glm::dvec3 H = first + second + third + fourth;
        
        vertex.push_back(H); 
        normals.push_back(glm::dvec3(0, 0, 1));
        uv.push_back(glm::dvec2(0, 0));
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
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}

Object3D* GetCatmutRollSpline(Scene* scene, std::string name,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color, std::vector<glm::dvec3> points) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;

    double offset = 0.01;
    int totalPoints = (points.size()-3) * (1.0 / offset);

    int inx =0;
    for(int i=1; i<points.size()-2; i++) {
        for(double t=0; t<1; t+=offset) {
            inx++;

            if(inx < totalPoints) {
                triangles.push_back(inx-1);
                triangles.push_back(inx);
            }

            // glm::dmat4 basis = glm::dmat4(0);
            // basis[0][0] = 0; basis[1][0] = -1; basis[2][0] =  2; basis[3][0] = -1; 
            // basis[0][1] = 2; basis[1][1] =  0; basis[2][1] = -5; basis[3][1] =  3; 
            // basis[0][2] = 0; basis[1][2] =  1; basis[2][2] =  4; basis[3][2] = -3; 
            // basis[0][3] = 0; basis[1][3] =  0; basis[2][3] = -1; basis[3][3] =  1;
            // basis *= 0.5;
            glm::dmat4 basis = glm::dmat4(0);
            basis[0][0] = 1; basis[1][0] = -3; basis[2][0] =  3; basis[3][0] = -1; 
            basis[0][1] = 4; basis[1][1] =  0; basis[2][1] = -6; basis[3][1] =  3; 
            basis[0][2] = 1; basis[1][2] =  3; basis[2][2] =  3; basis[3][2] = -3; 
            basis[0][3] = 0; basis[1][3] =  0; basis[2][3] =  0; basis[3][3] =  1;
            basis *= (1.0 / 6.0);

            glm::dmat4 geometry = glm::dmat4(0);
            geometry[0] = glm::dvec4(points[i-1], 1);
            geometry[1] = glm::dvec4(points[i], 1);
            geometry[2] = glm::dvec4(points[i+1], 1);
            geometry[3] = glm::dvec4(points[i+2], 1);

            glm::dvec4 tVec = glm::dvec4(1, t, t*t, t*t*t);

            glm::dvec3 H = geometry * basis * tVec;

            vertex.push_back(H); 
            normals.push_back(glm::dvec3(0, 0, 1));
            uv.push_back(glm::dvec2(0, 0));
            colors.push_back(_color);
        }
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
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}

double CoxDeBoor(double i, double k, double u, std::vector<double> knotVector) {
    int knotIndex = (int)i+2;
    if(k==0) {
        if(u >= knotVector[knotIndex-2] && u <= knotVector[knotIndex-1]) {
            return 1.0;
        } else {
            return 0.0; 
        }
    } else {
        double firstTerm = 0;
        {
            double first = (u - knotVector[knotIndex-2]);
            double numerator = CoxDeBoor(i, k-1, u, knotVector);
            double denominator = knotVector[knotIndex+k-2] - knotVector[knotIndex-2];
            if(denominator == 0) denominator = 1.0;
            firstTerm = (denominator == 0) ? 0 : first * (numerator / denominator);
        }

        double secondTerm = 0;
        {
            double first = (knotVector[knotIndex+k-1] - u);
            double numerator = CoxDeBoor(i+ 1.0, k-1.0, u, knotVector);
            double denominator = knotVector[knotIndex+k-1] - knotVector[knotIndex-1];
            if(denominator == 0) denominator = 1.0;
            secondTerm = (denominator == 0) ? 0 : first * (numerator / denominator);
        }

        return firstTerm + secondTerm;
    }
}

Object3D* GetNonUniformBSpline(Scene* scene, std::string name,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color, std::vector<glm::dvec3> points) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;

    double offset = 0.01;
    int totalPoints = (points.size()-3) * (1.0 / offset);

    std::vector<double> knotVector;
    knotVector.push_back(0.0);
    knotVector.push_back(0.0);
    knotVector.push_back(0.0);
    knotVector.push_back(0.0);
    knotVector.push_back(1.0);
    knotVector.push_back(2.0);
    knotVector.push_back(3.0);
    knotVector.push_back(4.0);
    knotVector.push_back(5.0);
    knotVector.push_back(5.0);
    knotVector.push_back(5.0);
    knotVector.push_back(5.0);

    int inx =0;
    double globalU = 0.0;
    for(int i=1; i<=points.size()-2; i++) {
        for(double u=0; u<1; u+=offset, globalU+=offset) {
            glm::dvec3 H = glm::dvec3(0);
            for(int k=0; k<=3; k++) {
                double N = CoxDeBoor((double)i+k-1, 3.0, globalU, knotVector);
                H +=  N * points[i+k-1];
            }

            vertex.push_back(H); 
            normals.push_back(glm::dvec3(0, 0, 1));
            uv.push_back(glm::dvec2(0, 0));
            colors.push_back(_color);
            inx++;
            if(inx < totalPoints) {
                triangles.push_back(inx-1);
                triangles.push_back(inx);
            }
        }
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
    material->SetShader(&scene->standardShaders.unlitMeshShader);


    newObject->AddComponent(material);
	newObject->transform = transform;
	newObject->AddComponent(mesh);

    return newObject;
}


Object3D* GetNURBS(Scene* scene, std::string name,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color, std::vector<glm::dvec4> points) {
    Object3D* newObject = new Object3D(name, scene);
    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;

    double offset = 0.01;
    int totalPoints = (points.size()-3) * (1.0 / offset);

    std::vector<double> knotVector;
    knotVector.push_back(0.0);
    knotVector.push_back(0.0);
    knotVector.push_back(0.0);
    knotVector.push_back(0.0);
    knotVector.push_back(1.0);
    knotVector.push_back(2.0);
    knotVector.push_back(3.0);
    knotVector.push_back(4.0);
    knotVector.push_back(5.0);
    knotVector.push_back(5.0);
    knotVector.push_back(5.0);
    knotVector.push_back(5.0);

    int inx =0;
    double globalU = 0.0;
    for(int i=1; i<=points.size()-2; i++) {
        for(double u=0; u<1; u+=offset, globalU+=offset) {
            glm::dvec4 H = glm::dvec4(0);
            for(int k=0; k<=3; k++) {
                double N = CoxDeBoor((double)i+k-1, 3.0, globalU, knotVector);
                double numerator =(N * points[i+k-1].w); 
                double denominator = 0;
                for(int l=0; l<=3; l++) {
                    denominator += CoxDeBoor((double)i+l-1, 3.0, globalU, knotVector) *  points[i+l-1].w;
                }
                
                H +=  N * points[i+k-1];
            }

            vertex.push_back(glm::dvec3(H) / H.w); 
            normals.push_back(glm::dvec3(0, 0, 1));
            uv.push_back(glm::dvec2(0, 0));
            colors.push_back(_color);
            inx++;
            if(inx < totalPoints) {
                triangles.push_back(inx-1);
                triangles.push_back(inx);
            }
        }
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
    material->SetShader(&scene->standardShaders.unlitMeshShader);

    newObject->AddComponent(material);
	newObject->transform = transform;
    newObject->AddComponent(mesh);

    return newObject;
}


Object3D* GetBezierSurface(Scene* scene, std::string name,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color, std::vector<glm::dvec4> controlPoints) {
    return nullptr;
}


Object3D* GetDirectionalLight(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
    Object3D* dirLight = new Object3D(name, scene);
	dirLight->transform->position = _position;
	dirLight->transform->rotation = _rotation;
	dirLight->transform->scale = _scale;
	LightComponent* lightComponent = new LightComponent(dirLight, _color, glm::dvec3(0.25, 0.05, 0.001), 0);
	dirLight->AddComponent(lightComponent);
	return dirLight;
}

Object3D* GetPointLight(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color) {
	Object3D* pointLight = new Object3D(name, scene);
	pointLight->transform->position = _position;
	pointLight->transform->rotation = _rotation;
	pointLight->transform->scale = _scale;
	LightComponent* lightComponent = new LightComponent(pointLight, _color, glm::dvec3(0.25, 0.05, 0.001), 1);
	pointLight->AddComponent(lightComponent);
	return pointLight;
}

Object3D* GetSpotLight(Scene* scene, std::string name, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color, double FOV) {
	Object3D* spotLight = new Object3D(name, scene);
	spotLight->transform->position = _position;
	spotLight->transform->rotation = _rotation;
	spotLight->transform->scale = _scale;
	LightComponent* lightComponent = new LightComponent(spotLight, _color, glm::dvec3(0.25, 0.05, 0.001), 2);
	lightComponent->fov = FOV;
	spotLight->AddComponent(lightComponent);
	return spotLight;
}


Object3D* ObjectFromModelFile(Scene* scene, std::string name, std::string filename) {
    Object3D* newObject = new Object3D(name, scene);

    std::vector<glm::dvec3> vertex;
    std::vector<glm::dvec3> normals;
    std::vector<glm::dvec2> uv;
    std::vector<glm::dvec4> colors;
    std::vector<int> triangles;

    Util::FileIO::LoadModel(filename, &vertex, &normals, &uv, &colors, &triangles);

    MeshFilterComponent* mesh = new MeshFilterComponent(newObject);
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, false);
    //Setup transform
    TransformComponent* transform = new  TransformComponent(newObject );
    
    //Setup material
    MaterialComponent* material = new  MaterialComponent(newObject);
    material->SetShader(&scene->standardShaders.blinnPhongShader);

    BoundingBoxComponent* boundingBox = new  BoundingBoxComponent(newObject);


    newObject->AddComponent(material);
    newObject->AddComponent(transform);
    newObject->AddComponent(mesh);
    newObject->AddComponent(boundingBox);

    return newObject;
}


}
}