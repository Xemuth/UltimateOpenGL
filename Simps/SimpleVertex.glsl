#ifndef _UltimateOpenGL_SIMPLE_VERTEX
#define _UltimateOpenGL_SIMPLE_VERTEX
SHADER(330 core,
	/*
		This Shader is used to Generate automatique Shader using UltimateOpenGL.
		\n It is not elegant but necessary because Annotation(@) will be at reading transform to
		Commentaire bracket(//) Allowing the compilation of annotation not used by GLSL
		compiler.
	*/
	@STRUCTURE\n
	@LAYOUT_VARIABLE\n
	@OUT_VARIABLE\n
	@UNIFORM\n
	@FUNCTION_PROTOTYPE\n

	void main()\n
	{\n
		@CUSTOM_MAIN\n
	}\n
	
	@FUNCTION_DEFINITION\n
)
#endif