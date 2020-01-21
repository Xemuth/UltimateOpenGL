#ifndef _UltimateOpenGL_UltimateOpenGL_h_
#define _UltimateOpenGL_UltimateOpenGL_h_
/*
	Main class of Ultimate OpenGL.
	Context is carrying a scenes set and
	give flexible way to load and unload scene
*/
#include <chrono>
#include <ctime>

#include "Definition.h"
#include "Texture.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"

#include "Scene.h"
#include "GameObject.h"
#include "Object3D.h"
#include "Mesh.h"

class UltimateOpenGL_Context{
	private:
		/*
			Set of scenes
		*/
		Upp::ArrayMap<Upp::String,Scene> AllScenes; //all scene
		Scene* ActiveScene=nullptr; //Context must have active Scene.
		
		Upp::Sizef ScreenSize; //size of window carrying the OpenGL Context

		//Time Manageur
		/*
			Time is importante to generate random things
			or make rotation using COS or SIN function.
		*/
		Upp::Thread timeManageur;
		bool started = false; //Set to true when context is loaded
		std::chrono::time_point<std::chrono::high_resolution_clock> start,end; //High resolution clock
		double DeltaTime=0.0f,LastTime=0.0f,lastFrame =0.0f,Timer=0.0f; //Used to calculate time since behining
		int bufferFrame =0,frameCount = 0; //used to calculate FPS
		
		double GetTime();
		void StartTimer();
		
		//Textures
		/*
			Textures is loaded and carried by context.
			Thanks to this, we can reuse multiple texture in differente scene
			without having to load them multiple time.
			However, if you want to do optimised scene load and unload you must
			don't forget to load texture and Unload texture using AddTexture and RemoveTexture
			To retrieve loaded texture in scene or 3D object,  you must use GetTexture(String name);
		*/
		inline static int TextureCompteur=0;
		Upp::ArrayMap<Upp::String,Texture> textures;
	public:
		UltimateOpenGL_Context();
		UltimateOpenGL_Context(float ScreenHeight,float ScreenWidth);
		~UltimateOpenGL_Context();
		
		//time Management
		double GetEllapsedTime();
		double GetDeltaTime();
		int GetFPS();
		
		//ScreenManagement
		void SetScreenSize(float ScreenHeight,float ScreenWidth);
		Upp::Sizef GetScreenSize();
		
		//scenes Management
		/*
			See explications above.
			Without scene setted, Context.Draw will raise Assertion
		*/
		Scene& AddScene(const Upp::String& name);
		Scene& GetActiveScene();
		Scene& GetScene(const Upp::String& name);
		Upp::ArrayMap<Upp::String,Scene>& GetAllScene();
		bool SetActiveScene(const Upp::String& name);
		bool RemoveScene(const Upp::String& nameOfScene);
		
		//Texture Management
		/*
			See explications above
		*/
		Texture AddTexture(const Upp::String& TextureName,const Upp::String& textureFilePath,TextureType  _type = TT_DIFFUSE ,bool loadDefault=false, bool flipLoad=true);
		bool RemoveTexture(const Upp::String& TextureName);
		Texture GetTexture(const Upp::String& TextureName);
		Upp::ArrayMap<Upp::String,Texture>& GetTextures();
		
		void Trace(bool b = true);//Log all OpenGL Error
		void Draw(const Upp::String& SceneToDraw="",const Upp::String& CameraToUse=""); //Draw active scene and active camera or specified Scene and camera to draw
		void Initialise(); //Initialise context, start timer,  setup some OpenGL Feature like anti allising or blendig
		
		//****************Static part **************************
		/*
			This part give some basic function to do
			convertion of String path or Color transformation
		*/
		static Upp::Vector<int> TransformGlmColorToRGB(glm::vec3 FloatColor);
		static Upp::Vector<int> TransformFloatColorToRGB(float RedFloat,float GreenFloat,float BlueFloat);
		static glm::vec3 TransformRGBToFloatColor(int Red,int Green,int Blue);
		static glm::vec3 TransformVectorToFloatColor(Upp::Vector<int> rgb);
		static int ColorUniformisation(int ColorRgb);
		static float ColorUniformisation(float ColorFloat);
		static Upp::String TransformFilePath(const Upp::String& FilePath);
};

#endif
