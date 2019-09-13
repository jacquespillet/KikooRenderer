#include "PostProcess.hpp"
#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"


namespace KikooRenderer {
namespace CoreEngine {
    PostProcess::PostProcess() {
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

        //main
        void main()
        {   
            // vec3 hdrColor = texture(albedoTexture, fragmentUv).rgb;
            outputColor = vec4(1,0,0,1);
        }
        )";
        shader.name = "nullPostProcess";
        std::cout << "StandardShaders: Compiling shader" << std::endl; 
        shader.Compile();
        
        quad = GetQuad(nullptr, "plane", glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(1, 1, 1, 1));
        material = (MaterialComponent*) quad->GetComponent("Material");
        material->SetShader(&shader);
        
        // int exposureLocation = ogl->glGetUniformLocation(quadShader.programShaderObject, "exposure"); 
        // ogl->glUniform1f(exposureLocation, exposure);
        quad->Enable();        
    }
    void PostProcess::Run(Framebuffer* famebufferIn, Framebuffer* famebufferOut) {
        famebufferOut->Enable();
        //Attach framebufferInTexture as a albedo texture
        quad->Render();
        famebufferOut->Disable();


    }
}
}