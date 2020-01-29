#include "GameObject.h"
GameObject::GameObject(GameObjectType got) : transform(){ //BeCarefull of setting up the scene;
	type = got;
}
GameObject::GameObject(Scene& _scene, GameObjectType got) : transform(){
	type = got;
	scene = &_scene;
}
GameObject::GameObject(const GameObject& gameObject) : transform(){
	name=gameObject.name;
	type=gameObject.type;
	scene=nullptr;//gameObject.scene;
	transform=gameObject.transform;
	transform.ClearChildrens();

	onDraw=gameObject.onDraw;
	onDrawEventActivate=gameObject.onDrawEventActivate;
	
	onTransform=gameObject.onTransform;
	onTransformEventActivate=gameObject.onTransformEventActivate;

	drawable=gameObject.drawable;
}
GameObject::~GameObject(){}
GameObject& GameObject::operator=(const GameObject& gameObject){
	name=gameObject.name;
	type=gameObject.type;
	scene=nullptr;//gameObject.scene;
	transform=gameObject.transform;
	transform.ClearChildrens();
	//transform.SetGameObject(*this);
    
	onDraw=gameObject.onDraw;
	onDrawEventActivate=gameObject.onDrawEventActivate;
	
	onTransform=gameObject.onTransform;
	onTransformEventActivate=gameObject.onTransformEventActivate;

	drawable=gameObject.drawable;
	return *this;
}
//******************Scene part**************************
Scene& GameObject::GetScene()const{
	if(!scene) ASSERT("UltimateOpenGL : GetScene on NullPtr !");
	return *scene;
}
GameObject& GameObject::SetScene(Scene& _scene){
	scene = &_scene;
	return *this;
}
//******************Name part**************************
Upp::String GameObject::GetName()const{
	return name;
}
GameObject& GameObject::SetName(const Upp::String& _name){
	name = _name;
	return *this;
}

GameObjectType GameObject::GetType(){
	return type;
}
	
//******************Draw part**************************
GameObject& GameObject::SetOnDrawFunction(ACTION_FUNCTION myFunction){
	onDraw = myFunction;
	return *this;
}
ACTION_FUNCTION GameObject::GetOnDrawFunction(){
	return onDraw;
}
GameObject& GameObject::EnableDrawEvent(){
	onDrawEventActivate = true;
	return *this;
}
GameObject& GameObject::DisableDrawEvent(){
	onDrawEventActivate = false;
	return *this;
}
bool GameObject::IsDrawEventActivated(){
	return onDrawEventActivate;
}

//******************Transform part*********************
GameObject& GameObject::SetOnTransformFunction(ACTION_FUNCTION myFunction){
	onTransform = myFunction;
	return *this;
}
ACTION_FUNCTION GameObject::GetOnTransformFunction(){
	return onTransform;
}
GameObject& GameObject::EnableTransformEvent(){
	onTransformEventActivate = true;
	return *this;
}
GameObject& GameObject::DisableTransformEvent(){
	onTransformEventActivate = false;
	return *this;
}
bool GameObject::IsTransformEventActivated(){
	return onTransformEventActivate;
}

//******************Drawable part**********************
GameObject& GameObject::EnableDraw(){
	drawable = true;
	return *this;
}
GameObject& GameObject::DisableDraw(){
	drawable = false;
	return *this;
}
bool GameObject::IsDrawable(){
	return drawable;
}

//******************Transform part**********************
Transform& GameObject::GetTransform(){
	return transform;
}
GameObject& GameObject::SetTransform(const Transform& _transform){
	transform = _transform;
	return *this;
}