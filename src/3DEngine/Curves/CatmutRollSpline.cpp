#include "CatmutRollSpline.hpp"

#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"
#include "../Components/MeshFilterComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {

CatmutRollSpline::CatmutRollSpline(std::string name, Scene* scene) : Object3D(name, scene) {
    points.push_back(glm::vec3(1, 2, 0));
    points.push_back(glm::vec3(2, 0, 0));
    points.push_back(glm::vec3(3, -1, 0));
    points.push_back(glm::vec3(5, -2, 0));
    points.push_back(glm::vec3(6, 1, 0));

    line = new Object3D(name, scene);
    mesh = new MeshFilterComponent(line);
    material = new MaterialComponent(line);

    ComputePositions();

    //Setup transform
    TransformComponent* transform = new TransformComponent(line );
    this->transform = transform;
    
    //Setup material
    material->albedo = glm::vec4(1);
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);

    line->AddComponent(material);
	line->transform = transform;
    line->AddComponent(mesh);
    
}

void CatmutRollSpline::WindowResize(int w, int h) {
}


void CatmutRollSpline::Start() {
    line->Start();
	started = true;
}

void CatmutRollSpline::Enable() {
    line->Enable();
	enabled = true;
}

void CatmutRollSpline::Update() {
    if(isEdit) {
        for(int i=0; i<editingObjects.size(); i++) {
            if(editingObjects[i]->transform->hasChanged) {
                std::cout << "HERE "<< std::endl;
                points[i] = editingObjects[i]->transform->GetWorldPosition();
                ComputePositions();
                editingObjects[i]->transform->hasChanged = false;

                line->scene->glWindow->makeCurrent();
                mesh->RebuildBuffers();
                line->scene->glWindow->doneCurrent();
                
                line->scene->triggerRefresh = true;
            }
        }
    }
}

Object3D* CatmutRollSpline::Intersects(Geometry::Ray ray, double& _distance) {
    _distance = 100000;
    double minDistance = 100000;
    Object3D* closest = nullptr;
    if(isEdit) {
        if(editingObjects.size() > 0) {
			for(int i=0; i<editingObjects.size(); i++) {
				double distance;
				Object3D* intersectedObject = editingObjects[i]->Intersects(ray, distance);

				if(intersectedObject != nullptr) {
					if(distance < minDistance) {
						minDistance = distance;
						closest = intersectedObject;
					}
				}
			}     
        }
    }
    _distance = minDistance;
    return closest;
}

void CatmutRollSpline::ComputePositions() {
    int totalPoints = (points.size()-3) * (1.0 / offset);
    for(int i=0; i<points.size(); i++) {
        if(i < editingObjects.size()) {
            editingObjects[i]->transform->position = points[i];
        } else {
            Object3D* vert = GetCube(scene, "points_" + std::to_string(i), points[i], glm::vec3(0, 0, 0), glm::vec3(0.05), glm::vec4(0.0, 0.0, 0.0, 1.0));
            vert->SetIsVertex(true);
            vert->Start();
            vert->Enable();
            editingObjects.push_back(vert);
        }
    }

    vertex.resize(0);
    normals.resize(0);
    uv.resize(0);
    colors.resize(0);
    int inx =0;
    for(int i=1; i<points.size()-2; i++) {
        for(double t=0; t<1; t+=offset) {
            inx++;

            if(inx < totalPoints) {
                triangles.push_back(inx-1);
                triangles.push_back(inx);
            }

            glm::mat4 basis = glm::mat4(0);
            basis[0][0] = 1; basis[1][0] = -3; basis[2][0] =  3; basis[3][0] = -1; 
            basis[0][1] = 4; basis[1][1] =  0; basis[2][1] = -6; basis[3][1] =  3; 
            basis[0][2] = 1; basis[1][2] =  3; basis[2][2] =  3; basis[3][2] = -3; 
            basis[0][3] = 0; basis[1][3] =  0; basis[2][3] =  0; basis[3][3] =  1;
            basis *= (1.0 / 6.0);

            glm::mat4 geometry = glm::mat4(0);
            geometry[0] = glm::vec4(points[i-1], 1);
            geometry[1] = glm::vec4(points[i], 1);
            geometry[2] = glm::vec4(points[i+1], 1);
            geometry[3] = glm::vec4(points[i+2], 1);

            glm::vec4 tVec = glm::vec4(1, t, t*t, t*t*t);

            glm::vec3 H = geometry * basis * tVec;

            vertex.push_back(H); 
            normals.push_back(glm::vec3(0, 0, 1));
            uv.push_back(glm::vec2(0, 0));
            colors.push_back(glm::vec4(1));
        }
    }

    //Setup mesh
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);
    mesh->drawingMode = GL_LINES;
}

std::vector<QWidget*> CatmutRollSpline::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    res.push_back(this->line->transform->GetInspector());
    
    QGroupBox* mainGroupbox = new QGroupBox("Catmut Roll Inspector");
    QVBoxLayout* mainLayout = new QVBoxLayout();
    
    Vector3ArrayInspector* pointsInspector = new Vector3ArrayInspector("Control points", points, glm::vec3(0, 0, 0)); 
    QObject::connect(pointsInspector, &Vector3ArrayInspector::Modified, [this](std::vector<glm::vec3> vectors) {
        points = std::vector<glm::vec3>(vectors);
        line->scene->glWindow->makeCurrent();
        ComputePositions();
        mesh->RebuildBuffers();
        line->scene->glWindow->doneCurrent();
        
        line->scene->triggerRefresh = true;
    });    
    mainLayout->addWidget(pointsInspector);

    mainGroupbox->setLayout(mainLayout);
    res.push_back(mainGroupbox);

    return res;
}


void CatmutRollSpline::Render(glm::mat4* overrideViewMatrixp) {
    GETGL
    if(isEdit) {
        for(int i=0; i<editingObjects.size(); i++) {
            editingObjects[i]->Render();
        }
    }
    line->Render();
}

void CatmutRollSpline::DepthRenderPass(LightComponent* light) {
}

void CatmutRollSpline::Destroy() {
}

void CatmutRollSpline::Recompute() {
}


}
}