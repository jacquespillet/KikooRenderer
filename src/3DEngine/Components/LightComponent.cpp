#include "LightComponent.hpp"
#include "TransformComponent.hpp"
#include "../Scene.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {

LightComponent::LightComponent(Object3D* object, glm::dvec4 color, glm::dvec3 attenuation, int type) : Component("Light", object) {
    this->type = type;
    this->attenuation =  attenuation;
    this->color = color;   

    object3D->scene->glWindow->makeCurrent();
    //TODO : Mettre les shaders static et compilés au start 
    if(type==0) {
        //Avant dernier arg TRUE pour debug, doit etre FALSE
        depthFBO = new Framebuffer(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, false, true);
        lightProjection  = glm::orthoLH(-20.0, 20.0, -20.0, 20.0, 1.0, 100.0);

        depthPassShader.vertSrc= R"(
        #version 440
        layout(location = 0) in vec3 position;

        uniform mat4 modelViewProjectionMatrix;
        void main()
        {
            gl_Position = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
        }
        )";

        depthPassShader.fragSrc = R"(
        #version 440
        layout(location = 0) out vec4 outputColor; 
        void main()
        {   
            outputColor = vec4(0);
        }
        )";
        depthPassShader.name = "quad";
        depthPassShader.isLit = false;
        depthPassShader.isDepthPass = true;
        std::cout << "StandardShaders: Compiling depthPassShader" << std::endl; 
        depthPassShader.Compile();


    } else if(type == 1) {
        depthCubeFBO = new CubeFramebuffer(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, true, true);
        lightSpaceMatrices.resize(6);

        cubeDepthPassShader.vertSrc= R"(
        #version 330 core
        layout(location = 0) in vec3 position;

        uniform mat4 modelMatrix;
        void main()
        {
            gl_Position = modelMatrix * vec4(position.x, position.y, position.z, 1.0f);
        }
        )";

        cubeDepthPassShader.geometrySrc= R"(
        #version 330 core
        layout (triangles) in;
        layout (triangle_strip, max_vertices=18) out;

        uniform mat4 shadowMatrices[6];

        out vec4 FragPos; // FragPos from GS (output per emitvertex)

        void main()
        {
            for(int face = 0; face < 6; ++face)
            {
                gl_Layer = face; // built-in variable that specifies to which face we render.
                for(int i = 0; i < 3; ++i) // for each triangle's vertices
                {
                    FragPos = gl_in[i].gl_Position;
                    gl_Position = shadowMatrices[face] * FragPos;
                    EmitVertex();
                }    
                EndPrimitive();
            }
        }  
        )";


        cubeDepthPassShader.fragSrc = R"(
        #version 330 core
        in vec4 FragPos;

        uniform vec3 lightPos;
        uniform float farPlane;

        void main()
        {
            // get distance between fragment and light source
            float lightDistance = length(FragPos.xyz - lightPos);
            
            // map to [0;1] range by dividing by farPlane
            lightDistance = lightDistance / farPlane;
            
            // write this as modified depth
            gl_FragDepth = lightDistance;
        }
        )";
        cubeDepthPassShader.name = "quad";
        cubeDepthPassShader.isLit = false;
        cubeDepthPassShader.isDepthPass = true;
        std::cout << "StandardShaders: Compiling cubeDepthPassShader" << std::endl; 
        cubeDepthPassShader.Compile();
    } else if(type==2) {
        //Avant dernier arg TRUE pour debug, doit etre FALSE
        depthFBO = new Framebuffer(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, false, true);
        lightProjection  = glm::perspectiveLH(80.0f, 1.0f, 0.01f, 100.0f);
        // lightProjection  = glm::perspectiveLH((float)(DEGTORAD * fov), 1.0f, 0.01f, 100.0f);

        depthPassShader.vertSrc= R"(
        #version 440
        layout(location = 0) in vec3 position;

        uniform mat4 modelViewProjectionMatrix;
        void main()
        {
            gl_Position = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
        }
        )";

        depthPassShader.fragSrc = R"(
        #version 440
        layout(location = 0) out vec4 outputColor; 
        void main()
        {   
            outputColor = vec4(0);
        }
        )";
        depthPassShader.name = "quad";
        depthPassShader.isLit = false;
        depthPassShader.isDepthPass = true;
        std::cout << "StandardShaders: Compiling depthPassShader" << std::endl; 
        depthPassShader.Compile();


    }


    object3D->scene->glWindow->doneCurrent();
}

void LightComponent::RenderDepthMap() {
    GETGL;

    ogl->glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    if(type==0) {
        depthFBO->Enable(); 
        ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);

        glm::dmat4 model = object3D->transform->GetWorldRotationMatrix();
        glm::dvec3 lightPos = -20.0 * glm::dvec4(glm::column(model, 2));
        model = glm::translate(model, lightPos);
        viewMat = glm::inverse(model);
        lightSpaceMatrix = lightProjection * viewMat;

        ogl->glCullFace(GL_FRONT);
        for(int i=0; i<object3D->scene->objects3D.size(); i++) {
            if(object3D->scene->objects3D[i] && object3D->scene->objects3D[i]->visible ) {
                MaterialComponent* material = (MaterialComponent*) object3D->scene->objects3D[i]->GetComponent("Material");
                if(material != nullptr) {
                    Shader* tmpShader = material->shader;
                    ShaderParams* tmpParams = material->params;
                    material->SetShader(&depthPassShader);
                    object3D->scene->objects3D[i]->DepthRenderPass(this); 
                    material->SetShader(tmpShader);
                    material->params = tmpParams;
                }
            }
        }
        ogl->glCullFace(GL_BACK);
        depthFBO->Disable();
    } else if(type == 1) {

        glm::vec3 lightPos = glm::vec3(object3D->transform->position);
        nearPlane = 0.001;
        farPlane  = 100.0;

        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)1.0, nearPlane, farPlane);

        lightSpaceMatrices[0] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        lightSpaceMatrices[1] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        lightSpaceMatrices[2] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
        lightSpaceMatrices[3] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
        lightSpaceMatrices[4] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        lightSpaceMatrices[5] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

        depthCubeFBO->Enable();
        ogl->glClear(GL_DEPTH_BUFFER_BIT);

        for(int i=0; i<object3D->scene->objects3D.size(); i++) {
            if(object3D->scene->objects3D[i] && object3D->scene->objects3D[i]->visible ) {
                MaterialComponent* material = (MaterialComponent*) object3D->scene->objects3D[i]->GetComponent("Material");
                if(material != nullptr) {
                    Shader* tmpShader = material->shader;
                    ShaderParams* tmpParams = material->params;
                    material->SetShader(&cubeDepthPassShader);
                    
                    object3D->scene->objects3D[i]->DepthRenderPass(this); 
                    
                    material->SetShader(tmpShader);
                    material->params = tmpParams;
                }
            }
        }
        depthCubeFBO->Disable();
    } else  if(type==2) {
        depthFBO->Enable(); 
        ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);

        glm::dmat4 model = object3D->transform->GetWorldModelMatrix();
        viewMat = glm::inverse(model);
        lightSpaceMatrix = lightProjection * viewMat;

        for(int i=0; i<object3D->scene->objects3D.size(); i++) {
            if(object3D->scene->objects3D[i] && object3D->scene->objects3D[i]->visible ) {
                MaterialComponent* material = (MaterialComponent*) object3D->scene->objects3D[i]->GetComponent("Material");
                if(material != nullptr) {
                    Shader* tmpShader = material->shader;
                    ShaderParams* tmpParams = material->params;
                    material->SetShader(&depthPassShader);
                    object3D->scene->objects3D[i]->DepthRenderPass(this); 
                    material->SetShader(tmpShader);
                    material->params = tmpParams;
                }
            }
        }

        depthFBO->Disable();
    }
}

void LightComponent::SetShaderUniforms() {
    if(type==0) {
        
    } else {

    }
}

void LightComponent::OnStart(){}
void LightComponent::OnEnable(){}
void LightComponent::OnUpdate(){}
void LightComponent::OnRender(){} 
void LightComponent::OnDestroy(){} 
void LightComponent::Recompute(){} 

}
}