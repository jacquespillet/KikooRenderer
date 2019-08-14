#include "Texture.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace KikooRenderer
{
	
	
namespace CoreEngine
{
	Texture::Texture(std::string path, GLuint _texIndex) { 
		texIndex = _texIndex;
		LoadFromFile(path);
	}

	void Texture::Use() {
		GETGL;
		ogl->glActiveTexture(texIndex);
		ogl->glBindTexture(GL_TEXTURE_2D, glTex);
	}

	void Texture::LoadFromFile(std::string path) {
		GETGL
		ogl->glActiveTexture(texIndex);
		ogl->glGenTextures(1, &glTex);  
		ogl->glBindTexture(GL_TEXTURE_2D, glTex);

		ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		
		ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);  
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		GLint texType = (nrChannels == 3) ? GL_RGB : GL_RGBA;
		if (data)
		{
			ogl->glTexImage2D(GL_TEXTURE_2D, 0, texType, width, height, 0, texType, GL_UNSIGNED_BYTE, data);
			ogl->glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
			return;
		} 
		stbi_image_free(data);
		ogl->glBindTexture(GL_TEXTURE_2D, 0);
		loaded = true;  
	}
}


}