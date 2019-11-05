#include "Texture3D.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer
{
	
	
namespace CoreEngine
{
	Texture3D::Texture3D(GLuint _texIndex, std::vector<uint8_t> data, int width, int height,int depth, int bpp) { 
		GETGL
		texIndex = _texIndex;		
		ogl->glGenTextures(1, &glTex);  
		ogl->glBindTexture(GL_TEXTURE_3D, glTex);

		ogl->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		ogl->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		ogl->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		ogl->glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLint texType;
		if(bpp == 4) {
			texType = GL_RGBA;
		} else if(bpp == 3) {
			texType = GL_RGB;
		} else if(bpp == 1) {
			texType = GL_RED;
		}

		ogl->glTexImage3D(GL_TEXTURE_3D, 0, texType, width, height,depth, 0, texType, GL_UNSIGNED_BYTE, (unsigned char *)data.data());
		ogl->glGenerateMipmap(GL_TEXTURE_3D);
		
		ogl->glBindTexture(GL_TEXTURE_3D, 0);
		loaded = true;  
	}
	

	void Texture3D::Use() {
		GETGL;
		ogl->glActiveTexture(texIndex);
		ogl->glBindTexture(GL_TEXTURE_3D, glTex);
	}

	Texture3D::~Texture3D() {
		if(loaded) {
			GETGL
		}
	}
}


}