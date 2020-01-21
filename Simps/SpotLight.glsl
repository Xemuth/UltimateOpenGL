#ifndef _UltimateOpenGL_V3_SPOTLIGHT_SHADER
#define _UltimateOpenGL_V3_SPOTLIGHT_SHADER
#define LIGHT_SPOT_STRUCT()STRINGIFY(\n\
	struct SpotLight {\n\
	    vec3 position;\n\
	    vec3 direction;\n\
	    float cutOff;\n\
	    float outerCutOff;\n\
	    float constant;\n\
	    float linear;\n\
	    float quadratic;\n\
	    vec3 ambient;\n\
	    vec3 diffuse;\n\
	    vec3 specular;\n\
	};\n\
)
#define LIGHT_SPOT_TEXTURE_PROTOTYPE()STRINGIFY(\n\
	vec3 CalcTextureSpotLight(MaterialTexture material,SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n\
)

#define LIGHT_SPOT_TEXTURE_FUNCTION()STRINGIFY(\n\
	vec3 CalcTextureSpotLight(MaterialTexture material,SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){\n\
	    vec3 lightDir = normalize(light.position - fragPos);\n\
	    float diff = max(dot(normal, lightDir), 0.0);\n\
	    vec3 reflectDir = reflect(-lightDir, normal);\n\
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n\
	    float distance = length(light.position - fragPos);\n\
	    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n\
	    float theta = dot(lightDir, normalize(-light.direction));\n\
	    float epsilon = light.cutOff - light.outerCutOff;\n\
	    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);\n\
	    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\n\
	    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\n\
	    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n\
	    ambient *= attenuation * intensity;\n\
	    diffuse *= attenuation * intensity;\n\
	    specular *= attenuation * intensity;\n\
	    return (ambient + diffuse + specular);\n\
	}\n\
)	
#define LIGHT_SPOT_COLOR_PROTOTYPE()STRINGIFY(\n\
	vec3 CalcColorSpotLight(MaterialColor material,SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n\
)

#define LIGHT_SPOT_COLOR_FUNCTION()STRINGIFY(\n\
	vec3 CalcColorSpotLight(MaterialColor material,SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){\n\
	    vec3 lightDir = normalize(light.position - fragPos);\n\
	    float diff = max(dot(normal, lightDir), 0.0);\n\
	    vec3 reflectDir = reflect(-lightDir, normal);\n\
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n\
	    float distance = length(light.position - fragPos);\n\
	    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n\
	    float theta = dot(lightDir, normalize(-light.direction));\n\
	    float epsilon = light.cutOff - light.outerCutOff;\n\
	    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);\n\
	    vec3 ambient = light.ambient *material.diffuse;\n\
	    vec3 diffuse = light.diffuse * diff *material.diffuse;\n\
	    vec3 specular = light.specular * spec * material.specular;\n\
	    ambient *= attenuation * intensity;\n\
	    diffuse *= attenuation * intensity;\n\
	    specular *= attenuation * intensity;\n\
	    return (ambient + diffuse + specular);\n\
	}\n\
)
#endif