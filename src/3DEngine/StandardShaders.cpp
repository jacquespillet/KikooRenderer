#include "StandardShaders.hpp"
#include "Shaders/UnlitShader.hpp"
#include "Shaders/BlinnPhongShader.hpp"
#include "Shaders/PBRShader.hpp"
#include "Shaders/GouraudShader.hpp"
#include "Shaders/SelectedShader.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
ShaderParams* StandardShaders::GetParamsById(int id) {
	if(id == SHADER_IDS::UNLIT)   return new UnlitMeshParams();
	if(id == SHADER_IDS::GOURAUD) return new GouraudParams();
	if(id == SHADER_IDS::BLINNPHONG) return new BlinnPhongParams();
	if(id == SHADER_IDS::PBR) return new PBRParams();
	if(id == SHADER_IDS::SELECTED) return new SelectedShaderParams();
}

StandardShaders::StandardShaders() {
	shaders = std::vector<Shader*>();
}

void StandardShaders::Compile()
{

//Unlit
unlitMeshShader = GetUnlitShader();
unlitMeshShader.SetId(SHADER_IDS::UNLIT);
unlitMeshShader.name = "Unlit";
shaders.push_back(&unlitMeshShader);
std::cout << "StandardShaders:Compile: Compiling unlitMeshShader" << std::endl; 
unlitMeshShader.Compile();

//Gouraud
GetGouraudShader(&gouraudShader);
gouraudShader.SetId(SHADER_IDS::GOURAUD);
gouraudShader.name = "Gouraud";
shaders.push_back(&gouraudShader);
std::cout << "In vector " << &gouraudShader << std::endl;
std::cout << "StandardShaders:Compile: Compiling gouraudShader" << std::endl; 
gouraudShader.Compile();

//Blinn phong
blinnPhongShader = GetBlinnPhongShader();
blinnPhongShader.SetId(SHADER_IDS::BLINNPHONG);
blinnPhongShader.name = "Blinn Phong";
shaders.push_back(&blinnPhongShader);
std::cout << "StandardShaders:Compile: Compiling blinnPhongShader" << std::endl; 
blinnPhongShader.Compile();

//Physically based rendering
PBRShader = GetPBRShader();
PBRShader.SetId(SHADER_IDS::PBR);
PBRShader.name = "PBR";
shaders.push_back(&PBRShader);
std::cout << "StandardShaders:Compile: Compiling PBRShader" << std::endl; 
PBRShader.Compile();


selectedObjectShader = GetSelectedShader();
selectedObjectShader.SetId(SHADER_IDS::SELECTED);
selectedObjectShader.name = "Selected";
shaders.push_back(&selectedObjectShader);
std::cout << "StandardShaders:Compile: Compiling selectedObjectShader" << std::endl; 
selectedObjectShader.Compile();

}
}
}