#include "Transform.h"
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
	return *this;
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
//******************Position part******************
Transform& Transform::Move(glm::vec3 move,bool updateChildrens){//Move the position from the vec3 arg
	position  += move;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Move(move,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetNewPosition(glm::vec3 newPosition,bool updateChildrens){//set the new position of object //It update all the child by : newPose + (old parent pose - old child pos);
	glm::vec3 buffer = newPosition;
	buffer +=newPosition;
	newPosition = newPosition;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Move(buffer,updateChildrens);
		}
	}
	return *this;
}
//******************Scale part*********************
Transform& Transform::Scale(glm::vec3 scallar,bool updateChildrens){
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
	quaterion =quaterion *_quaterion ;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->Rotate(_quaterion,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetNewRotation(glm::quat newQuaterion,bool updateChildrens){
	quaterion =newQuaterion;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SetNewRotation(newQuaterion,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::RotateFromEulerAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
	if(EulerAngles.x > 0){
		Rotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(1.0f,0.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.y > 0){
		Rotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,1.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.z > 0){
		Rotate(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,0.0f,1.0f)),updateChildrens);
	}
	return *this;
}
Transform& Transform::SetNewRotationEulerAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
	if(EulerAngles.x > 0){
		SetNewRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(1.0f,0.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.y > 0){
		SetNewRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,1.0f,0.0f)),updateChildrens);
	}
	if(EulerAngles.z > 0){
		SetNewRotation(glm::angleAxis(glm::radians(angleInDegree), glm::vec3(0.0f,0.0f,1.0f)),updateChildrens);
	}
	return *this;
}
//******************Second Rotation part***********
Transform& Transform::SecondRotate(glm::quat _quaterion,bool updateChildrens){
	secondQuaterion =secondQuaterion *_quaterion ;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SecondRotate(_quaterion,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SetNewSecondRotation(glm::quat newQuaterion,bool updateChildrens){
	quaterion =newQuaterion;
	if(updateChildrens){
		for(Transform* ptr1 : childrens){
			ptr1->SetNewSecondRotation(newQuaterion,updateChildrens);
		}
	}
	return *this;
}
Transform& Transform::SecondRotateEulerAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
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
Transform& Transform::SetNewSecondRotationEulerAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens){
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
Transform&  Transform::LookAt(glm::vec3 direction,bool updateChildrens){
	glm::quat buffer = RotationBetweenVectors(direction, position);
	SetNewRotation(buffer);
	if(updateChildrens){
		for(Transform* ptr1 :childrens){
			ptr1->SetNewRotation(buffer);
		}
	}
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
glm::mat4 Transform::GetViewMatrix(){
	return glm::lookAt(position,position + Front, Up);
}