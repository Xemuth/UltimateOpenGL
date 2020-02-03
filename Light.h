#ifndef _UltimateOpenGL_Light_h_
#define _UltimateOpenGL_Light_h_
#include "GameObject.h"
class Light : public GameObject
{
	protected:
		glm::vec3 ambient=glm::vec3(0.05f, 0.05f, 0.05f);
	    glm::vec3 diffuse=glm::vec3(0.8f, 0.8f, 0.8f);
	    glm::vec3 specular=glm::vec3(1.0f,1.0f,1.0f);
	    
	    Light(glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f));
		Light(Scene& _scene,GameObjectType got,glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f));
	public:
		Light(Scene& _scene);
		Light(Scene& _scene, glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f));
		Light(Light& light); //Be carefull of setting the Scene
		virtual ~Light();
				
		Light& operator=(Light& light); //Be carefull of setting the Scene
		Light& SetAmbient(glm::vec3& _ambient);
		glm::vec3 GetAmbient() const;
		Light& SetDiffuse(glm::vec3& _diffuse);
		glm::vec3 GetDiffuse() const;
		Light& SetSpecular(glm::vec3& _specular);
		glm::vec3 GetSpecular() const;
		
		Light& SetPosition(glm::vec3& _position, bool updateChildrens = true);
		Light& Move(glm::vec3& _position, bool updateChildrens = true);
		glm::vec3 GetPosition() const;
		
		virtual void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera){}
		virtual void Load(){}
};

class DirLight : public Light
{
	protected:
		glm::vec3 direction=glm::vec3(0.0f,-1.0f,0.0f);
	public:
		DirLight(Scene& _scene);
		DirLight(Scene& _scene,glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f));
		DirLight(DirLight& dirLight);
		virtual ~DirLight();
		
		DirLight& operator=(DirLight& dirLight);
		DirLight& SetDirection(glm::vec3 _direction);
		glm::vec3 GetDirection() const;
};

class SpotLight : public Light
{
	protected:
	    glm::vec3 direction=glm::vec3(0.0f,-1.0f,0.0f);
	    float cutOff=glm::cos(glm::radians(12.5f));
	    float outerCutOff=glm::cos(glm::radians(15.0f));
	    float constant=1.0f;
	    float linear=0.09f;
	    float quadratic=0.032f;
	public:
		SpotLight(Scene& _scene);
		SpotLight(Scene& _scene, glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _position=glm::vec3(0.0f,0.0f,0.0f), glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f), float _cutOff=glm::cos(glm::radians(12.5f)), float _outerCutOff=glm::cos(glm::radians(15.0f)), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		SpotLight(SpotLight& spotLight);//Be carefull of setting the Scene
		virtual ~SpotLight();
		
		SpotLight& operator=(SpotLight& spotLight);//Be carefull of setting the Scene
		SpotLight& SetDirection(glm::vec3 _direction=glm::vec3(0.0f,-1.0f,0.0f));
		glm::vec3 GetDirection() const;
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
	    float constant=1.0f;
	    float linear=0.09f;
	    float quadratic=0.032f;
	public:
		PointLight(Scene& _scene);
		PointLight(Scene& _scene,glm::vec3 _ambient=glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse=glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 _position=glm::vec3(0.0f,0.0f,0.0f), float _constant=1.0f, float _linear=0.09f, float _quadratic=0.032f);
		PointLight(PointLight& _pointLight);//Be carefull of setting the Scene
		virtual ~PointLight();
		
		PointLight& operator=(PointLight& _pointLight);//Be carefull of setting the Scene
		PointLight& SetConstant(float _constant=1.0f);
		float GetConstant() const;
		PointLight& SetLinear(float _linear=0.09f);
		float GetLinear() const;
		PointLight& SetQuadratic(float _quadratic=0.032f);
		float GetQuadratic() const;
};
#endif
