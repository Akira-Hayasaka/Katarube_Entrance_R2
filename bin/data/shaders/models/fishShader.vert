#version 150
#extension GL_ARB_explicit_attrib_location : enable

#pragma include "./animation/animatedInstance.vert"
#pragma include "./shading/shadingOut.vert"

uniform mat4 camMvpMatrix;
uniform sampler2DRect modelTransTexture;

void main()
{
    mat4 transformMatrix = getMat(modelTransTexture, gl_InstanceID, 0.0);
    vec4 vPos = transformMatrix * getAnimatedVertPos(position);
    
    setShadingOut(vPos);
    
    gl_Position = camMvpMatrix * vPos;
}