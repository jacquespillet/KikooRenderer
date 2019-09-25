#include "PBRShader.hpp"

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
    
    return PBRShader;
}
}
}