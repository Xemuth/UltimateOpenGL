#ifndef _UltimateOpenGL_V3_VERTEX_SHADER
#define _UltimateOpenGL_V3_VERTEX_SHADER
SHADER(330 core,

//	layout (location = 0) in vec3 aPos;\n
//	layout (location = 1) in vec3 aNormal;\n

	@LAYOUT_POSITION\n
	@LAYOUT_NORMAL\n
	@LAYOUT_TEXTURE_COORD\n
	@LAYOUT_TANGEANT\n
	@LAYOUT_BITANGEANT\n
	@LAYOUT_MODEL_MATRIX\n
	
	/*
		This Shader is used to Generate automatique Shader using UltimateOpenGL.
		\n It is not elegant but necessary because Annotation(@) will be at reading transform to
		Commentaire bracket(//) Allowing the compilation of annotation not used by GLSL
		compiler.
	*/
	
	@OUT_VECTOR_TEXCOORDS\n
	out vec3 Normal;\n
	out vec3 FragPos;\n	

	//uniform mat4 transform; Seems useless to the moment
	//uniform mat4 model;\n
	@UNIFORM_MODEL\n	
	uniform mat4 view;\n
	uniform mat4 projection;\n
	
	void main()\n
	{\n
		@FRAGPOS_CALCULATION\n
		@NORMAL_CALCULATION\n
		@POSITION_CALCULATION\n
		@TEXTCOORDS_CALCULATION\n
	}\n
)
#endif