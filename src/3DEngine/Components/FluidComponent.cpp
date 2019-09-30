#include "FluidComponent.hpp"
#include "MeshFilterComponent.hpp"
#include "3DEngine/Scene.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {
    FluidComponent::FluidComponent(Object3D* object) : Component("Fluid", object) {
        meshFilter = nullptr;
        /* initialize random seed: */
        srand (time(NULL));
        numVert = width * height;

        float f1 = c * c * t * t / (d * d);
        float f2 = 1.0F / (mu * t + 2);
        k1 = (4.0F - 8.0F * f1) * f2;
        k2 = (mu * t - 2) * f2;
        k3 = 2.0F * f1 * f2;

        buffer[0].resize(numVert);
        buffer[1].resize(numVert);
        normal.resize(numVert);
        tangent.resize(numVert);

        glm::vec2 center = glm::vec2((width * d) / 2,(height * d) / 2);
        long a = 0;
        for (long j = 0; j < height; j++)  {  
            float y = d * j;
            for (long i = 0; i < width; i++)   {
                // float z = (float)i / (float)width * 3;
                float z = 0;
                glm::vec2 position= glm::vec2(d * i, y);
                float distanceWithCenter = glm::distance(position, center);
                if(distanceWithCenter < 1) {
                    z = -(1-distanceWithCenter) * 10;
                }
                buffer[0][a] = glm::vec3(d * i, y, z);
                buffer[1][a] = buffer[0][a];
                
                normal[a] = glm::vec3(0.0F, 0.0F, 1.0F * d);
                tangent[a] = glm::vec3(1.0F * d, 0.0F, 0.0F);
                a++;
            }
        }        
    }
   
    void FluidComponent::OnStart() {
    }

	void FluidComponent::Recompute() {
	}

    void FluidComponent::OnEnable() {
        
    }
    
    void FluidComponent::OnUpdate() {
        if(meshFilter == nullptr) {
            meshFilter = object3D->GetComponent<MeshFilterComponent>();
        }

        for (long j = 1; j < height - 1; j++)  {   
            for (long i = 1; i < width - 1; i++)   {
                int inx = width * j + i;
                buffer[1 - renderBuffer][inx].z = k1 * buffer[renderBuffer][inx].z + k2 * buffer[1 - renderBuffer][inx].z
                + k3 * (buffer[renderBuffer][inx + 1].z + buffer[renderBuffer][inx - 1].z 
                + buffer[renderBuffer][inx + width].z + buffer[renderBuffer][inx - width].z);   
            }  
        } 
        renderBuffer = 1 - renderBuffer;

        for (long j = 1; j < height - 1; j++)  {   
            for (long i = 1; i < width - 1; i++)   {
                int inx = width * j + i;
                normal[inx].x = buffer[renderBuffer][inx - 1].z - buffer[renderBuffer][inx + 1].z;    
                normal[inx].y = buffer[renderBuffer][inx - width].z - buffer[renderBuffer][inx + width].z;    
                tangent[inx].z = buffer[renderBuffer][inx + 1].z - buffer[renderBuffer][inx - 1].z;   
            }
        }

        for(int i=0; i<meshFilter->vertices.size(); i++) {
            meshFilter->vertices[i].position.x = buffer[renderBuffer][i].x;
            meshFilter->vertices[i].position.y = buffer[renderBuffer][i].z;
            meshFilter->vertices[i].position.z = buffer[renderBuffer][i].y;
            
            meshFilter->vertices[i].normal.x = normal[i].x;
            meshFilter->vertices[i].normal.y = normal[i].z;
            meshFilter->vertices[i].normal.z = normal[i].y;
        }

        meshFilter->RebuildBuffers();
        object3D->scene->triggerRefresh = true;
    }
    
    void FluidComponent::OnRender() {

    }
    
    void FluidComponent::OnDestroy() {

    }    
}
}