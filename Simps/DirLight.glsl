#ifndef _UltimateOpenGL_V3_DIRLIGHT_SHADER
#define _UltimateOpenGL_V3_DIRLIGHT_SHADER

#define LIGHT_DIR_STRUCT()STRINGIFY(\n\
	struct DirLight {\n\
	    vec3 direction;\n\
	    vec3 ambient;\n\
	    vec3 diffuse;\n\
	    vec3 specular;\n\
	};\n\
)

#define LIGHT_DIR_TEXTURE_PROTOTYPE()STRINGIFY(\n\
	vec3 CalcTextureDirLight(MaterialTexture material,DirLight light, vec3 normal, vec3 viewDir);\n\
)

#define LIGHT_DIR_TEXTURE_FUNCTION()STRINGIFY(\n\
	vec3 CalcTextureDirLight(MaterialTexture material,DirLight light, vec3 normal, vec3 viewDir){\n\
	    vec3 lightDir = normalize(-light.direction);\n\
	    float diff = max(dot(normal, lightDir), 0.0);\n\
	    vec3 reflectDir = reflect(-lightDir, normal);\n\
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n\
	    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));\n\
	    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));\n\
	    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));\n\
	    return (ambient + diffuse + specular);\n\
	}\n\
)


#define LIGHT_DIR_COLOR_PROTOTYPE()STRINGIFY(\n\
	vec3 CalcColorDirLight(MaterialColor material,DirLight light, vec3 normal, vec3 viewDir);\n\
)

#define LIGHT_DIR_COLOR_FUNCTION()STRINGIFY(\n\
	vec3 CalcColorDirLight(MaterialColor material,DirLight light, vec3 normal, vec3 viewDir){\n\
	    vec3 lightDir = normalize(-light.direction);\n\
	    float diff = max(dot(normal, lightDir), 0.0);\n\
	    vec3 reflectDir = reflect(-lightDir, normal);\n\
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n\
	    vec3 ambient  = light.ambient  * material.diffuse;\n\
	    vec3 diffuse  = light.diffuse  * diff * material.diffuse;\n\
	    vec3 specular = light.specular * spec * material.specular;\n\
	    return (ambient + diffuse + specular);\n\
	}\n\
)

#endif