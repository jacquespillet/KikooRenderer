#pragma once

#include "Util/Common.h"
#include "Object3D.hpp"
#include "Cubemap.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {
class Object3D;
class CubeFramebuffer {
public: 
	CubeFramebuffer(int width=800, int height = 600, int internalColorFormat = GL_RGB, int colorFormat = GL_RGB, int colorType = GL_UNSIGNED_BYTE, bool hasColor=true, bool hasDepth=true); 
	void Enable();
	void Disable();
	void Destroy();
	void Clear();
	unsigned int cubemap;
	unsigned int depthCubemap;
	GLint defaultFBO;

	int width, height;
	int colorFormat;

	//Directly renders the object to the target object
	void RenderOnObect(std::vector<Object3D*>& objectsToRender, Object3D* target);
	void RenderOnObect(Object3D* objectToRender, Object3D* target);

	//Render object to the current FBO
	void RenderObjectsToFBO(std::vector<Object3D*>& objectsToRender);
	void RenderObjectToFBO(Object3D* objectToRender);

	//Render the FBO to a target object	
	void RenderFBOToObject(Object3D* target, bool renderDepth=false);


	Cubemap* GetColorTexture();
	Cubemap* GetDepthTexture();

private:
	unsigned int fbo;
	unsigned int rbo;
};
}
}