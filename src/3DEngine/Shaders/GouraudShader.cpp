#include "GouraudShader.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{

void GetGouraudShader(Shader* gouraudShader) {

    std::ifstream t("Shaders/Standard/Gouraud.vert");
    std::stringstream vertBuffer;
    vertBuffer << t.rdbuf();
    gouraudShader->vertSrc= vertBuffer.str();

    
    t = std::ifstream ("Shaders/Standard/Gouraud.frag");
    std::stringstream fragBuffer;
    fragBuffer << t.rdbuf();
    gouraudShader->fragSrc= fragBuffer.str();
}

}
}