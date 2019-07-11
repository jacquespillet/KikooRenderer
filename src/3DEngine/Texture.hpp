#pragma once

#include "Common/Common.h"


namespace KikooRenderer
{
	
	
namespace CoreEngine
{
	

class Texture
{		
	public:
		bool loaded = false; 
		GLuint glTex;
		GLint texIndex;
		int width, height, nrChannels;
		
		Texture(){}
		Texture(std::string path, GLuint _texIndex);
		void Use();
		void LoadFromFile(std::string path);

};
	
	
}


}