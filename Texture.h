#ifndef _UltimateOpenGl_V3_Texture_h_
#define _UltimateOpenGl_V3_Texture_h_

#include "Definition.h"

struct TextureParameter : Upp::Moveable<TextureParameter>{
	public:
			TextureParameter();
			TextureParameter(GLenum _target,GLenum _pname,int _params);
			TextureParameter(const TextureParameter& _textureParameter);
			TextureParameter& operator=(const TextureParameter& _textureParameter);
			
			GLenum target;
			GLenum pname;
			int params;
};

class Texture : Upp::Moveable<Texture>{ //only support 2D textures actually //Don't support IML format and UPP FileImage system atm
	private:
		Upp::String pathTexture = "";
		Upp::String textureName = "";
		GLenum format=GL_RGB;
		TextureType type =DIFFUSE;
		
		
		int width=-1;
		int height=-1;
		int nrChannels=-1;
		
		//unsigned char *data; //Data of texture, Its commented because I delete this at the end of Loading so.. Maybe in further we gonna uncomment it
		//Texture dont have "parameteri wrapper" atm
		bool GenerateMipMap = true;
		
		bool loaded =false;
		unsigned int ID;
		void LoadDefaultname();
		
		int TextureIterator =0; //Used to track the number of generated texture  (allow us to do some glActiveTexture(GL_TEXTURE1) where 1 depending on int Number of Generated Texture)
		
		Upp::Vector<TextureParameter> textureparameters; //Vector of parameter. Must be redifed to set custom texture loading parameter
		void LoadTextureParameter();
	public:
	//glDeleteTextures(1, &texname);
	
	Texture();
	Texture(Upp::String _path);
	Texture(Upp::String _path,Upp::String _Name);
	Texture(const Texture& texture);
	~Texture();
	
	Texture& operator=(const Texture& texture);
	
	void LoadDefaultTextureParameter();
	void ClearTextureParameter();
	void SetTextureParameter(Upp::Vector<TextureParameter>& parameters);
	TextureParameter& AddTextureParameter(TextureParameter parameter);
	void RemoveTextureParameter(int number);
	const Upp::Vector<TextureParameter>& GetTextureParameters()const;
	
	bool SetPath(Upp::String _path);
	void SetName(Upp::String _name); //By Default the name of the texture gonna be is name from loading files
	
	Texture& SetType(TextureType _type);
	TextureType GetType();
	
	bool IsMipMap() const; //Return if GenerateMipMap is to true or false
	void ActivateMipMapGeneration(bool _activate);
	
	unsigned int GetId() const;
	Upp::String GetName() const;
	Upp::String GetPath() const;
	int GetTextureIterator() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetNrChannels() const;
	TextureColorSample GetTextureColor() const;
	
	bool IsLoaded() const;
	bool Load(unsigned int ActiveIterator =0,bool loadDefault = false,bool flipLoad = true);
	bool Use();
	bool Remove();
};
#endif
