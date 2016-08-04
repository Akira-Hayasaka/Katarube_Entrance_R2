#version 330 core
precision highp float;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4  position;
in vec2  texcoord;
in vec4  color;
in vec3  normal;

out vec2 texcoordVarying;

void main()
{
    texcoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}