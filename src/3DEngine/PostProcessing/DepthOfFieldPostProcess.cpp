#include "DepthOfFieldPostProcess.hpp"
#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"


namespace KikooRenderer {
namespace CoreEngine {
    DepthOfFieldPostProcess::DepthOfFieldPostProcess(Scene* scene): PostProcess(scene)  {
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

        uniform float focusDistance;
        uniform float focusRange;
        uniform float bokehRadius;

        uniform float nearClippingPlane;
        uniform float farClippingPlane;

        const int kernelSampleCount = 22;
        const vec2 kernel[kernelSampleCount] = {
            vec2(0, 0),
            vec2(0.53333336, 0),
            vec2(0.3325279, 0.4169768),
            vec2(-0.11867785, 0.5199616),
            vec2(-0.48051673, 0.2314047),
            vec2(-0.48051673, -0.23140468),
            vec2(-0.11867763, -0.51996166),
            vec2(0.33252785, -0.4169769),
            vec2(1, 0),
            vec2(0.90096885, 0.43388376),
            vec2(0.6234898, 0.7818315),
            vec2(0.22252098, 0.9749279),
            vec2(-0.22252095, 0.9749279),
            vec2(-0.62349, 0.7818314),
            vec2(-0.90096885, 0.43388382),
            vec2(-1, 0),
            vec2(-0.90096885, -0.43388376),
            vec2(-0.6234896, -0.7818316),
            vec2(-0.22252055, -0.974928),
            vec2(0.2225215, -0.9749278),
            vec2(0.6234897, -0.7818316),
            vec2(0.90096885, -0.43388376),
        };


        //main
        void main()
        {   
            float depth = texture(depthTexture, fragmentUv).r;
            float z = depth * 2.0 - 1.0; // back to NDC 
            z = (2.0 * nearClippingPlane * farClippingPlane) / (farClippingPlane + nearClippingPlane - z * (farClippingPlane - nearClippingPlane));

            float coc = (z - focusDistance) / focusRange;
            coc = clamp(coc, -1, 1);

            vec3 color = vec3(0, 0, 0);
            for (int k = 0; k < kernelSampleCount; k++) {
                vec2 o = kernel[k] * texelSize.xy * bokehRadius * coc;
                color += texture(albedoTexture, fragmentUv + o).rgb;
            }
            color *= 1.0 / kernelSampleCount;

            outputColor = vec4(color.rgb, 1);
        }
        )";
        shader.name = "nullDepthOfFieldPostProcess";
        shader.Compile();
        
        quad = GetQuad(scene, "plane", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1, 1, 1, 1));
        material = quad->GetComponent<MaterialComponent>();
        material->SetShader(&shader);
        
        quad->Enable();        
    }
    void DepthOfFieldPostProcess::Run(Framebuffer* framebufferIn, Framebuffer* framebufferOut) {
        GETGL
        framebufferOut->Enable();
        
        ogl->glClearColor(1.0, 0, 0, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
		
        
        ogl->glUseProgram(shader.programShaderObject);

        GLuint loc = ogl->glGetUniformLocation(shader.programShaderObject, "texelSize");
        ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0 / (float)framebufferIn->width, 1.0 / (float)framebufferIn->height, 0)));

        material->albedoTex.glTex =  framebufferIn->texture;
		material->albedoTex.loaded = true;
		material->albedoTex.texIndex = GL_TEXTURE0;

        ogl->glActiveTexture(GL_TEXTURE1);
        ogl->glBindTexture(GL_TEXTURE_2D, framebufferIn->depthTexture);
        ogl->glUniform1i(ogl->glGetUniformLocation(shader.programShaderObject, "depthTexture"), 1);
        
        ogl->glUniform1f(ogl->glGetUniformLocation(shader.programShaderObject, "focusDistance"), focusDistance);
        ogl->glUniform1f(ogl->glGetUniformLocation(shader.programShaderObject, "focusRange"), focusRange);
        ogl->glUniform1f(ogl->glGetUniformLocation(shader.programShaderObject, "bokehRadius"), bokehRadius);
        
        ogl->glUniform1f(ogl->glGetUniformLocation(shader.programShaderObject, "nearClippingPlane"), scene->camera->nearClip);
        ogl->glUniform1f(ogl->glGetUniformLocation(shader.programShaderObject, "farClippingPlane"), scene->camera->farClip);
        

        //Attach framebufferInTexture as a albedo texture
        quad->Render();
        framebufferOut->Disable();
    }
}
}