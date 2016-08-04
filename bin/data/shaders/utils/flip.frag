#pragma include "../commom/header.glsl"

uniform sampler2DRect tex;
uniform vec2 texRes;
uniform float vertFlip; // 0 = false, 1 = true;
uniform float horizFlip; // 0 = false, 1 = true;

in VSOUT
{
    vec2 texCoord;
} fsIn;

out vec4 fragColor;

void main()
{
    vec2 texCoord = fsIn.texCoord;
    
    if (horizFlip == 1.0)
    {
        texCoord.x = texRes.x - texCoord.x;
    }
    
    if (vertFlip == 1.0)
    {
        texCoord.y = texRes.y - texCoord.y;
    }
    
    vec4 col = texture(tex, texCoord);
    
    fragColor = col;
}