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

            uniform float brightnessThreshold;

            //main
            void main()
            {   
                vec3 color = texture(albedoTexture, fragmentUv).rgb;
                float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
                if(brightness > brightnessThreshold)
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
        uniform float weight7[4]   = float[](0.214607,	0.189879,	0.131514,	0.071303);
        uniform float weight9[5]   = float[](0.227027,  0.1945946,  0.1216216,  0.054054,   0.016216);
        uniform float weight11[6]  = float[](0.198596,	0.175713,	0.121703,	0.065984,	0.028002,	0.0093);
        uniform float weight13[7]  = float[](0.197641,	0.174868,	0.121117,	0.065666,	0.027867,	0.009255,	0.002406);
        uniform float weight15[8]  = float[](0.197448,	0.174697,	0.120999,	0.065602,	0.02784,	0.009246,	0.002403,	0.000489);
        uniform float weight17[9]  = float[](0.197417,	0.17467,	0.12098,	0.065592,	0.027835,	0.009245,	0.002403,	0.000489,	0.000078);
        uniform float weight19[10] = float[](0.197413,	0.174667,	0.120978,	0.065591,	0.027835,	0.009245,	0.002403,	0.000489,	0.000078,	0.00001);

        uniform float kernelRadius;
        uniform float kernelSize;
        uniform int horizontal;

        //main
        void main()
        {
            vec2 texOffset = (horizontal > 0) ? vec2(texelSize.x, 0) : vec2(0.0, texelSize.y);
            vec3 result = texture(albedoTexture, fragmentUv).rgb * weight7[0];
            for(int i = 1; i <= 3; ++i)
            {
                result += texture(albedoTexture, fragmentUv + texOffset * i * kernelRadius).rgb * weight7[i];
                result += texture(albedoTexture, fragmentUv - texOffset * i * kernelRadius).rgb * weight7[i];
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

        quad = GetQuad(scene, "plane", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1, 1, 1, 1));
        material = quad->GetComponent<MaterialComponent>();
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
        
        GLuint loc = ogl->glGetUniformLocation(thresholdShader.programShaderObject, "texelSize");
        ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0 / (float)framebufferIn->width, 1.0 / (float)framebufferIn->height, 0)));
        
        
        ogl->glUniform1f(ogl->glGetUniformLocation(thresholdShader.programShaderObject, "brightnessThreshold"), brightnessThreshold);
        

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
        ogl->glUniform1f(ogl->glGetUniformLocation(blurShader.programShaderObject, "kernelRadius"), kernelRadius);
        ogl->glUniform1i(ogl->glGetUniformLocation(blurShader.programShaderObject, "kernelSize"), kernelSize);

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
        ogl->glUniform1f(ogl->glGetUniformLocation(blurShader.programShaderObject, "kernelRadius"), kernelRadius);
        ogl->glUniform1i(ogl->glGetUniformLocation(blurShader.programShaderObject, "kernelSize"), kernelSize);

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