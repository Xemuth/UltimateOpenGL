#ifndef _UltimateOpenGL_Texture_h_
#define _UltimateOpenGL_Texture_h_
#include "Definition.h"


class Texture : Upp::Moveable<Texture>{ //only support 2D textures actually //Don't support IML format and UPP FileImage system atm
	private:
		Upp::String Path = "";
		Upp::String Name = "";
		
		GLenum format=GL_RGB;
		TextureType type =TT_DIFFUSE;

		int width=-1,height=-1,nrChannels=-1;
		int TextureIterator =0; //Used to track the number of generated texture  (allow us to do some glActiveTexture(GL_TEXTURE1) where 1 depending on int Number of Generated Texture)
		bool MipMap = true,Loaded = false, FlipOnLoad = true;
		unsigned int ID;
		
		Texture& LoadDefaultname();
		
		Upp::Array<TextureParameter> textureParameters; //Vector of parameter. Must be redifed to do custom texture loading
		Texture& LoadTextureParameter();
	public:
		Texture();
		Texture(const Upp::String& _path);
		Texture(const Upp::String& _path,const Upp::String& _Name);
		Texture(const Texture& texture);
		Texture& operator=(const Texture& texture);
		~Texture();

		Texture& LoadDefaultTextureParameter();
		TextureParameter& AddTextureParameter(const TextureParameter& parameter);
		Upp::Array<TextureParameter>& GetTextureParameters();

		Texture& SetPath(const Upp::String& _path);
		Upp::String GetPath();
		Texture& SetName(const Upp::String& _name); //By Default the name of the texture gonna be is name from loading files
		Upp::String GetName();
		Texture& SetType(TextureType _type);
		TextureType GetType();

		unsigned int GetId();

		bool IsMipMap(); //Return if GenerateMipMap is to true or false
		Texture& EnableMipMap();
		Texture& DisableMipMap();
		
		bool IsFlippedOnLoad();
		Texture& EnableFlipOnLoad();
		Texture& DisableFlipOnLoad();

		int GetTextureIterator();
		int GetWidth();
		int GetHeight();
		int GetNrChannels();

		bool IsLoaded();
		bool Load(unsigned int ActiveIterator =0);
		bool Reload(unsigned int ActiveIterator =0);
		bool Unload();
		bool Use();
};

//*****************Material***********//
struct TextureInformation{
		Texture texture;
		Texture Specular;
		// IF their is other kind of teture to add we must set it here and Update Generate
		// Shader
		float mix = 1.0f;
		
		TextureInformation& SetTexture(Texture& _texture);
		TextureInformation& SetSpecular(Texture& _specular);
		TextureInformation& SetMix(float _mix = 1.0f);
};
class Material{
	private:
		float shininess=32.0f; //Brilliance en français
		float diffuse=0.64f; //Valeur de diffusion

		glm::vec3 ambient=glm::vec3(1.0f, 0.5f, 0.31f);
	    glm::vec3 specular=glm::vec3(0.5f, 0.5f, 0.5f);

		enum WhatToUse{TEXTURE, COLOR, BOTH};
		WhatToUse whatToUse = COLOR;// what to use.
		Upp::Array<TextureInformation> TextureToUse; //the float is used to setUp the MixValue of texture. If you only have one texture set it to One.
															  //If you have 2 textures and want a 50%/50% set it to 0.5f to boath texture.
										  
		bool defaultColor = true; //used to know if this material have been modified and should be use.
		glm::vec4 color = glm::vec4( glm::vec3(1,1,1),1.0f); //Color with Alpha
	public:
		Material()= default;
		Material(const Material& material);
		Material& operator=(const Material& material);
		
		Material& UseTextures();
		Material& UseColor();
		Material& UseBoth();

		Material& SetColor(int red,int green, int blue, int alpha);
		Material& SetColor(glm::vec4 _color);
		Material& SetColor(glm::vec3 _color);
		
		Material& SetDiffuse(float _diffuse);
		Material& SetShininess(float _shininess);
		Material& SetAmbient(glm::vec3 _ambient);
		Material& SetSpecular(glm::vec3 _specular);
		
		glm::vec4 GetColor();
		glm::vec3 GetAmbient();
		glm::vec3 GetSpecular();
		float GetShininess();
		float GetDiffuse();
		
		TextureInformation& AddTexture(Texture& t1, float MixValueInPourcentage = 1.0f, Texture tSpeculare = Texture());
		Material& RemoveTexture(int iterator =0);
		Upp::Array<TextureInformation>& GetTextures();
		
		bool HaveBeenSetUp(); //Used to know if this Material have been setting up (to know if UOGL should use it).
};

#endif
