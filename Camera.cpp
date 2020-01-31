#include "Camera.h"
#include "UltimateOpenGL.h"
#include "Scene.h"
Camera::Camera(){} //be carefull of setting scene correctly
Camera::Camera(Scene& _scene){
	scene = &_scene;
}
Camera::Camera(Scene& _scene,const Upp::String& _name){
	scene = &_scene;
	Name = _name;
}
Camera& Camera::SetScene(Scene& _scene){
	scene = &_scene;
	return *this;
}
Camera& Camera::SetName(const Upp::String& value){
	Name = value;
	return *this;
}
Camera& Camera::SetMouvementSpeed(float value){
	MouvementSpeed = value;
	return *this;
}
Camera& Camera::SetMouseSensitivity(float value){
	MouseSensitivity = value;
	return *this;
}
Camera& Camera::SetFOV(float value){
	FOV = value;
	return *this;
}
Camera& Camera::SetDrawDisanceMax(float value){
	DrawDisanceMax = value;
	return *this;
}
Camera& Camera::SetDrawDistanceMin(float value){
	DrawDistanceMin = value;
	return *this;
}
Scene& Camera::GetScene(){
	ASSERT_(scene , "Camera named \""+ Name +"\" object have a nullptr Scene object !");
	return *scene;
}
Upp::String Camera::GetName(){
	return Name;
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
float Camera::GetRealMouseSensitivity(){
	return MouseSensitivity * 0.02f;
}
Camera& Camera::SetMaxFOV(float value){
	MaxFOV = value;
	return *this;
}
Camera& Camera::SetMinFOV(float value){
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
	xoffset *= GetRealMouseSensitivity();
	yoffset *= GetRealMouseSensitivity();
	GetTransform().UpdateQuaterion(yoffset,xoffset);
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