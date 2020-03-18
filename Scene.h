#ifndef _UltimateOpenGL_Scene_h_
#define _UltimateOpenGL_Scene_h_
#include "Definition.h"
#include "Light.h"
#include "Shader.h"
#include "Material.h"
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
		
		Color_Material SkyBox;
		
	public:
		Scene();
		Scene(UltimateOpenGL_Context& _context);
		Scene(UltimateOpenGL_Context& _context, const Upp::String& _name);
		Scene& operator= (Scene& _scene); //Context will be copied. So don't forget to redefine context if you wnat to use different one
		~Scene();

		UltimateOpenGL_Context& GetContext();
		Upp::String GetName();
		Color_Material& GetSkyBox();
		Upp::ArrayMap<Upp::String,GameObject>& GetAllGameObjects();
		Upp::ArrayMap<Upp::String,Camera>& GetAllCameras();
		Upp::ArrayMap<Upp::String,Light>& GetAllLights();
		
		Scene& SetContext(UltimateOpenGL_Context& _context);
		Scene& SetName(const Upp::String& _name);
		Scene& SetSkyBox(Color_Material& _skyBox);
								 
		template <class T,class... Args>
		T& CreateCamera(const Upp::String& _CameraName, Args&&... args){//if the game object exists then it will remove it to create new one
			try{
				if(AllCameras.Find(_CameraName) ==-1){
					T& camera =  AllCameras.Create<T>(_CameraName,std::forward<Args>(args)...);
					camera.SetName(_CameraName);
					camera.SetScene(*this);
					if(!ActiveCamera) ActiveCamera = &camera;
					return camera;
				}else{
					RemoveCamera(_CameraName);
					T& camera =  AllCameras.Create<T>(_CameraName,std::forward<Args>(args)...);
					camera.SetName(_CameraName);
					camera.SetScene(*this);
					if(!ActiveCamera) ActiveCamera = &camera;
					return camera;
				}
			}catch(...){
				throw UOGLException(6,"Error : T& Scene::CreateCamera(...) => Error on convertion of the camera !",1);
			}
		}
		
		template <class T>
		T& AddCamera(const Upp::String& _CameraName,T& CameraToAdd){//if the game object exists then it will remove it to create new one
			try{
				if(AllCameras.Find(_CameraName) ==-1){
					T& type = AllCameras.Create<T>(_CameraName,CameraToAdd);
					type.SetScene(*this);
					type.SetName(_CameraName);
					if(!ActiveCamera) ActiveCamera = &type;
					return type;
				}else{
					RemoveCamera(_CameraName);
					T& type = AllCameras.Create<T>(_CameraName,CameraToAdd);
					type.SetScene(*this);
					type.SetName(_CameraName);
					if(!ActiveCamera) ActiveCamera = &type;
					return type;
				}
			}catch(...){
				throw UOGLException(6,"Error : T& Scene::AddCamera(...) => Error on convertion of the camera !",1);
			}
		}
		
		template <class T>
		T& GetCamera(const Upp::String& _CameraName){//Throw exception if gameObject don't exists
			if(AllCameras.Find(_CameraName) != -1){
				try{
					return dynamic_cast<T&>(AllCameras.Get(_CameraName));
				}catch(...){
					throw UOGLException(6,"Error : T& Scene::GetCamera(...) => Error on convertion of the Camera !",1);
				}
			}
			throw UOGLException(6,"Error : T& Scene::GetCamera(...) => No GameObject named \""+ _CameraName +"\" exists !",1);
		}
		
		template <class T>
		bool IsCameraIsTypeOf(const Upp::String& _CameraName){//Return true if the game object exits and is type of template, else return false
			if(AllCameras.Find(_CameraName) != -1){
				try{
					return (typeid(dynamic_cast<T&>(AllCameras.Get(_CameraName)))== typeid(T));
				}catch(...){
					return false;
				}
			}
			return false;
		}
								
		bool IsCameraExist(const Upp::String& _CameraName);//Return true if the camera exists
		Scene& RemoveCamera(const Upp::String& _CameraName);//Will remove camera if it exist
		Scene& SetActiveCamera(const Upp::String& _CameraName);//If name is incorrect then LOG will raise warning and active Camera will be set to the default one.
		Camera& GetActiveCamera();//Raise Assertion if active camera has not been set !
		
		/*
		*	GameObject and inheritance function
		*/
		template <class T,class... Args>
		T& CreateGameObject(const Upp::String& _ObjectName, Args&&... args){//if the game object exists then it will remove it to create new one
			try{
				if(AllGamesObjects.Find(_ObjectName) ==-1){
					T& gameObject =  AllGamesObjects.Create<T>(_ObjectName,std::forward<Args>(args)...);
					gameObject.SetScene(*this);
					gameObject.SetName(_ObjectName);
					return gameObject;
				}else{
					RemoveGameObject(_ObjectName);
					T& gameObject =  AllGamesObjects.Create<T>(_ObjectName,std::forward<Args>(args)...);
					gameObject.SetScene(*this);
					gameObject.SetName(_ObjectName);
					return gameObject;
				}
			}catch(...){
				throw UOGLException(6,"Error : T& Scene::CreateGameObject(...) => Error on convertion of the game Object !",1);
			}
		}
		
		template <class T>
		T& AddGameObject(const Upp::String& _ObjectName,T& ObjectToAdd){//if the game object exists then it will remove it to create new one
			try{
				if(AllGamesObjects.Find(_ObjectName) ==-1){
					T& gameObject  = AllGamesObjects.Create<T>(ObjectToAdd);
					gameObject.SetScene(*this);
					gameObject.SetName(_ObjectName);
					return gameObject;
				}else{
					RemoveGameObject(_ObjectName);
					T& gameObject  = AllGamesObjects.Create<T>(ObjectToAdd);
					gameObject.SetScene(*this);
					gameObject.SetName(_ObjectName);
					return gameObject;
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
		
		Scene& Load();
		bool IsLoaded();
		Scene& Draw(const Upp::String& CameraToUse = "");
};
#endif
