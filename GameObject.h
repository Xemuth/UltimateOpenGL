#ifndef _UltimateOpenGL_GameObject_h_
#define _UltimateOpenGL_GameObject_h_
#include "Scene.h"

class GameObject{
	protected:
		Upp::String name="";
		// GameObjectType have been removed
		Scene* scene = nullptr; //we use ptr because we want be able to create object without attaching it to a scene then load it on multiple scene
		
		Transform transform;
		
		GAMEOBJECT_FUNCTION onDraw =nullptr;
		bool onDrawEventActivate = true;
		
		GAMEOBJECT_FUNCTION onTransform=nullptr;
		bool onTransformEventActivate = true;
	
		bool drawable = true;
		GameObject();
	public:
		GameObject(Scene& _scene);
		GameObject(const GameObject& gameObject);
		GameObject& operator=(const GameObject& gameObject);
		virtual ~GameObject();
				
		//******************Scene part*************************
		Scene& GetScene()const;
		GameObject& SetScene(Scene& _scene);
		//******************Name part**************************
		Upp::String GetName()const;
		GameObject& SetName(const Upp::String& _name);
		
		//******************Draw part**************************
		GameObject& SetOnDrawFunction(GAMEOBJECT_FUNCTION myFunction);//void (*myFunction)(GameObject& myGameObject));
		GAMEOBJECT_FUNCTION GetOnDrawFunction();
		GameObject& EnableDrawEvent();
		GameObject& DisableDrawEvent();
		bool IsDrawEventActivated();
		
		//******************Transform part*********************
		GameObject& SetOnTransformFunction(GAMEOBJECT_FUNCTION myFunction);//void (*myFunction)(GameObject& myGameObject));
		GAMEOBJECT_FUNCTION GetOnTransformFunction();
		GameObject& EnableTransformEvent();
		GameObject& DisableTransformEvent();
		bool IsTransformEventActivated();
		
		//******************Drawable part**********************
		GameObject& EnableDraw();
		GameObject& DisableDraw();
		bool IsDrawable();
		
		//******************Transform part*********************
		Transform& GetTransform();
		GameObject& SetTransform(const Transform& _transform);

		
		virtual void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera) = 0;//Force override
		virtual void Load() = 0;//Force override
};
#endif
