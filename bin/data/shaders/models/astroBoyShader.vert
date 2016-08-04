#version 150

#pragma include "./animation/animatedInstance.vert"
#pragma include "./shading/shadingOut.vert"

uniform mat4 camMvpMatrix;
uniform sampler2DRect modelTransTexture;

in vec4  position;

void main()
{
    mat4 transformMatrix = getMat(modelTransTexture, gl_InstanceID, 0.0);
    vec4 vPos = transformMatrix * getAnimatedVertPos(position);
    
    setShadingOut(vPos);
    
    gl_Position = camMvpMatrix * vPos;
}