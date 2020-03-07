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
	/*
	layout (location = 6) in float aUseTextures;\n //Act as boolean
	layout (location = 7) in float aUseColors;\n //Act as boolean
	layout (location = 8) in float aShininess;
	layout (location = 9) in float aDiffuse;
	layout (location = 10) in vec3 aAmbient;
	layout (location = 11) in vec3 aSpeculare;
	layout (location = 12) in sampler2D aTexture;\n
	layout (location = 13) in sampler2D aSpeculareTextures;\n
	layout (location = 14) in float aTextureMix;\n
	layout (location = 15) in sampler2D aTexture2;\n
	layout (location = 16) in sampler2D aSpeculareTextures2;\n
	layout (location = 17) in float aTextureMix2;\n
	layout (location = 18) in sampler2D aTexture3;\n
	layout (location = 19) in sampler2D aSpeculareTextures3;\n
	layout (location = 20) in int aTextureMix3;\n
	*/
	layout (location = 6) in mat4 aMatricesModels;\n
	\n
	out vec4 inColor;\n
	out vec3 Normal;
	out vec3 FragPos;\n
	\n
	uniform mat4 view;\n
	uniform mat4 projection;\n
	uniform mat4 model;\n
	\n
	void main()\n
	{\n
		 inColor = aColors;\n
		//inColor = texture(aTexture, aTextCoords);\n
		//gl_Position = projection * view * model *  vec4(aPos, 1.0f);\n
		//gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n
		 FragPos = vec3(model * vec4(aPos, 1.0));
		 Normal = mat3(transpose(inverse(model))) * aNorm;
		 gl_Position = projection * view * model * vec4(aPos, 1.0f);
	}\n
)
#endif