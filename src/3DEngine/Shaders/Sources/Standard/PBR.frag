 #version 440

        struct LightSource
        {
                int type;
                vec3 position;
                vec3 attenuation;
                vec3 direction;
                vec4 color;
        };

        float Fresnel(vec3 toLight, vec3 toCamera) {
            vec3 halfwayVec = normalize(toLight + toCamera);
            float dotLH =  max(dot(toLight, halfwayVec), 0);
            float refractionInx = (1.0 + sqrt(0.8)) / (1.0 - sqrt(0.8));

            float g = sqrt(refractionInx * refractionInx - 1.0 + pow(dotLH, 2.0)); 

            float firstTerm = 0.5 * (pow(g - dotLH, 2.0) / pow(g + dotLH, 2.0));
            float secondTermNumerator = pow(dotLH * (g + dotLH) - 1, 2.0); 
            float secondTermDenominator = pow(dotLH * (g - dotLH) + 1, 2.0);

            float result = firstTerm * (secondTermNumerator / secondTermDenominator + 1.0); 

            return result;
        }


        float MicrofacetsDistrib(vec3 toLight, vec3 toCamera, vec3 normal, float roughness) {
            vec3 halfwayVec = normalize(toLight + toCamera);
            float dotNH = max(dot(normal, halfwayVec), 0);

            float result = (1 / (4 * roughness * roughness * pow(dotNH, 4))) * exp( (dotNH * dotNH -1) / (roughness * roughness * dotNH * dotNH ));
            return result;
        }

        float GeometricalAttenuation(vec3 toLight, vec3 toCamera, vec3 normal) {
            vec3 halfwayVec = normalize(toLight + toCamera);
            float dotNH = dot(normal, halfwayVec);
            float dotNV = dot(normal, toCamera);
            float dotNL = dot(normal, toLight);
            float dotLH = dot(toLight, halfwayVec);
            
            
            float G1 = (2 * dotNH * dotNV) / dotLH;
            float G2 = (2 * dotNH * dotNL) / dotLH;

            return min (1, min(G1, G2));
        }

        layout(location = 0) out vec4 outputColor; 

        uniform LightSource lights[4];
        uniform int numLights; 
        uniform vec4 albedo; 
        uniform float roughness;
        uniform float specularFrac;

        in vec3 fragPos;
        in vec3 fragNormal;
        in vec3 fragToCam;

        void main()
        {
            // float roughness = 0.5;
            // float specularFrac = 0.5;
            float diffuseFrac = 1 - specularFrac;

            vec4 finalColor = vec4(0, 0, 0, 1);
            for(int i=0; i<numLights; i++) {
                vec3 lightDirection = normalize(lights[i].direction);
                float attenuation = 1;
                if(lights[i].type == 1) { //Point light
                    float distance = distance(fragPos.xyz, lights[i].position);
                    attenuation = 1 / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
                    lightDirection = normalize(fragPos.xyz - lights[i].position);
                }
                if(lights[i].type == 2) { //Spot light
                    lightDirection = normalize(fragPos.xyz - lights[i].position);
                    float distance = distance(fragPos.xyz, lights[i].position);
                    float numerator = pow(max(dot(-normalize(lights[i].direction), -lightDirection), 0), 64);
                    attenuation = numerator / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
                }
                vec3 toLight = -lightDirection;

                float fresnelFactor = Fresnel(toLight, fragToCam);
                float microfacetsDistribution = MicrofacetsDistrib(toLight, fragToCam, fragNormal, roughness);
                float geometricalAttenuation = GeometricalAttenuation(toLight, fragToCam, fragNormal);
                float finalFactor = fresnelFactor * ( (microfacetsDistribution * geometricalAttenuation) / (3.14 * dot(fragNormal, fragToCam) * dot(fragNormal, toLight) ) ); 
                
                float diffuseFactor = diffuseFrac * max(dot(normalize(fragNormal), toLight), 0);
                finalColor.rgb += attenuation * (diffuseFactor * lights[i].color.rgb * albedo.rgb +  specularFrac * lights[i].color.rgb * albedo.rgb * finalFactor);
            }



            outputColor = finalColor;
        }