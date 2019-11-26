#include "GameObject.h"

Camera::Camera(glm::vec3 position , glm::vec3 up , float yaw, float pitch ): Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM){
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch): Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM){
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}
// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix(){
	if(binded &&(KeyBinding || MouseBinding)){
		if(KeyBinding){ 
			Position = binded->GetTransform().GetPosition();
		}
		if(MouseBinding){ 
			Front = binded->GetTransform().GetFront();
			//Position.z += 0.6f;
		}
	}else if(!warning && !binded &&(KeyBinding || MouseBinding)){
		if(MouseBinding)Upp::Cout() << "Warning : You activate Object Mouse CallBack binding but you dindn't bind something to it !\n";
    	if(KeyBinding)Upp::Cout() << "Warning : You activate Object Keybooard CallBack binding but you dindn't bind something to it !\n";
		warning = true;
	}
	return glm::lookAt(Position, Position + Front, Up);	
}
// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime){
	float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
		    Position += Front * velocity;
		if (direction == BACKWARD)
		    Position -= Front * velocity;
		if (direction == LEFT)
		    Position -= Right * velocity;
		if (direction == RIGHT)
		    Position += Right * velocity;
}
// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch){
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;
	Yaw   += xoffset;
	Pitch += yoffset;
	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
	    if (Pitch > 89.0f)
	        Pitch = 89.0f;
	    if (Pitch < -89.0f)
	        Pitch = -89.0f;
	}
	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}
// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset){
	if (Zoom >= 1.0f && Zoom <= 45.0f)
	    Zoom -= yoffset;
	if (Zoom <= 1.0f)
	    Zoom = 1.0f;
	if (Zoom >= 45.0f)
	    Zoom = 45.0f;
}
void Camera::updateCameraVectors(){
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	//Cout() << front.x << " , " << front.y <<" , " << front.z << "\n";
	Front = glm::normalize(front);
	ActualQuaterionRoation =glm::quat(glm::vec3(front.x, front.y, front.z));
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::ActivateKeyBinding(bool _activate){
	Upp::Cout() << "KEY BINDING BOOL"<<"\n";
	KeyBinding = _activate;
}
void Camera::ActivateMouseBinding(bool _activate){
	Upp::Cout() << "MOUSE BINDING BOOL"<<"\n";
	MouseBinding = _activate;
}

void Camera::BindToObject(Object3D *object){
	Upp::Cout() << "BINDING"<<"\n";
	binded = object;
}
bool Camera::IsKeyBindingActivate(){
	return KeyBinding;
}

bool Camera::IsMouseBindingActivate(){
	return MouseBinding;
}
bool Camera::IsObjectBinded(){
	return (binded != nullptr);
}
glm::vec3 Camera::GetPosition(){return Position;}
glm::vec3 Camera::GetFront(){return Front;}
glm::vec3 Camera::GetUp(){return Up;}
glm::vec3 Camera::GetRight(){return Right;}
glm::vec3 Camera::GetWorldUp(){return WorldUp;}
glm::quat Camera::GetActualQuaterionRoation(){return ActualQuaterionRoation;}
float Camera::GetYaw(){return Yaw;}
float Camera::GetPitch(){return Pitch;}
float Camera::GetMovementSpeed(){return MovementSpeed;}
float Camera::GetMouseSensitivity(){return MouseSensitivity;}
float Camera::GetZoom(){return Zoom;}
void Camera::SetPosition(glm::vec3 _Position){Position=_Position;}
void Camera::SetFront(glm::vec3 _Front){Front=_Front;}
void Camera::SetUp(glm::vec3 _Up){Up=_Up;}
void Camera::SetRight(glm::vec3 _Right){Right=_Right;}
void Camera::SetWorldUp(glm::vec3 _WorldUp){WorldUp=_WorldUp;}
void Camera::SetYaw(float _Yaw){Yaw=_Yaw;}
void Camera::SetPitch(float _Pitch){Pitch=_Pitch;}
void Camera::SetMovementSpeed(float _MovementSpeed){MovementSpeed=_MovementSpeed;}
void Camera::SetMouseSensitivity(float _MouseSensitivity){MouseSensitivity=_MouseSensitivity;}
void Camera::SetZoom(float _Zoom){Zoom=_Zoom;}
void Camera::SetScene(Scene* _scene){scene=_scene;}
Scene* Camera::GetScene(){return scene;}
Upp::String Camera::GetName(){return name;}
void Camera::SetName(const Upp::String& _name){name= _name;}