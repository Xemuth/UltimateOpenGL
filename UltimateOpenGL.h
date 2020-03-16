#ifndef _UltimateOpenGL_UltimateOpenGL_h_
#define _UltimateOpenGL_UltimateOpenGL_h_
/*
	Main class of Ultimate OpenGL.
	Context is carrying a scenes set and
	give flexible way to load and unload scene
*/
#include <chrono>
#include <ctime>

#include "Definition.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"
#include "Shader.h"
#include "Light.h"
#include "Scene.h"
#include "GameObject.h"

#include "Object3D.h"
#include "Mesh.h"

class UltimateOpenGL_Context{
	private:
		/*
			Set of scenes
		*/
		Upp::ArrayMap<Upp::String,Scene> AllScenes; //all scene
		Scene* ActiveScene=nullptr; //Context must have active Scene.
		
		Upp::Sizef ScreenSize; //size of window carrying the OpenGL Context

		//Time Manageur
		/*
			Time is importante to generate random things
			or make rotation using COS or SIN function.
		*/
		Upp::Thread timeManageur;
		bool started = false; //Set to true when context is loaded
		std::chrono::time_point<std::chrono::high_resolution_clock> start,end; //High resolution clock
		double DeltaTime=0.0f,LastTime=0.0f,lastFrame =0.0f,Timer=0.0f; //Used to calculate time since behining
		int bufferFrame =0,frameCount = 0; //used to calculate FPS
		
		double GetTime();
		void StartTimer();
	
		inline static int TextureCompteur=0;
		Upp::ArrayMap<Upp::String,Material> AllMaterials;
	public:
		UltimateOpenGL_Context();
		UltimateOpenGL_Context(float ScreenHeight,float ScreenWidth);
		~UltimateOpenGL_Context();
		
		//time Management
		double GetEllapsedTime();
		double GetDeltaTime();
		int GetFPS();
		
		//ScreenManagement
		void SetScreenSize(float ScreenHeight,float ScreenWidth);
		Upp::Sizef GetScreenSize();
		
		//scenes Management
		/*
			See explications above.
			Without scene setted, Context.Draw will raise Assertion
		*/
		Scene& CreateScene(const Upp::String& name);
		Scene& GetActiveScene();
		Scene& GetScene(const Upp::String& name);
		Upp::ArrayMap<Upp::String,Scene>& GetAllScene();
		bool SetActiveScene(const Upp::String& name);
		bool RemoveScene(const Upp::String& nameOfScene);
		
		//Texture Management
		/*
			See explications above
		*/

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
		
		
		template <class T,class... Args>
		T& CreateMaterial(const Upp::String& _MaterialName, Args&&... args){
			try{
				if(AllMaterials.Find(_MaterialName) ==-1){
					return AllMaterials.Create<T>(_MaterialName,std::forward<Args>(args)...).SetName(_MaterialName);
				}else{
					RemoveMaterial(_MaterialName);
					return AllMaterials.Create<T>(_MaterialName,std::forward<Args>(args)...).SetName(_MaterialName);
				}
			}catch(...){
				throw UOGLException(6,"Error : T& UltimateOpenGL_Context::CreateMaterial(...) => Error on convertion of the Material !",1);
			}
		}
		template <class T>
		T& AddMaterial(const Upp::String& _MaterialName,T& MaterialToAdd){
			try{
				if(AllMaterials.Find(_ObjectName) ==-1){
					auto& type = (AllMaterials.Create<T>(_ObjectName) = ObjectToAdd);
					type.SetScene(*this);
					type.SetName(_ObjectName);
					return type;
				}else{
					RemoveGameObject(_ObjectName);
					auto& type = (AllMaterials.Create<T>(_ObjectName) = ObjectToAdd);
					type.SetScene(*this);
					type.SetName(_ObjectName);
					return type;
				}
			}catch(...){
				throw UOGLException(6,"Error : T& Scene::AddGameObject(...) => Error on convertion of the game Object !",1);
			}
		}
		template <class T>
		T& GetMaterial(const Upp::String& _MaterialName){
		}
		template <class T>
		bool IsMaterialTypeOf(const Upp::String& _MaterialName){
		}
		bool IsMaterialExist(const Upp::String& _MaterialName); //Return true if the light  exists
		Scene& RemoveMaterial(const Upp::String& _MaterialName); //Will remove light if it exist
		Upp::ArrayMap<Upp::String,Material>& GetAllMaterials();
		
		void Trace(bool b = true);//Log all OpenGL Error
		void Draw(const Upp::String& SceneToDraw="",const Upp::String& CameraToUse=""); //Draw active scene and active camera or specified Scene and camera to draw
		void Initialise(); //Initialise context, start timer,  setup some OpenGL Feature like anti allising or blendig
};
#endif
