#include "Scene.h"
#include "UltimateOpenGL.h"
Scene::Scene(){}
Scene::Scene(UltimateOpenGL_Context& _context){
	context = &_context;
}
Scene::Scene(UltimateOpenGL_Context& _context, const Upp::String& _name){
	context = &_context;
	name = _name;
}
Scene& Scene::operator=(Scene& _scene){
	context = _scene.context;
	name = _scene.name;
	loaded = _scene.loaded;
	//Object cant be copied
	/*
	for(const Upp::String& gm :  _scene.AllGamesObjects.GetKeys()){
		auto& e = AllGamesObjects.Add(gm,_scene.AllGamesObjects.Get(gm));
		e.SetScene(*this);
	}*/
	for(const Upp::String& cam : _scene.AllCameras.GetKeys()){
		auto& e = AllCameras.Add(cam, _scene.AllCameras.Get(cam));
		if(&_scene.AllCameras.Get(cam) == _scene.ActiveCamera) ActiveCamera = &e;
		e.SetScene(*this);
	}
	
	shader = _scene.shader;
	SkyBox = _scene.SkyBox;
	return *this;
}
Scene::~Scene(){}
UltimateOpenGL_Context& Scene::GetContext(){
	ASSERT_(context,"Scene::GetContext() Scene have nullptr Context");
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
Camera& Scene::CreateCamera(const Upp::String& _CameraName){//if the Camera exists then it will remove it to create new one
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
Camera& Scene::AddCamera(const Upp::String& _CameraName, Camera& camera){ //Copying camera and give it a new name
	Upp::String newName = _CameraName;
	int cpt = 1;
	while(AllCameras.Find(newName) != -1){
		newName = newName + "(" + Upp::AsString(cpt) + ")";
		cpt++;
	}
	Camera& added = AllCameras.Add(newName,camera);
	added.SetName(newName);
	added.SetScene(*this);
	if(!ActiveCamera) ActiveCamera = &added;
	return added;
}
Camera& Scene::AddCamera(Camera& camera){//Copy the camera using name of camera
	Upp::String newName = camera.GetName();
	int cpt = 1;
	while(AllCameras.Find(newName) != -1){
		newName = newName + "(" + Upp::AsString(cpt) + ")";
		cpt++;
	}
	Camera& added = AllCameras.Add(newName,camera);
	added.SetName(newName);
	added.SetScene(*this);
	if(!ActiveCamera) ActiveCamera = &added;
	return added;
}
Camera& Scene::GetCamera(const Upp::String& _CameraName){
	if(AllCameras.Find(_CameraName) != -1){
		return AllCameras.Get(_CameraName);
	}
	throw UOGLException(4,"Error : Camera& Scene::GetCamera(const Upp::String&) => No camera named \""+ _CameraName +"\" have been found !",1);
}
Camera& Scene::GetActiveCamera(){ //Raise Assertion if active camera has not been set !
	ASSERT_(ActiveCamera,"Active Camera have nullptr object, be sure to Set ActiveCamere by using SetActiveCamera(const Upp::String&) or by creating the first one of the scene");
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
	if( IsLoaded()){
		Camera& camera = GetActiveCamera();
		if(CameraToUse.GetCount() > 0){
			if( AllCameras.Find(CameraToUse) != -1){
				camera = AllCameras.Get(CameraToUse);
			}
		}
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 transform = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetTransform().GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);
		float screenSizeX = 800.0f;
		float screenSizeY = 600.0f;	
		screenSizeX=(float) GetContext().GetScreenSize().cx;
		screenSizeY=(float) GetContext().GetScreenSize().cy;	

		projection = glm::perspective(glm::radians(camera.GetFOV()),(float)( screenSizeX / screenSizeY),camera.GetDrawDistanceMin(),camera.GetDrawDisanceMax());//We calculate Projection here since multiple camera can have different FOV
																																							      //I will also provide différent camera parameter in futurs
							
		glClearColor(SkyBox.GetColor().x,SkyBox.GetColor().y,SkyBox.GetColor().z,SkyBox.GetColor().w); //définie la couleur de fond dans la fenetre graphique 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //nétoie la fenetre graphique et la regénère Scene::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform){
			
	}
	return *this;
}