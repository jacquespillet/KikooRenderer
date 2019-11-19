#pragma once
#include "Util/Common.h"

namespace KikooRenderer{
namespace OfflineRenderer {


class MicroFacets {
    public: 
        MicroFacets();
        float D(glm::vec3 wh);
        float G(glm::vec3 wo, glm::vec3 wi);
        void SetRoughness(float roughness);
        int type;
    private:
        float BeckmannDistribution(glm::vec3 wh);
        float TrowbridgeReitzDistribution(glm::vec3 wh);

        float G1(glm::vec3 wo);

        float Lambda(glm::vec3 w);

        float roughness = 0.5;
        float alphax;
        float alphay;
};

}
}