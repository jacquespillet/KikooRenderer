#pragma once

#include "Util/Common.h"
#include "Shader.hpp"
// #include "Shaders/ShaderParams.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
enum SHADER_IDS { UNLIT=0, SELECTED = 1, GOURAUD = 2, BLINNPHONG = 3, PBR = 4};
class ShaderParams;

class StandardShaders
{
    public:         
		// Scene* scene;

		StandardShaders();
		int numShaders = 5;
		std::string ids[5] = { "unlit", "gouraud", "blinn phong", "pbr", "selected"};
		std::vector<Shader*> shaders;

		Shader unlitMeshShader;
        Shader selectedObjectShader;
        Shader gouraudShader;
        Shader blinnPhongShader;
        Shader PBRShader;
        void Compile();

        static ShaderParams* GetParamsById(int id);
};
}
}