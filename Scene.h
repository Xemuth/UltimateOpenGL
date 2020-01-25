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
		Camera* ActiveCamera= nullptr;
		
		Shader shader;
		Material SkyBox;
	public:
		Scene();
		Scene(UltimateOpenGL_Context& _context);
		Scene(UltimateOpenGL_Context& _context, const Upp::String& _name);
		Scene& operator= (Scene& _scene); //Context will be copied. So don't forget to redefine context if you wnat to use different one
		~Scene();

		UltimateOpenGL_Context& GetContext();
		Upp::String GetName();
		Shader& GetShader();
		Material& GetSkyBox();
		Upp::ArrayMap<Upp::String,GameObject>& GetAllGameObjects();
		Upp::ArrayMap<Upp::String,Camera>& GetAllCameras();
		
		Scene& SetContext(UltimateOpenGL_Context& _context);
		Scene& SetName(const Upp::String& _name);
		Scene& SetShader(Shader& _shader);
		Scene& SetSkyBox(Material& _skyBox);
		
		Camera& CreateCamera(const Upp::String& _CameraName);//if the Camera exists then it will remove it to create new one
		Camera& AddCamera(const Upp::String& _CameraName, Camera& camera); //Copying camera and give it a new name
		Camera& AddCamera(Camera& camera); //Copy the camera using name of camera
		Camera& GetCamera(const Upp::String& _CameraName);
		Camera& GetActiveCamera(); //Raise Assertion if active camera has not been set !
		Scene& SetActiveCamera(const Upp::String& _CameraName);//If name is incorrect then LOG will raise warning and active Camera will be set to the default one.
				                              //If no camera exist then Default camera will be set to NullPtr raising Assertion error on next GetActiveCamera() Call
		Scene& RemoveCamera(const Upp::String& _CameraName); //if the removed camera is the active one then activeCamera ptr will get the default camera.
											 //If no camera exist then Default camera will be set to NullPtr raising Assertion error on next GetActiveCamera() Call
		template <class T,class... Args>
		T& CreateGameObject(const Upp::String& _ObjectName, Args&&... args);//if the game object exists then it will remove it to create new one
		template <class T>
		T& AddGameObject(const Upp::String& _ObjectName,T& ObjectToAdd); //if the game object exists then it will remove it to create new one
		template <class T>
		T& GetGameObject(const Upp::String& _ObjectName); //Throw exception if gameObject don't exists
		template <class T>
		bool IsGameObjectIsTypeOf(const Upp::String& _ObjectName); //Return true if the game object exits and is type of template, else return false
		bool IsGameObjectExist(const Upp::String& _ObjectName); //Return true if the game object exists
		Scene& RemoveGameObject(const Upp::String& _ObjectName); //Will remove gameObject if it exist
		
		Scene& Load();
		bool IsLoaded();
		Scene& Draw(const Upp::String& CameraToUse = "");
};
#endif
