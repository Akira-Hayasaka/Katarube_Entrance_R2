#version 150
precision highp float;

#pragma include "./shading/shadingIn.frag"
#pragma include "../effects/fog.frag"

uniform float bBlack; // 0.0 = false, 1.0 = true;

out vec4 fragColor;

void main()
{
    if (bBlack == 1.0)
    {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        vec4 texColor = texture(diffuseTex, shIn.texCoordVarying) * globalColor;
        vec3 diffuse_albedo = texColor.rgb * 1.6;
        
        vec3 N = normalize(shIn.N);
        vec3 L = normalize(shIn.L);
        vec3 V = normalize(shIn.V);
        vec3 R = reflect(-L, N);
        vec3 diffuse = max(dot(N, L), 0.6) * diffuse_albedo;
        vec3 specular_albedo = vec3(0.0);
        float specular_power = 128.0;
        vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;
        
        vec3 finalColor = diffuse + specular;
        
        // env map (spherical)
        vec3 U = normalize(shIn.view);
        vec3 eR = reflect(U, normalize(shIn.N));
        eR.z += 1.0;
        float m = 0.5 * inversesqrt(dot(eR, eR));
        vec2 tc = eR.xy * m + vec2(0.5);
        tc *= envTexRes;
        vec3 envCol = texture(envTex, tc).rgb;
        finalColor = mix(finalColor, envCol, 0.2);
        
        // make a littl bit bright
        finalColor *= 1.3;
        
        // opacity map
        vec4 opacity = texture(opacityTex, shIn.texCoordVarying);
        float alpha = opacity.r;
        
        // fog
    //    finalColor = getFoggedColor(finalColor);
        
        fragColor = vec4(finalColor, alpha);
    }
}