#include "BloomPostProcess.hpp"
#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"


namespace KikooRenderer {
namespace CoreEngine {
    BloomPostProcess::BloomPostProcess(Scene* scene): PostProcess(scene)  {
        thresholdShader.vertSrc= R"(
        //attribs
        #version 440
        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;
        layout(location = 2) in vec2 uv;
        layout(location = 3) in vec4 color;

        //outputs
        out vec2 fragmentUv;
        //main
        void main()
        {
            fragmentUv = vec2(uv.x, uv.y);
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
        }
        )";

        thresholdShader.fragSrc = R"(
            //inputs
            #version 440
            //output
            layout(location = 0) out vec4 outputColor; 

            in vec2 fragmentUv;
            
            uniform sampler2D albedoTexture;
            uniform sampler2D depthTexture;
            uniform vec3 texelSize;

            //main
            void main()
            {   
                vec3 color = texture(albedoTexture, fragmentUv).rgb;
                float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
                if(brightness > 0.3)
                    outputColor = vec4(color.rgb, 1.0);
                else
                    outputColor = vec4(0.0, 0.0, 0.0, 1.0);
            }  
        )";
        thresholdShader.name = "thresholdShader";
        thresholdShader.Compile();

        blurShader.vertSrc= R"(
        //attribs
        #version 440
        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;
        layout(location = 2) in vec2 uv;
        layout(location = 3) in vec4 color;

        //outputs
        out vec2 fragmentUv;
        //main
        void main()
        {
            fragmentUv = vec2(uv.x, uv.y);
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
        }
        )";        

        blurShader.fragSrc = R"(
        //inputs
        #version 440
        //output
        layout(location = 0) out vec4 outputColor; 

        in vec2 fragmentUv;
        
        uniform sampler2D albedoTexture;
        // uniform sampler2D depthTexture;
        uniform vec3 texelSize;
        uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
        
        uniform int horizontal;

        //main
        void main()
        {   
            vec3 result = texture(albedoTexture, fragmentUv).rgb * weight[0];
            if(horizontal > 0)
            {
                for(int i = 1; i < 5; ++i)
                {
                    result += texture(albedoTexture, fragmentUv + vec2(texelSize.x * i, 0.0)).rgb * weight[i];
                    result += texture(albedoTexture, fragmentUv - vec2(texelSize.x * i, 0.0)).rgb * weight[i];
                }
            }
            else
            {
                for(int i = 1; i < 5; ++i)
                {
                    result += texture(albedoTexture, fragmentUv + vec2(0.0, texelSize.y * i)).rgb * weight[i];
                    result += texture(albedoTexture, fragmentUv - vec2(0.0, texelSize.y * i)).rgb * weight[i];
                }
            }
            outputColor = vec4(result.rgb, 1.0);
        }
        )";
        blurShader.name = "blurShader";
        blurShader.Compile();
        
        blendShader.vertSrc= R"(
        //attribs
        #version 440
        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;
        layout(location = 2) in vec2 uv;
        layout(location = 3) in vec4 color;

        //outputs
        out vec2 fragmentUv;
        //main
        void main()
        {
            fragmentUv = vec2(uv.x, uv.y);
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
        }
        )";        

        blendShader.fragSrc = R"(
        //inputs
        #version 440
        //output
        layout(location = 0) out vec4 outputColor; 

        in vec2 fragmentUv;
        
        uniform sampler2D albedoTexture;
        uniform sampler2D bloomTexture;
        
        //main
        void main()
        {   
            vec3 originalColor = texture(albedoTexture, fragmentUv).rgb;
            vec3 bloomColor = texture(bloomTexture, fragmentUv).rgb;
            outputColor = vec4(originalColor.rgb + bloomColor.rgb, 1.0);
        }
        )";
        blendShader.name = "blendShader";
        blendShader.Compile();

        quad = GetQuad(scene, "plane", glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(1, 1, 1, 1));
        material = (MaterialComponent*) quad->GetComponent("Material");
        quad->Enable();        

        thresholdFramebuffer = new Framebuffer(scene->windowWidth, scene->windowHeight,  GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);
        alternateFramebuffer = new Framebuffer(scene->windowWidth, scene->windowHeight,  GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);
    }
    void BloomPostProcess::Run(Framebuffer* framebufferIn, Framebuffer* framebufferOut) {
        GETGL       

        //1. _________________________________________________________________
        //Render framebufferIn thresholded to thresholdFramebuffer
        thresholdFramebuffer->Enable();
        material->SetShader(&thresholdShader);
        
        ogl->glClearColor(1.0, 1.0, 1.0, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
        
        ogl->glUseProgram(thresholdShader.programShaderObject);
        
        GLuint loc = ogl->glGetUniformLocation(shader.programShaderObject, "texelSize");
        ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0 / (float)framebufferIn->width, 1.0 / (float)framebufferIn->height, 0)));
        
        material->albedoTex.glTex =  framebufferIn->texture;
		material->albedoTex.loaded = true;
		material->albedoTex.texIndex = GL_TEXTURE0;

        quad->Render();            
        thresholdFramebuffer->Disable();

        //2. _________________________________________________________________
        //   Blur thresholdFramebuffer horizontally into framebufferOut
        
        framebufferOut->Enable();
                
        ogl->glClearColor(1.0, 1.0, 1.0, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
        material->SetShader(&blurShader);
        
        ogl->glUseProgram(blurShader.programShaderObject);
        
        loc = ogl->glGetUniformLocation(blurShader.programShaderObject, "texelSize");
        ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0 / (float)framebufferIn->width, 1.0 / (float)framebufferIn->height, 0)));

        ogl->glUniform1i(ogl->glGetUniformLocation(blurShader.programShaderObject, "horizontal"), 1);

        material->albedoTex.glTex =  thresholdFramebuffer->texture;
		material->albedoTex.loaded = true;
		material->albedoTex.texIndex = GL_TEXTURE0;

        quad->Render();
        framebufferOut->Disable();    
        
        //3. _________________________________________________________________
        //Blur framebufferOut vertically into alternateFramebuffer
        alternateFramebuffer->Enable();
                
        ogl->glClearColor(1.0, 1.0, 1.0, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
        material->SetShader(&blurShader);
        
        ogl->glUseProgram(blurShader.programShaderObject);
        
        loc = ogl->glGetUniformLocation(blurShader.programShaderObject, "texelSize");
        ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0 / (float)framebufferIn->width, 1.0 / (float)framebufferIn->height, 0)));

        ogl->glUniform1i(ogl->glGetUniformLocation(blurShader.programShaderObject, "horizontal"), 0);

        material->albedoTex.glTex =  framebufferOut->texture;
		material->albedoTex.loaded = true;
		material->albedoTex.texIndex = GL_TEXTURE0;

        quad->Render();
        alternateFramebuffer->Disable(); 

        //4. _________________________________________________________________
        //Add framebufferIn & alternateFramebuffer into framebufferOut
        framebufferOut->Enable();
                
        ogl->glClearColor(1.0, 1.0, 1.0, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
        material->SetShader(&blendShader);
        
        ogl->glUseProgram(blendShader.programShaderObject);
        
        material->albedoTex.glTex =  framebufferIn->texture;
		material->albedoTex.loaded = true;
		material->albedoTex.texIndex = GL_TEXTURE0;

        ogl->glActiveTexture(GL_TEXTURE1);
        ogl->glBindTexture(GL_TEXTURE_2D, alternateFramebuffer->texture);
        ogl->glUniform1i(ogl->glGetUniformLocation(blendShader.programShaderObject, "bloomTexture"), 1);        

        quad->Render();
        framebufferOut->Disable();
    }
}
}