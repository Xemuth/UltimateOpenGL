#ifndef _UltimateOpenGL_POINTLIGHT_SHADER
#define _UltimateOpenGL_POINTLIGHT_SHADER
#define LIGHT_POINT_STRUCT()STRINGIFY(\n\
	struct PointLight {\n\
	    vec3 position;\n\
	    float constant;\n\
	    float linear;\n\
	    float quadratic;\n\
	    vec3 ambient;\n\
	    vec3 diffuse;\n\
	    vec3 specular;\n\
	};\n\
)

#define LIGHT_POINT_TEXTURE_PROTOTYPE()STRINGIFY(\n\
	vec3 CalcTexturePointLight(MaterialTexture material,PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n\
)

#define LIGHT_POINT_TEXTURE_FUNCTION()STRINGIFY(\n\
vec3 CalcTexturePointLight(MaterialTexture material,PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){\n\
	    vec3 lightDir = normalize(light.position - fragPos);\n\
	    float diff = max(dot(normal, lightDir), 0.0);\n\
	    vec3 reflectDir = reflect(-lightDir, normal);\n\
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n\
	    float distance    = length(light.position - fragPos); \n\
	    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n\
	    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));\n\
	    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));\n\
	    vec3 specular;\n\
	    if(material.useSpecular ==1) specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n\
	    ambient  *= attenuation;\n\
	    diffuse  *= attenuation;\n\
	    specular *= attenuation;\n\
	    return (ambient + diffuse + specular);\n\
	}\n\
)

#define LIGHT_POINT_COLOR_PROTOTYPE()STRINGIFY(\n\
	vec3 CalcColorPointLight(MaterialColor material,PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n\
)

#define LIGHT_POINT_COLOR_FUNCTION()STRINGIFY(\n\
vec3 CalcColorPointLight(MaterialColor material,PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){\n\
	    vec3 lightDir = normalize(light.position - fragPos);\n\
	    float diff = max(dot(normal, lightDir), 0.0);\n\
	    vec3 reflectDir = reflect(-lightDir, normal);\n\
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n\
	    float distance    = length(light.position - fragPos); \n\
	    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n\
	    vec3 ambient  = light.ambient  * material.diffuse;\n\
	    vec3 diffuse  = light.diffuse  * diff *material.diffuse;\n\
	    vec3 specular;\n\
	    if(material.useSpecular ==1) specular = light.specular * spec * material.specular;\n\
	    ambient  *= attenuation;\n\
	    diffuse  *= attenuation;\n\
	    specular *= attenuation;\n\
	    return (ambient + diffuse + specular);\n\
	}\n\
)
#endif