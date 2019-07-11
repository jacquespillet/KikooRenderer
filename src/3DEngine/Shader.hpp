#pragma once

#include "Common/Common.h"


namespace KikooRenderer
{
	
	
namespace CoreEngine
{
	

class Shader
{		
	private:
		bool compiled; 
		int identifier = 0;

	public:
		std::string name; 
		
		//shader source	
		std::string vertSrc;
		std::string fragSrc;
	
		//shader programs
		int programShaderObject;
		int vertexShaderObject;
		int fragmentShaderObject;

		//ShaderUniformsObject
		//	ShaderUniformObjects->setAllUniforms();
		
		Shader();
		
		void SetId(int id) { this->identifier = id;}
		int GetId() { return this->identifier;}

		void Compile();
};
	
	
}


}