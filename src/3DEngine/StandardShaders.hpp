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
        void Compile();
};
}
}