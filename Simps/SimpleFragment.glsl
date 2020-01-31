#ifndef _UltimateOpenGL_SIMPLE_FRAGMENT
#define _UltimateOpenGL_SIMPLE_FRAGMENT
SHADER(330 core,
	out vec4 FragColor;\n

	//in vec4 inColor;\n
	//in vec2 TexCoord;\n
	void main()\n
	{\n
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n
	}\n
)
#endif