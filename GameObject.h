#ifndef _UltimateOpenGL_GameObject_h_
#define _UltimateOpenGL_GameObject_h_
#include "Definition.h"
#include "Camera.h"
class GameObject{
	protected:
		Upp::String name="";
		GameObjectType type= GOT_UNKNOW;
		Scene* scene = nullptr; //we use ptr because we want be able to create object without attaching it to a scene then load it on multiple scene
		Transform transform;
		
		ACTION_FUNCTION onDraw =nullptr;
		bool onDrawEventActivate = true;
		
		ACTION_FUNCTION onTransform=nullptr;
		bool onTransformEventActivate = true;
	
		bool drawable = true;
		GameObject(GameObjectType got); //BeCarefull of setting up the scene;
	public:
		GameObject(Scene& _scene, GameObjectType got);
		GameObject(const GameObject& gameObject);
		GameObject& operator=(const GameObject& gameObject);
				
		//******************Scene part*************************
		Scene& GetScene()const;
		GameObject& SetScene(Scene& _scene);
		//******************Name part**************************
		Upp::String GetName()const;
		GameObject& SetName(const Upp::String& _name);
		//******************Type part**************************
		GameObjectType GetType();
		
		//******************Draw part**************************
		GameObject& SetOnDrawFunction(ACTION_FUNCTION myFunction);//void (*myFunction)(GameObject& myGameObject));
		ACTION_FUNCTION GetOnDrawFunction();
		GameObject& EnableDrawEvent();
		GameObject& DisableDrawEvent();
		bool IsDrawEventActivated();
		
		//******************Transform part*********************
		GameObject& SetOnTransformFunction(ACTION_FUNCTION myFunction);//void (*myFunction)(GameObject& myGameObject));
		ACTION_FUNCTION GetOnTransformFunction();
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
