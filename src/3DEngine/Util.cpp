#include "Util.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
namespace Util {
    bool RayBoxTest(glm::vec3 rayOrig, glm::vec3 rayDir, glm::mat4 transform, glm::vec3 minScale,glm::vec3 maxScale, double& distance)
    {
        //transform ray
        glm::mat4 invTr = glm::inverse(transform); 
        rayOrig = glm::vec3( invTr * glm::vec4(rayOrig, 1) );
        rayDir = glm::vec3( invTr * glm::vec4(rayDir, 0) );
        
        //AABB Ray intersection 
        glm::vec3 rayDirInv = glm::vec3(1.0f / rayDir.x, 1.0f / rayDir.y, 1.0f / rayDir.z); 
        glm::vec3 t1 = (minScale - rayOrig) * rayDirInv;
        glm::vec3 t2 = (maxScale - rayOrig) * rayDirInv;
        
        glm::vec3 intersectionPosition = rayOrig + t2.x * rayDir;
        float tmin = (std::max)( (std::max)( (std::min)(t1.x, t2.x), (std::min)(t1.y, t2.y)), (std::min)(t1.z, t2.z));
        float tmax = (std::min)( (std::min)( (std::max)(t1.x, t2.x), (std::max)(t1.y, t2.y)), (std::max)(t1.z, t2.z));
        bool raybox = tmax >= (std::max)(0.0f, tmin);
        distance = tmin; 
       
        if(!raybox)
            distance = -1; 
        return raybox;
    }

	bool RayWireCircleTest(glm::vec3 rayOrig, glm::vec3 rayDir, glm::mat4 transform, float radius, double& distance) {
		glm::vec3 circlePosition = glm::vec3(glm::column(transform, 3));
		double circleDistance = glm::length(circlePosition);
		glm::vec3 normal = -glm::column(transform, 2);
		glm::vec4 plane = glm::vec4(normal.x, normal.y, normal.z, circleDistance);

		distance = -glm::dot(plane, glm::vec4(rayOrig, 1)) / glm::dot(plane, glm::vec4(rayDir, 0));
		glm::vec3 intersectPos = rayOrig + (float)distance * rayDir;


		double intersectionDistance = glm::distance(intersectPos, circlePosition);
		if(intersectionDistance <= radius + 0.1 && intersectionDistance >= radius - 0.1 ) {
			return true;
		} else {
			return false;
		}
	}
    
    bool CameraBoxTest(CameraScene& camera, TransformComponent* transform) {
		Geometry::Planes planes = camera.GetPlanes();

		glm::vec3 position = transform->position;
		glm::mat4 cubeTransform = transform->GetModelMatrix();
        //Transform them
		glm::vec3 S = glm::normalize(glm::column(cubeTransform, 0)) * transform->scale.x;
		glm::vec3 R = glm::normalize(glm::column(cubeTransform, 1)) * transform->scale.x;
		glm::vec3 T = glm::normalize(glm::column(cubeTransform, 2)) * transform->scale.x;

        //Right plane
		glm::vec3 N = glm::vec3(planes.rightPlane);
		double effectiveRadius = 0.5 * (std::abs(glm::dot(R, N)) + std::abs(glm::dot(S, N)) + std::abs(glm::dot(T, N)));
		double dotPlaneCenter = glm::dot(planes.rightPlane, glm::vec4(position, 1));
		
		if (dotPlaneCenter <= -effectiveRadius) {
			return false;
		}

		//Left plane
		N = glm::vec3(planes.leftPlane);
		effectiveRadius = 0.5 * (std::abs(glm::dot(R, N)) + std::abs(glm::dot(S, N)) + std::abs(glm::dot(T, N)));
		dotPlaneCenter = glm::dot(planes.leftPlane, glm::vec4(position, 1));

		if (dotPlaneCenter <= -effectiveRadius) {
			return false;
		}


		//top plane
		N = glm::vec3(planes.topPlane);
		effectiveRadius = 0.5 * (std::abs(glm::dot(R, N)) + std::abs(glm::dot(S, N)) + std::abs(glm::dot(T, N)));
		dotPlaneCenter = glm::dot(planes.topPlane, glm::vec4(position, 1));

		if (dotPlaneCenter <= -effectiveRadius) {
			return false;
		}

		//bottomplane
		N = glm::vec3(planes.bottomPlane);
		effectiveRadius = 0.5 * (std::abs(glm::dot(R, N)) + std::abs(glm::dot(S, N)) + std::abs(glm::dot(T, N)));
		dotPlaneCenter = glm::dot(planes.bottomPlane, glm::vec4(position, 1));

		if (dotPlaneCenter <= -effectiveRadius) {
			return false;
		}


		//near plane
		N = glm::vec3(planes.nearPlane);
		effectiveRadius = 0.5 * (std::abs(glm::dot(R, N)) + std::abs(glm::dot(S, N)) + std::abs(glm::dot(T, N)));
		dotPlaneCenter = glm::dot(planes.nearPlane, glm::vec4(position, 1));

		if (dotPlaneCenter <= -effectiveRadius) {
			return false;
		}


		//farplane
		N = glm::vec3(planes.farPlane);
		effectiveRadius = 0.5 * (std::abs(glm::dot(R, N)) + std::abs(glm::dot(S, N)) + std::abs(glm::dot(T, N)));
		dotPlaneCenter = glm::dot(planes.farPlane, glm::vec4(position, 1));

		if (dotPlaneCenter <= effectiveRadius) {
			return false;
		}

        return true;
    }

}
    
}
}