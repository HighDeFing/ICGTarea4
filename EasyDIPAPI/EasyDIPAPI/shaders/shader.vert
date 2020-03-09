#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 mModelView;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 normal;
} vs_out;

void main()
{
    gl_Position =  projection * mModelView * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(mModelView)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}