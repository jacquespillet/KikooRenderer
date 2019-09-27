#include "Framebuffer.hpp"
#include "Components/MaterialComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {

	Framebuffer::Framebuffer(int width, int height,int internalColorFormat, int colorFormat, int colorType, bool hasColor, bool hasDepth, bool multisampled) {
		GETGL
		this->width = width;
		this->height = height;
		this->colorFormat = colorFormat;
		this->internalColorFormat = internalColorFormat;
		this->internalColorFormat = internalColorFormat;
		this->colorType = colorType;
		this->multisampled = multisampled;
		
		ogl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
		if(!multisampled) {

			ogl->glGenFramebuffers(1, &fbo);
			ogl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			
			if(hasColor) {
				// create a color attachment texture
				ogl->glGenTextures(1, &texture);
				ogl->glBindTexture(GL_TEXTURE_2D, texture);
				ogl->glTexImage2D(GL_TEXTURE_2D, 0, internalColorFormat, width, height, 0, colorFormat, colorType, NULL);
				ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT ); 
				// ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
				
				ogl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
				ogl->glBindTexture(GL_TEXTURE_2D, 0);
			} else {
				ogl->glDrawBuffer(GL_NONE);
				ogl->glReadBuffer(GL_NONE);	
			}
			
			if(hasDepth) {
					//Create depth texture
					ogl->glGenTextures(1, &depthTexture);
					ogl->glBindTexture(GL_TEXTURE_2D, depthTexture);
					ogl->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
					ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					
					// ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
					// ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
					// float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
					// ogl->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 

					ogl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
					ogl->glBindTexture(GL_TEXTURE_2D, 0);
			} else {
				// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
				ogl->glGenRenderbuffers(1, &rbo);
				ogl->glBindRenderbuffer(GL_RENDERBUFFER, rbo);
				ogl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
				ogl->glBindRenderbuffer(GL_RENDERBUFFER, 0);
				ogl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
			}

			// ogl->glEnable(GL_CULL_FACE); 
			// ogl->glCullFace(GL_BACK);

			// ogl->glEnable(GL_BLEND);
			// ogl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			// ogl->glEnable(GL_STENCIL_TEST);    
			// ogl->glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
			
			//disable writting to depth buffer
			ogl->glEnable(GL_DEPTH_TEST);

			// now that we actually created the fbo and added all attachments we want to check if it is actually complete now
			if (ogl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << width << "  " << height << std::endl;
			} else {
				std::cout << "Succeffuly created FBO"<<std::endl;
			}
		} else {
			ogl->glGenFramebuffers(1, &fbo);
			ogl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			// create a multisampled color attachment texture
			ogl->glGenTextures(1, &texture);
			ogl->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
			ogl->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, internalColorFormat, width, height, GL_TRUE);
			ogl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);
			
			if(hasDepth) {
				//Create depth texture
				ogl->glGenTextures(1, &depthTexture);
				ogl->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthTexture);
				ogl->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT, width, height, GL_TRUE);
				ogl->glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				ogl->glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				
				ogl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthTexture, 0);
			} else {
				// create a (also multisampled) renderbuffer object for depth and stencil attachments
				ogl->glGenRenderbuffers(1, &rbo);
				ogl->glBindRenderbuffer(GL_RENDERBUFFER, rbo);
				ogl->glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
				ogl->glBindRenderbuffer(GL_RENDERBUFFER, 0);
				ogl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
			}


			if (ogl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			} else {
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer OK!" << std::endl;
			}
		}
		ogl->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);		
		ogl->glBindTexture(GL_TEXTURE_2D, 0);
		ogl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
	}

	void Framebuffer::Clear() {
		GETGL
		Enable();
		ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
		ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		Disable();
	}
	void Framebuffer::CopyToFramebuffer(Framebuffer* otherFB) {
    	GETGL
		GLint currentFBO;
		ogl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
		ogl->glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		ogl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, otherFB->fbo);
		ogl->glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		ogl->glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		ogl->glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
	}


	void Framebuffer::Enable() {
		GETGL
		ogl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
		ogl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}

	void Framebuffer::Disable() {
		GETGL
		ogl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
	}

	Framebuffer::~Framebuffer() {
		Destroy();
	}

	void Framebuffer::Destroy() {
		GETGL
		ogl->glDeleteFramebuffers(1, &fbo);
		ogl->glDeleteTextures(1, &depthTexture);
		ogl->glDeleteTextures(1, &texture);
	}

	void Framebuffer::RenderOnObect(std::vector<Object3D*>& objectsToRender, Object3D* target) {
		GETGL
		Enable();
		RenderObjectsToFBO(objectsToRender);
		RenderFBOToObject(target);
		Disable();
	}
	
	void Framebuffer::RenderOnObect(Object3D* objectToRender, Object3D* target) {
		GETGL
		Enable();
		RenderObjectToFBO(objectToRender);
		RenderFBOToObject(target);
		Disable();
	}

	//Is it useful ? 
	void Framebuffer::RenderObjectsToFBO(std::vector<Object3D*>& objectsToRender) {
		GETGL
		for(int i=0; i<objectsToRender.size(); i++) {
            if(objectsToRender[i] && objectsToRender[i]->visible ) {
                objectsToRender[i]->Render(); 
            }
        }
	}

	void Framebuffer::RenderObjectToFBO(Object3D* objectToRender) {
		GETGL
		if(objectToRender->visible) objectToRender->Render(); 
	}

	void Framebuffer::RenderFBOToObject(Object3D* target, bool renderDepth) {
		GETGL
		Texture albedoTex;
		albedoTex.glTex = renderDepth ? depthTexture : texture;
		albedoTex.loaded = true;
		albedoTex.texIndex = GL_TEXTURE0;
		MaterialComponent* material = target->GetComponent<MaterialComponent>();
		material->albedoTex = albedoTex;
		target->Render();	
	}


	// Texture* Framebuffer::GetColorTexture(){}
	// Texture* Framebuffer::GetDepthTexture(){}
}
}