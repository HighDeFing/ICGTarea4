#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProj;


out vec2 TexCoord;

void main()
{
    gl_Position =  mProj * mView * mModel * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
}