#pragma once

#include "Util/Common.h"


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
		
		Texture() { loaded = false; }
		Texture(std::string path, GLuint _texIndex, bool enableFilter = true);
		Texture(GLuint _texIndex, std::vector<uint8_t> data, int width, int height, int bpp);
		~Texture();
		void Use();
		void LoadFromFile(std::string path);
};
	
	
}


}