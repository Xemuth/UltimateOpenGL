#include "GameObject.h"
#include "Scene.h"
GameObject::GameObject(){}
GameObject::GameObject(const Upp::String& _name){
	name = _name;
}
GameObject::GameObject(Scene* _scene){
	scene =_scene;
}
GameObject::GameObject(Scene* _scene,const Upp::String& _name){
	name = _name;
	scene =_scene;
}
GameObject::GameObject(GameObject& object){
	name=object.GetName();
	Scene* scene = object.GetScene();
	
	transform = object.GetTransform();

	for(const Upp::String& light : object.dirLights.GetKeys()){
		dirLights.Add(light,object.dirLights.Get(light));
	}
	for(const Upp::String& light : object.pointLights.GetKeys()){
		pointLights.Add(light,object.pointLights.Get(light));
	}
	for(const Upp::String& light : object.spotLights.GetKeys()){
		spotLights.Add(light,object.spotLights.Get(light));
	}
}
GameObject::~GameObject(){}

void GameObject::operator=(GameObject& object){
	name=object.GetName();
	Scene* scene = object.GetScene();
	
	transform = object.GetTransform();
	

	for(const Upp::String& light : object.dirLights.GetKeys()){
		dirLights.Add(light,object.dirLights.Get(light));
	}
	for(const Upp::String& light : object.pointLights.GetKeys()){
		pointLights.Add(light,object.pointLights.Get(light));
	}
	for(const Upp::String& light : object.spotLights.GetKeys()){
		spotLights.Add(light,object.spotLights.Get(light));
	}	
}

Scene* GameObject::GetScene()const{
	return scene;
}
GameObject& GameObject::SetScene(Scene* _scene){
	scene = _scene;
	return *this;
}

Upp::String GameObject::GetName()const{
	return name;
}
GameObject& GameObject::SetName(const Upp::String& _name){
	name = _name;
	return *this;	
}

void GameObject::SetOnDrawFunction(void (*myFunction)(GameObject& myGameObject)){
	onDraw = myFunction;
	LOG("Info : OnDrawFunction have been set sucefully on " + this->GetName() +" object !\n");
}
ACTION_FUNCTION GameObject::GetOnDrawFunction(){
	return onDraw;
}
void GameObject::SetOnTransformFunction(void (*myFunction)(GameObject& myGameObject)){
	onTransform = myFunction;
	LOG("Info : OnTransformFunction have been set sucefully on " + this->GetName() +" object !\n");
}
ACTION_FUNCTION GameObject::GetOnTransformFunction(){
	return onTransform;
}

void GameObject::DisableDrawForNextFrame(){
	DontDraw=true;
}
bool GameObject::IsDrawableDuringThisFrame(){
	return DontDraw;
}

//******************Transform part**************************
Transform& GameObject::GetTransform(){
	return transform;
}
void GameObject::SetTransform(const Transform& _transform){
	transform = _transform;
}

//******************Light part******************************
Upp::VectorMap<Upp::String,DirLight>& GameObject::GetDirLights(){
	return dirLights;
}
DirLight& GameObject::AddDirLight(const Upp::String& _name){
	if(dirLights.Find(_name) ==-1){
		return dirLights.Add(_name);
	}else{
		return dirLights.Get(_name);
	}
}
Upp::VectorMap<Upp::String,SpotLight>& GameObject::GetSpotLights(){
	return spotLights;
}
SpotLight& GameObject::AddSpotLight(const Upp::String& _name){
	if(spotLights.Find(_name) ==-1){
		return spotLights.Add(_name);
	}else{
		return spotLights.Get(_name);
	}	
}
Upp::VectorMap<Upp::String,PointLight>& GameObject::GetPointLights(){
	return pointLights;
}
PointLight& GameObject::AddPointLight(const Upp::String& _name){
	if(pointLights.Find(_name) ==-1){
		return pointLights.Add(_name);
	}else{
		return pointLights.Get(_name);
	}	
}

//Draw
void GameObject::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera){
	if(onDraw !=nullptr){
		onDraw(*this);	
	}
}