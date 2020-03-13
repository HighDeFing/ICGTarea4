#version 330 core
out vec4 FragColor;

uniform vec4 my_color;

uniform bool texture_drawing;
uniform bool only_color;
uniform bool only_texture;
uniform bool texture_with_color;

uniform bool bambient;
uniform bool bdiffuse;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos; 

uniform vec3 lightPos; 
uniform vec3 lightColor;

uniform vec3 objectColor;
uniform vec3 pointsColor;
uniform vec3 wireColor;

uniform sampler2D ourTexture;

void main()
{
	
    //FragColor = my_color;
	if(texture_drawing){
		if(texture_with_color)
		{
		FragColor = texture(ourTexture, TexCoord) * vec4(objectColor, 1.0);
		}
		if(only_color)
		{
		FragColor = my_color;
		}
		if(only_texture)
		{
		FragColor = texture(ourTexture, TexCoord);
		}
	}
	else
	{
		if(bambient && !bdiffuse) //ambiental y no difusa
		 {
		 float ambientStrength = 0.8;
		 vec3 ambient = ambientStrength * lightColor;

		 vec3 result = ambient * objectColor;
		 FragColor = vec4(result, 1.0);
		 } 
		 else
		 {
			 if(bdiffuse && !bambient) //difusa y no ambiental
			 {
			 vec3 norm = normalize(Normal);
			 vec3 lightDir = normalize(lightPos - FragPos);
			 float diff = max(dot(norm, lightDir), 0.0);
			 vec3 diffuse = diff * lightColor;
			 vec3 result = (diffuse) * objectColor;
			 FragColor = vec4(result, 1.0);
			 } 
			 else
				 {
					if(bdiffuse && bambient) //difusa y ambiental
					{
					 float ambientStrength = 0.8;
					 vec3 ambient = ambientStrength * lightColor;

					 vec3 norm = normalize(Normal);
					 vec3 lightDir = normalize(lightPos - FragPos);
					 float diff = max(dot(norm, lightDir), 0.0);
					 vec3 diffuse = diff * lightColor;
					 vec3 result = (diffuse + ambient) * objectColor;
					 FragColor = vec4(result, 1.0);
					}
					else
						 {
						 FragColor = my_color;
						 }
				 }
		 }	    
	}
} 