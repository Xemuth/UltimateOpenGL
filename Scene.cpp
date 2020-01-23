#include "Scene.h"

Scene::Scene(){}
Scene::Scene(UltimateOpenGL_Context& _context){
	context = &_context;
}
Scene::Scene(UltimateOpenGL_Context& _context, const Upp::String& _name){
	context = &_context;
	name = _name;
}
Scene::~Scene(){}
UltimateOpenGL_Context& Scene::GetContext(){
	ASSERT_(!context,"Scene::GetContext() Scene have nullptr Context");
	return *context;
}
Upp::String Scene::GetName(){
	return name;
}
Shader& Scene::GetShader(){
	return shader;
}
Material& Scene::GetSkyBox(){
	return SkyBox;
}
Upp::ArrayMap<Upp::String,GameObject>& Scene::GetAllGameObjects(){
	return AllGamesObjects;
}
Upp::ArrayMap<Upp::String,Camera>& Scene::GetAllCameras(){
	return AllCameras;
}
Scene& Scene::SetContext(UltimateOpenGL_Context& _context){
	context = &_context;
	return *this;
}
Scene& Scene::SetName(const Upp::String& _name){
	name = _name;
	return *this;
}
Scene& Scene::SetShader(Shader& _shader){
	shader = _shader;
	return *this;
}
Scene& Scene::SetSkyBox(Material& _skyBox){
	SkyBox = _skyBox;
	return *this;
}
Camera& Scene::AddCamera(const Upp::String& _CameraName){//if the Camera exists then it will remove it to create new one
	if(AllCameras.Find(_CameraName) ==-1){
		Camera& added = AllCameras.Create<Camera>(_CameraName,*this,_CameraName);
		if(!ActiveCamera) ActiveCamera = &added;
		return added;
	}else{
		RemoveCamera(_CameraName);
		Camera& added = AllCameras.Create<Camera>(_CameraName,*this,_CameraName);
		if(!ActiveCamera) ActiveCamera = &added;
		return added;
	}
}
Camera& Scene::GetCamera(const Upp::String& _CameraName){
	if(AllCameras.Find(_CameraName) != -1){
		return AllCameras.Get(_CameraName);
	}
	throw UOGLException(4,"Error : Camera& Scene::GetCamera(const Upp::String&) => No camera named \""+ _CameraName +"\" have been found !",1);
}
Camera& Scene::GetActiveCamera(){ //Raise Assertion if active camera has not been set !
	ASSERT_(!ActiveCamera,"Active Camera have nullptr object, be sure to Set ActiveCamere by using SetActiveCamera(const Upp::String&) or by creating the first one of the scene");
	return *ActiveCamera;
}
Scene& Scene::SetActiveCamera(const Upp::String& _CameraName){//If name is incorrect then LOG will raise warning and active Camera will be set to the default one.If no camera exist then Default camera will be set to NullPtr raising Assertion error on next GetActiveCamera() Call
	if(AllCameras.Find(_CameraName) != -1){
		ActiveCamera = &AllCameras.Get(_CameraName);
	}
	return *this;
}
Scene& Scene::RemoveCamera(const Upp::String& _CameraName){//if the removed camera is the active one then activeCamera ptr will get the default camera.If no camera exist then Default camera will be set to NullPtr raising Assertion error on next GetActiveCamera() Call
	if(AllCameras.Find(_CameraName) != -1){
		if(& AllCameras.Get(_CameraName) == ActiveCamera) ActiveCamera = nullptr;
		AllCameras.Remove(AllCameras.Find(_CameraName));
	}
	return *this;
}
template <class T,class... Args>
T& Scene::CreateGameObject(const Upp::String& _ObjectName, Args&&... args){//if the game object exists then it will remove it to create new one
	try{
		if(AllGamesObjects.Find(_ObjectName) ==-1){
			return dynamic_cast<T&>(AllGamesObjects.Create<T>(_ObjectName,std::forward<Args>(args)...));
		}else{
			RemoveGameObject(_ObjectName);
			return dynamic_cast<T&>(AllGamesObjects.Create<T>(_ObjectName,std::forward<Args>(args)...));
		}
	}catch(...){
		throw UOGLException(6,"Error : T& Scene::CreateGameObject(...) => Error on convertion of the game Object !",1);
	}
}
template <class T>
T& Scene::AddGameObject(const Upp::String& _ObjectName,T& ObjectToAdd){//if the game object exists then it will remove it to create new one
	try{
		if(AllGamesObjects.Find(_ObjectName) ==-1){
			auto& type = (dynamic_cast<T&>(AllGamesObjects.Create<T>(_ObjectName)) = ObjectToAdd);
			type.SetScene(*this);
			type.SetName(_ObjectName);
			return type;
		}else{
			RemoveGameObject(_ObjectName);
			auto& type = (dynamic_cast<T&>(AllGamesObjects.Create<T>(_ObjectName)) = ObjectToAdd);
			type.SetScene(*this);
			type.SetName(_ObjectName);
			return type;
		}
	}catch(...){
		throw UOGLException(6,"Error : T& Scene::AddGameObject(...) => Error on convertion of the game Object !",1);
	}
}
template <class T>
T& Scene::GetGameObject(const Upp::String& _ObjectName){//Throw exception if gameObject don't exists
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
bool Scene::IsGameObjectIsTypeOf(const Upp::String& _ObjectName){//Return true if the game object exits and is type of template, else return false
	if(AllGamesObjects.Find(_ObjectName) != -1){
		try{
			return (typeid(dynamic_cast<T&>(AllGamesObjects.Get(_ObjectName)))== typeid(T));
		}catch(...){
			return false;
		}
	}
	return false;
}
bool Scene::IsGameObjectExist(const Upp::String& _ObjectName){//Return true if the game object exists
	if(AllGamesObjects.Find(_ObjectName) != -1)return true;
	return false;
}
Scene& Scene::RemoveGameObject(const Upp::String& _ObjectName){//Will remove gameObject if it exist
	if(AllGamesObjects.Find(_ObjectName) != -1){
		AllGamesObjects.Remove(AllGamesObjects.Find(_ObjectName));
	}
	return *this;
}
Scene& Scene::Load(){
	return *this;
}
bool Scene::IsLoaded(){
	return true;
}
Scene& Scene::Draw(const Upp::String& CameraToUse){
	return *this;
}