#version 330 core
precision highp float;

#pragma include "./ShaderHelpers.glslinc"
#pragma include "./SimplexNoiseDerivatives4D.glslinc"

uniform sampler2D particlePosAndAgeTexture;
uniform sampler2D particleVelTexture;

uniform float time;
uniform float timeStep;

uniform float particleMaxAge;

uniform float noisePositionScale = 1.5;
uniform float noiseMagnitude = 0.075;
uniform float noiseTimeScale = 1.0 / 4000.0;
uniform float noisePersistence = 0.2;
uniform vec3 baseSpeed = vec3(0.5, 0.0, 0.0);
uniform float spawnRadius;
uniform float bSpawn; // 0 = false 1 = true;

const int OCTAVES = 3;

in vec2 texcoordVarying;

layout (location = 0) out vec4 outputColor1;
layout (location = 1) out vec4 outputColor2;

void main (void)
{
    vec2 texCoord = texcoordVarying;
    
    vec4 posAndAge = texture(particlePosAndAgeTexture, texCoord);
    
    vec3 pos = posAndAge.xyz;
    float age = posAndAge.w;
    
    age += timeStep;
    
    if (age > particleMaxAge)
    {
        age = 0.0;
        pos = randomPointOnSphere(vec3(rand(texCoord + pos.xy), rand(texCoord.xy + pos.yz), rand(texCoord.yx + pos.yz))) * spawnRadius;
    }
    
    vec3 noisePosition = pos  * noisePositionScale;
    float noiseTime = time * noiseTimeScale;
    
    vec3 noiseVelocity = curlNoise(noisePosition, noiseTime, OCTAVES, noisePersistence) * noiseMagnitude;
    vec3 totalVelocity = baseSpeed + noiseVelocity;
    
    vec3 newPos = pos + totalVelocity * timeStep;
    vec3 vel = newPos - pos;
    
    pos = newPos;
    
    outputColor1 = vec4(pos, age);
    outputColor2 = vec4(vel, 1.0);
}