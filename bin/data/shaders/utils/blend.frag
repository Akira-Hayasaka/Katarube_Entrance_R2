#version 150

#pragma include "../utils/psBlend.glsl"

uniform sampler2D base;
uniform sampler2D blend;
uniform int blendMode;
uniform vec2 renderRes;

in VSOUT
{
    vec2 texCoord;
} fsin;

out vec4 outputColor;

void main ()
{
    vec4 baseCol = texture(base, fsin.texCoord / renderRes);
    vec4 blendCol = texture(blend, fsin.texCoord / renderRes);
//    outputColor = getBlendCol(baseCol, blendCol, blendMode);
    outputColor = getBlendCol(blendCol, baseCol, blendMode);
}