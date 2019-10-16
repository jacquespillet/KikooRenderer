#include "UnlitShader.hpp"
#include "3DEngine/Scene.hpp"

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
    unlitMeshShader.isLit = false;
    unlitMeshShader.isDepthPass = false;


    unlitMeshShader.SetId(SHADER_IDS::UNLIT);
    unlitMeshShader.name = "Unlit";
    std::cout << "StandardShaders:Compile: Compiling unlitMeshShader" << std::endl; 

    return unlitMeshShader;
}
}
}