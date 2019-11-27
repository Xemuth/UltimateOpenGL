#ifndef _UltimateOpenGl_UltimateScene_h_
#define _UltimateOpenGl_UltimateScene_h_
#include "Definition.h"

#include "GameObject.h"
#include "UltimateOpenGL.h"
class UltimateOpenGL_Context;

class Scene : public Upp::Moveable<Scene>{
	protected:
		UltimateOpenGL_Context* context= nullptr;
		Upp::String name="";
		
		bool loaded = true;
		
		glm::vec3 BackGroundColor = glm::vec3(0.4f,0.5f,0.8f);

		Upp::ArrayMap<Upp::String,GameObject> AllGamesObjects;
		Upp::ArrayMap<Upp::String,Camera> AllCameras;
		
		Camera* ActiveCamera= nullptr;
		//Load Default light 
		void prepareDefaultLight();
		
	public:
		Scene();
		Scene(UltimateOpenGL_Context* _context_);
		~Scene();
		
		//Context
		UltimateOpenGL_Context* GetContext();
		void SetContext(UltimateOpenGL_Context* _context_);
		//Name
		Upp::String GetName();
		void SetName(const Upp::String& _name);
		
		//BackGround
		void SetBackGroundColor(glm::vec3 color);
		glm::vec3 GetBackGroundColor();
		
		
		//GameObject
		Upp::ArrayMap<Upp::String,GameObject>& GetAllGameObject();
		template <class T,class... Args>
		T& CreateGameObject(const Upp::String& Name, Args&&... args){
			T t;
			if(dynamic_cast<GameObject*>(&t)){
				if(AllGamesObjects.Find(Name) ==-1){
					T& myObject = AllGamesObjects.Create<T>(Name,std::forward<Args>(args)...);
					myObject.SetName(Name);
					myObject.SetScene(this);
					return myObject;
				}else{
					return AllGamesObjects.Get(Name);
				}
			}
			throw UGLException(6,"GameObject& Scene::CreateGameObject<type T>(String name) => Invalide type not inherited from gameObject !",1);
		}
		template <class T>
		T& AddGameObject(const Upp::String& name,T& myObject){
			if(AllGamesObjects.Find(name)==-1){
				AllGamesObjects.Add(name, myObject).SetScene(this);
				myObject.SetName(name);
				myObject.SetScene(this);
				return myObject;
			}
			return AllGamesObjects.Get(name);
		}
		template <class T>
		T& GetGameObject(const Upp::String& name){
			if(AllGamesObjects.Find(name)!=-1){
				return AllGamesObjects.Get(name);
			}
			throw UGLException(3,"GameObject& Scene::GetGameObject[String] => Unknow name",1);
		}
		bool RemoveGameObject(const Upp::String& name);
		 
	
		//Camera 
		Camera& AddCamera(const Upp::String& name);
		Camera& GetActiveCamera();
		Camera& GetCamera(const Upp::String& name);
		Upp::ArrayMap<Upp::String,Camera>& GetAllCamera();
		bool SetActiveCamera(const Upp::String& name);
		bool RemoveCamera(const Upp::String& name);
		
		
		//Lights
		int GetNumberOfDirLight();
		int GetNumberOfSpotLight();
		int GetNumberOfPointLight();
		
		//Logique
		void Load();
		bool IsLoaded();
		void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform);
};
#endif
