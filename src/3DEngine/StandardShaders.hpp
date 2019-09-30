#pragma once

#include "Util/Common.h"
#include "Shader.hpp"
// #include "Shaders/ShaderParams.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
enum SHADER_IDS { UNLIT=0,  GOURAUD = 1, BLINNPHONG = 2, PBR = 3, SELECTED = 4};
class ShaderParams;

class StandardShaders
{
    public:         
		// Scene* scene;

		StandardShaders();
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