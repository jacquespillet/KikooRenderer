#include "Material.hpp"
#include "../Shapes/Shape.hpp"
#include "Geometry/Util.h"
#include <stb_image.h>

#include "../Util/Geometry.hpp"


namespace KikooRenderer{
namespace OfflineRenderer {

    Material::Material(glm::vec4 albedo) : albedo(albedo) {}

    float Material::ScatterPdf(KikooRenderer::Geometry::Ray in,  Point point, KikooRenderer::Geometry::Ray& scattered) {
        float cosine = glm::dot(point.normal, glm::normalize(-in.direction));
        if(cosine < 0) cosine=0;
        return cosine;
    }

    bool Material::Scatter(KikooRenderer::Geometry::Ray in,  Point point, ScatterRecord& scatterRecord) {
        if(emitter) return false;
        
        // // //Transform ray to tangent space
        glm::mat4 worldToTangent(1);
        worldToTangent[0] = glm::vec4(glm::normalize(point.tangent), 0);
        worldToTangent[1] = glm::vec4(glm::normalize(point.bitangent), 0);
        worldToTangent[2] = glm::vec4(glm::normalize(point.normal), 0);
        worldToTangent[3] = glm::vec4(0, 0, 0, 1);

        scatterRecord.isSpecular = brdf->isSpecular;
        scatterRecord.attenuation = albedo;
        scatterRecord.pdf = new CosinePdf(worldToTangent);

        return true;
    }

    glm::vec3 Material::emitted(const Geometry::Ray& rayIn, Point& rec) {
        if(emitter) {
            if (glm::dot(glm::vec3(0, -1, 0), rayIn.direction) < 0.0) return albedo * 5;
            else return glm::vec3(0);            
        }  else return glm::vec3(0);
    }

    void Material::LoadTexture(std::string fileName) {
        stbi_set_flip_vertically_on_load(true);  
		unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
		if(data) {
			std::cout << "Texture:Constructor:SUCCESS" << std::endl;
            textureData = std::vector<uint8_t>(data, data + (width * height * nrChannels));
            hasTexture = true;
        }else
		{
			std::cout << "Texture:Constructor: ERROR::Failed to load texture" << std::endl;
			return;
		} 
		stbi_image_free(data);
    }

}
}