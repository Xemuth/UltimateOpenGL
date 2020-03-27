#ifndef _UltimateOpenGL_Transform_h_
#define _UltimateOpenGL_Transform_h_
#include "Definition.h"
class Transform{
	protected:
		Upp::Vector<Transform*> childrens;

		glm::vec3 position = glm::vec3(0.0f);
		glm::quat quaterion = glm::quat(1.0f,0.0f,0.0f,0.0f);
		glm::quat secondQuaterion = glm::quat(1.0f,0.0f,0.0f,0.0f);

		glm::vec3 scale= glm::vec3(1.0f);
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		
		Camera* camera = nullptr;
		GameObject* gameObject = nullptr;
		bool eventEnable = true;
		
		void LaunchTransformEvent();
		
		/*
		float Yaw = -90.0f;
		float MaxYaw =+89.99f,MinYaw=-89.99f;
		bool LimiteYaw = false;
		
		float Pitch = 0.0f;
		float MaxPitch =89.99f,MinPitch=-89.99f;
		bool LimitePitch = true;
		
		float Roll = 0.0f;
		float MaxRoll =0.0f,MinRoll=0.0f;
		bool LimiteRoll= true;
		
		*/
	public:
		Transform();
		Transform(const Transform& _transform);
		Transform& operator=(const Transform& _transform);
		
		Transform& SetCamera(Camera& _camera);
		Transform& SetGameObject(GameObject& _gameObject);
		Transform& EnableTransformEvent();
		Transform& DisableTransformEvent();
		bool IsTransformEventEnable();
		
		//******************Setter/Getter*******************
		Transform& SetFront(const glm::vec3& _front);
		Transform& SetUp(const glm::vec3& _up);
		Transform& SetRight(const glm::vec3& _right);
		Transform& SetWorldUp(const glm::vec3& _worldUp);

		const glm::vec3& GetFront()const;
		const glm::vec3& GetUp()const;
		const glm::vec3& GetRight()const;
		const glm::vec3& GetWorldUp()const;
		const glm::vec3& GetPosition()const;
		const glm::quat& GetQuaterion()const;
		const glm::quat& GetSecondQuaterion()const;
		const glm::vec3& GetScallabeMatrix()const;
		const glm::mat4 GetModelMatrixScaller()const;
		const Upp::Vector<Transform*>& GetChildrens()const;

		//******************Childs part********************
		Transform& ClearChildrens();
		Transform& AddChildren(Transform& _ptr);
		Transform& AddChildrens(const Upp::Vector<Transform*>& _childrens);
		Transform& RemoveChildrens(const Upp::Vector<Transform*>& _childrens);
		Transform& RemoveChildren(Transform& _ptr);
		int NumberOfChildrens();


		glm::mat4 GetModelMatrice()const;
		glm::mat4 GetViewMatrix()const;

		//******************Position part******************
		Transform& Move(glm::vec3 move,bool updateChildrens = true); //Move the position from the vec3 arg
		Transform& SetNewPosition(glm::vec3 newPosition,bool updateChildrens = true);//set the new position of object //It update all the child by : newPose + (old parent pose - old child pos);

		//******************Scale part*********************
		Transform& Scale(glm::vec3 scallar,bool updateChildrens=true);
		Transform& SetNewScale(glm::vec3 newScallar,bool updateChildrens=true); //we ensure the scall is made by mutiply 1.0f model matrix to new scallar

		//******************Rotation part******************
		Transform& Rotate(glm::quat quaterion,bool updateChildrens = true);
		Transform& SetNewRotation(glm::quat newQuaterion,bool updateChildrens = true);
		Transform& RotateFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens = true);
		Transform& SetNewRotationFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens = true);

		//******************Second Rotation part***********
		Transform& SecondRotate(glm::quat _quaterion,bool updateChildrens = true);
		Transform& SetNewSecondRotation(glm::quat newQuaterion,bool updateChildrens = true);
		Transform& SecondRotateFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens = true);
		Transform& SetNewSecondRotationFromAngles(float angleInDegree,glm::vec3 EulerAngles,bool updateChildrens = true);

		//******************Quick function part*************
		Transform& LookAt(glm::vec3 const& lookTo,bool inverse = false,bool updateChildrens = true);
		Transform& ProcessMouseMouvement(float Yaw,float Pitch,float Roll,bool updateChildrens = true);
		Transform& UpdateByEuler(float Yaw,float Pitch,float Roll,bool updateChildrens = true);
		Transform& RecalculateFrontUpRight(bool updateChildrens = false);
		
		
		glm::quat QuatLookAt(glm::vec3 const& lookFrom,glm::vec3 const& lookTo,glm::vec3 const& up);
		glm::vec3 QuaterionToEuler(glm::quat const& quaterion);
		glm::quat EulerToQuaterion(float Yaw,float Pitch,float Roll);
};
#endif
