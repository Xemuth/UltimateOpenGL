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
		Texture(Upp::String _path);
		Texture(Upp::String _path,Upp::String _Name);
		Texture(const Texture& texture);
		Texture& operator=(const Texture& texture);
		~Texture();

		Texture& LoadDefaultTextureParameter();
		TextureParameter& AddTextureParameter(const TextureParameter& parameter);
		Upp::Array<TextureParameter>& GetTextureParameters();

		Texture& SetPath(Upp::String _path);
		Upp::String GetPath();
		Texture& SetName(Upp::String _name); //By Default the name of the texture gonna be is name from loading files
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

#endif
