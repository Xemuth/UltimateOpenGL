#ifndef _UltimateOpenGl_UltimateCamera_h_
#define _UltimateOpenGl_UltimateCamera_h_
#include "Definition.h"

#ifndef flagUPPGL
	#include <glad/glad.h>
#else
	#include <GLCtrl/GLCtrl.h>
#endif

class Object3D;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera : Upp::Moveable<Camera>{
	private:
		bool warning =false;
		
		Scene* scene =nullptr;
		Upp::String name;
		
	    // Calculates the front vector from the Camera's (updated) Euler Angles
	    void updateCameraVectors();
		
		// Camera Attributes
	    glm::vec3 Position;
	    glm::vec3 Front;
	    glm::vec3 Up;
	    glm::vec3 Right;
	    glm::vec3 WorldUp;
	    // Euler Angles
	    float Yaw;
	    float Pitch;
	    // Camera options
	    float MovementSpeed;
	    float MouseSensitivity;
	    float Zoom;
		
		glm::quat ActualQuaterionRoation;
		
		bool KeyBinding = false;
		bool MouseBinding = false;
		Object3D *binded = nullptr;
	public:
	    // Constructor with vectors
	    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	    // Constructor with scalar values
	    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	    
	    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	    glm::mat4 GetViewMatrix();
	    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	    void ProcessMouseScroll(float yoffset);
	    
	    void ActivateKeyBinding(bool _activate);
	    void ActivateMouseBinding(bool _activate);
	    void BindToObject(Object3D* object);
	    bool IsKeyBindingActivate();
	    bool IsMouseBindingActivate();
	    bool IsObjectBinded();
	    
	    glm::vec3 GetPosition();
	    glm::vec3 GetFront();
	    glm::vec3 GetUp();
	    glm::vec3 GetRight();
	    glm::vec3 GetWorldUp();
	    glm::quat GetActualQuaterionRoation();
	    float GetYaw();
	    float GetPitch();
	    float GetMovementSpeed();
	    float GetMouseSensitivity();
	    float GetZoom();
	    
	    void SetPosition(glm::vec3 _Position);
	    void SetFront(glm::vec3 _Front);
	    void SetUp(glm::vec3 _Up);
	    void SetRight(glm::vec3 _Right);
	    void SetWorldUp(glm::vec3 _WorldUp);
	    void SetYaw(float _Yaw);
	    void SetPitch(float _Pitch);
	    void SetMovementSpeed(float _MovementSpeed);
	    void SetMouseSensitivity(float _MouseSensitivity);
	    void SetZoom(float _Zoom);
	    
	    //Scene
	    void SetScene(Scene* _scene);
	    Scene* GetScene();
	    //Name
		Upp::String GetName();
		void SetName(const Upp::String& _name);
	    
};
#endif
