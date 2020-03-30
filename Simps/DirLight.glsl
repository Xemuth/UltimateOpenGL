#ifndef _UltimateOpenGL_DIRLIGHT_SHADER
#define _UltimateOpenGL_DIRLIGHT_SHADER

#define LIGHT_DIR_STRUCT()STRINGIFY(\n\
	struct DirLight {\n\
	    vec3 direction;\n\
	    vec3 ambient;\n\
	    vec3 diffuse;\n\
	    vec3 specular;\n\
	};\n\
)

#define LIGHT_DIR_TEXTURE_PROTOTYPE()STRINGIFY(\n\
	vec4 CalcTextureDirLight(MaterialTexture material,DirLight light, vec3 normal, vec3 viewDir);\n\
)

#define LIGHT_DIR_TEXTURE_FUNCTION()STRINGIFY(\n\
	vec4 CalcTextureDirLight(MaterialTexture material,DirLight light, vec3 normal, vec3 viewDir){\n\
	    vec3 lightDir = normalize(-light.direction);\n\
	    float diff = max(dot(normal, lightDir), 0.0);\n\
	    vec3 reflectDir = reflect(-lightDir, normal);\n\
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n\
	    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TextureCoordinate));\n\
	    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TextureCoordinate));\n\
	    vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoordinate));\n\
	    return vec4((ambient + diffuse + specular),1.0);\n\
	}\n\
)


#define LIGHT_DIR_COLOR_PROTOTYPE()STRINGIFY(\n\
	vec4 CalcColorDirLight(MaterialColor material,DirLight light, vec3 normal, vec3 viewDir);\n\
)

#define LIGHT_DIR_COLOR_FUNCTION()STRINGIFY(\n\
	vec4 CalcColorDirLight(MaterialColor material,DirLight light, vec3 normal, vec3 viewDir){\n\
	    vec3 lightDir = normalize(-light.direction);\n\
	    float diff = max(dot(normal, lightDir), 0.0);\n\
	    vec3 reflectDir = reflect(-lightDir, normal);\n\
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n\
	    vec4 ambient  = vec4(light.ambient,1.0)  * material.diffuse;\n\
	    vec4 diffuse  = vec4(light.diffuse,1.0)  * diff * material.diffuse;\n\
	    vec4 specular = vec4(light.specular * spec * material.specular,1.0);\n\
	    return (ambient + diffuse + specular);\n\
	}\n\
)

#endif