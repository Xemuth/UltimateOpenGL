#ifndef _UltimateOpenGL_Light_h_
#define _UltimateOpenGL_Light_h_
#include "Shader.h"
class Light
{
	protected:
		Scene* scene = nullptr;
		Upp::String name = "defaultLight";
		
		const LightType type = LT_LIGHT;
		
		LIGHT_FUNCTION onDraw =nullptr;
		bool onDrawEventActivate = true;
		
		bool drawable = true;
		
		glm::vec3 ambient=glm::vec3(0.05f, 0.05f, 0.05f);
	    glm::vec3 diffuse=glm::vec3(0.8f, 0.8f, 0.8f);
	    glm::vec3 specular=glm::vec3(1.0f,1.0f,1.0f);
	    
	public:
		virtual ~Light();
		virtual Light* Clone() =0;
		/*
			All This function is attended to be rewritted in inherrited class,
			It will allow UOGL to retrieve the good data structure and all good function
			that will allow a proper calculation of lighting during the generation of custom
			shader
		*/
		virtual const Upp::String& GetShaderDataStructure() = 0; //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual const Upp::String& GetShaderColorCalculationFunction() = 0; //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual const Upp::String& GetShaderColorPrototypeFunction() = 0;//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual const Upp::String& GetShaderTextureCalculationFunction() = 0; //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual const Upp::String& GetShaderTexturePrototypeFunction() = 0;//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		/*
			This one allow you to define how you want to send data to your shader
		*/
		virtual void SentToShader(Shader& shader,const Upp::String& CustomName = "") = 0; //This function MUST be rewritted in inherrited class, It will allow UOGL to send good data to a shader
		
		Light& SetScene(Scene& _scene);
		Scene& GetScene(); //Raise Assertion in case of no scene setted !
		
		Light& SetName(const Upp::String& _name);
		Upp::String GetName();
		
		Light& SetOnDrawFunction(LIGHT_FUNCTION myFunction);
		LIGHT_FUNCTION GetOnDrawFunction();
		Light& EnableDrawEvent();
		Light& DisableDrawEvent();
		bool IsDrawEventActivated();
		
		Light& EnableDraw();
		Light& DisableDraw();
		bool IsDrawable();
		
		const LightType GetLightType(){return type;}
		
		Light& SetAmbient(glm::vec3& _ambient);
		glm::vec3 GetAmbient() const;
		Light& SetDiffuse(glm::vec3& _diffuse);
		glm::vec3 GetDiffuse() const;
		Light& SetSpecular(glm::vec3& _specular);
		glm::vec3 GetSpecular() const;
};

class DirLight : public Light
{
	protected:
		const LightType type = LT_DIRLIGHT;
		
		glm::vec3 direction=glm::vec3(0.0f,-1.0f,0.0f);
	public:
		DirLight(Scene& _scene);
		DirLight(Scene& _scene,const Upp::String& _name);
		DirLight(Scene& _scene,const Upp::String& _name,glm::vec3 _direction);
		DirLight(glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f));
		DirLight(glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f));
		DirLight(DirLight& dirLight);
		virtual ~DirLight();
		
		DirLight& operator=(DirLight& dirLight);
		virtual DirLight* Clone();
		virtual const Upp::String& GetShaderDataStructure(); //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual const Upp::String& GetShaderColorCalculationFunction(); //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual const Upp::String& GetShaderColorPrototypeFunction();//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual const Upp::String& GetShaderTextureCalculationFunction(); //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual const Upp::String& GetShaderTexturePrototypeFunction();//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual void SentToShader(Shader& shader,const Upp::String& CustomName = "");
		
		DirLight& SetDirection(glm::vec3& _direction);
		glm::vec3 GetDirection() const;
};

class SpotLight : public Light
{
	protected:
		const LightType type = LT_SPOTLIGHT;
		glm::vec3 position=glm::vec3(0.0f,1.0f,0.0f);
	    glm::vec3 direction=glm::vec3(0.0f,-1.0f,0.0f);
	    float cutOff=glm::cos(glm::radians(12.5f));
	    float outerCutOff=glm::cos(glm::radians(15.0f));
	    float constant=1.0f;
	    float linear=0.09f;
	    float quadratic=0.032f;
	public:
		SpotLight(Scene& _scene);
		SpotLight(Scene& _scene,const Upp::String& _name);
		SpotLight(Scene& _scene,const Upp::String& _name, glm::vec3 _position, glm::vec3 _direction, float _cutOff, float _outerCutOff, float _constant, float _linear, float _quadratic);
		SpotLight(glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _position=glm::vec3(0.0f,0.0f,0.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f), float _cutOff=glm::cos(glm::radians(12.5f)), float _outerCutOff=glm::cos(glm::radians(15.0f)), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		SpotLight(glm::vec3 _position=glm::vec3(0.0f,1.0f,0.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f), float _cutOff=glm::cos(glm::radians(12.5f)), float _outerCutOff=glm::cos(glm::radians(15.0f)), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		SpotLight(SpotLight& spotLight);//Be carefull of setting the Scene
		virtual ~SpotLight();
		
		SpotLight& operator=(SpotLight& spotLight);//Be carefull of setting the Scene
		virtual SpotLight* Clone();
		virtual const Upp::String& GetShaderDataStructure(); //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual const Upp::String& GetShaderColorCalculationFunction(); //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual const Upp::String& GetShaderColorPrototypeFunction();//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual const Upp::String& GetShaderTextureCalculationFunction(); //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual const Upp::String& GetShaderTexturePrototypeFunction();//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual void SentToShader(Shader& shader,const Upp::String& CustomName = "");
		
		SpotLight& SetDirection(glm::vec3& _direction);
		glm::vec3 GetDirection() const;
		SpotLight& SetPosition(glm::vec3& _position);
		glm::vec3 GetPosition() const;
		SpotLight& SetConstant(float _constant=1.0f);
		float GetConstant() const;
		SpotLight& SetLinear(float _linear=0.09f);
		float GetLinear() const;
		SpotLight& SetQuadratic(float _quadratic=0.032f);
		float GetQuadratic() const;
		SpotLight& SetCutOff(float _cutOff=glm::cos(glm::radians(12.5f)));
		float GetCutOff() const;
		SpotLight& SetOuterCutOff(float _outerCutOff=glm::cos(glm::radians(15.0f)));
		float GetOuterCutOff() const;
};

class PointLight : public Light
{
	protected:
		const LightType type = LT_POINTLIGHT;
		
		glm::vec3 position = glm::vec3(0.0f,1.0f,0.0f);
	    float constant=1.0f;
	    float linear=0.09f;
	    float quadratic=0.032f;
	public:
		PointLight(Scene& _scene);
		PointLight(Scene& _scene,const Upp::String& _name);
		PointLight(Scene& _scene,const Upp::String& _name,glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _position, float _constant, float _linear, float _quadratic);
		PointLight(Scene& _scene,const Upp::String& _name,glm::vec3 _position=glm::vec3(0.0f,1.0f,0.0f), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		PointLight(glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _position=glm::vec3(0.0f,1.0f,0.0f), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		PointLight(glm::vec3 _position=glm::vec3(0.0f,1.0f,0.0f), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		PointLight(PointLight& _pointLight);//Be carefull of setting the Scene
		virtual ~PointLight();
		virtual PointLight* Clone();
		PointLight& operator=(PointLight& _pointLight);//Be carefull of setting the Scene
		
		virtual const Upp::String& GetShaderDataStructure(); //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual const Upp::String& GetShaderColorCalculationFunction(); //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual const Upp::String& GetShaderColorPrototypeFunction();//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual const Upp::String& GetShaderTextureCalculationFunction(); //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual const Upp::String& GetShaderTexturePrototypeFunction();//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		
		virtual void SentToShader(Shader& shader,const Upp::String& CustomName = "");
		
		PointLight& SetPosition(glm::vec3& _position);
		glm::vec3 GetPosition() const;
		PointLight& SetConstant(float _constant=1.0f);
		float GetConstant() const;
		PointLight& SetLinear(float _linear=0.09f);
		float GetLinear() const;
		PointLight& SetQuadratic(float _quadratic=0.032f);
		float GetQuadratic() const;
};
#endif
