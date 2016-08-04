#version 330 core
precision highp float;

#pragma include "./ShaderHelpers.glslinc"
#pragma include "../models/shading/shadingOut.vert"

uniform vec2 resolution;
uniform sampler2D particlePosAndAgeTexture;
uniform sampler2D particleVelTexture;
uniform sampler2D dynamicInstanceParamTexture;
uniform sampler2D staticInstanceParamTexture;
uniform float particleMaxAge;
uniform float scale;
uniform vec3 offset;

uniform float scaleFactor;

out VSOUT
{
    vec3 vertCol;
    vec2 texCoordVarying;
    float bDiscard; // 0 = false, 1 = true;
} vsout;

void main ()
{
    vsout.texCoordVarying = texcoord;
    
    // Figure out the texture coordinate our data is on from the instance ID
    vec2 instanceTexcoord;
    instanceTexcoord.x = mod(gl_InstanceID, resolution.x) / resolution.x;
    instanceTexcoord.y = floor(gl_InstanceID / resolution.x) / resolution.y;
    
    // Grab our data
    vec4 staticInstanceParam = texture(staticInstanceParamTexture, instanceTexcoord);
    
    if (staticInstanceParam.x != 1.0)
    {
        vsout.bDiscard = 1.0;
        
        gl_Position = vec4(0.0);
    }
    else
    {
        vsout.bDiscard = 0.0;
        
        vec4 particleData = texture(particlePosAndAgeTexture, instanceTexcoord);
        vec3 particleVel = texture(particleVelTexture, instanceTexcoord).xyz;
        vec4 dynamicInstanceParam = texture(dynamicInstanceParamTexture, instanceTexcoord);
        vsout.vertCol = dynamicInstanceParam.xyz;
        
        float ageFrac = particleData.w / particleMaxAge;
        vec4 vertexPos = position;
        vec3 particlePos = particleData.xyz;
        
        // We are going to scale the particle so it comes in and out,
        // as the vertex position is in model space, we can just multiply it to grow or shrink it
        vertexPos *= smoothStepInOut(0.0, 0.1, 0.9, 1.0, ageFrac) * (dynamicInstanceParam.w * scaleFactor);
        
        // Rotate the vertex of our mesh (in model space) to face the velocity direction
        vec3 vel = vec3(rotationMatrix(vec3(0.0, 1.0, 0.0), 90.0) * vec4(particleVel, 0.0));
        mat4 lookAt = makeLookAt(vec3(0,0,0), vel, vec3(0,1,0));

        vertexPos = lookAt * vertexPos;
        
        // We add the rotated model space vertex pos to the particle pos to get the final position in space
        vec3 newVertexPos = (particlePos + vertexPos.xyz) * scale;
        newVertexPos += offset;

        
        setShadingOut(vec4(newVertexPos, 1.0));
        
        gl_Position = modelViewProjectionMatrix * vec4(newVertexPos, 1.0);
    }
}