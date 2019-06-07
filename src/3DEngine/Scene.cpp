#include "Scene.hpp"
#include "Util.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {

namespace CoreEngine {
    Scene::Scene() : camera(CameraScene(this, 1.0, 70 * Util::DEGTORAD, 0.1, 1000.0, 1.0)){
        this->started = false;
    }

    void Scene::Start() {
        this->started = true;
        OnStart();

        //Start each Object3D in scene
        Object3D* newObject = new Object3D("Triangle", this);
        std::vector<glm::dvec3> vertex;
        std::vector<glm::dvec3> normals;
        std::vector<glm::dvec2> uv;
        std::vector<glm::dvec4> colors;
        std::vector<int> triangles;

        //Front 1
        vertex.push_back(glm::dvec3(-0.5, -0.5, -0.5)); //bottom left
        vertex.push_back(glm::dvec3(-0.5, 0.5, -0.5)); // top left
        vertex.push_back(glm::dvec3(0.5, 0.5, -0.5)); //Top right
        vertex.push_back(glm::dvec3(0.5, -0.5, -0.5)); //Bottom right
        normals.push_back(glm::dvec3(0, 0, 1));
        normals.push_back(glm::dvec3(0, 0, 1));
        normals.push_back(glm::dvec3(0, 0, 1));
        normals.push_back(glm::dvec3(0, 0, 1));
        uv.push_back(glm::dvec2(0, 0));
        uv.push_back(glm::dvec2(0, 0));
        uv.push_back(glm::dvec2(0, 0));
        uv.push_back(glm::dvec2(0, 0));
        colors.push_back(glm::dvec4(255.0, 0, 0, 255));
        colors.push_back(glm::dvec4(0.0, 255, 0, 255));
        colors.push_back(glm::dvec4(0.0, 0, 255, 255));
        colors.push_back(glm::dvec4(255.0, 0, 0, 255));
        // colors.push_back(glm::dvec4(1.0, 0, 0, 1));
        // colors.push_back(glm::dvec4(1.0, 0, 0, 1));
        // colors.push_back(glm::dvec4(1.0, 0, 0, 1));
        // colors.push_back(glm::dvec4(1.0, 0, 0, 1));

        triangles.push_back(0);
        triangles.push_back(2);
        triangles.push_back(1);

        triangles.push_back(3);
        triangles.push_back(2);
        triangles.push_back(0);

        MeshFilterComponent* mesh = new MeshFilterComponent();
        mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);
        TransformComponent* transform = new TransformComponent();
        
        newObject->AddComponent(mesh);
        newObject->AddComponent(transform);

        objects3D.push_back(newObject);

        //Start each object
    }

    void Scene::Enable() {
        standardShaders.Compile();
        //Enable each Object3D in scene
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->Enable();
        }
    }

    void Scene::Render() {
        GETGL
        ogl->glClearColor(100, 0, 0, 0);
        // ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render each object
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->Render();
        }
    }

    //Update : 
    //Foreach object 
    //  If not started :
    //      Start it
    //      Enable it
    //Foreach object
    //  Update 

    //AddObject

    //RemoveObject

    //FindObjectByName

    //Enable

    //Disable

    //Destroy


    void Scene::OnKeyPressEvent(QKeyEvent *e){
    	OnKeyPressEvent(e);
        std::cout << "key" << std::endl;
    }

    void Scene::SetWindowSize(int w, int h) {
        this->windowWidth = w;
        this->windowHeight = h;
        float aspectRatio = w/h;

        this->camera.aspect = aspectRatio;
        this->camera.UpdateProjectionMatrix();
    }
}
}