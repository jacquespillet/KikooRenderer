#include "Box.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {
    Box::Box(glm::vec3 position, glm::vec3 size, Material* material) : material(material), invTransf(glm::mat4(1)){
        this->position = position;
        this->size = size;

        glm::mat4 transf(1);
        transf = glm::scale(transf, size);
        transf = glm::translate(transf, position);
        invTransf = glm::inverse(transf);
    };

    glm::vec3 Box::GetPosition(double time) {
        return this->position;
    }


    double Box::HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint) {
        glm::vec3 rayOrig = glm::vec3( invTransf * glm::vec4(ray.origin, 1) );
        glm::vec3 rayDir = glm::vec3( invTransf * glm::vec4(ray.direction, 0) );
        double distance;

        //AABB Ray intersection 
        glm::vec3 rayDirInv = glm::vec3(1.0f / rayDir.x, 1.0f / rayDir.y, 1.0f / rayDir.z); 
        glm::vec3 t1 = (glm::vec3(-0.5) - rayOrig) * rayDirInv;
        glm::vec3 t2 = (glm::vec3(0.5) - rayOrig) * rayDirInv;
        
        glm::vec3 intersectionPosition = rayOrig + t2.x * rayDir;
        float tmin = (std::max)( (std::max)( (std::min)(t1.x, t2.x), (std::min)(t1.y, t2.y)), (std::min)(t1.z, t2.z));
        float tmax = (std::min)( (std::min)( (std::max)(t1.x, t2.x), (std::max)(t1.y, t2.y)), (std::max)(t1.z, t2.z));
        bool raybox = tmax >= (std::max)(0.0f, tmin);

        if(tmin < 0) {
            distance = tmax;
        } else {
            distance = tmin;
        }
       
        if(!raybox)
            distance = -1; 
        glm::vec3 hitPos = ray.pointAtPosition(distance);

        glm::vec3 normal;
        if(hitPos.x == 0.5) normal = glm::vec3(1, 0, 0);
        else if(hitPos.x == -0.5) normal = glm::vec3(-1, 0, 0);
        else if(hitPos.y ==  0.5) normal = glm::vec3(0, 1, 0);
        else if(hitPos.y == -0.5) normal = glm::vec3(0, -1, 0);
        else if(hitPos.z ==  0.5) normal = glm::vec3(0, 0, 1);
        else if(hitPos.z == -0.5) normal = glm::vec3(0, 0, -1);

        hitPoint = {
            distance, 
            hitPos,
            normal,
            material,
            glm::vec2(0)
        };        
        
        return distance;
    }
}
}