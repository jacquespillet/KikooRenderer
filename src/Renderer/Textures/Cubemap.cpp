#include "Cubemap.hpp"
#include <stb_image.h>

namespace KikooRenderer{
namespace OfflineRenderer {

Cubemap::Cubemap(){}

Cubemap::Cubemap(std::vector<std::string> files) {
    this->files = files;
    for(int i=0; i<files.size(); i++) {
        stbi_set_flip_vertically_on_load(true);  
		unsigned char *data = stbi_load(files[i].c_str(), &width, &height, &nrChannels, 0);
		std::vector<uint8_t> textureData;
        if(data) {
			std::cout << "Texture:Constructor:SUCCESS" << std::endl;
            textureData = std::vector<uint8_t>(data, data + (width * height * nrChannels));
        }else
		{
			std::cout << "Texture:Constructor: ERROR::Failed to load texture" << std::endl;
			return;
		} 
		stbi_image_free(data);
        texturesData.push_back(textureData);
    }
}

glm::vec4 Cubemap::Sample(glm::vec3 stp) {
    FACE faceToSample;

    float max = -999999999;
    if(stp.x > 0 && stp.x > max) {max = stp.x; faceToSample = FACE::RIGHT;}
    else if(stp.x < 0 && -stp.x > max) {max = stp.x; faceToSample = FACE::LEFT;}
    else if(stp.y > 0 &&  stp.y > max) {max = stp.y; faceToSample = FACE::TOP;}
    else if(stp.y < 0 && -stp.y > max) {max = stp.y; faceToSample = FACE::BOTTOM;}
    else if(stp.z > 0 &&  stp.z > max) {max = stp.z; faceToSample = FACE::BACK;}
    else if(stp.z < 0 && -stp.z > max) {max = stp.z; faceToSample = FACE::FRONT;}
    
    float s = stp.x; float t = stp.y; float p = stp.z;
    float u, v;
    if(faceToSample == FACE::RIGHT) {
        u = 0.5 - p / (2 * s); 
        v = 0.5 - t / (2 * s); 
    } else if(faceToSample == FACE::LEFT) {
        u = 0.5 - p / (2 * s); 
        v = 0.5 + t / (2 * s); 
    }else if(faceToSample == FACE::TOP) {
        u = 0.5 + s / (2 * t); 
        v = 0.5 + p / (2 * t); 
    }else if(faceToSample == FACE::BOTTOM) {
        u = 0.5 - s / (2 * t); 
        v = 0.5 + p / (2 * t); 
    }else if(faceToSample == FACE::BACK) {
        u = 0.5 + s / (2 * p); 
        v = 0.5 - t / (2 * p); 
    }else if(faceToSample == FACE::FRONT) {
        u = 0.5 + s / (2 * p); 
        v = 0.5 + t / (2 * p); 
    }

    int texCoordsX = (int)(u * (float)width);
    int texCoordsY = (int)(v * (float)height);
    int inx = texCoordsY * width * nrChannels + texCoordsX * nrChannels;
    
    float r, g, b, a;

    r =  (float)(texturesData[faceToSample][inx]) / 255.0f;
    if(nrChannels >1)  g =  (float)(texturesData[faceToSample][inx + 1]) / 255.0f;
    if(nrChannels > 2) b =  (float)(texturesData[faceToSample][inx + 2]) / 255.0f;
    if(nrChannels > 3) a =  (float)(texturesData[faceToSample][inx + 3]) / 255.0f;

    return glm::vec4(r, g, b, a);
}

}
}