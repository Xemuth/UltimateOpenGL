#ifndef _UltimateOpenGL_V3_Transform_h_
#define _UltimateOpenGL_V3_Transform_h_
#include "GameObject.h"

class GameObject;

class Transform{
	private:
		GameObject* gameObject = nullptr;
		
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat quaterion = glm::quat(1.0f,0.0f,0.0f,0.0f);
		glm::quat quaterionBuffer = glm::quat(1.0f,0.0f,0.0f,0.0f);
		
		glm::vec3 scale= glm::vec3(1.0f);
		glm::mat4 modelMatrix = glm::mat4(1.0f); 
	
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		
		glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
	
		Upp::Vector<Transform*> childs; 
		
		void LaunchEvent();
	public:
		Transform();
		Transform(glm::vec3 _position);
		Transform(glm::vec3 _position,glm::quat _quaterion);
		Transform(glm::vec3 _position,glm::quat _quaterion,glm::vec3 _scale);
		Transform(const Transform& transform);
		
		Transform& operator=(const Transform& _transform);
		
		//Setter/Getter
		const glm::vec3& GetPosition()const;
		const glm::quat& GetQuaterion()const;
		const glm::quat& GetSecondQuaterion()const;
		const glm::vec3& GetScallabeMatrix()const;
		const glm::mat4 GetModelMatrixScaller()const;
		const Upp::Vector<Transform*>& GetChilds()const;

		//Childs manipulation
		void ClearChilds();
		void AddChild(Transform * _ptr);
		void AddChilds(const Upp::Vector<Transform*> _childs);
		
		void RemoveChilds(const Upp::Vector<Transform*> _childs);
		void RemoveChild(Transform * _ptr);
		int NumberOfChilds();
		
		void SetGameObject(GameObject* _gameObject);
		GameObject* GetGameObject();
		
		//Wolrd coordinate
		glm::vec3 GetFront();
		Transform& SetFront(glm::vec3 _front);
		
		glm::vec3 GetUp();
		Transform& SetUp(glm::vec3 _up);
		
		glm::vec3 GetRight();
		Transform& SetRight(glm::vec3 _right);
		
		glm::vec3 GetWorldUp();
		Transform& SetWorldUp(glm::vec3 _worldUp);
		
		
		//Position transformation
		Transform& MoveFrom(glm::vec3 from,bool updateChilds = true); //Move the position from the vec3 arg
		Transform& SetNewPosition(glm::vec3 newPose,bool updateChilds = true); //set the new position of object //It update all the child by : newPose + (old parent pose - old child pos);
		
		//Rotation transformation
		Transform& RotateFrom(glm::quat quaterion,bool updateChilds = true);
		Transform& SetNewRotation(glm::quat quaterion,bool updateChilds = true);
		Transform& RotateFromEulerAngles(float angle,glm::vec3 EulerAngles,bool updateChilds = true);
		Transform& SetNewRotationEulerAngles(float angle,glm::vec3 EulerAngles,bool updateChilds = true);
		
		//Rotation trnasformation buffer
		Transform& SecondRotateFrom(glm::quat quaterion,bool updateChilds = true);
		Transform& SecondSetNewRotation(glm::quat quaterion,bool updateChilds = true);
		Transform& SecondRotateFromEulerAngles(float angle,glm::vec3 EulerAngles,bool updateChilds = true);
		Transform& SecondSetNewRotationEulerAngles(float angle,glm::vec3 EulerAngles,bool updateChilds = true);
		
		Transform& LookAt(glm::vec3 direction,bool updateChilds = true);
		
		//Scale transformation
		Transform& ScaleFrom(glm::vec3 _scallar,bool updateChilds=true);
		Transform& ScaleNewValue(glm::vec3 _scallar,bool updateChilds=true); //we ensure the scall is made by mutiply 1.0f model matrix to new scallar
};
#endif
