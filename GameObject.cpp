#include "GameObject.h"
#include "Scene.h"
#include "Shader.h"
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
	ThisObjectCarryLight();
}
GameObject::~GameObject(){
	if(scene){
		int cpt = 0;
		for(Light* ptr : scene->GetAllLights()){
			for(Light* myLight : GetAllLightsOftheObject()){
				if(ptr == myLight){
					scene->GetAllLights().Remove(cpt,1);
					break;
				}
			}
			cpt++;
		}
	}
}

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
	ThisObjectCarryLight();
}

Scene* GameObject::GetScene()const{
	return scene;
}
GameObject& GameObject::SetScene(Scene* _scene){
	scene = _scene;
	ThisObjectCarryLight();
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

void GameObject::DisableDraw(){
	DontDraw=true;
	for(Light* ptr : GetAllLightsOftheObject()){
		ptr->IsDrawable();
	}
}
void GameObject::EnableDraw(){
	DontDraw=false;
	for(Light* ptr : GetAllLightsOftheObject()){
		ptr->IsDrawable(true);
	}
}

bool GameObject::IsDrawable(){
	return !DontDraw;
}

//******************Transform part**************************
Transform& GameObject::GetTransform(){
	return transform;
}
void GameObject::SetTransform(const Transform& _transform){
	transform = _transform;
}

//******************Light part******************************

Upp::Array<Light*> GameObject::GetAllLightsOftheObject(){
	Upp::Array<Light*> AL;
	for(const Upp::String& Str : dirLights.GetKeys()){
		AL.Add(&dirLights.Get(Str));
	}
	for(const Upp::String& Str : spotLights.GetKeys()){
		AL.Add(&spotLights.Get(Str));
	}
	for(const Upp::String& Str : pointLights.GetKeys()){
		AL.Add(&pointLights.Get(Str));
	}
	return AL;
}

void GameObject::ThisObjectCarryLight(){
	if(scene && (dirLights.GetCount()> 0 || spotLights.GetCount() > 0 || pointLights.GetCount() > 0)){
		bool trouver = false;
		for(Light* myLight : GetAllLightsOftheObject()){
			for(Light* ptr : scene->GetAllLights()){
				if(ptr == myLight){
					trouver=true;
					break;
				}
			}
			if(!trouver)scene->GetAllLights().Add(myLight);
			else trouver = false;
		}
	}
}
const Upp::Vector<Upp::String>& GameObject::GetAllDirLightsKeys(){
	return dirLights.GetKeys();
}
DirLight& GameObject::GetDirLight(const Upp::String& _name){
	if(dirLights.Find(_name) !=-1){
		return dirLights.Get(_name);
	}
	throw UGLException(15,"PointLight& GameObject::GetPointLights(...) => DirLight named "+ _name +" Do not exists",1);
}
DirLight& GameObject::AddDirLight(const Upp::String& _name){
	if(dirLights.Find(_name) ==-1){
		auto& e = dirLights.Add(_name);
		e.SetName(_name);
		ThisObjectCarryLight();
		return e;
	}else{
		return dirLights.Get(_name);
	}
}

const Upp::Vector<Upp::String>& GameObject::GetAllSpotLightsKeys(){
	return spotLights.GetKeys();
}
SpotLight& GameObject::GetSpotLight(const Upp::String& _name){
	if(spotLights.Find(_name) !=-1){
		return spotLights.Get(_name);
	}
	throw UGLException(16,"PointLight& GameObject::GetPointLights(...) => SpotLight named "+ _name +" Do not exists",1);
}
SpotLight& GameObject::AddSpotLight(const Upp::String& _name){
	if(spotLights.Find(_name) ==-1){
		auto& e =spotLights.Add(_name);
		e.SetName(_name);
		ThisObjectCarryLight();
		return e;
	}else{
		return spotLights.Get(_name);
	}	
}
		
const Upp::Vector<Upp::String>& GameObject::GetAllPointLightsKeys(){
	return pointLights.GetKeys();
}
PointLight& GameObject::GetPointLight(const Upp::String& _name){
	if(pointLights.Find(_name) !=-1){
		return pointLights.Get(_name);
	}
	throw UGLException(17,"PointLight& GameObject::GetPointLights(...) => PointLight named "+ _name +" Do not exists",1);
}
PointLight& GameObject::AddPointLight(const Upp::String& _name){
	if(pointLights.Find(_name) ==-1){
		auto& e = pointLights.Add(_name);
		e.SetName(_name);
		ThisObjectCarryLight();
		return e;
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