#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

#include "../BRDF/BRDF.hpp"
#include "../BRDF/TorranceSparrow.hpp"

#include "Renderer/PDF/cosinePdf.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {
struct Point;

struct ScatterRecord {
    Geometry::Ray specularRay;
    bool isSpecular;
    glm::vec3 attenuation;
    Pdf* pdf;
};

class Material {
    public: 
        // Material(){}
        Material(glm::vec4 albedo);
        glm::vec4 albedo;
        virtual bool Scatter(KikooRenderer::Geometry::Ray in,  Point point, ScatterRecord& scatterRecord);
        virtual float ScatterPdf(KikooRenderer::Geometry::Ray in,  Point point, KikooRenderer::Geometry::Ray& scattered);

        BRDF brdf;
        TorranceSparrow brdf2;

        bool useBrdf = false;

        bool emitter = false;
        virtual glm::vec3 emitted(const Geometry::Ray& rayIn, Point& rec);

        void LoadTexture(std::string fileName);
        std::vector<uint8_t> textureData;
        int width=0; 
        int height=0;
        int nrChannels=0;
        bool hasTexture = false;
};

}
}