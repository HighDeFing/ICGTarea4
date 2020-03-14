#version 330 core
out vec4 FragColor;

uniform vec4 my_color;

uniform bool texture_drawing;
uniform bool only_color;
uniform bool only_texture;
uniform bool texture_with_color;

uniform bool bspecular;
uniform bool bambient;
uniform bool bdiffuse;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos; 

uniform vec3 viewPos;
uniform vec3 lightPos; 
uniform vec3 lightColor;

uniform vec3 objectColor;
uniform vec3 pointsColor;
uniform vec3 wireColor;

uniform float intensity_ambiental;
uniform float intensity_specular;

uniform sampler2D ourTexture;

void main()
{
	
    //FragColor = my_color;
			// ambient
			vec4 Light_Textura;
			float ambientStrength = intensity_ambiental;
			vec3 ambient = ambientStrength * lightColor;
  	
			// diffuse 
			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(lightPos - FragPos);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = diff * lightColor;
    
			// specular
			float specularStrength = intensity_specular;
			vec3 viewDir = normalize(viewPos - FragPos);
			vec3 reflectDir = reflect(-lightDir, norm);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			vec3 specular = specularStrength * spec * lightColor;  
			if(bambient && bdiffuse && bspecular)
			{
			vec3 result = (ambient + diffuse + specular);
			Light_Textura = vec4(result, 1.0);
			}
			if(bambient && bdiffuse && !bspecular)
			{
			vec3 result = (ambient + diffuse);
			 Light_Textura =  vec4(result, 1.0);
			}
			if(bambient && !bdiffuse && bspecular)
			{
			vec3 result = (ambient + specular);
			 Light_Textura = vec4(result, 1.0);
			}
			if(bambient && !bdiffuse && !bspecular)
			{
			vec3 result = ambient;
			 Light_Textura = vec4(result, 1.0);
			}
			if(!bambient && bdiffuse && bspecular)
			{
			vec3 result = diffuse + specular;
			 Light_Textura = vec4(result, 1.0);
			}
			if(!bambient && bdiffuse && !bspecular)
			{
			vec3 result = diffuse;
			 Light_Textura = vec4(result, 1.0);
			}
			if(!bambient && !bdiffuse && bspecular)
			{
			vec3 result = specular;
			Light_Textura = vec4(result, 1.0);
			}
			if(!bambient && !bdiffuse && !bspecular)
			{
			vec3 result = vec3(1.0f);
			Light_Textura = vec4(result, 1.0);
			}
		if(texture_drawing)
		{
				if(texture_with_color)
				{ 
					FragColor = texture(ourTexture, TexCoord) * vec4(objectColor, 1.0) * Light_Textura;
				}
				if(only_color)
				{  
					FragColor = vec4(objectColor, 1.0) * Light_Textura;
					//FragColor = my_color;
				}
				if(only_texture)
				{
					FragColor = texture(ourTexture, TexCoord) * Light_Textura;
				}
		}
		else
		{
			vec4 result = Light_Textura * vec4(objectColor, 1.0);
			FragColor = result; 
		}
} 