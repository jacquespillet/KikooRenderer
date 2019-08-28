#pragma once

#include "Util/Common.h"
#include "Object3D.hpp"
#include "Texture.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {
class Object3D;
class Framebuffer {
public: 
	Framebuffer(int width=800, int height = 600, int internalColorFormat = GL_RGB, int colorFormat = GL_RGB, int colorType = GL_UNSIGNED_BYTE);
	
	void Enable();
	void Disable();
	void Destroy();
	void Clear();
	unsigned int texture;
	unsigned int depthTexture;
	GLint defaultFBO;

	int width, height;
	int colorFormat;

	void RenderOnObect(std::vector<Object3D*>& objectsToRender, Object3D* target);

	Texture* GetColorTexture();
	Texture* GetDepthTexture();

private:
	unsigned int fbo;
	unsigned int rbo;
};
}
}