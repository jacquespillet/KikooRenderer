#pragma once

#include "Util/Common.h"


namespace KikooRenderer
{
	
	
namespace CoreEngine
{
	

class Texture3D
{		
	public:
		bool loaded = false; 
		GLuint glTex;
		GLint texIndex;
		int width, height, nrChannels;
		
		Texture3D() { loaded = false; }
		Texture3D(GLuint _texIndex, std::vector<uint8_t> data, int width, int height, int depth, int bpp);
		~Texture3D();
		void Use();
};
	
	
}


}