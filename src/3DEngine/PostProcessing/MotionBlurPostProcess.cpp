#include "MotionBlurPostProcess.hpp"
#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"


namespace KikooRenderer {
namespace CoreEngine {
    MotionBlurPostProcess::MotionBlurPostProcess(Scene* scene): PostProcess(scene)  {
        shader.vertSrc= R"(
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

        shader.fragSrc = R"(
        //inputs
        #version 440
        in vec2 fragmentUv;

        //output
        layout(location = 0) out vec4 outputColor; 
        uniform sampler2D albedoTexture;
        uniform sampler2D depthTexture;
        uniform vec3 texelSize;

        uniform mat4 inverseViewProjection;
        uniform mat4 previousViewProjection;

        uniform float velocityMultiplier = 1.0 / 8.0;
        uniform int numSamples = 4;

        //main
        void main()
        {   
            float depth = texture(depthTexture, fragmentUv).r;
            vec3 color = texture(albedoTexture, fragmentUv).rgb;

            vec4 H = vec4(fragmentUv.x * 2 - 1, (1 - fragmentUv.y) * 2 - 1, depth, 1);
            vec4 D = inverseViewProjection * H;
            vec4 worldPos = D / D.w;

            vec4 currentPos = H;
            vec4 previousPos = previousViewProjection * worldPos;
            previousPos /= previousPos.w;
            vec2 velocity = (currentPos.xy - previousPos.xy) * velocityMultiplier;

            vec2 texCoord = fragmentUv;
            texCoord += velocity;
            for(int i = 1; i < numSamples; ++i, texCoord += velocity)
            {
                vec4 currentColor = texture(albedoTexture, texCoord);
                color += currentColor.rgb;
            }
            vec3 finalColor = color / float(numSamples);
            outputColor = vec4(finalColor.rgb, 1); 
        }
        )";
        shader.name = "nullDepthOfFieldPostProcess";
        shader.Compile();
        
        quad = GetQuad(scene, "plane", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1, 1, 1, 1));
        material =  quad->GetComponent<MaterialComponent>();
        material->shader = shader;
        
        quad->Enable();        
    }
    void MotionBlurPostProcess::Run(Framebuffer* framebufferIn, Framebuffer* framebufferOut) {
        GETGL
        framebufferOut->Enable();
        
        ogl->glClearColor(1.0, 0, 0, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
		
        
        ogl->glUseProgram(material->shader.programShaderObject);

        GLuint loc = ogl->glGetUniformLocation(material->shader.programShaderObject, "texelSize");
        ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0 / (float)framebufferIn->width, 1.0 / (float)framebufferIn->height, 0)));

        material->albedoTex.glTex =  framebufferIn->texture;
		material->albedoTex.loaded = true;
		material->albedoTex.texIndex = GL_TEXTURE0;

        ogl->glActiveTexture(GL_TEXTURE1);
        ogl->glBindTexture(GL_TEXTURE_2D, framebufferIn->depthTexture);
        ogl->glUniform1i(ogl->glGetUniformLocation(material->shader.programShaderObject, "depthTexture"), 1);

        glm::mat4 projectionMatrix = glm::mat4(scene->camera->GetProjectionMatrix());
        glm::mat4 viewProjectionMat = projectionMatrix * scene->camera->viewMatrix;
        glm::mat4 previousViewProjectionMat = projectionMatrix * scene->camera->previousViewMatrix;

        ogl->glUniformMatrix4fv(ogl->glGetUniformLocation(material->shader.programShaderObject, "inverseViewProjection"), 1, false, glm::value_ptr(glm::inverse(viewProjectionMat)));
        ogl->glUniformMatrix4fv(ogl->glGetUniformLocation(material->shader.programShaderObject, "previousViewProjection"), 1, false, glm::value_ptr(previousViewProjectionMat));
        
        ogl->glUniform1f( ogl->glGetUniformLocation(material->shader.programShaderObject, "velocityMultiplier"), velocityMultiplier);
        ogl->glUniform1i( ogl->glGetUniformLocation(material->shader.programShaderObject, "numSamples"), numSamples);
        
        // ogl->glUniformMatrix4fv(ogl->glGetUniformLocation(this->shader->programShaderObject, "currentViewMatrix"); , 1, false, glm::value_ptr(scene->camera->viewMatrix));
        
        //Attach framebufferInTexture as a albedo texture
        quad->Render();
        framebufferOut->Disable();
    }
}
}