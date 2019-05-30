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

	public:
		std::string name; 
		
		//shader source	
		std::string vertSrc;
		std::string fragSrc;
	
		//shader programs
		int programShaderObject;
		int vertexShaderObject;
		int fragmentShaderObject;
		
		Shader();
		void Compile();
};
	
	
}


}