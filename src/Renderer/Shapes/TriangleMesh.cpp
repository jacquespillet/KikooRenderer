#include "TriangleMesh.hpp"
#include "Util/ModelLoader.hpp"
#include "../Util/Geometry.hpp"

#include "Util/ThreadingUtil.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {
    TriangleMesh::TriangleMesh(glm::vec3 position, glm::vec3 size, Material* material, std::string filename) : material(material), invTransf(glm::mat4(1)){
        this->position = position;
        this->size = size;
        Util::FileIO::LoadModel(filename, &vertex, &normals, &uv, &colors, &triangles);

        Init();
    };

    TriangleMesh::TriangleMesh(glm::vec3 position, glm::vec3 size, Material* material, std::vector<glm::vec3> vertex,std::vector<glm::vec3> normals,std::vector<glm::vec2> uv, std::vector<int> triangles): material(material), invTransf(glm::mat4(1)) {
        this->position = position;
        this->size = size;
        this->triangles = triangles;
        this->normals = normals;
        this->vertex = vertex;
        this->uv = uv;

        Init();
    }

    void TriangleMesh::Init() {

        glm::mat4 transf(1);
        transf = glm::translate(transf, position);
        transf = glm::scale(transf, size);
        invTransf = glm::inverse(transf);


        CalculateTangents(tangents, bitangents, vertex,  normals, uv, triangles);
        // for(int i=0; i<tangents.size(); i++) {
        //     std::cout << glm::to_string(vertex[i]) << "  " <<glm::to_string(tangents[i]) << "  " <<glm::to_string(bitangents[i]) << "  " <<std::endl;
        // }

        bounds = Bounds(vertex, transf);
    }

    glm::vec3 TriangleMesh::GetPosition(double time) {
        return this->position;
    }

    bool rayTriangleIntersect( 
        const glm::vec3 &orig, const glm::vec3 &dir, 
        const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, 
        float &t, float &u, float &v) 
    { 
        const float kEpsilon = 1e-8; 

        glm::vec3 v0v1 = v1 - v0; 
        glm::vec3 v0v2 = v2 - v0; 
        glm::vec3 pvec = glm::cross(dir, v0v2);
        float det = glm::dot(v0v1, pvec);
    
        // ray and triangle are parallel if det is close to 0
        if (std::abs(det) < kEpsilon) return false; 
    
        float invDet = 1 / det; 
    
        glm::vec3 tvec = orig - v0; 
        u = glm::dot(tvec, pvec) * invDet; 
        if (u < 0 || u > 1) return false; 
    
        glm::vec3 qvec = glm::cross(tvec, v0v1); 
        v = glm::dot(dir, qvec) * invDet; 
        if (v < 0 || u + v > 1) return false; 
    
        t = glm::dot(v0v2, qvec) * invDet; 
    
        return true; 
    }     

    void TriangleMesh::GetWorldBounds(glm::vec3& min, glm::vec3& max) {
    }

    void TriangleMesh::GetSurfaceProperties( 
        const uint32_t &triIndex, 
        const glm::vec2 &uv, 
        glm::vec3 &hitNormal, 
        glm::vec3 &hitTangent, 
        glm::vec3 &hitBitangent, 
        glm::vec2 &hitTextureCoordinates) const 
    { 
        const glm::vec3 n0 = this->normals[triangles[triIndex]]; 
        const glm::vec3 n1 = this->normals[triangles[triIndex + 1]]; 
        const glm::vec3 n2 = this->normals[triangles[triIndex + 2]]; 
        hitNormal = (1 - uv.x - uv.y) * n0 + uv.x * n1 + uv.y * n2;
 
        // texture coordinates
        const glm::vec2 st0 = this->uv[triangles[triIndex]]; 
        const glm::vec2 st1 = this->uv[triangles[triIndex + 1]]; 
        const glm::vec2 st2 = this->uv[triangles[triIndex + 2]]; 
        hitTextureCoordinates = (1 - uv.x - uv.y) * st0 + uv.x * st1 + uv.y * st2; 

        const glm::vec3 t0 = this->tangents[triangles[triIndex]]; 
        const glm::vec3 t1 = this->tangents[triangles[triIndex + 1]]; 
        const glm::vec3 t2 = this->tangents[triangles[triIndex + 2]]; 
        hitTangent = (1 - uv.x - uv.y) * t0 + uv.x * t1 + uv.y * t2; 

        const glm::vec3 b0 = this->bitangents[triangles[triIndex]]; 
        const glm::vec3 b1 = this->bitangents[triangles[triIndex + 1]]; 
        const glm::vec3 b2 = this->bitangents[triangles[triIndex + 2]]; 
        hitBitangent = (1 - uv.x - uv.y) * b0 + uv.x * b1 + uv.y * b2;
    }     
    TriangleMesh::~TriangleMesh() {
        delete material;
    }


    double TriangleMesh::HitRay(KikooRenderer::Geometry::Ray ray, double tMin, double tMax, Point& hitPoint) {
        double distance;
        if(distance = bounds.Hit(ray) >0) { // BB Check

            uint32_t j = 0; 
            bool isect = false;
            glm::vec2 uv;

            glm::vec3 hitNormal;
            glm::vec3 hitTangent;
            glm::vec3 hitBitangent;
            glm::vec2 hitUv;

            KikooRenderer::Geometry::Ray tmpRay = ray;
            
            ray.origin = glm::vec3( invTransf * glm::vec4(ray.origin, 1) );
            ray.direction = glm::vec3( invTransf * glm::vec4(ray.direction, 0) );
            // ray.direction = glm::normalize(glm::vec3( invTransf * glm::vec4(ray.direction, 0) ));

            double tNear = 9999999;
            {
                for (uint32_t i = 0; i < triangles.size(); i+=3) { 
                    const glm::vec3 &v0 = vertex[triangles[i]]; 
                    const glm::vec3 &v1 = vertex[triangles[i + 1]]; 
                    const glm::vec3 &v2 = vertex[triangles[i + 2]]; 
                    float t = tNear, u, v; 
                    if (rayTriangleIntersect(ray.origin, ray.direction, v0, v1, v2, t, u, v) && t < tNear) { 
                        tNear = t; 
                        uv.x = u; 
                        uv.y = v; 
                        isect |= true; 
                        GetSurfaceProperties(i, uv, hitNormal, hitTangent, hitBitangent, hitUv);
                    }
                }            
            }
            
            if(isect)  {
                hitPoint = {
                    tNear, 
                    tmpRay.pointAtPosition(tNear),
                    hitNormal,
                    material,
                    hitUv,
                    hitTangent, 
                    hitBitangent
                }; 
                return tNear;
            }
            else return -1;
        } else return -1;
    }

    float TriangleMesh::pdfValue(glm::vec3 origin, glm::vec3 direction) {
        Point point;
        if(this->HitRay(Geometry::Ray(origin, glm::normalize(direction)), 0.001, std::numeric_limits<float>::max(), point)) {
            // float area = size.x * size.y * size.z;
            float area = 0.2*0.2;
            float distance_squared = point.t * point.t;
            float cosine = fabs(glm::dot(direction, point.normal));
            return  distance_squared / (cosine * area);
        } else return 0;     
    }

    glm::vec3 TriangleMesh::random(glm::vec3 origin) {
        double halfX = size.x /2;
        double halfY = size.y /2;
        double halfZ = size.z /2;

        glm::vec3 onLight = glm::vec3(Geometry::RandomInRange(-halfX, halfX), 
                                      Geometry::RandomInRange(-halfY, halfY), 
                                      Geometry::RandomInRange(-halfZ, halfZ));
        onLight += position;
        
        return onLight - origin;
    }

}
}