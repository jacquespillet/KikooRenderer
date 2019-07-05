#pragma once

#include "Common/Common.h"
#include "Shader.hpp"


namespace KikooRenderer
{
	
	
namespace CoreEngine
{

class StandardShaders
{
    public:         
        Shader unlitMeshShader;
        Shader selectedObjectShader;
        void Compile();
};
}
}