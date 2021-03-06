#pragma once
#include "Shape.hpp"
#include "Geometry/Ray.hpp"
#include "../Materials/Material.hpp"
#include "../SpatialAcceleration/Bounds.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {

class Material;
class TriangleMesh : public Shape {
    public:
        glm::vec3 position;
        glm::vec3 size;
        glm::mat4 invTransf;
        
        TriangleMesh(glm::vec3 position, glm::vec3 size, Material* material, std::string fileName);
        TriangleMesh(glm::vec3 position, glm::vec3 size, Material* material, std::vector<glm::vec3> vertex,std::vector<glm::vec3> normals,std::vector<glm::vec2> uv, std::vector<int> triangles);
        ~TriangleMesh();
        double HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint);
        glm::vec3 GetPosition(double time);
        glm::vec3 GetSize(double time);
        void GetWorldBounds(glm::vec3& min, glm::vec3& max);

        float pdfValue(glm::vec3 origin, glm::vec3 direction, std::vector<Shape*>& shapes);
        glm::vec3 random(glm::vec3 origin);

private:
        void Init();
        
        Bounds bounds;

        glm::mat4 transf;

        void GetSurfaceProperties( const uint32_t &triIndex,  const glm::vec2 &uv, glm::vec3 &hitNormal,glm::vec3 &hitTangent,glm::vec3 &hitBitangent, glm::vec2 &hitTextureCoordinates) const;

        std::vector<glm::vec3> vertex;
        std::vector<glm::vec4> colors;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> bitangents;
        std::vector<glm::vec2> uv;
        std::vector<int> triangles;              
};
}
}