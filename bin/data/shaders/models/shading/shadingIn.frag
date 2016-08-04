uniform sampler2DRect diffuseTex;
uniform sampler2DRect opacityTex;
uniform sampler2DRect envTex;
uniform sampler2DRect bumpTex;
uniform vec2 envTexRes;

const vec4 globalColor = vec4(1.0);

in SHADINGOUT
{
    vec2 texCoordVarying;
    vec3 position;
    vec3 view;
    vec3 N;
    vec3 L;
    vec3 V;
} shIn;