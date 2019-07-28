#include "Util.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
namespace Util {
    bool RayBoxTest(glm::dvec3 rayOrig, glm::dvec3 rayDir, glm::dmat4 transform, glm::dvec3 minScale,glm::dvec3 maxScale, double& distance)
    {
        //transform ray
        glm::dmat4 invTr = glm::inverse(transform); 
        rayOrig = glm::dvec3( invTr * glm::vec4(rayOrig, 1) );
        rayDir = glm::dvec3( invTr * glm::vec4(rayDir, 0) );
        
        //AABB Ray intersection 
        glm::dvec3 rayDirInv = glm::dvec3(1.0f / rayDir.x, 1.0f / rayDir.y, 1.0f / rayDir.z); 
        glm::dvec3 t1 = (minScale - rayOrig) * rayDirInv;
        glm::dvec3 t2 = (maxScale - rayOrig) * rayDirInv;
        
        glm::dvec3 intersectionPosition = rayOrig + t2.x * rayDir;
        float tmin = (std::max)( (std::max)( (std::min)(t1.x, t2.x), (std::min)(t1.y, t2.y)), (std::min)(t1.z, t2.z));
        float tmax = (std::min)( (std::min)( (std::max)(t1.x, t2.x), (std::max)(t1.y, t2.y)), (std::max)(t1.z, t2.z));
        bool raybox = tmax >= (std::max)(0.0f, tmin);
        distance = tmin; 
       
        if(!raybox)
            distance = -1; 
        return raybox;
    }
    
    bool cameraBoxTest(CameraScene& camera, glm::dvec3 position, glm::dvec3 size) {
        return true;
    }

}
    
}
}