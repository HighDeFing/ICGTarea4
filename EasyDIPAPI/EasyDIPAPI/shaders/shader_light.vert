#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProj;

void main()
{
	gl_Position = mProj * mView * mModel * vec4(aPos, 1.0);
}