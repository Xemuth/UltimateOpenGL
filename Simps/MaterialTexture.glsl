#define MATERIAL_TEXTURE_STRUCT() STRINGIFY(\
	struct MaterialTexture {\
	    sampler2D diffuse;\
	    sampler2D specular;\
	    int useSpecular;\
	    float shininess;\
	};\
)
