#include "Framebuffer.hpp"
#include "Components/MaterialComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {

	Framebuffer::Framebuffer(int width, int height,int internalColorFormat, int colorFormat, int colorType) {
		GETGL
		this->width = width;
		this->height = height;
		this->colorFormat = colorFormat;
		

		ogl->glGenFramebuffers(1, &fbo);
		ogl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		// create a color attachment texture
		ogl->glGenTextures(1, &texture);
		ogl->glBindTexture(GL_TEXTURE_2D, texture);
		ogl->glTexImage2D(GL_TEXTURE_2D, 0, internalColorFormat, width, height, 0, colorFormat, colorType, NULL);
		ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		ogl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		ogl->glBindTexture(GL_TEXTURE_2D, 0);
		
		//Create depth texture
		ogl->glGenTextures(1, &depthTexture);
		ogl->glBindTexture(GL_TEXTURE_2D, depthTexture);
		ogl->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
		ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
		ogl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
		ogl->glBindTexture(GL_TEXTURE_2D, 0);

		//// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		// ogl->glGenRenderbuffers(1, &rbo);
		// ogl->glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		// ogl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		// ogl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		
		// now that we actually created the fbo and added all attachments we want to check if it is actually complete now
		if (ogl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		ogl->glBindRenderbuffer(GL_RENDERBUFFER, 0);
		ogl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
	}

	void Framebuffer::Clear() {
		GETGL
		Enable();
		ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
		ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		Disable();
	}

	void Framebuffer::Enable() {
		GETGL
		ogl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
		ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	}

	void Framebuffer::Disable() {
		GETGL
		ogl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Destroy() {
		GETGL
		ogl->glDeleteFramebuffers(1, &fbo);
	}

	void Framebuffer::RenderOnObect(std::vector<Object3D*>& objectsToRender, Object3D* target) {
		GETGL
		GLint previousFBO; 
		ogl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFBO);
		this->Enable();

        for(int i=0; i<objectsToRender.size(); i++) {
            if(objectsToRender[i] && objectsToRender[i]->visible ) {
                objectsToRender[i]->Render(); 
            }
        }

		Disable();
		ogl->glBindFramebuffer(GL_FRAMEBUFFER, previousFBO);
				
		Texture albedoTex;
		albedoTex.glTex = texture;
		albedoTex.loaded = true;
		albedoTex.texIndex = GL_TEXTURE0;
		MaterialComponent* material = (MaterialComponent*)target->GetComponent("Material");
		material->albedoTex = albedoTex;
		
		target->Render();
	}

	// Texture* Framebuffer::GetColorTexture(){}
	// Texture* Framebuffer::GetDepthTexture(){}
}
}