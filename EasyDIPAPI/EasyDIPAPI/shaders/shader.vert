#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProj;

out VS_OUT {
    vec3 normal;
} vs_out;

void main()
{
    gl_Position =  mProj * mView * mModel * vec4(aPos, 1.0f);
	mat3 normalMatrix = mat3(transpose(inverse(mModel)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}