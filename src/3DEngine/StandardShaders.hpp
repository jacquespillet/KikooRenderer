#pragma once

#include "Util/Common.h"
#include "Shader.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
enum SHADER_IDS { UNLIT=0, SELECTED = 1, GOURAUD = 2, BLINNPHONG = 3, PBR = 4};

struct gouraudParams {
	float albedoInfluence;
	std::string textureStr;
};

class StandardShaders
{
    public:         
		int numShaders = 5;
		std::string ids[5] = { "unlit", "selected", "gouraud", "blinn phong", "pbr" };
		Shader unlitMeshShader;
        Shader selectedObjectShader;
        Shader gouraudShader;
        Shader blinnPhongShader;
        Shader PBRShader;
        void Compile();
};
}
}