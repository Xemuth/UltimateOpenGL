#ifndef _UltimateOpenGL_UltimateOpenGl_h_
#define _UltimateOpenGL_UltimateOpenGl_h_

#include "Definition.h"

Upp::String& IncludeShader(Upp::String& shader);

#include "Texture.h"
#include "Shader.h"
#include "GameObject.h"
#include "Object3D.h"
#include "Mesh.h"

#include "Scene.h"
#include "Camera.h"

#include <chrono>
#include <ctime>

class UltimateOpenGL_Context{
	private:
		Upp::ArrayMap<Upp::String,Scene> AllScenes;
		Scene* ActiveScene=nullptr;
		
		Upp::Sizef ScreenSize;

		//Time Manageur
		Upp::Thread timeManageur;
		double GetTime();
		void StartTimer();
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
		std::chrono::time_point<std::chrono::high_resolution_clock> end;
		bool started = false;
		double Timer=0.0f;
		double DeltaTime=0.0f;
		double LastTime =0.0f; //Used to calculate FPS
		double lastFrame =0.0f;
		int bufferFrame =0;
		int frameCount = 0; //FPS
		
		//textures
		inline static int TextureCompteur=0;
		Upp::VectorMap<Upp::String,Texture> textures;
	
	public:
		UltimateOpenGL_Context();
		~UltimateOpenGL_Context();
		
		//time Management
		double GetEllapsedTime();	
		double GetDeltaTime();
		int GetFPS();
		
		//ScreenManagement
		void SetScreenSize(float ScreenHeight,float ScreenWidth);
		Upp::Sizef GetScreenSize();
		
		
		//scenes
		Scene& AddScene(const Upp::String& name);
		Scene& GetActiveScene();
		Scene& GetScene(const Upp::String& name);
		Upp::ArrayMap<Upp::String,Scene>& GetAllScene();
		bool SetActiveScene(const Upp::String& name);
		bool RemoveScene(const Upp::String& nameOfScene);
		
		//Texture Management
		Texture AddTexture(const Upp::String& TextureName,const Upp::String& textureFilePath,TextureType  _type = DIFFUSE ,bool loadDefault=false, bool flipLoad=true);
		Texture GetTexture(const Upp::String& TextureName);
		Upp::VectorMap<Upp::String,Texture>& GetTextures();
		
		void Draw();
		
		
		//****************Static part **************************
		Upp::Vector<int> TransformGlmColorToRGB(glm::vec3 FloatColor);
		Upp::Vector<int> TransformFloatColorToRGB(float RedFloat,float GreenFloat,float BlueFloat);
		glm::vec3 TransformRGBToFloatColor(int Red,int Green,int Blue);
		glm::vec3 TransformVectorToFloatColor(Upp::Vector<int> rgb);
		int ColorUniformisation(int ColorRgb);
		float ColorUniformisation(float ColorFloat);
};


#endif