#ifndef _UltimateOpenGL_Camera_h_
#define _UltimateOpenGL_Camera_h_
#include "Transform.h"

class Camera{
	protected:
		Scene* scene = nullptr;
		Upp::String Name="defaultCamera";
	
		Transform transform; //The Camera Transform object
	
		CameraType type = CT_PERSPECTIVE;
		
		CAMERA_FUNCTION OnTransform=nullptr;
		bool OnTransformEventActivate = true;
		
		float MaxFOV = 130.0f;
	    float MinFOV = 10.0f;
	    bool  LimiteFOV = true;
	    float FOV =90.0f;

	    float DrawDisanceMax = 300.0f;
		float DrawDistanceMin = 0.1f;
	public:
		Camera& operator=(Camera& camera);
		virtual Camera* Clone()=0;
		virtual ~Camera(){}
		
		Camera& SetScene(Scene& _scene);
		Scene& GetScene(); //Raise Assertion if Scene have not been set
		
		Camera& SetName(const Upp::String& _name);
		Upp::String GetName()const;
		
		Camera& SetTransform(Transform& value);
		Transform& GetTransform();
		
		Camera& SetCameraType(CameraType value);
		CameraType GetCameraType()const;
		
		Camera& SetOnTransformFunction(CAMERA_FUNCTION myFunction);
		CAMERA_FUNCTION GetOnTransformFunction()const;
		Camera& EnableTransformEvent();
		Camera& DisableTransformEvent();
		bool IsTransformEventActivated();
				
		Camera& SetFOV(float value);
		float GetFOV()const;
		Camera& SetMaxFOV(float value);
		float GetMaxFOV()const;
		Camera& SetMinFOV(float value);
		float GetMinFOV()const;
		
		Camera& EnableLimiteFOV();
		Camera& DisableLimiteFOV();
		bool IsFOVLimited()const;
		
		Camera& SetDrawDisanceMax(float value);
		float GetDrawDisanceMax()const;
		Camera& SetDrawDistanceMin(float value);
		float GetDrawDistanceMin()const;
		
		virtual glm::mat4 GetProjectionMatrix(Upp::Sizef ScreenSize)const =0;
		virtual glm::mat4 GetViewMatrix()const =0;
		
		virtual Camera& ProcessKeyboardMouvement(Camera_Movement direction)=0;
		virtual Camera& ProcessMouveMouvement(float xoffset, float yoffset)=0;
		virtual Camera& ProcessMouseScroll(float yoffset)=0;
};

class CameraQuaterion : public Camera{
	private:
	    float MouvementSpeed = 5.0f;;
	    float MouseSensitivity = 0.1f;
		float GetRealMouseSensitivity();
	public:
		CameraQuaterion(); //be carefull of setting scene correctly
		CameraQuaterion(Scene& _scene);
		CameraQuaterion(Scene& _scene,const Upp::String& _name);
		CameraQuaterion(CameraQuaterion& cameraQuaterion);
		virtual CameraQuaterion* Clone();
		
		CameraQuaterion& SetMouvementSpeed(float value = 15.0f);
		float GetMouvementSpeed();
		CameraQuaterion& SetMouseSensitivity(float value = 0.01f);
		float GetMouseSensitivity();
		
		virtual glm::mat4 GetProjectionMatrix(Upp::Sizef ScreenSize)const;
		virtual glm::mat4 GetViewMatrix()const;
		
		virtual CameraQuaterion& ProcessKeyboardMouvement(Camera_Movement direction);
		virtual CameraQuaterion& ProcessMouveMouvement(float xoffset, float yoffset);
		virtual CameraQuaterion& ProcessMouseScroll(float yoffset);
};

class CameraEuler : public Camera{
	//TODO
};

#endif
