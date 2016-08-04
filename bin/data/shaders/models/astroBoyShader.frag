#version 150

#pragma include "./shading/phong.frag"

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
        fragColor = vec4(getPhongColor(), 1.0);
    }
}