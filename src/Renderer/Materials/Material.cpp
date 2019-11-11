#include "Material.hpp"
#include "../Shapes/Shape.hpp"
#include "Geometry/Util.h"
#include <stb_image.h>

namespace KikooRenderer{
namespace OfflineRenderer {

    Material::Material(glm::vec4 albedo) : albedo(albedo) {}
    bool Material::Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::vec3& attenuation, KikooRenderer::Geometry::Ray& scattered) {
        glm::vec3 target = point.position + point.normal + Geometry::randomInSphere();
        scattered = Geometry::Ray(point.position, target-point.position);
        if(hasTexture) {
            float texCoordX = std::min((float)width, std::max(0.0f, point.uv.x * (float)width));
            float texCoordY = std::min((float)width, std::max(0.0f, point.uv.y * (float)height));

            attenuation.r = (float)(textureData[std::floor(texCoordY * width + texCoordX)]) / 255;
            attenuation.g = (float)(textureData[std::floor(texCoordY * width + texCoordX + 1)]) / 255;
            attenuation.b = (float)(textureData[std::floor(texCoordY * width + texCoordX + 2)]) / 255;
            // attenuation.r = point.uv.x;
            // attenuation.g = point.uv.y;
            // attenuation.b = 0;
        } else {
            attenuation = albedo;
        }
        return true;
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