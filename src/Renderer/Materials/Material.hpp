#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {
struct Point;
    
class Material {
    public: 
        Material(){}
        Material(glm::vec4 albedo);
        glm::vec4 albedo;
        virtual bool Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::vec3& attenuation, KikooRenderer::Geometry::Ray& scattered);

        bool emitter = false;
        virtual glm::vec3 emitted();

        void LoadTexture(std::string fileName);
        std::vector<uint8_t> textureData;
        int width=0; 
        int height=0;
        int nrChannels=0;
        bool hasTexture = false;
};

}
}