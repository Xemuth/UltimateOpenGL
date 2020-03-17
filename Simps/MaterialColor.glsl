#ifndef _UltimateOpenGL_MATERIAL_COLOR_SHADER
#define _UltimateOpenGL_MATERIAL_COLOR_SHADER
#define MATERIAL_COLOR_STRUCT() STRINGIFY(\
	struct MaterialColor {\
	    vec3 ambient;\
	    vec4 diffuse;\
	    vec3 specular;\
	    float shininess;\
	};\
)
#endif 