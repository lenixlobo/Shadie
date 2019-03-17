#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	//ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

 	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = diff * lightColor;

	//specular
	float specularstr = 0.35;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectdir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectdir),0.0),64);
	vec3 specular = specularstr*spec*lightColor;


 	vec3 result = (ambient+diffuse)*objectColor;
	FragColor = vec4(result,1.0);
}