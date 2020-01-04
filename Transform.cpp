#include "UltimateOpenGL.h"


Transform::Transform(){
}
Transform::Transform(glm::vec3 _position){
	position = _position;
}
Transform::Transform(glm::vec3 _position,glm::quat _quaterion){
	position = _position;
	quaterion = _quaterion;
}
Transform::Transform(glm::vec3 _position,glm::quat _quaterion,glm::vec3 _scale){
	position = _position;
	quaterion = _quaterion;
	scale = _scale;
}
Transform::Transform(const Transform& transform){
	gameObject = transform.gameObject;
		
	position = transform.position;
	quaterion = transform.quaterion;
	quaterionBuffer = transform.quaterionBuffer;
	
	scale= transform.scale;
	modelMatrix = transform.modelMatrix; 
	
	Front = transform.Front;
	Up =transform.Up;
	Right = transform.Right;
	WorldUp = transform.WorldUp;
}
Transform& Transform::operator=(const Transform& _transform){
	gameObject = _transform.gameObject;
		
	position = _transform.position;
	quaterion = _transform.quaterion;
	quaterionBuffer = _transform.quaterionBuffer;
	
	scale= _transform.scale;
	modelMatrix = _transform.modelMatrix; 
	
	Front = _transform.Front;
	Up =_transform.Up;
	Right = _transform.Right;
	WorldUp = _transform.WorldUp;
	return *this;
}

void Transform::LaunchEvent(){
	if(gameObject !=nullptr && gameObject->GetOnTransformFunction() != nullptr && gameObject->GetScene() != nullptr && gameObject->GetScene()->IsLoaded()) gameObject->GetOnTransformFunction()(*gameObject);
}
//Setter/Getter
const glm::vec3& Transform::GetPosition()const{
	return position;
}
const glm::quat& Transform::GetQuaterion()const{
	return quaterion;
}
const glm::quat& Transform::GetSecondQuaterion()const{
	return quaterionBuffer;
}
const glm::vec3& Transform::GetScallabeMatrix()const{
	return scale;
}
const glm::mat4 Transform::GetModelMatrixScaller()const{
	return modelMatrix;
}
const Upp::Vector<Transform*>& Transform::GetChilds()const{
	return childs;
}
glm::vec3 Transform::GetFront(){
	return Front;
}
Transform& Transform::SetFront(glm::vec3 _front){
	Front=  _front;
	return *this;
}
glm::vec3 Transform::GetUp(){
	return Up;
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
    return glm::quat(
        s * 0.5f, 
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );
 
}
Transform& Transform::SetUp(glm::vec3 _up){
	Up = _up;
	return *this;
}
glm::vec3 Transform::GetRight(){
	return Right;
}
Transform& Transform::SetRight(glm::vec3 _right){
	Right = _right;
	return *this;
}
Transform& Transform::LookAt(glm::vec3 direction,bool updateChilds){
	glm::quat buffer = RotationBetweenVectors(direction, position);
	SetNewRotation(buffer);
	if(updateChilds){
		for(Transform* ptr1 :childs){
			ptr1->SetNewRotation(buffer);	
		}
	}
	return *this;
}
glm::vec3 Transform::GetWorldUp(){
	return WorldUp;
}
Transform& Transform::SetWorldUp(glm::vec3 _worldUp){
	WorldUp= _worldUp;
	return *this;
}
//Childs manipulation
void Transform::ClearChilds(){
	childs.Clear();
}
void Transform::AddChild(Transform * _ptr){
	childs.Add(_ptr);
}
void Transform::AddChilds(const Upp::Vector<Transform*> _childs){
	childs.Append(_childs);
}
void Transform::RemoveChilds(const Upp::Vector<Transform*> _childs){
	int cpt = 0;
	for(const Transform* ptr1 : _childs){
		cpt= 0;
		for(const Transform* ptr2 : childs){
			if(ptr2 == 	ptr1) {
				childs.Remove(cpt,1);
				break;
			}
			cpt++;
		}
	}
}
void Transform::RemoveChild(Transform * _ptr){
	int cpt = 0;
	for(const Transform* ptr2 : childs){
		if(ptr2 == 	_ptr) {
			childs.Remove(cpt,1);
			break;
		}
		cpt++;
	}
}
int Transform::NumberOfChilds(){
	return childs.GetCount();
}

void Transform::SetGameObject(GameObject* _gameObject){
	gameObject=_gameObject;
}
GameObject* Transform::GetGameObject(){
	return gameObject;
}

//Position transformation
Transform& Transform::MoveFrom(glm::vec3 from,bool updateChilds ){//Move the position from the vec3 arg
	LaunchEvent();
	position  += from;
	if(updateChilds){
		for(Transform* ptr1 : childs){
			ptr1->MoveFrom(from,updateChilds);
		}	
	}
	return *this;
} 
Transform& Transform::SetNewPosition(glm::vec3 newPose,bool updateChilds ){//set the new position of object //It update all the child by : childpos + (old parent pose -newPose);
	LaunchEvent();
	glm::vec3 buffer = position;
	buffer +=newPose;
	position = newPose;
	
	if(updateChilds){
		for(Transform* ptr1 : childs){
			ptr1->MoveFrom(buffer,updateChilds);
		}
	}
	return *this;
} 


//Rotation transformation
Transform& Transform::RotateFrom(glm::quat _quaterion,bool updateChilds ){
	quaterion =quaterion *_quaterion ;	
	if(updateChilds){
		for(Transform* ptr1 : childs){
			ptr1->RotateFrom(_quaterion,updateChilds);
		}	
	}
	return *this;
}
Transform& Transform::SetNewRotation(glm::quat _quaterion,bool updateChilds ){
	quaterion =_quaterion;
	if(updateChilds){
		for(Transform* ptr1 : childs){
			ptr1->SetNewRotation(_quaterion,updateChilds);
		}	
	}
	return *this;
}
Transform& Transform::RotateFromEulerAngles(float angle,glm::vec3 EulerAngles,bool updateChilds ){
	if(EulerAngles.x > 0){
		RotateFrom( glm::angleAxis(angle, glm::vec3(1.0f,0.0f,0.0f)),updateChilds);
	}
	if(EulerAngles.y > 0){
		RotateFrom( glm::angleAxis(angle, glm::vec3(0.0f,1.0f,0.0f)),updateChilds);
	}
	if(EulerAngles.z > 0){
		RotateFrom( glm::angleAxis(angle, glm::vec3(0.0f,0.0f,1.0f)),updateChilds);
	}
	return *this;
}
Transform& Transform::SetNewRotationEulerAngles(float angle,glm::vec3 EulerAngles,bool updateChilds ){
	if(EulerAngles.x > 0){
		SetNewRotation( glm::angleAxis(angle, glm::vec3(1.0f,0.0f,0.0f)),updateChilds);
	}
	if(EulerAngles.y > 0){
		SetNewRotation( glm::angleAxis(angle, glm::vec3(0.0f,1.0f,0.0f)),updateChilds);
	}
	if(EulerAngles.z > 0){
		SetNewRotation( glm::angleAxis(angle, glm::vec3(0.0f,0.0f,1.0f)),updateChilds);
	}
	return *this;
}
//Rotation transformation buffer
Transform& Transform::SecondRotateFrom(glm::quat _quaterion,bool updateChilds ){
	quaterionBuffer =_quaterion *quaterionBuffer;
	if(updateChilds){
		for(Transform* ptr1 : childs){
			ptr1->SecondRotateFrom(_quaterion,updateChilds);
		}	
	}
	return *this;
}
Transform& Transform::SecondSetNewRotation(glm::quat _quaterion,bool updateChilds ){
	quaterionBuffer =_quaterion;
	if(updateChilds){
		for(Transform* ptr1 : childs){
			ptr1->SecondSetNewRotation(_quaterion,updateChilds);
		}	
	}
	return *this;
}
Transform& Transform::SecondRotateFromEulerAngles(float angle,glm::vec3 EulerAngles,bool updateChilds ){
	SecondRotateFrom( glm::angleAxis(angle, EulerAngles),updateChilds);
	return *this;
}
Transform& Transform::SecondSetNewRotationEulerAngles(float angle,glm::vec3 EulerAngles,bool updateChilds ){
	SecondSetNewRotation(glm::angleAxis(angle, EulerAngles),updateChilds);
	return *this;
}

//Scale transformation
Transform& Transform::ScaleFrom(glm::vec3 _scallar,bool updateChilds){
	scale = _scallar;
	modelMatrix =  glm::scale(modelMatrix,scale);
	if(updateChilds){
		for(Transform* ptr1 : childs){
			ptr1->ScaleFrom(_scallar,updateChilds);
		}	
	}
	return *this;
}
Transform& Transform::ScaleNewValue(glm::vec3 _scallar,bool updateChilds){ //we ensure the scall is made by mutiply 1.0f model matrix to new scallar
	scale = _scallar;
	modelMatrix =  glm::scale(glm::mat4(1.0f),scale);
	if(updateChilds){
		for(Transform* ptr1 : childs){
			ptr1->ScaleNewValue(scale,updateChilds);
		}	
	}
	return *this;
}