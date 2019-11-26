#include "UltimateOpenGL.h"

#include "Simps/MaterialTexture.glsl"
#include "Simps/MaterialColor.glsl"
#include "Simps/PointLight.glsl"
#include "Simps/SpotLight.glsl"
#include "Simps/DirLight.glsl"

UltimateOpenGL_Context::UltimateOpenGL_Context(){
	StartTimer();
}
UltimateOpenGL_Context::~UltimateOpenGL_Context(){
}

Upp::String& IncludeShader(Upp::String& shader){
	shader.Replace("MATERIAL_TEXTURE_STRUCT()",MATERIAL_TEXTURE_STRUCT());
	shader.Replace("MATERIAL_COLOR_STRUCT()",MATERIAL_COLOR_STRUCT());
	
	shader.Replace("LIGHT_POINT_STRUCT()",LIGHT_POINT_STRUCT());
	shader.Replace("LIGHT_POINT_TEXTURE_PROTOTYPE()",LIGHT_POINT_TEXTURE_PROTOTYPE());
	shader.Replace("LIGHT_POINT_TEXTURE_FUNCTION()",LIGHT_POINT_TEXTURE_FUNCTION());
	shader.Replace("LIGHT_POINT_COLOR_PROTOTYPE()",LIGHT_POINT_COLOR_PROTOTYPE());
	shader.Replace("LIGHT_POINT_COLOR_FUNCTION()",LIGHT_POINT_COLOR_FUNCTION());
	
	shader.Replace("LIGHT_SPOT_STRUCT()",LIGHT_SPOT_STRUCT());
	shader.Replace("LIGHT_SPOT_TEXTURE_PROTOTYPE()",LIGHT_SPOT_TEXTURE_PROTOTYPE());
	shader.Replace("LIGHT_SPOT_TEXTURE_FUNCTION()",LIGHT_SPOT_TEXTURE_FUNCTION());
	shader.Replace("LIGHT_SPOT_COLOR_PROTOTYPE()",LIGHT_SPOT_COLOR_PROTOTYPE());
	shader.Replace("LIGHT_SPOT_COLOR_FUNCTION()",LIGHT_SPOT_COLOR_FUNCTION());
	
	shader.Replace("LIGHT_DIR_STRUCT()",LIGHT_DIR_STRUCT());
	shader.Replace("LIGHT_DIR_TEXTURE_PROTOTYPE()",LIGHT_DIR_TEXTURE_PROTOTYPE());
	shader.Replace("LIGHT_DIR_TEXTURE_FUNCTION()",LIGHT_DIR_TEXTURE_FUNCTION());
	shader.Replace("LIGHT_DIR_COLOR_PROTOTYPE()",LIGHT_DIR_COLOR_PROTOTYPE());
	shader.Replace("LIGHT_DIR_COLOR_FUNCTION()",LIGHT_DIR_COLOR_FUNCTION());
	shader = Replace(shader,Upp::Vector<Upp::String>{"\""},Upp::Vector<Upp::String>{""});
	return shader;
}


//time management
void UltimateOpenGL_Context::StartTimer(){
	start= std::chrono::high_resolution_clock::now();
}
double UltimateOpenGL_Context::GetTime(){
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end-start;
	Timer =diff.count();
	return Timer;
}
int UltimateOpenGL_Context::GetFPS(){
	return frameCount;
}
double UltimateOpenGL_Context::GetEllapsedTime(){
	return Timer;
}
double UltimateOpenGL_Context::GetDeltaTime(){
	return DeltaTime;
}


//Screen size
void UltimateOpenGL_Context::SetScreenSize(float ScreenHeight,float ScreenWidth){
	ScreenSize.cx = ScreenWidth;
	ScreenSize.cy = ScreenHeight;
}
Upp::Sizef UltimateOpenGL_Context::GetScreenSize(){
	return ScreenSize;
}

//scene
Scene& UltimateOpenGL_Context::AddScene(const Upp::String name){
	if(AllScenes.Find(name) ==-1){
		Scene& s=	AllScenes.Create(name,this);
		s.SetName(name);
		s.SetContext(this);
		if(ActiveScene == nullptr) ActiveScene = &s;
		return c;
	}else{
		return AllScenes.Get(name);
	}
}
Scene& UltimateOpenGL_Context::GetActiveScene(){
	if(ActiveScene == nullptr) throw UGLException(4,"Scene& UltimateOpenGL_Context::GetActiveScene() => No scene have been binded",1);
		return *ActiveScene;
}
Scene& UltimateOpenGL_Context::GetScene(const Upp::String& name){
	if(AllScenes.Find(name) !=-1){
		return AllScenes.Get(name);
	}
	throw UGLException(8,"Scene& UltimateOpenGL_Context::GetScene(const Upp::String& name) => Scene named "+ name +" Do not exists",1);
}
Upp::ArrayMap<Upp::String,Scene>& UltimateOpenGL_Context::GetAllScene(){
	return AllScenes;
}
bool UltimateOpenGL_Context::SetActiveScene(const Upp::String& name){
	if(AllScenes.Find(name) !=-1){
		ActiveScene = &AllScenes.Get(name);
		return true;
	}
	return false;
}
bool UltimateOpenGL_Context::RemoveScene(const Upp::String& nameOfScene){
	if(AllScenes.Find(name) !=-1){
		if(ActiveScene == &AllScenes.Get(name)) ActiveScene==nullptr;
		AllScenes.remove(AllScenes.Find(name));
		return true;
	}
	return false;
}

//Textures
bool UltimateOpenGL_Context::AddTextures(Upp::String TextureName,Upp::String textureFilePath, TextureColorSample colorSample, bool loadDefault, bool flipLoad){ //Add and load Texture
	if(textures.Find(TextureName) == -1){
		Texture& texture =textures.Add(TextureName);
		texture.SetPath(textureFilePath);
		texture.SetName(TextureName);
		texture.SetColorSample(colorSample);
		if(!texture.Load(TextureCompteur,loadDefault,flipLoad)){
			textures.RemoveKey(TextureName);
			LOG("Error : AddTextures(String,String,ENUM_TextureColorSample,bool,bool) Loading error !\n");
			return false;
		}else{
			LOG("Info : Texture " + TextureName +" Loaded with success !\n");
			TextureCompteur++;
			return true;
		}
		return false;
	}
	return false;
}
Upp::VectorMap<Upp::String,Texture>& UltimateOpenGL_Context::GetTextures(){
	return textures;
}

//Logique
void UltimateOpenGL_Context::Draw(){
	try{
		//HEre I must do DeltaTime Calculation
		float currentFrame = GetTime(); //Calcules le nombre de frames par seconde
		DeltaTime = currentFrame - lastFrame;//Calcules le nombre de frames par seconde
		lastFrame = currentFrame;//Calcules le nombre de frames par seconde
		
		//Fps counter
		bufferFrame++;
		if( currentFrame -  LastTime >= 1.0){
			frameCount =bufferFrame;
			bufferFrame=0;
			LastTime = currentFrame;
		}
		
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 transform = glm::mat4(1.0f);
		view = GetActiveScene().GetActiveCamera().GetViewMatrix();
		projection = glm::perspective(glm::radians(90.0f),(float)( ScreenSize.cx / ScreenSize.cy), 0.1f, 100.0f);
		GetActiveScene().Draw(model,view,projection,transform);
	}catch(UGLException exception){
		LOG("Error, context don't have anyScene to Draw ! " + Upp::AsString(exception.what()));
		exit(1);
	}
}