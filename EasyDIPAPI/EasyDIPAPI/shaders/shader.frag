#version 330 core
out vec4 FragColor;
uniform vec4 my_color;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = my_color;
	FragColor = texture(ourTexture, TexCoord) * my_color;
} 