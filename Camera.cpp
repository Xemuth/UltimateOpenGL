#include "Camera.h"
#include "UltimateOpenGL.h"
#include "Scene.h"
//Camera CLASS
Camera&  Camera::operator=(Camera& camera){
	scene = camera.scene;
	Name= camera.Name;
	
	transform = camera.transform; //The Camera Transform object
	
	type = camera.type;
	
	OnTransform= camera.OnTransform;
	OnTransformEventActivate = camera.OnTransformEventActivate;
	
	MaxFOV = camera.MaxFOV;
	MinFOV = camera.MinFOV;
	LimiteFOV = camera.LimiteFOV;
	FOV = camera.FOV;
	
	DrawDisanceMax = camera.DrawDisanceMax;
	DrawDistanceMin = camera.DrawDistanceMin;
	return *this;
}
Camera& Camera::SetScene(Scene& _scene){
	scene = &_scene;
	return *this;
}
Scene& Camera::GetScene(){ //Raise Assertion if Scene have not been set
	ASSERT_(scene,"ERROR Scene& GetScene() : Camera named "+ Name +" Scene have nullPtr !");
	return *scene;
}
Camera& Camera::SetName(const Upp::String& _name){
	if(_name.GetCount() > 0) Name = _name;
	return *this;
}
Upp::String Camera::GetName()const{return Name;}
Camera& Camera::SetTransform(Transform& value){
	transform = value;
	transform.SetCamera(*this);
	return *this;
}
Transform& Camera::GetTransform(){
	return transform;
}
Camera& Camera::SetCameraType(CameraType value){
	type = value;
	return *this;
}
CameraType Camera::GetCameraType()const{
	return type;
}
Camera& Camera::SetOnTransformFunction(CAMERA_FUNCTION myFunction){
	OnTransform = myFunction;
	return *this;
}
CAMERA_FUNCTION Camera::GetOnTransformFunction()const{
	return OnTransform;
}
Camera& Camera::EnableTransformEvent(){
	OnTransformEventActivate = true;
	return *this;
}
Camera& Camera::DisableTransformEvent(){
	OnTransformEventActivate = false;
	return *this;
}
bool Camera::IsTransformEventActivated(){
	return OnTransformEventActivate;
}
Camera& Camera::SetFOV(float value){
	if(LimiteFOV){
		if(value < MinFOV) FOV = MinFOV;
		else if(value > MaxFOV)FOV = MaxFOV;
		else FOV = value;
	}else{
		FOV = value;
	}
	return *this;
}
float Camera::GetFOV()const{return FOV;}
Camera& Camera::SetMaxFOV(float value){
	if(MaxFOV <= MinFOV){
		LOG("ERROR: Camera::SetMaxFOV(float) the float passed is equal or lower to MinFOV(" + Upp::AsString(MinFOV) +"), IT CANT BE Possible. MaxFOV Have been set to MinFOV + 10.0f");
		MaxFOV = MinFOV + 10.0f;
	}else{
		MaxFOV = value;
	}
	return *this;
}
float Camera::GetMaxFOV()const{return MaxFOV;}
Camera& Camera::SetMinFOV(float value){
	//You can set a negativ number here, I dont see the point but you can
	//However, it cant be superior as MaxFOV
	if(MinFOV >= MaxFOV){
		LOG("ERROR: Camera::SetMinFOV(float) the float passed is equal or Higher to MaxFOV(" + Upp::AsString(MaxFOV) +"), IT CANT BE Possible. MinFOV Have been set to MaxFOV - 10.0f");
		MinFOV = MaxFOV -10.0f;
	}else{
		MinFOV = value;
	}
	return *this;
}
float Camera::GetMinFOV()const{return MinFOV;}

Camera& Camera::EnableLimiteFOV(){LimiteFOV = true; return *this;}
Camera& Camera::DisableLimiteFOV(){LimiteFOV = false; return *this;}
bool Camera::IsFOVLimited()const{return LimiteFOV;}

Camera& Camera::SetDrawDisanceMax(float value){
	if(value <= DrawDistanceMin){
		LOG("ERROR: Camera::SetDrawDisanceMax(float) the float passed is equal or lower to DrawDistanceMin(" + Upp::AsString(DrawDistanceMin) +"), IT CANT BE Possible. DrawDistanceMax Have been set to DrawDistanceMin + 100.0f");
		DrawDisanceMax = DrawDistanceMin + 100.0f;
		
	}else if(value <= DrawDistanceMin){
		LOG("ERROR: Camera::SetDrawDisanceMax(float) the float passed is equal or lower to DrawDistanceMin(" + Upp::AsString(DrawDistanceMin) +"), IT CANT BE Possible. DrawDisanceMax Have been set to DrawDistanceMin + 100.0f");
		DrawDisanceMax = DrawDistanceMin + 100.0f;
	}else{
		DrawDisanceMax = value;
	}
	return *this;
}
float Camera::GetDrawDisanceMax()const{return DrawDisanceMax;}
Camera& Camera::SetDrawDistanceMin(float value){
	if(value <= 0){
		LOG("ERROR: Camera::SetDrawDistanceMin(float) the float passed is equal or lower to 0, IT CANT BE Possible. DrawDistanceMin Have been set to 0.001f");
		DrawDistanceMin = 0.001f;
	}else if(value >= DrawDisanceMax){
		LOG("ERROR: Camera::SetDrawDistanceMin(float) the float passed is equal or Higher to DrawDisanceMax(" + Upp::AsString(DrawDisanceMax) +"), IT CANT BE Possible. DrawDistanceMin Have been set to 0.001f");
		DrawDistanceMin = 0.001f;
	}else{
		DrawDistanceMin = value;
	}
	return *this;
}
float Camera::GetDrawDistanceMin()const{return DrawDistanceMin;}

//CameraQuaterion CLASS

CameraQuaterion::CameraQuaterion(){} //be carefull of setting scene correctly
CameraQuaterion::CameraQuaterion(Scene& _scene){
	scene = &_scene;
}
CameraQuaterion::CameraQuaterion(Scene& _scene,const Upp::String& _name){
	scene = &_scene;
	Name = _name;
}
CameraQuaterion::CameraQuaterion(CameraQuaterion& cameraQuaterion){
	static_cast<Camera&>(*this) = static_cast<Camera&>(cameraQuaterion); //WTF
	MouvementSpeed = cameraQuaterion.MouvementSpeed;
	MouseSensitivity = cameraQuaterion.MouseSensitivity;
}
CameraQuaterion* CameraQuaterion::Clone(){
	return new CameraQuaterion(*this);
}
CameraQuaterion& CameraQuaterion::SetMouvementSpeed(float value){
	MouvementSpeed = value;
	return *this;
}
CameraQuaterion& CameraQuaterion::SetMouseSensitivity(float value){
	MouseSensitivity = value;
	return *this;
}
float CameraQuaterion::GetMouvementSpeed(){
	return MouvementSpeed;
}
float CameraQuaterion::GetMouseSensitivity(){
	return MouseSensitivity;
}
float CameraQuaterion::GetRealMouseSensitivity(){
	return MouseSensitivity * 0.02f;
}
glm::mat4 CameraQuaterion::GetProjectionMatrix(Upp::Sizef ScreenSize)const{
	if(type == CT_PERSPECTIVE){
		return glm::perspective(glm::radians(GetFOV()),(float)( ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
	}else if(type == CT_ORTHOGRAPHIC){
		return glm::ortho(0.0f,(float)ScreenSize.cy, 0.0f,(float)ScreenSize.cx, GetDrawDistanceMin(),GetDrawDisanceMax());
	}else if(type == CT_FRUSTUM){
		return glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f);
	}else{
		return glm::perspective(glm::radians(GetFOV()),(float)( ScreenSize.cx / ScreenSize.cy),GetDrawDistanceMin(),GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
	}
}
glm::mat4 CameraQuaterion::GetViewMatrix()const{
	return transform.GetViewMatrix();
}

CameraQuaterion& CameraQuaterion::ProcessKeyboardMouvement(Camera_Movement direction){
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
CameraQuaterion& CameraQuaterion::ProcessMouveMouvement(float xoffset, float yoffset){
	xoffset *= GetRealMouseSensitivity();
	yoffset *= GetRealMouseSensitivity();
	GetTransform().UpdateQuaterion(yoffset,xoffset);
	return *this;
}
CameraQuaterion& CameraQuaterion::ProcessMouseScroll(float yoffset){
	if(LimiteFOV && FOV >= MinFOV && FOV <= MaxFOV){
		FOV -=yoffset;
		if(FOV <= MinFOV) FOV = MinFOV;
		if(FOV >= MaxFOV) FOV = MaxFOV;
	}else if(!LimiteFOV){
		FOV -= yoffset;
	}
	return *this;
}