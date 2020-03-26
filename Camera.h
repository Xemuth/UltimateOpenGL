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
		Camera();
		Camera(Camera& camera);
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
		
		virtual glm::mat4 GetProjectionMatrix(Upp::Sizef ScreenSize);
		virtual glm::mat4 GetViewMatrix() =0;
		
		virtual Camera& SetPosition(glm::vec3 const& position);
		virtual Camera& LookAt(glm::vec3 const& lookTo);
		
		virtual Camera& ProcessKeyboardMouvement(Camera_Movement direction)=0;
		virtual Camera& ProcessMouveMouvement(float xoffset, float yoffset)=0;
		virtual Camera& ProcessMouseScroll(float yoffset);
};



class CameraQuaterion : public Camera{
	protected:
	    float MouvementSpeed = 5.0f;
	    float MouseSensitivity = 0.1f;
		float GetRealMouseSensitivity();
	public:
		CameraQuaterion(); //be carefull of setting scene correctly
		CameraQuaterion(Scene& _scene);
		CameraQuaterion(Scene& _scene,const Upp::String& _name);
		CameraQuaterion(CameraQuaterion& cameraQuaterion);
		CameraQuaterion& operator=(CameraQuaterion& cameraQuaterion);
		virtual CameraQuaterion* Clone();
		
		CameraQuaterion& SetMouvementSpeed(float value = 15.0f);
		float GetMouvementSpeed();
		CameraQuaterion& SetMouseSensitivity(float value = 0.01f);
		float GetMouseSensitivity();
		
		//virtual glm::mat4 GetProjectionMatrix(Upp::Sizef ScreenSize)const;
		virtual glm::mat4 GetViewMatrix();
		
		virtual CameraQuaterion& ProcessKeyboardMouvement(Camera_Movement direction);
		virtual CameraQuaterion& ProcessMouveMouvement(float xoffset, float yoffset);
};

class CameraEuler : public Camera{
	protected:
		float MouseSensitivity = 0.1f;
		float MovementSpeed = 15.0f;
		
		float Yaw = -90.0f;
	    float Pitch = 0.0f;
	    float Roll = 0.0f;
	    
	    float MinPitch = -89.9f;
	    float MaxPitch = 89.9f;
	    bool ActivatePitch = true; //Activate rotation on pitch
	    bool ConstraintPitchEnable = true;
	    
	    float MinYaw = -179.9f;
	    float MaxYaw = 179.9f;
	    bool ActivateYaw = true; //Activate rotation on Yaw
	    bool ConstraintYawEnable = false;
	    
	    float MinRoll = -179.9f;
	    float MaxRoll = 179.9f;
	    bool ActivateRoll = false; //Activate rotation on Roll
	    bool ConstraintRollEnable = false;
	public:
		CameraEuler(); //Be carefull of setting scene correctly
		CameraEuler(Scene& _scene);
		CameraEuler(Scene& _scene,const Upp::String& _name);
		CameraEuler(CameraEuler& cameraEuler);
		CameraEuler& operator=(CameraEuler& cameraEuler);
		virtual CameraEuler* Clone();
		
		CameraEuler& SetMouseSensitivity(float value = 0.01f);
		float GetMouseSensitivity()const;
		CameraEuler& SetMouvementSpeed(float value = 15.0f);
		float GetMouvementSpeed()const;
		
		CameraEuler& SetYaw(float value);
		CameraEuler& SetPitch(float value);
		CameraEuler& SetRoll(float value);
		
		float GetYaw()const;
		float GetPitch()const;
		float GetRoll()const;
		
		CameraEuler& SetMinPitch(float value);
		CameraEuler& SetMaxPitch(float value);
		CameraEuler& SetMinYaw(float value);
		CameraEuler& SetMaxYaw(float value);
		CameraEuler& SetMinRoll(float value);
		CameraEuler& SetMaxRoll(float value);
		
		float GetMinPitch()const;
		float GetMaxPitch()const;
		float GetMinYaw()const;
		float GetMaxYaw()const;
		float GetMinRoll()const;
		float GetMaxRoll()const;
		
		CameraEuler& EnablePitch();
		CameraEuler& EnableYaw();
		CameraEuler& EnableRoll();
		
		CameraEuler& DisablePitch();
		CameraEuler& DisableYaw();
		CameraEuler& DisableRoll();
		
		bool IsPitchEnable()const;
		bool IsYawEnable()const;
		bool IsRollchEnable()const;
		
		CameraEuler& EnableConstraintPitch();
		CameraEuler& EnableConstraintYaw();
		CameraEuler& EnableConstraintRoll();
		
		CameraEuler& DisableConstraintPitch();
		CameraEuler& DisableConstraintYaw();
		CameraEuler& DisableConstraintRoll();
		
		bool IsConstraintPitchEnable()const;
		bool IsConstraintYawEnable()const;
		bool IsConstraintRollchEnable()const;

		virtual glm::mat4 GetViewMatrix();
		
		virtual CameraEuler& LookAt(glm::vec3 const& lookTo);
		
		virtual CameraEuler& ProcessKeyboardMouvement(Camera_Movement direction);
		virtual CameraEuler& ProcessMouveMouvement(float xoffset, float yoffset);
};

#endif
