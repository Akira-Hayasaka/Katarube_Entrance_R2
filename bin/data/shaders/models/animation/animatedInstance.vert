precision highp float;

uniform sampler2DRect animationTexture;

in float boneID0;
in float boneID1;
in float boneID2;
in float boneID3;
in float weight0;
in float weight1;
in float weight2;
in float weight3;

mat4 getMat(sampler2DRect tex, float stepx, float stepy)
{
    float x = stepx * 4.0;
    float y = stepy;
    
    mat4 mat = mat4(texture(tex, vec2((x+0.5), y+0.5)),
                    texture(tex, vec2((x+1.5), y+0.5)),
                    texture(tex, vec2((x+2.5), y+0.5)),
                    texture(tex, vec2((x+3.5), y+0.5)));
    return mat;
}

vec4 getAnimatedVertPos(vec4 vertPos)
{
    vec4 animPos = vec4(0.0);
    animPos += weight0 * (getMat(animationTexture, boneID0, gl_InstanceID) * vertPos);
    animPos += weight1 * (getMat(animationTexture, boneID1, gl_InstanceID) * vertPos);
    animPos += weight2 * (getMat(animationTexture, boneID2, gl_InstanceID) * vertPos);
    animPos += weight3 * (getMat(animationTexture, boneID3, gl_InstanceID) * vertPos);
    return animPos;
}