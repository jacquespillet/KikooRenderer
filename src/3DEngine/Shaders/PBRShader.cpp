#include "PBRShader.hpp"

#include "3DEngine/Scene.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
Shader GetPBRShader() {
    Shader PBRShader;

    std::ifstream t("Shaders/Standard/PBR.vert");
    std::stringstream vertBuffer;
    vertBuffer << t.rdbuf();
    PBRShader.vertSrc= vertBuffer.str();

    
    t = std::ifstream ("Shaders/Standard/PBR.frag");
    std::stringstream fragBuffer;
    fragBuffer << t.rdbuf();
    PBRShader.fragSrc= fragBuffer.str();
    
    
    PBRShader.SetId(SHADER_IDS::PBR);
    PBRShader.name = "PBR";
    std::cout << "StandardShaders:Compile: Compiling PBRShader" << std::endl; 

    return PBRShader;
}
}
}