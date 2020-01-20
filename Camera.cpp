#include "Camera.h"
Camera::Camera(){} //be carefull of setting scene correctly
Camera::Camera(Scene& _scene){
	scene = &_scene;
}
Camera::Camera(Scene& _scene,const Upp::String& _name){
	scene = &_scene;
	name = _name;
}
Camera& Camera::SetScene(Scene& _scene){
	scene = &_scene;
	return *this;
}
Camera& Camera::SetName(const Upp::String& value ="default"){
	name = value;
	return *this;
}
Camera& Camera::SetMouvementSpeed(float value = 15.0f){
	MouvementSpeed = value;
	return *this;
}
Camera& Camera::SetMouseSensitivity(float value = 0.01f){
	MouseSensitivity = value;
	return *this;
}
Camera& Camera::SetFOV(float value = 90.0f){
	FOV = value;
	return *this;
}
Camera& Camera::SetDrawDisanceMax(float value = 300.0f){
	DrawDisanceMax = value;
	return *this;
}
Camera& Camera::SetDrawDistanceMin(float value = 0.1f){
	DrawDistanceMin = value;
	return *this;
}
Scene& Camera::GetScene(){
	if(!scene) ASSERT("Camera named \""+ name +"\" object have a nullptr Scene object !");
	return *scene;
}
Upp::String Camera::GetName(){
	return name;
}
Transform& Camera::GetTransform(){
	return transform;
}
float Camera::GetMouvementSpeed(){
	return MouvementSpeed;
}
float Camera::GetMouseSensitivity(){
	return MouseSensitivity;
}
float Camera::GetFOV(){
	return FOV;
}
float Camera::GetDrawDisanceMax(){
	return DrawDisanceMax;
}
float Camera::GetDrawDistanceMin(){
	return DrawDistanceMin;
}
Camera& Camera::SetMaxFOV(float value = 130.0f){
	MaxFOV = value;
	return *this;
}
Camera& Camera::SetMinFOV(float value = 10.0f){
	MinFOV = value;
	return *this;
}
Camera& Camera::EnableLimiteFOV(){
	LimiteFOV = true;
	return *this;
}
Camera& Camera::DisableLimiteFOV(){
	LimiteFOV = false;
	return *this;
}
bool Camera::IsFOVLimited(){
	return LimiteFOV;
}
float Camera::GetMaxFOV(){
	return MaxFOV;
}
float Camera::GetMinFOV(){
	return MinFOV;
}
Camera& Camera::ProcessKeyboardMouvement(Camera_Movement direction){
	float velocity = MouvementSpeed * GetScene().GetContext().GetDeltaTime();
	if (direction == CM_FORWARD)
		transform.Move(transform.GetFront()* velocity);
	if (direction == CM_BACKWARD)
	    transform.Move(transform.GetFront()* -velocity);
	if (direction == CM_LEFT)
	    transform.Move(transform.GetRight()* -velocity);
	if (direction == CM_RIGHT)
	    transform.Move(transform.GetRight()* velocity);
	return *this;
}
Camera& Camera::ProcessMouveMouvement(float xoffset, float yoffset){
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;
	
	glm::vec3 front;
	front.x = cos(glm::radians(xoffset)) * cos(glm::radians(yoffset));
	front.y = sin(glm::radians(yoffset));
	front.z = sin(glm::radians(xoffset)) * cos(glm::radians(yoffset));
	
	transform.Rotate(glm::quat(glm::vec3(front.x, front.y, front.z)));
	return *this;
}
Camera& Camera::ProcessMouseScroll(float yoffset){
	if(LimiteFOV && FOV >= MinFOV && FOV <= MaxFOV){
		FOV -=yoffset;
		if(FOV <= MinFOV) FOV = MinFOV;
		if(FOV >= MaxFOV) FOV = MaxFOV;
	}else if(!LimiteFOV){
		FOV -= yoffset;
	}
	return *this;
}