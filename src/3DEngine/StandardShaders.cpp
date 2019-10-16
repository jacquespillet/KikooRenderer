#include "StandardShaders.hpp"
#include "Shaders/UnlitShader.hpp"
#include "Shaders/BlinnPhongShader.hpp"
#include "Shaders/PBRShader.hpp"
#include "Shaders/GouraudShader.hpp"
#include "Shaders/SelectedShader.hpp"
#include "Shaders/AxisHelperShader.hpp"

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
	if(id == SHADER_IDS::AXES_HELPER) return new AxesHelperParams();
	if(id == SHADER_IDS::OTHER) return new OtherShaderParams();
}

StandardShaders::StandardShaders() {
	shaders = std::vector<Shader*>();
}

void StandardShaders::Compile()
{

//Unlit
unlitMeshShader = GetUnlitShader();
unlitMeshShader.Compile();    
shaders.push_back(&unlitMeshShader);

//Gouraud
GetGouraudShader(&gouraudShader);
gouraudShader.Compile();
shaders.push_back(&gouraudShader);

//Blinn phong
blinnPhongShader = GetBlinnPhongShader();
blinnPhongShader.Compile();
shaders.push_back(&blinnPhongShader);

//Physically based rendering
PBRShader = GetPBRShader();
PBRShader.Compile();
shaders.push_back(&PBRShader);


selectedObjectShader = GetSelectedShader();
shaders.push_back(&selectedObjectShader);
selectedObjectShader.Compile();
}
}
}