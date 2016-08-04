uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

const vec3 light_pos = vec3(10.0, 10.0, 10.0);

in vec4  position;
in vec2  texcoord;
in vec4  color;
in vec3  normal;

out SHADINGOUT
{
    vec2 texCoordVarying;
    vec3 position;
    vec3 view;
    vec3 N;
    vec3 L;
    vec3 V;
} shOut;

void setShadingOut(vec4 vPos)
{
    shOut.N = mat3(modelViewMatrix) * normal;
    shOut.position = vec3(modelViewMatrix * vPos);
    shOut.L = light_pos - vPos.xyz;
    shOut.V = -vPos.xyz;
    shOut.texCoordVarying = (textureMatrix * vec4(texcoord.x, texcoord.y, 0, 1)).xy;
    shOut.view = vPos.xyz;
}