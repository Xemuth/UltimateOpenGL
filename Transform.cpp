#include "Transform.h"

#include "GameObject.h"
#include "Camera.h"
Transform::Transform(){}
Transform::Transform(const Transform& _transform){
	position = _transform.position;
	quaterion = _transform.quaterion;
	secondQuaterion = _transform.secondQuaterion;
	
	scale= _transform.scale;
	modelMatrix = _transform.modelMatrix;
	
	Front = _transform.Front;
	Up =_transform.Up;
	Right = _transform.Right;
	WorldUp = _transform.WorldUp;
	/*
	Yaw = _transform.Yaw;
	Pitch = _transform.Pitch;
	Roll = _transform.Roll;*/
}
Transform& Transform::operator=(const Transform& _transform){
	position = _transform.position;
	quaterion = _transform.quaterion;
	secondQuaterion = _transform.secondQuaterion;
	
	scale= _transform.scale;
	modelMatrix = _transform.modelMatrix;
	
	Front = _transform.Front;
	Up =_transform.Up;
	Right = _transform.Right;
	WorldUp = _transform.WorldUp;
	
	/*
	Yaw = _transform.Yaw;
	Pitch = _transform.Pitch;
	Roll = _transform.Roll;*/
	return *this;
}

void Transform::LaunchTransformEvent(){
	if(gameObject && !camera){
		 if(gameObject->GetOnTransformFunction()){
		     gameObject->GetOnTransformFunction()(*gameObject);
		 }
	}
	else if(!gameObject && camera){
		if(camera->GetOnTransformFunction()){
			camera->GetOnTransformFunction()(*camera);
		}
	}
}
Transform& Transform::SetCamera(Camera& _camera){
	if(gameObject)gameObject = nullptr;
	camera = &_camera;
	return *this;
}
Transform& Transform::SetGameObject(GameObject& _gameObject){
	if(camera) camera = nullptr;
	gameObject = &_gameObject;
	return *this;
}
Transform& Transform::EnableTransformEvent(){
	eventEnable = true;
	return *this;
}
Transform& Transform::DisableTransformEvent(){
	eventEnable = false;
	return *this;
}
bool Transform::IsTransformEventEnable(){
	return eventEnable;
}

//******************Setter/Getter*******************
Transform& Transform::SetFront(const glm::vec3& _front){
	Front = _front;
	return *this;
}
Transform& Transform::SetUp(const glm::vec3& _up){
	Up = _up;
	return *this;
}
Transform& Transform::SetRight(const glm::vec3& _right){
	Right = _right;
	return *this;
}
Transform& Transform::SetWorldUp(const glm::vec3& _worldUp){
	WorldUp = _worldUp;
	return *this;
}
const glm::vec3& Transform::GetFront()const{
	return Front;
}
const glm::vec3& Transform::GetUp()const{
	return Up;
}
const glm::vec3& Transform::GetRight()const{
	return Right;
}
const glm::vec3& Transform::GetWorldUp()const{
	return WorldUp;
}
const glm::vec3& Transform::GetPosition()const{
	return position;
}
const glm::quat& Transform::GetQuaterion()const{
	return quaterion;
}
const glm::quat& Transform::GetSecondQuaterion()const{
	return secondQuaterion;
}
const glm::vec3& Transform::GetScallabeMatrix()const{
	return scale;
}
const glm::mat4 Transform::GetModelMatrixScaller()const{
	return modelMatrix;
}
const Upp::Vector<Transform*>& Transform::GetChildrens()const{
	return childrens;
}
//******************Childs part********************
Transform& Transform::ClearChildrens(){
	childrens.Clear();
	return *this;
}
Transform& Transform::AddChildren(Transform& _ptr){
	bool trouver =false;
	for(Transform* t : childrens){
		if(t == &_ptr){
			trouver = true;
			break;
		}
	}
	if(!trouver) childrens.Add(&_ptr);
	else LOG("Warning : Transform& Transform::AddChild(Transform&)  Transform adresse " + Upp::AsString(&_ptr) +" is still children of this transform");
	return *this;
}
Transform& Transform::AddChildrens(const Upp::Vector<Transform*>& _childrens){
	bool trouver =false;
	for(Transform* t : _childrens){
		trouver =false;
		for(Transform* t2 : childrens){
			if(t == t2){
				trouver = true;
				break;
			}
		}
		if(!trouver) childrens.Add(t);
		else LOG("Warning : Transform& Transform::AddChilds(const Upp::Vector<Transform*>&)  Transform adresse " + Upp::AsString(t) +" is still children of this transform");
	}
	return *this;
}
Transform& Transform::RemoveChildrens(const Upp::Vector<Transform*>& _childrens){
	int cpt = 0;
	for(Transform* t : _childrens){
		cpt=0;
		for(Transform* t2 : childrens){
			if(t == t2){
				childrens.Remove(cpt,1);
				break;
			}
			cpt++;
		}
		if(cpt > childrens.GetCount())LOG("Warning : Transform& Transform::RemoveChilds(const Upp::Vector<Transform*>&) Transform adresse " + Upp::AsString(t) +" is not a children of this transform");
	}
	return *this;
}
Transform& Transform::RemoveChildren(Transform& _ptr){
	int cpt = 0;
	for(Transform* t : childrens){
		if(&_ptr == t){
			childrens.Remove(cpt,1);
			break;
		}
		cpt++;
	}
	if(cpt > childrens.GetCount()) LOG("Warning : Transform& Transform::RemoveChild(Transform&) Transform adresse " + Upp::AsString(&_ptr) +" is not child of this transform");
	return *this;
}
int Transform::NumberOfChildrens(){
	return childrens.GetCount();
}
/*
// Yaw Pitch Roll manipulation
Transform& Transform::EnableLimiteYaw(){
	LimiteYaw =true;
	return *this;
}
Transform& Transform::DisableLimiteYaw(){
	LimiteYaw = false;
	return *this;
}
const float Transform::GetMaxYaw()const{return MaxYaw;}
const float Transform::GetMinYaw()const{return MinYaw;}
Transform& Transform::SetMaxYaw(float _MaxYaw){
	MaxYaw = _MaxYaw;
	return *this;
}
Transform& Transform::SetMinYaw(float _MinYaw){
	MinYaw = _MinYaw;
	return *this;
}
Transform& Transform::EnableLimitePitch(){
	LimitePitch = true;
	return *this;
}
Transform& Transform::DisableLimitePitch(){
	LimitePitch = false;
	return *this;
}
const float Transform::GetMaxPitch()const{return MaxPitch;}
const float Transform::GetMinPitch()const{return MinPitch;}
Transform& Transform::SetMaxPitch(float _MaxPitch){
	MaxPitch = _MaxPitch;
	return *this;
}
Transform& Transform::SetMinPitch(float _MinPitch){
	MinPitch = _MinPitch;
	return *this;
}
Transform& Transform::EnableLimiteRoll(){
	LimiteRoll = true;
	return *this;
}
Transform& Transform::DisableLimiteRoll(){
	LimiteRoll = false;
	return *this;
}
const float Transform::GetMaxRoll()const{return MaxRoll;}
const float Transform::GetMinRoll()const{return MinRoll;}
Transform& Transform::SetMaxRoll(float _MaxRoll){
	MaxRoll = _MaxRoll;
	return *this;
}
Transform& Transform::SetMinRoll(float _MinRoll){
	MinRoll = _MinRoll;
	return *this;
}
const float Transform::GetYaw()const{return Yaw;}
const float Transform::GetPitch()const{return Pitch;}
const float Transform::GetRoll()const{return Roll;}
Transform& Transform::SetYaw(float _Yaw,bool updateChildrens){
	float newYaw = _Yaw - Yaw;
	Yaw = _Yaw;
	if(LimiteYaw){
		if(MaxYaw < Yaw) Yaw = MaxYaw;
		else if( MinYaw > Yaw) Yaw = MinYaw;
	}
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->IncreaseYaw(newYaw,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetPitch(float _Pitch,bool updateChildrens){
	float newPitch = _Pitch - Pitch;
	Pitch = _Pitch;
	if(LimitePitch){
		if(MaxPitch < Pitch) Pitch = MaxPitch;
		else if( MinPitch > Pitch) Pitch = MinPitch;
	}
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->IncreasePitch(newPitch,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetRoll(float _Roll,bool updateChildrens){
	float newRoll = _Roll - Roll;
	Roll = _Roll;
	if(LimiteRoll){
		if(MaxRoll < Roll) Roll = MaxRoll;
		else if( MinRoll > Roll) Roll = MinRoll;
	}
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->IncreaseRoll(newRoll,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::IncreaseYaw(float _Yaw,bool updateChildrens){
	Yaw += _Yaw;
	if(LimiteYaw){
		if(MaxYaw < Yaw) Yaw = MaxYaw;
		else if( MinYaw > Yaw) Yaw = MinYaw;
	}
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->IncreaseYaw(_Yaw,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::IncreasePitch(float _Pitch,bool updateChildrens){
	Pitch += _Pitch;
	if(LimitePitch){
		if(MaxPitch < Pitch) Pitch = MaxPitch;
		else if( MinPitch > Pitch) Pitch = MinPitch;
	}
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->IncreasePitch(_Pitch,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::IncreaseRoll(float _Roll,bool updateChildrens){
	Roll += _Roll;
	if(LimiteRoll){
		if(MaxRoll < Roll) Roll = MaxRoll;
		else if( MinRoll > Roll) Roll = MinRoll;
	}
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->IncreaseRoll(_Roll,updateChildrens);
		}
	}
	return *this;
}
*/
//******************Position part******************
Transform& Transform::Move(glm::vec3 move,bool updateChildrens){//Move the position from the vec3 arg
	if(eventEnable)LaunchTransformEvent();
	position  += move;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Move(move,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetNewPosition(glm::vec3 newPosition,bool updateChildrens){//set the new position of object //It update all the child by : newPose + (old parent pose - old child pos);
	if(eventEnable)LaunchTransformEvent();
	glm::vec3 buffer = position;
	buffer +=newPosition;
	position = newPosition;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Move(buffer,updateChildrens);
		}
	}
	return *this;
}
//******************Scale part*********************
Transform& Transform::Scale(glm::vec3 scallar,bool updateChildrens){
	if(eventEnable)LaunchTransformEvent();
	scale = scallar;
	modelMatrix =  glm::scale(modelMatrix,scale);
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Scale(scallar,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetNewScale(glm::vec3 newScallar,bool updateChildrens){//we ensure the scall is made by mutiply 1.0f model matrix to new scallar
	if(eventEnable)LaunchTransformEvent();
	scale = newScallar;
	modelMatrix =  glm::scale(glm::mat4(1.0f),scale);
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SetNewScale(scale,updateChildrens);
		}
	}
	return *this;
}
//******************Rotation part******************
Transform& Transform::Rotate(glm::quat _quaterion,bool updateChildrens){
	if(eventEnable)LaunchTransformEvent();
	quaterion =quaterion *_quaterion ;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Rotate(_quaterion,updateChildrens);
		}
	}
	//RecalculateEulerAngles(updateChildrens);
	return *this;
}
Transform& Transform::SetNewRotation(glm::quat newQuaterion,bool updateChildrens){
	if(eventEnable)LaunchTransformEvent();
	quaterion =newQuaterion;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SetNewRotation(newQuaterion,updateChildrens);
		}
	}
	//RecalculateEulerAngles(updateChildrens);
	return *this;
}
Transform& Transform::RotateFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
	if(eventEnable)LaunchTransformEvent();
	if(EulerAngles.x > 0){
		Rotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(1.0f,0.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.y > 0){
		Rotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,1.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.z > 0){
		Rotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,0.0f,1.0f)),updateChildrens);
	}
	//RecalculateEulerAngles(updateChildrens);
	return *this;
}
Transform& Transform::SetNewRotationFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
	if(eventEnable)LaunchTransformEvent();
	if(EulerAngles.x > 0){
		SetNewRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(1.0f,0.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.y > 0){
		SetNewRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,1.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.z > 0){
		SetNewRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,0.0f,1.0f)),updateChildrens);
	}
	//RecalculateEulerAngles(updateChildrens);
	return *this;
}
//******************Second Rotation part***********
Transform& Transform::SecondRotate(glm::quat _quaterion,bool updateChildrens){
	if(eventEnable)LaunchTransformEvent();
	secondQuaterion =secondQuaterion *_quaterion ;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SecondRotate(_quaterion,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetNewSecondRotation(glm::quat newQuaterion,bool updateChildrens){
	if(eventEnable)LaunchTransformEvent();
	quaterion =newQuaterion;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SetNewSecondRotation(newQuaterion,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SecondRotateFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
	if(eventEnable)LaunchTransformEvent();
	if(EulerAngles.x > 0){
		SecondRotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(1.0f,0.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.y > 0){
		SecondRotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,1.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.z > 0){
		SecondRotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,0.0f,1.0f)),updateChildrens);
	}
	return *this;
}
Transform& Transform::SetNewSecondRotationFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
	if(eventEnable)LaunchTransformEvent();
	if(EulerAngles.x > 0){
		SetNewSecondRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(1.0f,0.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.y > 0){
		SetNewSecondRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,1.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.z > 0){
		SetNewSecondRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,0.0f,1.0f)),updateChildrens);
	}
	return *this;
}
//******************Quick function part******************
/*
Transform& Transform::RecalculateEulerAngles(bool updateChildrens){
	glm::vec3 eulerAngles = glm::eulerAngles(quaterion);
	Yaw =eulerAngles.y;
	Pitch =eulerAngles.x;
	Roll =eulerAngles.z;
	if(LimiteYaw){
		if(MaxYaw < Yaw) Yaw = MaxYaw;
		else if( MinYaw > Yaw) Yaw = MinYaw;
	}
	if(LimitePitch){
		if(MaxPitch < Pitch) Pitch = MaxPitch;
		else if( MinPitch > Pitch) Pitch = MinPitch;
	}
	if(LimiteRoll){
		if(MaxRoll < Roll) Roll = MaxRoll;
		else if( MinRoll > Roll) Roll = MinRoll;
	}
	if(updateChildrens){
		for(Transform* ptr1 :childrens){
			ptr1->RecalculateEulerAngles(updateChildrens);
		}
	}
	return *this;
}*/
Transform& Transform::UpdateQuaterion(float Pitch, float Yaw, float Roll,bool updateChildrens){
	glm::quat key_quat = glm::quat(glm::vec3(-Pitch, Yaw, Roll));
	quaterion = key_quat * quaterion;
	quaterion = glm::normalize(quaterion);
	
	Front = glm::rotate(glm::inverse(quaterion), glm::vec3(0.0, 0.0, -1.0));
    Right = glm::rotate(glm::inverse(quaterion), glm::vec3(1.0, 0.0, 0.0));
    Up = glm::vec3(0.0, 1.0, 0.0);
	if(updateChildrens){
		for(Transform* ptr1 :childrens){
			ptr1->UpdateQuaterion(Pitch,Yaw,Roll,updateChildrens);
		}
	}
	//RecalculateEulerAngles(updateChildrens);
	return *this;
}

//Taken from https://stackoverflow.com/questions/18172388/glm-quaternion-lookat-function
glm::quat Transform::SafeQuatLookAt(glm::vec3 const& lookFrom,glm::vec3 const& lookTo,glm::vec3 const& up){
    glm::vec3  direction       = lookTo - lookFrom;
    float      directionLength = glm::length(direction);

    // Check if the direction is valid; Also deals with NaN
    if(!(directionLength > 0.0001))
        return glm::quat(1, 0, 0, 0); // Just return identity

    // Normalize direction
    direction /= directionLength;

    // Is the normal up (nearly) parallel to direction?
    return glm::quatLookAt(direction, up);
}


Transform&  Transform::LookAt(glm::vec3 const& lookTo,bool updateChildrens){
//	glm::quat buffer = RotationBetweenVectors(direction, position);
	glm::quat buffer = glm::inverse(SafeQuatLookAt(position,lookTo,Up));
	SetNewRotation(buffer,updateChildrens);
	/*if(updateChildrens){
		for(Transform* ptr1 :childrens){
			ptr1->Rotate(buffer,updateChildrens);
		}
	}*/
	return *this;
}
glm::quat Transform::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest){
    start = normalize(start);
    dest = normalize(dest);
    float cosTheta = dot(start, dest);
    glm::vec3 rotationAxis;
    if (cosTheta < -1 + 0.001f){
        // cas spécifique lorsque les vecteurs ont des directions opposées :
        // il n'y pas d'axe de rotation "idéal"
        // Donc, devinez-en un, n'importe lequel fonctionnera tant qu'il est perpendiculaire avec start
        rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
        if (glm::length2(rotationAxis) < 0.01 ) // pas de chance, ils sont parallèles, essayez encore !
            rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
        rotationAxis = normalize(rotationAxis);
        return glm::angleAxis(180.0f, rotationAxis);
    }
    rotationAxis = cross(start, dest);
    float s = sqrt( (1+cosTheta)*2 );
    float invs = 1 / s;
    return glm::quat(s*0.5f,rotationAxis.x * invs,rotationAxis.y * invs,rotationAxis.z * invs);
}
glm::mat4 Transform::GetModelMatrice()const{
	return glm::translate(glm::mat4(1.0f),position) * glm::mat4_cast(quaterion) * modelMatrix;
}
glm::mat4 Transform::GetViewMatrix()const{
	//return glm::lookAt(position,position + Front, Up);
	//temporary frame quaternion from pitch,yaw,roll
	//here roll is not used
	glm::mat4 rotate = glm::mat4_cast(quaterion);
	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate,-position);
	return rotate * translate;
}