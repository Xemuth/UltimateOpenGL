#ifndef _UltimateOpenGL_Light_h_
#define _UltimateOpenGL_Light_h_
#include "GameObject.h"
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
		DirLight();
		DirLight(Scene& _scene);
		DirLight(Scene& _scene,const Upp::String& _name);
		DirLight(Scene& _scene,const Upp::String& _name,glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f));
		DirLight(Scene& _scene,const Upp::String& _name,glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f));
		DirLight(glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f));
		DirLight(glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f));
		DirLight(DirLight& dirLight);
		virtual ~DirLight();
		
		DirLight& operator=(DirLight& dirLight);
		
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
		SpotLight();
		SpotLight(Scene& _scene);
		SpotLight(Scene& _scene,const Upp::String& _name);
		SpotLight(Scene& _scene,const Upp::String& _name, glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _position=glm::vec3(0.0f,0.0f,0.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f), float _cutOff=glm::cos(glm::radians(12.5f)), float _outerCutOff=glm::cos(glm::radians(15.0f)), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		SpotLight(Scene& _scene,const Upp::String& _name, glm::vec3 _position=glm::vec3(0.0f,1.0f,0.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f), float _cutOff=glm::cos(glm::radians(12.5f)), float _outerCutOff=glm::cos(glm::radians(15.0f)), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		SpotLight(glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _position=glm::vec3(0.0f,0.0f,0.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f), float _cutOff=glm::cos(glm::radians(12.5f)), float _outerCutOff=glm::cos(glm::radians(15.0f)), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		SpotLight(glm::vec3 _position=glm::vec3(0.0f,1.0f,0.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f), float _cutOff=glm::cos(glm::radians(12.5f)), float _outerCutOff=glm::cos(glm::radians(15.0f)), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		SpotLight(SpotLight& spotLight);//Be carefull of setting the Scene
		virtual ~SpotLight();
		
		SpotLight& operator=(SpotLight& spotLight);//Be carefull of setting the Scene
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
		PointLight();
		PointLight(Scene& _scene);
		PointLight(Scene& _scene,const Upp::String& _name);
		PointLight(Scene& _scene,const Upp::String& _name,glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _position=glm::vec3(0.0f,0.0f,0.0f), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		PointLight(Scene& _scene,const Upp::String& _name,glm::vec3 _position=glm::vec3(0.0f,1.0f,0.0f), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		PointLight(glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _position=glm::vec3(0.0f,1.0f,0.0f), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		PointLight(glm::vec3 _position=glm::vec3(0.0f,1.0f,0.0f), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		PointLight(PointLight& _pointLight);//Be carefull of setting the Scene
		virtual ~PointLight();
		
		PointLight& operator=(PointLight& _pointLight);//Be carefull of setting the Scene
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
