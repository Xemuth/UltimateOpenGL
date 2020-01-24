#include "UltimateOpenGL.h"
UltimateOpenGL_Context::UltimateOpenGL_Context(){
	ScreenSize.cx = 600;  //Default Size
	ScreenSize.cy = 600;
}
UltimateOpenGL_Context::UltimateOpenGL_Context(float ScreenHeight,float ScreenWidth){
	ScreenSize.cx = ScreenWidth;  //Default Size
	ScreenSize.cy = ScreenHeight;
}
UltimateOpenGL_Context::~UltimateOpenGL_Context(){}
void UltimateOpenGL_Context::Initialise(){
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);//Gestion of alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Gestion de l'alpha sur les textures
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO); //Gestion pour le RGB uniquement
	StartTimer();
}
void UltimateOpenGL_Context::Draw(const Upp::String& SceneToDraw,const Upp::String& CameraToUse){
	try{
		Scene* scene = &GetActiveScene();
		if(SceneToDraw.GetCount() > 0){
			if(AllScenes.Find(SceneToDraw) != -1){
				scene = &AllScenes.Get(SceneToDraw);
			}
		}
		//HEre I must do DeltaTime Calculation
		double currentFrame = GetTime(); //Calcules le nombre de frames par seconde //Changement made by Iñaki
		DeltaTime = currentFrame - lastFrame;//Calcules le nombre de frames par seconde
		lastFrame = currentFrame;//Calcules le nombre de frames par seconde
		//Fps counter
		bufferFrame++;
		if( currentFrame -  LastTime >= 1.0){
			frameCount =bufferFrame;
			bufferFrame=0;
			LastTime = currentFrame;
		}
		//scene->Draw(CameraToUse);
	}catch(UOGLException exception){
	//	ASSERT("Error have been raised to UltimateOpenGL_Context::Draw(...), " + Upp::AsString(+ exception.what()));
	}
}
Scene& UltimateOpenGL_Context::AddScene(const Upp::String& name){
	if(AllScenes.Find(name) ==-1){
		Scene& s= AllScenes.Create<Scene>(name);
		s.SetName(name);
		s.SetContext(*this);
		if(ActiveScene == nullptr) ActiveScene = &s;
		return s;
	}else{
		return AllScenes.Get(name);
	}
}
Scene& UltimateOpenGL_Context::GetActiveScene(){
	ASSERT_(ActiveScene == nullptr && AllScenes.GetCount() == 0,"UltimateOpenGL_Context have nullptr ActiveScene and don't have any scenes created");
	ActiveScene = &AllScenes[0];
	LOG("Warning : UltimateOpenGL_Context::GetActiveScene() have nullptr ActiveScene, Scene number 0 have been binded by default to active scene and has been returned");
	return *ActiveScene;
}
Scene& UltimateOpenGL_Context::GetScene(const Upp::String& name){
	if(AllScenes.Find(name) !=-1){
		return AllScenes.Get(name);
	}
	throw UOGLException(8,"Scene& UltimateOpenGL_Context::GetScene(const Upp::String& name) => Scene named "+ name +" Do not exists",1);
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
	if(AllScenes.Find(nameOfScene) !=-1){
		if(ActiveScene == &AllScenes.Get(nameOfScene)) ActiveScene=nullptr; //Changement made by Iñaki
		AllScenes.Remove(AllScenes.Find(nameOfScene));
		return true;
	}
	return false;
}
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
void UltimateOpenGL_Context::SetScreenSize(float ScreenHeight,float ScreenWidth){
	ScreenSize.cx = ScreenWidth;
	ScreenSize.cy = ScreenHeight;
}
Upp::Sizef UltimateOpenGL_Context::GetScreenSize(){
	return ScreenSize;
}
void GLAPIENTRY MessageCallback( GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message, void* userParam ){
	LOG("GL CALLBACK: " + Upp::String( (type == GL_DEBUG_TYPE_ERROR) ? "** GL ERROR **" : "" ) +" type = 0x" + type +", severity = 0x"+ severity +", message = " + message);
 // fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message );
}
void UltimateOpenGL_Context::Trace(bool b){
	if(b){
		glEnable( GL_DEBUG_OUTPUT );
		glDebugMessageCallback(static_cast<GLDEBUGPROC>(&MessageCallback), 0 );
	}else{
		glDisable( GL_DEBUG_OUTPUT );
	}
}
//Textures
Texture UltimateOpenGL_Context::AddTexture(const Upp::String& TextureName,const Upp::String& textureFilePath,TextureType _type,bool loadDefault, bool flipLoad){ //Add and load Texture
	if(textures.Find(TextureName) == -1){
		Texture& texture =textures.Add(TextureName);
		texture.SetPath(textureFilePath);
		texture.SetName(TextureName);
		if(!texture.Load(TextureCompteur)){
			textures.RemoveKey(TextureName);
			LOG("Error : UltimateOpenGL_Context::AddTexture(...) Loading error !\n");
			return Texture();
		}else{
			LOG("Info : UltimateOpenGL_Context::AddTexture(...) Texture " + TextureName +"  number " + Upp::AsString( TextureCompteur) +" Loaded with success with ID of " + Upp::AsString(texture.GetId()) +"!\n");
			TextureCompteur++;
			return texture;
		}
		return Texture();
	}
	LOG("Warning : UltimateOpenGL_Context::AddTexture(...) texture named " +TextureName +" already existe and have been loaded !");
	return textures.Get(TextureName);
}
Texture UltimateOpenGL_Context::GetTexture(const Upp::String& TextureName){
	if(textures.Find(TextureName)	!=-1)
		return textures.Get(TextureName);
	return Texture();
}

Upp::ArrayMap<Upp::String,Texture>& UltimateOpenGL_Context::GetTextures(){
	return textures;
}
