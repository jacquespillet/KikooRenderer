#pragma once

#include "Common/Common.h"
#include "Shader.hpp"


namespace KikooRenderer
{
	
	
namespace CoreEngine
{

enum SHADER_IDS { UNLIT, SELECTED, GOURAUD, BLINNPHONG, PBR};
class StandardShaders
{
    public:         
        Shader unlitMeshShader;
        Shader selectedObjectShader;
        Shader gouraudShader;
        Shader blinnPhongShader;
        Shader PBRShader;
        void Compile();
};
}
}