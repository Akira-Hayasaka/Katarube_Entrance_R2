#version 330 core
precision highp float;

#pragma include "../models/shading/shadingIn.frag"
#pragma include "../utils/colTweak.glsl"

uniform sampler2D drawing;

in VSOUT
{
    vec3 vertCol;
    vec2 texCoordVarying;
    float bDiscard; // 0 = false, 1 = true;    
} fsin;

out vec4 outputColor;

void main ()
{
    if (fsin.bDiscard == 1.0)
        discard;
    
    outputColor = texture(drawing, fsin.texCoordVarying);
}