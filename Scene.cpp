#include "UltimateOpenGL.h"

Scene::Scene(){}
Scene::Scene(UltimateOpenGL_Context& _context){
	context = &_context;
}
Scene::Scene(UltimateOpenGL_Context& _context, const Upp::String& _name){
	context = &_context;
	name = _name;
}
Scene::Scene(Scene& _scene){
	*this = _scene;
}
Scene& Scene::operator=(Scene& _scene){
	context = _scene.context;
	name = _scene.name;
	loaded = _scene.loaded;
	
	for (int i = 0; i < _scene.AllGamesObjects.GetCount(); ++i)
		AllGamesObjects.Add(_scene.AllGamesObjects.GetKey(i), Upp::pick(_scene.AllGamesObjects[i].Clone()));
	
	for (int i = 0; i < _scene.AllLights.GetCount(); ++i)
		AllLights.Add(_scene.AllLights.GetKey(i), Upp::pick(_scene.AllLights[i].Clone()));
	
	for (int i = 0; i < _scene.AllCameras.GetCount(); ++i){
		Camera& c = AllCameras.Add(_scene.AllCameras.GetKey(i), Upp::pick(_scene.AllCameras[i].Clone()));
		if(_scene.ActiveCamera == &(_scene.AllCameras[i])) ActiveCamera = &c;
	}

	SkyBox = _scene.SkyBox;
	return *this;
}
Scene::~Scene(){
	AllGamesObjects.Clear();
}
UltimateOpenGL_Context& Scene::GetContext(){
	ASSERT_(context,"Scene::GetContext() Scene have nullptr Context");
	return *context;
}
Upp::String Scene::GetName(){
	return name;
}
Color_Material& Scene::GetSkyBox(){
	return SkyBox;
}
Upp::ArrayMap<Upp::String,GameObject>& Scene::GetAllGameObjects(){
	return AllGamesObjects;
}
Upp::ArrayMap<Upp::String,Camera>& Scene::GetAllCameras(){
	return AllCameras;
}
Upp::ArrayMap<Upp::String,Light>& Scene::GetAllLights(){
	return AllLights;
}
Scene& Scene::SetContext(UltimateOpenGL_Context& _context){
	context = &_context;
	return *this;
}
Scene& Scene::SetName(const Upp::String& _name){
	name = _name;
	return *this;
}
Scene& Scene::SetSkyBox(Color_Material& _skyBox){
	SkyBox = _skyBox;
	return *this;
}
bool Scene::IsCameraExist(const Upp::String& _CameraName){ //Return true if the game object exists
	if(AllCameras.Find(_CameraName)){
		return true;
	}
	return false;
}
Scene& Scene::RemoveCamera(const Upp::String& _CameraName){ //Will remove gameObject if it exist
	if(AllCameras.Find(_CameraName) != -1){
		if(&AllCameras.Get(_CameraName) == ActiveCamera) ActiveCamera = nullptr;
		AllCameras.Remove(AllCameras.Find(_CameraName));
	}
	return *this;
}
Scene& Scene::SetActiveCamera(const Upp::String& _CameraName){//If name is incorrect then LOG will raise warning and active Camera will be set to the default one.
	if(AllCameras.Find(_CameraName) != -1){
		ActiveCamera =  &AllCameras.Get(_CameraName);
	}
	if(!ActiveCamera){
		ASSERT_(AllCameras.GetCount() == 0, "ActiveCamera is NullPtr, SetActive Have been called when AllCamera is empty, ensure you create a camera before tryng to set active or get one");
		ActiveCamera = &AllCameras.Get(AllCameras.GetKey(0));
	}
	return *this;
}
Camera& Scene::GetActiveCamera(){ //Raise Assertion if active camera has not been set !
	ASSERT_(ActiveCamera,"Active Camera have nullptr object, be sure to Set ActiveCamera by using SetActiveCamera(const Upp::String&) or by creating the first one of the scene");
	return *ActiveCamera;
}

bool Scene::IsLightExist(const Upp::String& _LightName){ //Return true if the light  exists
	if(AllLights.Find(_LightName) != -1)return true;
	return false;
}
Scene& Scene::RemoveLight(const Upp::String& _LightName){ //Will remove light if it exist
	if(AllLights.Find(_LightName) != -1){
		AllLights.Remove(AllLights.Find(_LightName));
	}
	return *this;
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
	if(!loaded){
		for(GameObject& gm : AllGamesObjects){
			gm.Load();
		}
		loaded = true;
	}
	return *this;
}
bool Scene::IsLoaded(){
	return loaded;
}
Scene& Scene::Draw(const Upp::String& CameraToUse){
	if( IsLoaded()){
		Camera& camera = GetActiveCamera();
		if(CameraToUse.GetCount() > 0){
			if( AllCameras.Find(CameraToUse) != -1){
				camera = AllCameras.Get(CameraToUse);
			}
		}
		if( camera.IsDrawEventActivated() && camera.GetOnDrawFunction()) camera.GetOnDrawFunction()(camera);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 transform = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		//LOG(Upp::String(glm::to_string(view)));
		
		glm::mat4 projection =camera.GetProjectionMatrix(GetContext().GetScreenSize());

		//I will also provide différent camera parameter in futur
		glClearColor(SkyBox.GetColor().x,SkyBox.GetColor().y,SkyBox.GetColor().z,SkyBox.GetColor().w);//définie la couleur de fond dans la fenetre graphique
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //nétoie la fenetre graphique et la regénère Scene::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform){
		//we must render all game object by distance To do proper blending (gestion of alpha)
		Upp::ArrayMap<float,GameObject*> sorted;
		for(const Upp::String& str : AllGamesObjects.GetKeys()){
			if( IsGameObjectTypeOf<Object3D>(str)){
				Object3D& object = GetGameObject<Object3D>(str);
				float distance = glm::length(camera.GetTransform().GetPosition() - object.GetTransform().GetPosition());
				sorted.Add(distance,&object);
			}
		}
		Upp::SortByKey(sorted,[](const float& a, const float& b) { return a>b; });
		for(GameObject* object : sorted.GetValues()){
			object->Draw(model,view,projection,transform,camera);
		}
	}
	return *this;
}