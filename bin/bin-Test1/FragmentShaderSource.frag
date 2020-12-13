 #version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 

struct Light {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;  
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform samplerCube depthMap;
uniform float far_plane;


in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

float ShadowCalculation(vec3, vec3, vec3);

void main()
{
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords)); 

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); 

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	float distance = length(lightPos - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	ambient = ambient * attenuation; 
	diffuse = diffuse * attenuation;
	specular = specular * attenuation;

	float shadow = ShadowCalculation(FragPos, norm, lightDir);                      
    vec3 result = (ambient + (1.0f - shadow)*(diffuse + specular));
	FragColor = vec4(result, 1.0f);
}

float ShadowCalculation(vec3 fragPos, vec3 norm, vec3 lightDir)
{
	vec3 fragToLight = fragPos - lightPos;

    float closestDepth = texture(depthMap, fragToLight).r; 
	closestDepth *= far_plane;
	float currentDepth = length(fragToLight);
	float bias = max(0.11 * (1.0 - dot(norm, lightDir)), 0.011);

	float shadow = 0.0;
	float samples = 4.0;
	float offset  = 0.1;
	for(float x = -offset; x < offset; x += offset / (samples * 0.5))
	{
	  for(float y = -offset; y < offset; y += offset / (samples * 0.5))
	  {
	     for(float z = -offset; z < offset; z += offset / (samples * 0.5))
		 {
		     float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; 
             closestDepth *= far_plane;   // обратное преобразование из диапазона [0;1]
             if(currentDepth - bias > closestDepth)
	              shadow += 1.0;
        }
    }
}
	shadow /= (samples * samples * samples);

//	if (length(fragToLight) > far_plane)
//		shadow = 0.0f;

    return shadow;
}