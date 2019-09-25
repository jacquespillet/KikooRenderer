#include "UnlitShader.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
Shader GetUnlitShader() {
    Shader unlitMeshShader;

    std::ifstream t("Shaders/Standard/Unlit.vert");
    std::stringstream vertBuffer;
    vertBuffer << t.rdbuf();
    unlitMeshShader.vertSrc= vertBuffer.str();

    
    t = std::ifstream ("Shaders/Standard/Unlit.frag");
    std::stringstream fragBuffer;
    fragBuffer << t.rdbuf();
    unlitMeshShader.fragSrc= fragBuffer.str();

    return unlitMeshShader;
}
}
}