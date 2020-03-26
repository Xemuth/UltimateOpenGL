#include "Camera.h"
#include "UltimateOpenGL.h"
#include "Scene.h"
//Camera CLASS
Camera::Camera(){}
Camera::Camera(Camera& camera){
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
}
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
		LOG("ERROR camera named :" + Name +" in Camera::SetMaxFOV(float) the float passed is equal or lower to MinFOV(" + Upp::AsString(MinFOV) +"), IT CANT BE Possible. MaxFOV Have been set to MinFOV + 10.0f");
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
		LOG("ERROR camera named :" + Name +" in Camera::SetMinFOV(float) the float passed is equal or Higher to MaxFOV(" + Upp::AsString(MaxFOV) +"), IT CANT BE Possible. MinFOV Have been set to MaxFOV - 10.0f");
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
		LOG("ERROR camera named :" + Name +" in Camera::SetDrawDisanceMax(float) the float passed is equal or lower to DrawDistanceMin(" + Upp::AsString(DrawDistanceMin) +"), IT CANT BE Possible. DrawDistanceMax Have been set to DrawDistanceMin + 100.0f");
		DrawDisanceMax = DrawDistanceMin + 100.0f;
		
	}else if(value <= DrawDistanceMin){
		LOG("ERROR camera named :" + Name +" in Camera::SetDrawDisanceMax(float) the float passed is equal or lower to DrawDistanceMin(" + Upp::AsString(DrawDistanceMin) +"), IT CANT BE Possible. DrawDisanceMax Have been set to DrawDistanceMin + 100.0f");
		DrawDisanceMax = DrawDistanceMin + 100.0f;
	}else{
		DrawDisanceMax = value;
	}
	return *this;
}
float Camera::GetDrawDisanceMax()const{return DrawDisanceMax;}
Camera& Camera::SetDrawDistanceMin(float value){
	if(value <= 0){
		LOG("ERROR camera named :" + Name +" in Camera::SetDrawDistanceMin(float) the float passed is equal or lower to 0, IT CANT BE Possible. DrawDistanceMin Have been set to 0.001f");
		DrawDistanceMin = 0.001f;
	}else if(value >= DrawDisanceMax){
		LOG("ERROR camera named :" + Name +" in Camera::SetDrawDistanceMin(float) the float passed is equal or Higher to DrawDisanceMax(" + Upp::AsString(DrawDisanceMax) +"), IT CANT BE Possible. DrawDistanceMin Have been set to 0.001f");
		DrawDistanceMin = 0.001f;
	}else{
		DrawDistanceMin = value;
	}
	return *this;
}
float Camera::GetDrawDistanceMin()const{return DrawDistanceMin;}

glm::mat4 Camera::GetProjectionMatrix(Upp::Sizef ScreenSize){
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
Camera& Camera::SetPosition(glm::vec3 const& position){
	transform.SetNewPosition(position);
	return *this;
}
Camera& Camera::LookAt(glm::vec3 const& lookTo){
	transform.LookAt(lookTo);
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
//CameraQuaterion CLASS

CameraQuaterion::CameraQuaterion(){} //be carefull of setting scene correctly
CameraQuaterion::CameraQuaterion(Scene& _scene){
	scene = &_scene;
}
CameraQuaterion::CameraQuaterion(Scene& _scene,const Upp::String& _name){
	scene = &_scene;
	Name = _name;
}
CameraQuaterion::CameraQuaterion(CameraQuaterion& cameraQuaterion) : Camera(cameraQuaterion){
	MouvementSpeed = cameraQuaterion.MouvementSpeed;
	MouseSensitivity = cameraQuaterion.MouseSensitivity;
}
CameraQuaterion& CameraQuaterion::operator=(CameraQuaterion& cameraQuaterion){
	Camera::operator=(cameraQuaterion);
	MouvementSpeed = cameraQuaterion.MouvementSpeed;
	MouseSensitivity = cameraQuaterion.MouseSensitivity;
	return *this;
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
glm::mat4 CameraQuaterion::GetViewMatrix(){
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

//CameraEuler CLASS
CameraEuler::CameraEuler(){transform.UpdateByEuler(Pitch,Yaw,Roll);} //Be carefull of setting scene correctly
CameraEuler::CameraEuler(Scene& _scene){scene = &_scene;transform.UpdateByEuler(Pitch,Yaw,Roll);}
CameraEuler::CameraEuler(Scene& _scene,const Upp::String& _name){scene = &_scene;Name = _name;transform.UpdateByEuler(Pitch,Yaw,Roll);}
CameraEuler::CameraEuler(CameraEuler& cameraEuler) : Camera(cameraEuler){
	MouseSensitivity = cameraEuler.MouseSensitivity;
	MovementSpeed = cameraEuler.MovementSpeed;
	
	Yaw = cameraEuler.Yaw;
	Pitch = cameraEuler.Pitch;
	Roll = cameraEuler.Roll;
	
	MinPitch = cameraEuler.MinPitch;
	MaxPitch = cameraEuler.MaxPitch;
	ActivatePitch = cameraEuler.ActivatePitch;
	ConstraintPitchEnable = cameraEuler.ConstraintPitchEnable;
	
	MinYaw = cameraEuler.MinYaw;
	MaxYaw = cameraEuler.MaxYaw;
	ActivateYaw = cameraEuler.ActivateYaw;
	ConstraintYawEnable = cameraEuler.ConstraintYawEnable;
	
	MinRoll = cameraEuler.MinRoll;
	MaxRoll = cameraEuler.MaxRoll;
	ActivateRoll = cameraEuler.ActivateRoll;
	ConstraintRollEnable = cameraEuler.ConstraintRollEnable;
	transform.UpdateByEuler(Pitch,Yaw,Roll);
}
CameraEuler& CameraEuler::operator=(CameraEuler& cameraEuler){
	Camera::operator=(cameraEuler);
	MouseSensitivity = cameraEuler.MouseSensitivity;
	MovementSpeed = cameraEuler.MovementSpeed;
	Yaw = cameraEuler.Yaw;
	Pitch = cameraEuler.Pitch;
	Roll = cameraEuler.Roll;
	
	MinPitch = cameraEuler.MinPitch;
	MaxPitch = cameraEuler.MaxPitch;
	ActivatePitch = cameraEuler.ActivatePitch;
	ConstraintPitchEnable = cameraEuler.ConstraintPitchEnable;
	
	MinYaw = cameraEuler.MinYaw;
	MaxYaw = cameraEuler.MaxYaw;
	ActivateYaw = cameraEuler.ActivateYaw;
	ConstraintYawEnable = cameraEuler.ConstraintYawEnable;
	
	MinRoll = cameraEuler.MinRoll;
	MaxRoll = cameraEuler.MaxRoll;
	ActivateRoll = cameraEuler.ActivateRoll;
	ConstraintRollEnable = cameraEuler.ConstraintRollEnable;
	transform.UpdateByEuler(Pitch,Yaw,Roll);
	return *this;
}
CameraEuler* CameraEuler::Clone(){
	return new CameraEuler(*this);
}
CameraEuler& CameraEuler::SetMouseSensitivity(float value){
	MouseSensitivity = value;
	return *this;
}
float CameraEuler::GetMouseSensitivity()const{
	return MouseSensitivity;
}
CameraEuler& CameraEuler::SetMouvementSpeed(float value){
	MovementSpeed = value;
	return *this;
}
float CameraEuler::GetMouvementSpeed()const{
	return MovementSpeed;
}
CameraEuler& CameraEuler::SetYaw(float value){
	if(ActivateYaw){
		if(ConstraintYawEnable){
			if(value < MinYaw) Yaw = MinYaw;
			else if(value > MaxYaw) Yaw = MaxYaw;
			else Yaw = value;
		}else{
			Yaw = value;
		}
	}
	return *this;
}
CameraEuler& CameraEuler::SetPitch(float value){
	if(ActivatePitch){
		if(ConstraintPitchEnable){
			if(value < MinPitch) Pitch = MinPitch;
			else if(value > MaxPitch) Pitch = MaxPitch;
			else Pitch = value;
		}else{
			Pitch = value;
		}
	}
	return *this;
}
CameraEuler& CameraEuler::SetRoll(float value){
	if(ActivateRoll){
		if(ConstraintRollEnable){
			if(value < MinRoll) Roll = MinRoll;
			else if(value > MaxRoll) Roll = MaxRoll;
			else Roll = value;
		}else{
			Roll = value;
		}
	}
	return *this;
}
float CameraEuler::GetYaw()const{
	return Yaw;
}
float CameraEuler::GetPitch()const{
	return Pitch;
}
float CameraEuler::GetRoll()const{
	return Roll;
}
CameraEuler& CameraEuler::SetMinPitch(float value){
	if(MinPitch >= MaxPitch){
		LOG("ERROR camera named :" + Name +" in CameraEuler::SetMinPitch(float) the float passed is equal or Higher to MaxPitch(" + Upp::AsString(MaxPitch) +"), IT CANT BE Possible. MinPitch Have been set to MaxPitch - 10.0f");
		MinPitch= MaxPitch -10.0f;
	}else{
		MinPitch= value;
	}
	return *this;
}
CameraEuler& CameraEuler::SetMaxPitch(float value){
	if(MaxPitch <= MinPitch){
		LOG("ERROR camera named :" + Name +" in Camera::SetMaxPitch(float) the float passed is equal or lower to MinPitch(" + Upp::AsString(MinPitch) +"), IT CANT BE Possible. MaxPitch Have been set to MinPitch + 10.0f");
		MaxPitch = MinPitch + 10.0f;
	}else{
		MaxPitch = value;
	}
	return *this;
}
CameraEuler& CameraEuler::SetMinYaw(float value){
	if(MinYaw >= MaxYaw){
		LOG("ERROR camera named :" + Name +" in CameraEuler::SetMinYaw(float) the float passed is equal or Higher to MaxYaw(" + Upp::AsString(MaxYaw) +"), IT CANT BE Possible. MinYaw Have been set to MaxYaw - 10.0f");
		MinYaw = MaxYaw -10.0f;
	}else{
		MinYaw = value;
	}
	return *this;
}
CameraEuler& CameraEuler::SetMaxYaw(float value){
	if(MaxYaw <= MinYaw){
		LOG("ERROR camera named :" + Name +" in Camera::SetMaxYaw(float) the float passed is equal or lower to MinYaw(" + Upp::AsString(MinYaw) +"), IT CANT BE Possible. MaxYaw Have been set to MinYaw + 10.0f");
		MaxYaw = MinYaw + 10.0f;
	}else{
		MaxYaw = value;
	}
	return *this;
}
CameraEuler& CameraEuler::SetMinRoll(float value){
	if(MinRoll >= MaxRoll){
		LOG("ERROR camera named :" + Name +" in CameraEuler::SetMinRoll(float) the float passed is equal or Higher to MaxRoll(" + Upp::AsString(MaxRoll) +"), IT CANT BE Possible. MinRoll Have been set to MaxRoll - 10.0f");
		MinRoll = MaxRoll -10.0f;
	}else{
		MinRoll = value;
	}
	return *this;
}
CameraEuler& CameraEuler::SetMaxRoll(float value){
		if(MaxRoll <= MinRoll){
		LOG("ERROR camera named :" + Name +" in Camera::SetMaxRoll(float) the float passed is equal or lower to MinRoll(" + Upp::AsString(MinRoll) +"), IT CANT BE Possible. MaxRoll Have been set to MinRoll + 10.0f");
		MaxRoll = MinRoll + 10.0f;
	}else{
		MaxRoll = value;
	}
	return *this;
}

float CameraEuler::GetMinPitch()const{
	return MinPitch;
}
float CameraEuler::GetMaxPitch()const{
	return MaxPitch;
}
float CameraEuler::GetMinYaw()const{
	return MinYaw;
}
float CameraEuler::GetMaxYaw()const{
	return MaxYaw;
}
float CameraEuler::GetMinRoll()const{
	return MinRoll;
}
float CameraEuler::GetMaxRoll()const{
	return MaxRoll;
}

CameraEuler& CameraEuler::EnablePitch(){
	ActivatePitch = true;
	return *this;
}
CameraEuler& CameraEuler::EnableYaw(){
	ActivateYaw = true;
	return *this;
}
CameraEuler& CameraEuler::EnableRoll(){
	ActivateRoll = true;
	return *this;
}

CameraEuler& CameraEuler::DisablePitch(){
	ActivatePitch = false;
	return *this;
}
CameraEuler& CameraEuler::DisableYaw(){
	ActivateYaw = false;
	return *this;
}
CameraEuler& CameraEuler::DisableRoll(){
	ActivateRoll = false;
	return *this;
}

bool CameraEuler::IsPitchEnable()const{
	return ActivatePitch;
}
bool CameraEuler::IsYawEnable()const{
	return ActivateYaw;
}
bool CameraEuler::IsRollchEnable()const{
	return ActivateRoll;
}

CameraEuler& CameraEuler::EnableConstraintPitch(){
	ConstraintPitchEnable = true;
	return *this;
}
CameraEuler& CameraEuler::EnableConstraintYaw(){
	ConstraintYawEnable = true;
	return *this;
}
CameraEuler& CameraEuler::EnableConstraintRoll(){
	ConstraintRollEnable = true;
	return *this;
}

CameraEuler& CameraEuler::DisableConstraintPitch(){
	ConstraintPitchEnable = false;
	return *this;
}
CameraEuler& CameraEuler::DisableConstraintYaw(){
	ConstraintYawEnable = false;
	return *this;
}
CameraEuler& CameraEuler::DisableConstraintRoll(){
	ConstraintRollEnable = true;
	return *this;
}

bool CameraEuler::IsConstraintPitchEnable()const{
	return ConstraintPitchEnable;
}
bool CameraEuler::IsConstraintYawEnable()const{
	return ConstraintYawEnable;
}
bool CameraEuler::IsConstraintRollchEnable()const{
	return ConstraintRollEnable;
}
glm::mat4 CameraEuler::GetViewMatrix(){
	return glm::lookAt(transform.GetPosition(), transform.GetPosition() + transform.GetFront(), transform.GetUp());
}

CameraEuler& CameraEuler::LookAt(glm::vec3 const& lookTo){
	//Taken from https://www.gamedev.net/forums/topic/666236-converting-axis-angles-forward-right-up-to-euler/
	transform.LookAt(lookTo);
	glm::vec3 rotation = transform.GetEulerAngleFromQuaterion();
	if(ActivatePitch)SetPitch(glm::degrees(rotation.x) );
	if(ActivateYaw)  SetYaw(glm::degrees(rotation.y) );
	if(ActivateRoll) SetRoll(glm::degrees(rotation.z) );
	transform.UpdateByEuler(Pitch,Yaw,Roll);
	return *this;
}

CameraEuler& CameraEuler::ProcessKeyboardMouvement(Camera_Movement direction){
	float velocity = MovementSpeed * GetScene().GetContext().GetDeltaTime();
	glm::vec3 position = transform.GetPosition();
	if (direction == CM_FORWARD)
	    position += transform.GetFront() * velocity;
	if (direction == CM_BACKWARD)
	    position -= transform.GetFront() * velocity;
	if (direction == CM_LEFT)
	    position -= transform.GetRight() * velocity;
	if (direction == CM_RIGHT)
	    position += transform.GetRight() * velocity;
	transform.SetNewPosition(position);
	return *this;
}
CameraEuler& CameraEuler::ProcessMouveMouvement(float xoffset, float yoffset){
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;
	SetYaw(Yaw + xoffset);
	SetPitch(Pitch + yoffset);
	transform.UpdateByEuler(Pitch,Yaw,Roll);
	return *this;
}