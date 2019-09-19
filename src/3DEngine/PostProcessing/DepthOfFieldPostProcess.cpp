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
        uniform vec3 texelSize;

        float focusDistance = 10;
        float focusRange = 3;


        //main
        void main()
        {   
            // float near = 0.1;
            // float far  = 1000.0;

            // float depth = texture(albedoTexture, fragmentUv).r;
            // float z = depth * 2.0 - 1.0; // back to NDC 
            // z = (2.0 * near * far) / (far + near - z * (far - near));

            // float coc = (z - focusDistance) / focusRange;
            // coc = clamp(coc, -1, 1);
            // if (coc < 0) {
            //     outputColor = coc * -vec4(1, 0, 0, 1);
            // } else {
            //     outputColor = vec4(coc, coc, coc, 1);
            // }

            vec3 color = 0;
            for (int u = -4; u <= 4; u++) {
                for (int v = -4; v <= 4; v++) {
                    vec2 o = vec2(u, v)  * texelSize.xy;
                    color += texture(albedoTexture, fragmentUv + o).rgb;
                }
            }
            color *= 1.0 / 81;
            outputColor = vec4(color.rgb, 1);

        }
        )";
        shader.name = "nullDepthOfFieldPostProcess";
        shader.Compile();
        
        quad = GetQuad(scene, "plane", glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(1, 1, 1, 1));
        material = (MaterialComponent*) quad->GetComponent("Material");
        material->SetShader(&shader);
        
        quad->Enable();        
    }
    void DepthOfFieldPostProcess::Run(Framebuffer* framebufferIn, Framebuffer* framebufferOut) {
        GETGL
        framebufferOut->Enable();
        
        ogl->glClearColor(1.0, 0, 0, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
		
        
        ogl->glUseProgram(shader.programShaderObject);

        GLuint loc = ogl->glGetUniformLocation(shader.programShaderObject, "inverseTextureSize");
        ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0 / (float)framebufferIn->width, 1.0 / (float)framebufferIn->height, 0)));

        material->albedoTex.glTex =  framebufferIn->texture;
		material->albedoTex.loaded = true;
		material->albedoTex.texIndex = GL_TEXTURE0;
        
        //Attach framebufferInTexture as a albedo texture
        quad->Render();
        framebufferOut->Disable();
    }
}
}