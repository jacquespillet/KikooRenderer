#include "NURBS.hpp"

#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"
#include "../Components/MeshFilterComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {

NURBS::NURBS(std::string name, Scene* scene) : Object3D(name, scene) {
    points.push_back(glm::vec3(1, 2, 0));
    points.push_back(glm::vec3(2, 0, 0));
    points.push_back(glm::vec3(3, -1, 0));
    points.push_back(glm::vec3(5, -2, 0));

    knotVector.push_back(0.0);
    knotVector.push_back(0.0);
    knotVector.push_back(0.0);
    knotVector.push_back(0.0);
    knotVector.push_back(1.0);
    knotVector.push_back(2.0);
    knotVector.push_back(3.0);
    knotVector.push_back(3.0);
    knotVector.push_back(3.0);
    knotVector.push_back(3.0);


    line = new Object3D(name, scene);
    mesh = new MeshFilterComponent(line);
    material = new MaterialComponent(line);

    ComputePositions();

    //Setup transform
    TransformComponent* transform = new TransformComponent(line );
    
    //Setup material
    material->albedo = glm::vec4(1);
    Shader shader = scene->standardShaders.unlitMeshShader;
    material->SetShader(shader);

    line->AddComponent(material);
	line->transform = transform;
    line->AddComponent(mesh);
    
    line->Start();
    line->Enable();
}

void NURBS::WindowResize(int w, int h) {
}


void NURBS::Start() {
	started = true;
}

void NURBS::Enable() {
	enabled = true;
}

void NURBS::Update() {}

double NURBS::CoxDeBoor(double i, double k, double u, std::vector<double> knotVector) {
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

void NURBS::ComputePositions() {
    int totalPoints = (points.size()-3) * (1.0 / offset);

    int inx =0;
    double globalU = 0.0;
    for(int i=1; i<=points.size()-2; i++) {
        for(double u=0; u<1; u+=offset, globalU+=offset) {
            glm::vec3 H = glm::vec3(0);
            for(int k=0; k<=3; k++) {
                double N = CoxDeBoor((double)i+k-1, 3.0, globalU, knotVector);
                H +=  N * points[i+k-1];
            }

            vertex.push_back(H); 
            normals.push_back(glm::vec3(0, 0, 1));
            uv.push_back(glm::vec2(0, 0));
            colors.push_back(glm::vec4(1));
            inx++;
            if(inx < totalPoints) {
                triangles.push_back(inx-1);
                triangles.push_back(inx);
            }
        }
    }

    //Setup mesh
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles);
    mesh->drawingMode = GL_LINES;
    mesh->primitiveSize = 10;
}

std::vector<QWidget*> NURBS::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    res.push_back(this->line->transform->GetInspector());
    
    QGroupBox* mainGroupbox = new QGroupBox("Catmut Roll Inspector");
    QVBoxLayout* mainLayout = new QVBoxLayout();
    
    Vector3ArrayInspector* wavesInspector = new Vector3ArrayInspector("Control points", points, glm::vec3(0, 0, 0)); 
    QObject::connect(wavesInspector, &Vector3ArrayInspector::Modified, [this](std::vector<glm::vec3> vectors) {
        points = vectors;
        ComputePositions();
        line->scene->triggerRefresh = true;
    });    
    mainLayout->addWidget(wavesInspector);

    mainGroupbox->setLayout(mainLayout);
    res.push_back(mainGroupbox);

    return res;
}


void NURBS::Render(glm::mat4* overrideViewMatrixp) {
    GETGL
    // ogl->glUseProgram(waterShader.programShaderObject);
    // ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "time"), scene->elapsedTime);
    
    // glm::mat4 viewProjection = scene->camera->GetProjectionMatrix() * scene->camera->GetViewMatrix();
    // int viewProjectionMatLoc = ogl->glGetUniformLocation(waterShader.programShaderObject, "viewProjectionMatrix"); 
    // ogl->glUniformMatrix4fv(viewProjectionMatLoc, 1, false, glm::value_ptr(viewProjection));

    // for(int i=0; i<waves.size(); i++) {
    //     std::string name = "waves[" + std::to_string(i) + "]";
    //     ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, name.c_str()), 1, glm::value_ptr(waves[i]));
    // }

    // // ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[0]"), 1, glm::value_ptr(glm::vec4(1, 0, 0.75, 6)));
    
    // ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "numWaves"), waves.size());

    line->Render();
}

void NURBS::DepthRenderPass(LightComponent* light) {
}

void NURBS::Destroy() {
}

void NURBS::Recompute() {
}


}
}