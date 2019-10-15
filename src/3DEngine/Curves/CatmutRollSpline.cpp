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

void CatmutRollSpline::WindowResize(int w, int h) {
}


void CatmutRollSpline::Start() {
	started = true;
}

void CatmutRollSpline::Enable() {
	enabled = true;
}

void CatmutRollSpline::Update() {}

void CatmutRollSpline::ComputePositions() {
    int totalPoints = (points.size()-3) * (1.0 / offset);

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
    mesh->primitiveSize = 10;
}

std::vector<QWidget*> CatmutRollSpline::GetInspectorWidgets() {
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


void CatmutRollSpline::Render(glm::mat4* overrideViewMatrixp) {
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

void CatmutRollSpline::DepthRenderPass(LightComponent* light) {
}

void CatmutRollSpline::Destroy() {
}

void CatmutRollSpline::Recompute() {
}


}
}