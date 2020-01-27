#ifndef _UltimateOpenGL_SIMPLE_VERTEX
#define _UltimateOpenGL_SIMPLE_VERTEX
SHADER(330 core,
	@LAYOUT_POSITION\n
	@LAYOUT_NORMAL\n
	@LAYOUT_TEXTURE_COORD\n
	@LAYOUT_TANGEANT\n
	@LAYOUT_BITANGEANT\n
	@LAYOUT_MODEL_MATRIX\n
	@LAYOUT_COLOR\n
	@LAYOUT_TEXTURE\n
	\n
	uniform mat4 model;\n
	uniform mat4 view;\n
	uniform mat4 projection;\n
	\n
	void main()\n
	{\n
		gl_Position = projection * view * model * vec4(aPos, 1.0f);\n
	}\n
)
#endif