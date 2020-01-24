#ifndef _UltimateOpenGL_Camera_h_
#define _UltimateOpenGL_Camera_h_
#include "Transform.h"
class Camera{
	private:
		Scene* scene = nullptr;
		Upp::String Name="default";
		
		Transform transform; //The Camera Transform object

	    float MouvementSpeed = 15.0f;;
	    float MouseSensitivity = 0.1f;
	    float FOV =90.0f;
	    
	    float MaxFOV = 130.0f;
	    float MinFOV = 10.0f;
	    bool  LimiteFOV = true;
	    
		float DrawDisanceMax = 300.0f;
		float DrawDistanceMin = 0.1f;
	public:
		Camera(); //be carefull of setting scene correctly
		Camera(Scene& _scene);
		Camera(Scene& _scene,const Upp::String& _name);
		
		Camera& SetScene(Scene& _scene);
		Camera& SetName(const Upp::String& value ="default");
		Camera& SetMouvementSpeed(float value = 15.0f);
		Camera& SetMouseSensitivity(float value = 0.01f);
		Camera& SetFOV(float value = 90.0f);
		
		Camera& SetMaxFOV(float value = 130.0f);
		Camera& SetMinFOV(float value = 10.0f);

		Camera& EnableLimiteFOV();
		Camera& DisableLimiteFOV();
		bool IsFOVLimited();
		Camera& SetDrawDisanceMax(float value = 300.0f);
		Camera& SetDrawDistanceMin(float value = 0.1f);
		
		Scene& GetScene();
		Upp::String GetName();
		Transform& GetTransform();
		float GetMouvementSpeed();
		float GetMouseSensitivity();
		float GetFOV();
		float GetDrawDisanceMax();
		float GetDrawDistanceMin();
		float GetMaxFOV();
		float GetMinFOV();
		
		Camera& ProcessKeyboardMouvement(Camera_Movement direction);
		Camera& ProcessMouveMouvement(float xoffset, float yoffset);
		Camera& ProcessMouseScroll(float yoffset);
};

#endif
