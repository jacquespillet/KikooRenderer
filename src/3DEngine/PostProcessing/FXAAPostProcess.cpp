#include "FXAAPostProcess.hpp"
#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"


namespace KikooRenderer {
namespace CoreEngine {
    FXAAPostProcess::FXAAPostProcess(Scene* scene) : PostProcess(scene) {
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
            fragmentUv = vec2(uv.x, -uv.y);
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
        uniform vec3 inverseTextureSize;

        //main
        void main()
        {   
            //Compute Luminance
            vec3 luma = vec3(0.299, 0.587, 0.114);
            float lumaTL = dot(luma, texture(albedoTexture, fragmentUv + vec2(-1, -1) * inverseTextureSize.xy).rgb);
            float lumaTR = dot(luma, texture(albedoTexture, fragmentUv + vec2( 1, -1) * inverseTextureSize.xy).rgb);
            float lumaBL = dot(luma, texture(albedoTexture, fragmentUv + vec2(-1,  1) * inverseTextureSize.xy).rgb);
            float lumaBR = dot(luma, texture(albedoTexture, fragmentUv + vec2( 1,  1) * inverseTextureSize.xy).rgb);
            float lumaM  = dot(luma, texture(albedoTexture, fragmentUv).rgb);

            //Compute direction
            vec2 dir;
            dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
            dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));

            float minValue = 1.0 / 128.0;
            float maxSpan = 8.0;
            float reduceMultiplier = 1.0 / 8.0;

            float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (0.25 * reduceMultiplier), minValue);
            float invDirAdjustment = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

            dir = clamp(dir * invDirAdjustment, vec2(-maxSpan, -maxSpan), vec2(maxSpan, maxSpan)) * inverseTextureSize.xy; //Does that work ??

            vec3 result1 = (1.0 / 2.0) * (texture(albedoTexture, fragmentUv + (dir * vec2(1.0/3.0 - 0.5))).rgb + 
                                           texture(albedoTexture, fragmentUv + (dir * vec2(2.0/3.0 - 0.5))).rgb);          

            vec3 result2 = result1 *  (1.0 / 2.0) + (1.0 / 2.0) * (texture(albedoTexture, fragmentUv + (dir * vec2(0.0/3.0 - 0.5))).rgb + 
                                           texture(albedoTexture, fragmentUv + (dir * vec2(3.0/3.0 - 0.5))).rgb); 

            float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
            float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
            float lumaResult2 = dot(luma, result2);

            if(lumaResult2 < lumaMin || lumaResult2 > lumaMax) {
                outputColor = vec4(result1, 1);
            } else {
                outputColor = vec4(result2, 1);
            }


            // vec3 color = texture(albedoTexture, fragmentUv).rgb;
            // float grayScale = (color.r + color.g + color.b) * 0.3333333;
            // outputColor = vec4(grayScale, grayScale, grayScale, 1);
            // outputColor = vec4(tempVal, tempVal, tempVal, 1);
            // outputColor = vec4(inverseTextureSize.x, inverseTextureSize.x, inverseTextureSize.x, 1);
        }
        )";
        shader.name = "nullFXAAPostProcess";
        shader.Compile();
        
        quad = GetQuad(scene, "plane", glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(1, 1, 1, 1));
        material = (MaterialComponent*) quad->GetComponent("Material");
        material->SetShader(&shader);
        
        quad->Enable();        
    }
    void FXAAPostProcess::Run(Framebuffer* framebufferIn, Framebuffer* framebufferOut) {
        GETGL
        // std::cout << "Writing " << framebufferOut << std::endl;
        framebufferOut->Enable();
        ogl->glClearColor(1.0, 0, 0, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);


		material->albedoTex.glTex =  framebufferIn->texture;
		material->albedoTex.loaded = true;
		material->albedoTex.texIndex = GL_TEXTURE0;

        ogl->glUseProgram(shader.programShaderObject);

        GLuint loc = ogl->glGetUniformLocation(shader.programShaderObject, "inverseTextureSize");
        // ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0 / (float)framebufferIn->width, 1.0 / (float)framebufferIn->height, 0)));
        ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(0)));

        //Attach framebufferInTexture as a albedo texture
        quad->Render();
        framebufferOut->Disable();
    }
}
}