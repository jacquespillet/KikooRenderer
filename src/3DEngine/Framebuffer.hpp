#pragma once

#include "Common/Common.h"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {
class Framebuffer {
public: 
	Framebuffer();
	
	void Enable();
	void Disable();
	void Destroy();
	void Clear();
	unsigned int texture;
	GLint defaultFBO;


private:
	unsigned int fbo;
	unsigned int rbo;
};
}
}