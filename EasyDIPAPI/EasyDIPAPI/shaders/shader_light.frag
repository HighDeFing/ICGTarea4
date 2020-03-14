#version 330 core
out vec4 FragColor;

uniform vec4 my_color;

void main()
{
    FragColor = my_color; // set alle 4 vector values to 1.0
}