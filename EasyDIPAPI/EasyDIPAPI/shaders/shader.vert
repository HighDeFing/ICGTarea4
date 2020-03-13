#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProj;
uniform mat3 mTransposeModel;


out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    //gl_Position =  mProj * mView * mModel * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;

	FragPos = vec3(mModel * vec4(aPos, 1.0));
	gl_Position = mProj * mView * vec4(FragPos, 1.0);

    Normal = mTransposeModel * aNormal;  
}