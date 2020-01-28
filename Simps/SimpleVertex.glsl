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

	layout (location = 0) in vec3 aPos;\n
	layout (location = 1) in vec3 aNorm;\n
	layout (location = 2) in vec2 aTextCoords;\n
	layout (location = 3) in vec3 aTangents;\n
	layout (location = 4) in vec3 aBiTangents;\n
	layout (location = 5) in vec4 aColors;\n
	layout (location = 6) in int aUseTextures;\n
	layout (location = 7) in int aTextures;\n
	layout (location = 8) in int aSpeculareTextures;\n
	layout (location = 9) in mat4 aMatricesModels;\n
	\n
	out vec4 inColor;\n
	\n
	uniform mat4 view;\n
	uniform mat4 projection;\n
	\n
	void main()\n
	{\n
		inColor = aColors;\n
		gl_Position = projection * view * aMatricesModels * vec4(aPos, 1.0f);\n
	}\n
)
#endif