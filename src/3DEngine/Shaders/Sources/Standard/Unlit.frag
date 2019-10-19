//inputs
    #version 440
    in vec4 fragmentColor; 
    in vec2 fragmentUv;
    in vec3 cubeTexCoords;
    //uniforms
    uniform int hasAlbedoTex;
    uniform sampler2D albedoTexture;
    uniform samplerCube cubemapTexture;

    uniform int hasCubemap;
    //output
    layout(location = 0) out vec4 outputColor; 
    //main
    void main()
    {
        if(hasCubemap > 0) {
            outputColor = vec4(texture(cubemapTexture, cubeTexCoords).xyz, 1);
            // outputColor = vec4(texture(cubeTexCoords).xyz, 1);
        } else {
            outputColor = (hasAlbedoTex==1) ? texture(albedoTexture, fragmentUv) : fragmentColor;
            // outputColor = vec4(fragmentUv.xy, 0, 1);
        }
    }