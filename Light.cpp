#include "Light.h"
//********************** Light ****************************
Light::~Light(){}
Light& Light::SetScene(Scene& _scene){
	scene = &_scene;
	return *this;
}
Scene& Light::GetScene(){ //Raise Assertion in case of no scene setted !
	if(!scene) ASSERT("UltimateOpenGL Light : GetScene have foound a nullptr scene !");
	return *scene;
}
Light& Light::SetName(const Upp::String& _name) {
	name = _name;
	return *this;
}
Upp::String Light::GetName()const{
	return name;
}
Light& Light::SetOnDrawFunction(LIGHT_FUNCTION myFunction){
	onDraw = myFunction;
	return *this;
}
LIGHT_FUNCTION Light::GetOnDrawFunction(){
	return onDraw;
}
Light& Light::EnableDrawEvent(){
	onDrawEventActivate  = true;
	return *this;
}
Light& Light::DisableDrawEvent(){
	onDrawEventActivate = false;
	return *this;
}
bool Light::IsDrawEventActivated(){
	return onDrawEventActivate;
}
Light& Light::EnableDraw(){
	drawable = true;
	return *this;
}
Light& Light::DisableDraw(){
	drawable = false;
	return *this;
}
bool Light::IsDrawable(){
	return drawable;
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
//********************** Dir Light **********************
DirLight::DirLight(Scene& _scene){
	scene = &_scene;
}
DirLight::DirLight(Scene& _scene,const Upp::String& _name){
	scene = &_scene;
	name = _name;
}
DirLight::DirLight(Scene& _scene,const Upp::String& _name,glm::vec3 _direction){
	scene = &_scene;
	name = _name;
	direction = _direction;
}
DirLight::DirLight(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _direction){
	ambient = _ambient;
	diffuse = _diffuse;
	specular = _specular;
	direction = _direction;
}
DirLight::DirLight(glm::vec3 _direction){
	direction = _direction;
}
DirLight::DirLight(DirLight& dirLight){
	scene = dirLight.scene;
	name = dirLight.name;
	ambient = dirLight.ambient;
	diffuse = dirLight.diffuse;
	specular = dirLight.specular;
	direction = dirLight.direction;
}
DirLight::~DirLight(){}
DirLight& DirLight::operator=(DirLight& dirLight){
	scene = dirLight.scene;
	name = dirLight.name;
	ambient = dirLight.ambient;
	diffuse = dirLight.diffuse;
	specular = dirLight.specular;
	direction = dirLight.direction;
	return *this;
}
DirLight* DirLight::Clone(){
	return new DirLight(*this);
}
const Upp::String DirLight::GetShaderDataStructure()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_DIR_STRUCT();
}
const Upp::String DirLight::GetShaderNameStructure()const{
	return "DirLight";
}
const Upp::String DirLight::GetShaderColorCalculationFunction()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_DIR_COLOR_FUNCTION();
}
const Upp::String DirLight::GetShaderColorPrototypeFunction()const{//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_DIR_COLOR_PROTOTYPE();
}
const Upp::String DirLight::GetShaderTextureCalculationFunction()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_DIR_TEXTURE_FUNCTION();
}
const Upp::String DirLight::GetShaderTexturePrototypeFunction()const{//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_DIR_TEXTURE_PROTOTYPE();
}

void DirLight::SentToShader(Shader& shader,const Upp::String& CustomName){
	if(shader.IsCompiled()){
		Upp::String nameToUse = (CustomName.GetCount()>0)? CustomName : name;
		shader.SetVec3(nameToUse +".ambient",ambient);
		shader.SetVec3(nameToUse +".diffuse",diffuse);
		shader.SetVec3(nameToUse +".specular",specular);
		shader.SetVec3(nameToUse +".direction",direction);
	}
}
DirLight& DirLight::SetDirection(glm::vec3& _direction){
	direction = _direction;
	return *this;
}
glm::vec3 DirLight::GetDirection() const{
	return direction;
}
//******************** Spot Light *************************
SpotLight::SpotLight(Scene& _scene){
	scene = &_scene;
}
SpotLight::SpotLight(Scene& _scene,const Upp::String& _name){
	scene = &_scene;
	name = _name;
}
SpotLight::SpotLight(Scene& _scene,const Upp::String& _name, glm::vec3 _position, glm::vec3 _direction, float _cutOff, float _outerCutOff, float _constant, float _linear, float _quadratic){
	scene = &_scene;
	name = _name;
	position = _position;
	direction = _direction;
	cutOff = _cutOff;
	outerCutOff = _outerCutOff;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}
SpotLight::SpotLight(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _position, glm::vec3 _direction, float _cutOff, float _outerCutOff, float _constant, float _linear, float _quadratic){
	ambient = _ambient;
	diffuse = _diffuse;
	specular = _specular;
	position = _position;
	direction = _direction;
	cutOff = _cutOff;
	outerCutOff = _outerCutOff;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}
SpotLight::SpotLight(glm::vec3 _position, glm::vec3 _direction, float _cutOff, float _outerCutOff, float _constant, float _linear, float _quadratic){
	position = _position;
	direction = _direction;
	cutOff = _cutOff;
	outerCutOff = _outerCutOff;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}
SpotLight::SpotLight(SpotLight& spotLight){//Be carefull of setting the Scene
	scene = spotLight.scene;
	name = spotLight.name;
	ambient = spotLight.ambient;
	diffuse = spotLight.diffuse;
	specular = spotLight.specular;
	position = spotLight.position;
	direction = spotLight.direction;
	cutOff = spotLight.cutOff;
	outerCutOff = spotLight.outerCutOff;
	constant = spotLight.constant;
	linear = spotLight.linear;
	quadratic = spotLight.quadratic;
}
SpotLight::~SpotLight(){}
SpotLight& SpotLight::operator=(SpotLight& spotLight){//Be carefull of setting the Scene
	scene = spotLight.scene;
	name = spotLight.name;
	ambient = spotLight.ambient;
	diffuse = spotLight.diffuse;
	specular = spotLight.specular;
	position = spotLight.position;
	direction = spotLight.direction;
	cutOff = spotLight.cutOff;
	outerCutOff = spotLight.outerCutOff;
	constant = spotLight.constant;
	linear = spotLight.linear;
	quadratic = spotLight.quadratic;
	return *this;
}
SpotLight* SpotLight::Clone(){
	return new SpotLight(*this);
}
const Upp::String SpotLight::GetShaderDataStructure()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_SPOT_STRUCT();
}
const Upp::String SpotLight::GetShaderNameStructure()const{
	return "SpotLight";
}
const Upp::String SpotLight::GetShaderColorCalculationFunction()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_SPOT_COLOR_FUNCTION();
}
const Upp::String SpotLight::GetShaderColorPrototypeFunction()const{//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_SPOT_COLOR_PROTOTYPE();
}
const Upp::String SpotLight::GetShaderTextureCalculationFunction()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_SPOT_TEXTURE_FUNCTION();
}
const Upp::String SpotLight::GetShaderTexturePrototypeFunction()const{//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_SPOT_TEXTURE_PROTOTYPE();
}

void SpotLight::SentToShader(Shader& shader,const Upp::String& CustomName){
	if(shader.IsCompiled()){
		Upp::String nameToUse = (CustomName.GetCount()>0)? CustomName : name;
		shader.SetVec3(nameToUse +".ambient",ambient);
		shader.SetVec3(nameToUse +".diffuse",diffuse);
		shader.SetVec3(nameToUse +".specular",specular);
		
		shader.SetVec3(nameToUse +".position",position);
		shader.SetVec3(nameToUse +".direction",direction);
		shader.SetFloat(nameToUse +".cutOff",cutOff);
		shader.SetFloat(nameToUse +".outerCutOff",outerCutOff);
		shader.SetFloat(nameToUse +".constant",constant);
		shader.SetFloat(nameToUse +".linear",linear);
		shader.SetFloat(nameToUse +".quadratic",quadratic);
	}
}

SpotLight& SpotLight::SetDirection(glm::vec3& _direction){
	direction = _direction;
	return *this;
}
glm::vec3 SpotLight::GetDirection() const{
	return direction;
}
SpotLight& SpotLight::SetPosition(glm::vec3& _position){
	position = _position;
	return *this;
}
glm::vec3 SpotLight::GetPosition() const{
	return position;
}
SpotLight& SpotLight::SetConstant(float _constant){
	constant = _constant;
	return *this;
}
float SpotLight::GetConstant() const{
	return constant;
}
SpotLight& SpotLight::SetLinear(float _linear){
	linear = _linear;
	return *this;
}
float SpotLight::GetLinear() const{
	return linear;
}
SpotLight& SpotLight::SetQuadratic(float _quadratic){
	quadratic = _quadratic;
	return *this;
}
float SpotLight::GetQuadratic() const{
	return quadratic;
}
SpotLight& SpotLight::SetCutOff(float _cutOff){
	cutOff = _cutOff;
	return *this;
}
float SpotLight::GetCutOff() const{
	return cutOff;
}
SpotLight& SpotLight::SetOuterCutOff(float _outerCutOff){
	outerCutOff = _outerCutOff;
	return *this;
}
float SpotLight::GetOuterCutOff() const{
	return outerCutOff;
}
//********************** Point Light ********************
PointLight::PointLight(Scene& _scene){
	scene = &_scene;
}
PointLight::PointLight(Scene& _scene,const Upp::String& _name){
	scene = &_scene;
	name = _name;
}
PointLight::PointLight(Scene& _scene,const Upp::String& _name,glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _position, float _constant, float _linear, float _quadratic){
	scene = &_scene;
	name = _name;
	ambient = _ambient;
	diffuse = _diffuse;
	specular = _specular;
	position = _position;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}
PointLight::PointLight(Scene& _scene,const Upp::String& _name,glm::vec3 _position, float _constant, float _linear, float _quadratic){
	scene = &_scene;
	name = _name;
	position = _position;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}
PointLight::PointLight(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _position, float _constant, float _linear, float _quadratic){
	ambient = _ambient;
	diffuse = _diffuse;
	specular = _specular;
	position = _position;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}
PointLight::PointLight(glm::vec3 _position, float _constant, float _linear, float _quadratic){
	position = _position;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}
PointLight::PointLight(PointLight& _pointLight){//Be carefull of setting the Scene
	scene = _pointLight.scene;
	name = _pointLight.name;
	ambient = _pointLight.ambient;
	diffuse = _pointLight.diffuse;
	specular = _pointLight.specular;
	position = _pointLight.position;
	constant = _pointLight.constant;
	linear = _pointLight.linear;
	quadratic = _pointLight.quadratic;
}
PointLight::~PointLight(){}
PointLight& PointLight::operator=(PointLight& _pointLight){//Be carefull of setting the Scene
	scene = _pointLight.scene;
	name = _pointLight.name;
	ambient = _pointLight.ambient;
	diffuse = _pointLight.diffuse;
	specular = _pointLight.specular;
	position = _pointLight.position;
	constant = _pointLight.constant;
	linear = _pointLight.linear;
	quadratic = _pointLight.quadratic;
	return *this;
}
PointLight* PointLight::Clone(){
	return new PointLight(*this);
}
const Upp::String PointLight::GetShaderDataStructure()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_POINT_STRUCT();
}
const Upp::String PointLight::GetShaderNameStructure()const{
	return "PointLight";
}
const Upp::String PointLight::GetShaderColorCalculationFunction()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_POINT_COLOR_FUNCTION();
}
const Upp::String PointLight::GetShaderColorPrototypeFunction()const{//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_POINT_COLOR_PROTOTYPE();
}
const Upp::String PointLight::GetShaderTextureCalculationFunction()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_POINT_TEXTURE_FUNCTION();
}
const Upp::String PointLight::GetShaderTexturePrototypeFunction()const{//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return LIGHT_POINT_TEXTURE_PROTOTYPE();
}

void PointLight::SentToShader(Shader& shader,const Upp::String& CustomName){
	if(shader.IsCompiled()){
		Upp::String nameToUse = (CustomName.GetCount()>0)? CustomName : name;
		shader.SetVec3(nameToUse +".ambient",ambient);
		shader.SetVec3(nameToUse +".diffuse",diffuse);
		shader.SetVec3(nameToUse +".specular",specular);
		
		shader.SetVec3(nameToUse +".position",position);
		shader.SetFloat(nameToUse +".constant",constant);
		shader.SetFloat(nameToUse +".linear",linear);
		shader.SetFloat(nameToUse +".quadratic",quadratic);
	}
}

PointLight& PointLight::SetPosition(glm::vec3& _position){
	position = _position;
	return *this;
}
glm::vec3 PointLight::GetPosition() const{
	return position;
}
PointLight& PointLight::SetConstant(float _constant){
	constant = _constant;
	return *this;
}
float PointLight::GetConstant() const{
	return constant;
}
PointLight& PointLight::SetLinear(float _linear){
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