#ifndef _UltimateOpenGL_Scene_h_
#define _UltimateOpenGL_Scene_h_
#include "Definition.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"
class Scene{
	protected:
		UltimateOpenGL_Context* context= nullptr;
		Upp::String name="default";
		bool loaded = false;
		
		Upp::ArrayMap<Upp::String,GameObject> AllGamesObjects;
		Upp::ArrayMap<Upp::String,Camera> AllCameras;
		Upp::ArrayMap<Upp::String,Light> AllLights; //Since UOGL V5, light is carried by scene !
		Camera* ActiveCamera= nullptr;

		Material SkyBox;
	public:
		Scene();
		Scene(UltimateOpenGL_Context& _context);
		Scene(UltimateOpenGL_Context& _context, const Upp::String& _name);
		Scene& operator= (Scene& _scene); //Context will be copied. So don't forget to redefine context if you wnat to use different one
		~Scene();

		UltimateOpenGL_Context& GetContext();
		Upp::String GetName();
		Material& GetSkyBox();
		Upp::ArrayMap<Upp::String,GameObject>& GetAllGameObjects();
		Upp::ArrayMap<Upp::String,Camera>& GetAllCameras();
		
		Scene& SetContext(UltimateOpenGL_Context& _context);
		Scene& SetName(const Upp::String& _name);
		Scene& SetSkyBox(Material& _skyBox);
		
		/*
		* Camera Function
		*/
		
		Camera& CreateCamera(const Upp::String& _CameraName);//if the Camera exists then it will remove it to create new one
		Camera& AddCamera(const Upp::String& _CameraName, Camera& camera); //Copying camera and give it a new name
		Camera& AddCamera(Camera& camera); //Copy the camera using name of camera
		Camera& GetCamera(const Upp::String& _CameraName);
		Camera& GetActiveCamera(); //Raise Assertion if active camera has not been set !
		Scene& SetActiveCamera(const Upp::String& _CameraName);//If name is incorrect then LOG will raise warning and active Camera will be set to the default one.
				                              //If no camera exist then Default camera will be set to NullPtr raising Assertion error on next GetActiveCamera() Call
		Scene& RemoveCamera(const Upp::String& _CameraName); //if the removed camera is the active one then activeCamera ptr will get the default camera.
											 //If no camera exist then Default camera will be set to NullPtr raising Assertion error on next GetActiveCamera() Call
		
		/*
		* Light and inheritance function
		*/
		template <class T,class... Args>
		T& CreateLight(const Upp::String& _LightName, Args&&... args){
			try{
				if(AllLights.Find(_LightName) == -1){
					return AllLights.Create<T>(_LightName,*this,_LightName,std::forward<Args>(args)...);
				}else{
					RemoveLight(_LightName);
					return AllLights.Create<T>(_LightName,*this,_LightName,std::forward<Args>(args)...);
				}
			}catch(...){
				throw UOGLException(6,"Error : T& Scene::CreateLight(...) => Error on convertion of the Light !",1);
			}
		}
		template <class T>
		T& AddLight(const Upp::String& _LightName,T& LightToAdd){
			try{
				if(AllLights.Find(_LightName) == -1){
					auto& type = AllLights.Create<T>(_LightName,LightToAdd);
					type.SetScene(*this);
					type.SetName(_LightName);
					return type;
				}else{
					RemoveLight(_LightName);
					auto& type = AllLights.Create<T>(_LightName,LightToAdd);
					type.SetScene(*this);
					type.SetName(_LightName);
					return type;
				}
			}catch(...){
				throw UOGLException(6,"Error : T& Scene::AddLight(...) => Error on convertion of the Light !",1);
			}
		}
		template <class T>
		T& GetLight(const Upp::String& _LightName){
			if(AllLights.Find(_LightName) != -1){
				try{
					return dynamic_cast<T&>(AllLights.Get(_LightName));
				}catch(...){
					throw UOGLException(6,"Error : T& Scene::GetLight(...) => Error on convertion of the Light !",1);
				}
			}
			throw UOGLException(6,"Error : T& Scene::GetLight(...) => No Light named \""+ _LightName +"\" exists !",1);
		}
		template <class T>
		bool IsLightTypeOf(const Upp::String& _LightName){
			if(AllLights.Find(_LightName) != -1){
				try{
					return (typeid(dynamic_cast<T&>(AllLights.Get(_LightName)))== typeid(T));
				}catch(...){
					return false;
				}
			}
			return false;
		}
		bool IsLightExist(const Upp::String& _LightName); //Return true if the light  exists
		Scene& RemoveLight(const Upp::String& _LightName); //Will remove light if it exist
		/*
		*	GameObject and inheritance function
		*/
		template <class T,class... Args>
		T& CreateGameObject(const Upp::String& _ObjectName, Args&&... args){//if the game object exists then it will remove it to create new one
			try{
				if(AllGamesObjects.Find(_ObjectName) ==-1){
					return AllGamesObjects.Create<T>(_ObjectName,*this,_ObjectName,std::forward<Args>(args)...);
				}else{
					RemoveGameObject(_ObjectName);
					return AllGamesObjects.Create<T>(_ObjectName,*this,_ObjectName,std::forward<Args>(args)...);
				}
			}catch(...){
				throw UOGLException(6,"Error : T& Scene::CreateGameObject(...) => Error on convertion of the game Object !",1);
			}
		}
		template <class T>
		T& AddGameObject(const Upp::String& _ObjectName,T& ObjectToAdd){//if the game object exists then it will remove it to create new one
			try{
				if(AllGamesObjects.Find(_ObjectName) ==-1){
					auto& type = (AllGamesObjects.Create<T>(_ObjectName) = ObjectToAdd);
					type.SetScene(*this);
					type.SetName(_ObjectName);
					return type;
				}else{
					RemoveGameObject(_ObjectName);
					auto& type = (AllGamesObjects.Create<T>(_ObjectName) = ObjectToAdd);
					type.SetScene(*this);
					type.SetName(_ObjectName);
					return type;
				}
			}catch(...){
				throw UOGLException(6,"Error : T& Scene::AddGameObject(...) => Error on convertion of the game Object !",1);
			}
		}
		template <class T>
		T& GetGameObject(const Upp::String& _ObjectName){//Throw exception if gameObject don't exists
			if(AllGamesObjects.Find(_ObjectName) != -1){
				try{
					return dynamic_cast<T&>(AllGamesObjects.Get(_ObjectName));
				}catch(...){
					throw UOGLException(6,"Error : T& Scene::GetGameObject(...) => Error on convertion of the game Object !",1);
				}
			}
			throw UOGLException(6,"Error : T& Scene::GetGameObject(...) => No GameObject named \""+ _ObjectName +"\" exists !",1);
		}
		template <class T>
		bool IsGameObjectIsTypeOf(const Upp::String& _ObjectName){//Return true if the game object exits and is type of template, else return false
			if(AllGamesObjects.Find(_ObjectName) != -1){
				try{
					return (typeid(dynamic_cast<T&>(AllGamesObjects.Get(_ObjectName)))== typeid(T));
				}catch(...){
					return false;
				}
			}
			return false;
		}
		bool IsGameObjectExist(const Upp::String& _ObjectName); //Return true if the game object exists
		Scene& RemoveGameObject(const Upp::String& _ObjectName); //Will remove gameObject if it exist
		
		Scene& Load();
		bool IsLoaded();
		Scene& Draw(const Upp::String& CameraToUse = "");
};
#endif
