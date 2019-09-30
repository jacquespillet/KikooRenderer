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
            fragmentUv = vec2(uv.x, uv.y);
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
        }
        )";
        std::string simpleFxaa = R"(
        //inputs
        #version 440
        in vec2 fragmentUv;

        //output
        layout(location = 0) out vec4 outputColor; 
        uniform sampler2D albedoTexture;
        uniform vec3 inverseTextureSize;

        
        uniform float minValue;
        uniform float maxSpan;
        uniform float reduceMultiplier;


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

            float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (0.25 * reduceMultiplier), minValue);
            float invDirAdjustment = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

            dir = clamp(dir * invDirAdjustment, vec2(-maxSpan, -maxSpan), vec2(maxSpan, maxSpan)) * inverseTextureSize.xy;

            vec3 result1 = 0.5 * (texture(albedoTexture, fragmentUv + (dir * vec2(1.0/3.0 - 0.5))).rgb + 
                                           texture(albedoTexture, fragmentUv + (dir * vec2(2.0/3.0 - 0.5))).rgb);          

            vec3 result2 = result1 *  0.5 + 0.5 * (texture(albedoTexture, fragmentUv + (dir * vec2(0.0/3.0 - 0.5))).rgb + 
                                           texture(albedoTexture, fragmentUv + (dir * vec2(3.0/3.0 - 0.5))).rgb); 

            float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
            float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
            float lumaResult2 = dot(luma, result2);

            if(lumaResult2 < lumaMin || lumaResult2 > lumaMax) {
                outputColor = vec4(result1, 1);
            } else {
                outputColor = vec4(result2, 1);
            }
        }
        )";

        std::string complexFxaa = R"(
        //inputs
        #version 440
        in vec2 fragmentUv;

        //output
        layout(location = 0) out vec4 outputColor; 
        uniform sampler2D albedoTexture;
        uniform vec3 inverseTextureSize;

        uniform float blendFactorMultiplier;

    	float contrastThreshold = 0.0312f;
        float relativeThreshold = 0.063f;

		struct EdgeData {
			bool isHorizontal;
            float pixelStep;
			float oppositeLuminance; 
            float gradient;
		};

        struct LuminanceData {
            float m;
            float e;
            float w;
            float s;
            float n;

			float ne;
            float nw;
            float se;
            float sw;

            float highest;
            float lowest;
            float contrast;
        };

        LuminanceData GetLuminanceNeighborhood(vec2 uv) {
            vec3 luma = vec3(0.2126729f,  0.7151522f, 0.0721750f);
    
            LuminanceData l;

			l.m = dot(luma, texture(albedoTexture, uv).rgb);
			l.n = dot(luma, texture(albedoTexture, uv + vec2( 0,  1) * inverseTextureSize.xy).rgb);
			l.e = dot(luma, texture(albedoTexture, uv + vec2( 1,  0) * inverseTextureSize.xy).rgb);
			l.s = dot(luma, texture(albedoTexture, uv + vec2( 0, -1) * inverseTextureSize.xy).rgb);
			l.w = dot(luma, texture(albedoTexture, uv + vec2(-1,  0) * inverseTextureSize.xy).rgb);

			l.ne = dot(luma, texture(albedoTexture, uv + vec2(  1,  1) * inverseTextureSize.xy).rgb);
			l.nw = dot(luma, texture(albedoTexture, uv + vec2( -1,  1) * inverseTextureSize.xy).rgb);
			l.se = dot(luma, texture(albedoTexture, uv + vec2(  1, -1) * inverseTextureSize.xy).rgb);
			l.sw = dot(luma, texture(albedoTexture, uv + vec2( -1, -1) * inverseTextureSize.xy).rgb);

			l.highest = max(max(max(max(l.n, l.e), l.s), l.w), l.m);
			l.lowest = min(min(min(min(l.n, l.e), l.s), l.w), l.m);
			l.contrast = l.highest - l.lowest;            
			return l;
        }

        float GetBlendFactor(LuminanceData l) {
			float f = 2 * (l.n + l.e + l.s + l.w);
			f += l.ne + l.nw + l.se + l.sw;
			f *= 1.0 / 12;
            f = abs(f - l.m);
            
			f = clamp(f / l.contrast, 0.05f, 1);

			float blendFactor = smoothstep(0, 0.9f, f);
			return blendFactor * blendFactor;
        }

		EdgeData DetermineEdge (LuminanceData l) {
			EdgeData e;
			float horizontal =
				abs(l.n + l.s - 2 * l.m) * 2 +
				abs(l.ne + l.se - 2 * l.e) +
				abs(l.nw + l.sw - 2 * l.w);
			
            float vertical =
				abs(l.e + l.w - 2 * l.m) * 2 +
				abs(l.ne + l.nw - 2 * l.n) +
				abs(l.se + l.sw - 2 * l.s);

			e.isHorizontal = horizontal >= vertical;

			float positiveLuminance = e.isHorizontal ? l.n : l.e;
			float negativeLuminance = e.isHorizontal ? l.s : l.w;
			
            float positiveGradient = abs(positiveLuminance - l.m);
			float negativeGradient = abs(negativeLuminance - l.m);

			e.pixelStep = e.isHorizontal ? inverseTextureSize.y : inverseTextureSize.x;            
   
            if (positiveGradient < negativeGradient) {
				e.pixelStep = -e.pixelStep;
				e.oppositeLuminance = negativeLuminance;
				e.gradient = negativeGradient;                
			} else {
				e.oppositeLuminance = positiveLuminance;
				e.gradient = positiveGradient;                
            }

            return e;
		}        

		float DetermineEdgeBlendFactor (LuminanceData l, EdgeData e, vec2 uv) {
            vec3 luma = vec3(0.2126729f,  0.7151522f, 0.0721750f);
            
			vec2 uvEdge = uv;
			vec2 edgeStep;
			if (e.isHorizontal) {
				uvEdge.y += e.pixelStep * 0.5;
				edgeStep = vec2(inverseTextureSize.x, 0);
			}
			else {
				uvEdge.x += e.pixelStep * 0.5;
				edgeStep = vec2(0, inverseTextureSize.y);
			}

            float edgeLuminance = (l.m + e.oppositeLuminance) * 0.5;
            float gradientThreshold = e.gradient * 0.25;

            vec2 positiveUv= uvEdge + edgeStep;
            float positiveLuminancedelta = dot(luma, texture(albedoTexture, positiveUv).rgb) - edgeLuminance;
            bool pAtEnd = abs(positiveLuminancedelta) >= gradientThreshold;

			for (int i = 0; i < 9 && !pAtEnd; i++) {
				positiveUv += edgeStep;
				positiveLuminancedelta =dot(luma, texture(albedoTexture, positiveUv).rgb) - edgeLuminance;
				pAtEnd = abs(positiveLuminancedelta) >= gradientThreshold;
			}


			vec2 negativeUv = uvEdge - edgeStep;
			float negativeLuminanceDelta = dot(luma, texture(albedoTexture, negativeUv).rgb) - edgeLuminance;
			bool negativeAtEnd = abs(negativeLuminanceDelta) >= gradientThreshold;

			for (int i = 0; i < 9 && !negativeAtEnd; i++) {
				negativeUv -= edgeStep;
				negativeLuminanceDelta = dot(luma, texture(albedoTexture, negativeUv).rgb) - edgeLuminance;
				negativeAtEnd = abs(negativeLuminanceDelta) >= gradientThreshold;
			}

			float pDistance, nDistance;
			if (e.isHorizontal) {
				pDistance = positiveUv.x - uv.x;
				nDistance = uv.x - negativeUv.x;
			}
			else {
				pDistance = positiveUv.y - uv.y;
				nDistance = uv.y - negativeUv.y;
			}

			float shortestDistance;
            bool deltaSign;
			if (pDistance <= nDistance) {
				shortestDistance = pDistance;
				deltaSign = positiveLuminancedelta >= 0;
			}
			else {
				shortestDistance = nDistance;
				deltaSign = negativeLuminanceDelta >= 0;
			}
            // return shortestDistance * 100;

			if (deltaSign == (l.m - edgeLuminance >= 0)) {
				return 0;
			}
            return 0.5 - shortestDistance / (pDistance + nDistance);;
		}


        vec4 GetFinalColor() {
            LuminanceData l = GetLuminanceNeighborhood(fragmentUv);
			float actualThreshold = max(contrastThreshold, relativeThreshold * l.highest);
            if (l.contrast < actualThreshold) {
				return vec4(texture(albedoTexture, fragmentUv).rgb, 1);
			}

            float blendFactor = GetBlendFactor(l);
            // return vec4(blendFactor, blendFactor, blendFactor, 1);

            EdgeData edge = DetermineEdge(l);
            // if(edge.isHorizontal) return vec4(1, 0, 0, 1);
            // else return vec4(0, 1, 0, 1);

			float edgeBlendFactor = DetermineEdgeBlendFactor(l, edge, fragmentUv);
            // return vec4(edgeBlendFactor, edgeBlendFactor, edgeBlendFactor, 1);

			float finalBlend = max(blendFactor, edgeBlendFactor)  * blendFactorMultiplier;

            vec2 blendUv = fragmentUv;
			if (edge.isHorizontal) {
                float height = 1.0 / inverseTextureSize.y;
				blendUv.y += edge.pixelStep * finalBlend;
                // return vec4(edge.pixelStep * finalBlend* height, 0, 0, 1);
			}
			else {
                float width = 1.0 / inverseTextureSize.x;
				blendUv.x += edge.pixelStep * finalBlend;
                // return vec4(0,edge.pixelStep * finalBlend * width, 0, 1);
			}
			return vec4(texture(albedoTexture, blendUv).rgb, 1);
        }
        

        //main
        void main()
        {   
            outputColor = GetFinalColor();
        }
        )";

        shader.fragSrc = complexFxaa;
        shader.name = "nullFXAAPostProcess";
        shader.Compile();
        
        quad = GetQuad(scene, "plane", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1, 1, 1, 1));
        material = quad->GetComponent<MaterialComponent>();
        material->SetShader(shader);
        
        quad->Enable();        
    }
    void FXAAPostProcess::Run(Framebuffer* framebufferIn, Framebuffer* framebufferOut) {
        GETGL
        // std::cout << "Writing " << framebufferOut << std::endl;
        framebufferOut->Enable();
        ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);


		material->albedoTex.glTex =  framebufferIn->texture;
		material->albedoTex.loaded = true;
		material->albedoTex.texIndex = GL_TEXTURE0;

        ogl->glUseProgram(shader.programShaderObject);

        GLuint loc = ogl->glGetUniformLocation(shader.programShaderObject, "inverseTextureSize");
        ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0 / (float)framebufferIn->width, 1.0 / (float)framebufferIn->height, 0)));

        loc = ogl->glGetUniformLocation(shader.programShaderObject, "minValue"); 
        ogl->glUniform1f(loc, minValue);

        loc = ogl->glGetUniformLocation(shader.programShaderObject, "maxSpan"); 
        ogl->glUniform1f(loc, maxSpan);

        loc = ogl->glGetUniformLocation(shader.programShaderObject, "reduceMultiplier"); 
        ogl->glUniform1f(loc, reduceMultiplier);

        loc = ogl->glGetUniformLocation(shader.programShaderObject, "blendFactorMultiplier"); 
        ogl->glUniform1f(loc, blendFactorMultiplier);

        //Attach framebufferInTexture as a albedo texture
        quad->Render();
        framebufferOut->Disable();
    }
}
}