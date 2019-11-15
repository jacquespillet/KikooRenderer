#include "Bounds.hpp"


namespace KikooRenderer {
namespace OfflineRenderer {

Bounds::Bounds() {
    min = glm::vec3(-1);
    max = glm::vec3(1);
}

Bounds::Bounds(std::vector<glm::vec3> vertices, glm::mat4 _transform) {
    max = glm::vec3(-999999999);
    min = glm::vec3(std::numeric_limits<float>::max());
    for(int i=0; i<vertices.size(); i++) {
        glm::vec3 transformedPos = _transform * glm::vec4(vertices[i], 1);
        if(transformedPos.x < min.x) min.x = transformedPos.x;
        if(transformedPos.y < min.y) min.y = transformedPos.y;
        if(transformedPos.z < min.z) min.z = transformedPos.z;

        if(transformedPos.x > max.x) max.x = transformedPos.x;
        if(transformedPos.y > max.y) max.y = transformedPos.y;
        if(transformedPos.z > max.z) max.z = transformedPos.z;
    }
    
    transform = glm::mat4(1);
    transform = glm::scale(transform, GetSize());
    transform = glm::translate(transform, GetCenter());

    inverseTransform = glm::inverse(transform);

    std::cout << "CREATED SHAPE " << glm::to_string(min) << " " << glm::to_string(max) << "  " << glm::to_string(transform) << std::endl;
}

Bounds::Bounds(std::vector<Bounds> bounds) {

}

Bounds::Bounds(Bounds b0, Bounds b1) {
    min = glm::min(b0.min, b1.min);
    max = glm::max(b0.max, b1.max);
}


glm::vec3 Bounds::GetCenter() {
    return (min + max) * 0.5;
}

glm::vec3 Bounds::GetSize() {
    return max - min;
}


double Bounds::Hit(KikooRenderer::Geometry::Ray ray) {
    Geometry::Ray tmpRay = ray;
    
    //Ray from world space to local space
    ray.origin = glm::vec3( inverseTransform * glm::vec4(ray.origin, 1) );
    ray.direction = glm::vec3( inverseTransform * glm::vec4(ray.direction, 0) );

    //AABB Ray intersection 
    glm::vec3 rayDirInv = glm::vec3(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z); 
    glm::vec3 t1 = (glm::vec3(-0.5) - ray.origin) * rayDirInv;
    glm::vec3 t2 = (glm::vec3(0.5) - ray.origin) * rayDirInv;
    
    glm::vec3 intersectionPosition = ray.origin + t2.x * ray.direction;
    float tmin = (std::max)( (std::max)( (std::min)(t1.x, t2.x), (std::min)(t1.y, t2.y)), (std::min)(t1.z, t2.z));
    float tmax = (std::min)( (std::min)( (std::max)(t1.x, t2.x), (std::max)(t1.y, t2.y)), (std::max)(t1.z, t2.z));
    bool raybox = tmax >= (std::max)(0.0f, tmin);
    
    double distance = -1;
    if(tmin < 0) {
        distance = tmax;
    } else {
        distance = tmin;
    }
    
    if(!raybox)
        distance = -1; 

    return distance;
}

}
}