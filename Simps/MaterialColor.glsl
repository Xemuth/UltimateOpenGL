#define MATERIAL_COLOR_STRUCT() STRINGIFY(\
	struct MaterialColor {\
	    vec3 ambient;\
	    vec3 diffuse;\
	    vec3 specular;\
	    float shininess;\
	    int useSpecular;\
	};\
)
