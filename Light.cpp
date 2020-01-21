#include "Light.h"

//********************** Light ****************************
Light::Light(Scene& _scene) : GameObject(_scene, GOT_UNKNOW){}
Light::Light(Scene& _scene,GameObjectType got,glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular) : GameObject(_scene, got){
	ambient  = _ambient;
	diffuse  = _diffuse;
	specular = _specular;
}
Light::Light(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular) : GameObject(GOT_UNKNOW){
	ambient  = _ambient;
	diffuse  = _diffuse;
	specular = _specular;
}
Light::Light(Scene& _scene, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular) : GameObject(_scene, GOT_UNKNOW){
	ambient  = _ambient;
	diffuse  = _diffuse;
	specular = _specular;
}


Light::Light(Light& light) : GameObject(GOT_UNKNOW){//Be carefull of setting the Scene
	ambient  = light.ambient;
	diffuse  = light.diffuse;
	specular = light.specular;
}
Light& Light::operator=(Light& light){//Be carefull of setting the Scene
	ambient  = light.ambient;
	diffuse  = light.diffuse;
	specular = light.specular;
	return *this;
}
Light& Light::SetAmbient(glm::vec3& _ambient){
	ambient = _ambient;
	return *this;
}
Light& Light::SetDiffuse(glm::vec3& _diffuse){
	diffuse = _diffuse;
	return *this;
}
Light& Light::SetSpecular(glm::vec3& _specular){
	specular = _specular;
	return *this;
}
glm::vec3 Light::GetAmbient() const{
	return ambient;
}
glm::vec3 Light::GetDiffuse() const{
	return diffuse;
}
glm::vec3 Light::GetSpecular() const{
	return specular;
}

Light& Light::SetPosition(glm::vec3& _position, bool updateChildrens){
	transform.SetNewPosition(_position,updateChildrens);
	return *this;
}
Light& Light::Move(glm::vec3& _position, bool updateChildrens){
	transform.Move(_position,updateChildrens);
	return *this;
}
glm::vec3 Light::GetPosition() const{
	return transform.GetPosition();
}

//********************** Dir Light **********************
DirLight::DirLight(Scene& _scene): Light(_scene, GOT_DIR_LIGHT){}
DirLight::DirLight(Scene& _scene,glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _direction) : Light(_scene, GOT_DIR_LIGHT,_ambient, _diffuse, _specular){
	direction = _direction;
}
DirLight::DirLight(DirLight& dirLight) : Light(dirLight){//Be carefull of setting the Scene
	ambient  = dirLight.ambient;
	diffuse  = dirLight.diffuse;
	specular = dirLight.specular;
	direction = dirLight.direction;
}
DirLight& DirLight::operator=(DirLight& dirLight){//Be carefull of setting the Scene
	static_cast<Light>(*this)=dirLight;
	ambient  = dirLight.ambient;
	diffuse  = dirLight.diffuse;
	specular = dirLight.specular;
	direction = dirLight.direction;
	return *this;
}
DirLight& DirLight::SetDirection(glm::vec3 _direction){
	direction = _direction;
	return *this;
}
glm::vec3 DirLight::GetDirection() const{
	return direction;
}
//******************** Spot Light *************************
SpotLight::SpotLight(Scene& _scene) : Light(_scene, GOT_SPOT_LIGHT){}
SpotLight::SpotLight(Scene& _scene, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _position, glm::vec3 _direction, float _cutOff, float _outerCutOff, float _constant, float _linear, float _quadratic): Light(_scene, GOT_SPOT_LIGHT,_ambient, _diffuse, _specular){
	transform.SetNewPosition(_position);
	direction = _direction;
	cutOff = _cutOff;
	outerCutOff = _outerCutOff;
    constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}
SpotLight::SpotLight(SpotLight& _spotLight) : Light(_spotLight){//Be carefull of setting the Scene
	ambient  = _spotLight.ambient;
	diffuse  = _spotLight.diffuse;
	specular = _spotLight.specular;
	transform = _spotLight.GetTransform();
	direction = _spotLight.direction;
	cutOff = _spotLight.cutOff;
	outerCutOff = _spotLight.outerCutOff;
    constant = _spotLight.constant;
	linear = _spotLight.linear;
	quadratic = _spotLight.quadratic;
}
SpotLight& SpotLight::operator=(SpotLight& _spotLight){//Be carefull of setting the Scene
	static_cast<Light>(*this)=_spotLight;
	ambient  = _spotLight.ambient;
	diffuse  = _spotLight.diffuse;
	specular = _spotLight.specular;
	transform = _spotLight.GetTransform();
	direction = _spotLight.direction;
	cutOff = _spotLight.cutOff;
	outerCutOff = _spotLight.outerCutOff;
    constant = _spotLight.constant;
	linear = _spotLight.linear;
	quadratic = _spotLight.quadratic;
	return *this;
}
SpotLight& SpotLight::SetDirection(glm::vec3 _direction){
	direction = _direction;
	return *this;
}
SpotLight& SpotLight::SetConstant(float _constant){
	constant = _constant;
	return *this;
}
SpotLight& SpotLight::SetLinear(float _linear){
	linear = _linear;
	return *this;
}
SpotLight& SpotLight::SetQuadratic(float _quadratic){
	quadratic = _quadratic;
	return *this;
}
SpotLight& SpotLight::SetCutOff(float _cutOff){
	cutOff = _cutOff;
	return *this;
}
SpotLight& SpotLight::SetOuterCutOff(float _outerCutOff){
	outerCutOff = _outerCutOff;
	return *this;
}
glm::vec3 SpotLight::GetDirection() const{
	return direction;
}
float SpotLight::GetConstant() const{
	return constant;
}
float SpotLight::GetLinear() const{
	return linear;
}
float SpotLight::GetQuadratic() const{
	return quadratic;
}
float SpotLight::GetCutOff() const{
	return cutOff;
}
float SpotLight::GetOuterCutOff() const{
	return outerCutOff;
}
//********************** Point Light ********************
PointLight::PointLight(Scene& _scene) : Light(_scene, GOT_POINT_LIGHT){}
PointLight::PointLight(Scene& _scene,glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _position, float _constant, float _linear, float _quadratic) : Light(_scene, GOT_POINT_LIGHT,_ambient, _diffuse, _specular){
    constant=_constant;
    linear=_linear;
    quadratic=_quadratic;
}
PointLight::PointLight(PointLight& _pointLight) : Light(_pointLight){//Be carefull of setting the Scene
    constant=_pointLight.constant;
    linear=_pointLight.linear;
    quadratic=_pointLight.quadratic;
}
PointLight& PointLight::operator=(PointLight& _pointLight){//Be carefull of setting the Scene
	static_cast<Light>(*this)=_pointLight;
    constant=_pointLight.constant;
    linear=_pointLight.linear;
    quadratic=_pointLight.quadratic;
    return *this;
}
PointLight& PointLight::SetConstant(float _constant){
	constant = _constant;
	return *this;
}
float PointLight::GetConstant() const{
	return constant;
}
PointLight& PointLight::SetLinear(float _linear){
	linear = _linear;
	return *this;
}
float PointLight::GetLinear() const{
	return linear;
}
PointLight& PointLight::SetQuadratic(float _quadratic){
	quadratic = _quadratic;
	return *this;
}
float PointLight::GetQuadratic() const{
	return quadratic;
}