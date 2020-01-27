#ifndef _UltimateOpenGL_SIMPLE_FRAGMENT
#define _UltimateOpenGL_SIMPLE_FRAGMENT
SHADER(330 core,
	out vec4 FragColor;\n
	in vec2 TexCoord;\n
	void main()\n
	{\n
		FragColor = vec4(0.99f, 0.72f, 0.074f,1.0f) ;\n
	}\n
)
#endif