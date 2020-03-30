#ifndef _UltimateOpenGL_Material_h_
#define _UltimateOpenGL_Material_h_
#include "Definition.h"
class Shader;
class Material {
	protected:
		Upp::String name = "default";
		
		float shininess=32.0f; //Brilliance en français
		float diffuse=0.64f; //Valeur de diffusion

		glm::vec3 ambient=glm::vec3(1.0f, 0.5f, 0.31f);
	    glm::vec3 specular=glm::vec3(0.5f, 0.5f, 0.5f);
	    
	    bool loaded = false;
	public:
		float GetShininess();
		Material& SetShininess(float _Shininess);
		float GetDiffuse();
		Material& SetDiffuse(float _Diffuse);
		glm::vec3 GetAmbient();
		Material& SetAmbient(glm::vec3 _Ambient);
		glm::vec3 GetSpecular();
		Material& SetSpecular(glm::vec3 _Specular);
		
		Upp::String GetName()const;
		Material& SetName(const Upp::String& _name);
		
		virtual ~Material();
		virtual Material& operator=(Material& material); //YOU MUST ADD COPY OPERATOR IN INHERITTED CLASS
		
		virtual Material& Load() = 0;
		virtual Material& Unload() = 0;
		virtual Material& Reload() = 0;
		virtual bool IsLoaded()const = 0;
		virtual const Material& Use()const = 0;
		
		
		/*** Obsolete ***/
		
		virtual Upp::String GetCalculationCode(const Upp::String& CustomName ="")const= 0;
		virtual Upp::String GetShaderNameStructure()const= 0;
		virtual Upp::String GetShaderDataStructure()const= 0; //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual Upp::String GetShaderMaterialFunction()const= 0; //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual Upp::String GetShaderMaterialPrototypeFunction()const= 0;//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual void SentToShader(Shader& shader,const Upp::String& CustomName = "")const= 0; //This function MUST be rewritted in inherrited class, It will allow UOGL to send good data to a shader
};

class Color_Material : public Material{ //this one is named :Color_Material instead of Color becasue Color already exist in Upp Namespace
	private:
		glm::vec4 color = glm::vec4(glm::vec3(1,1,1),1.0f); //Color with Alpha
	public:
		Color_Material();
		Color_Material(int red,int green, int blue);
		Color_Material(int red,int green, int blue, int alpha);
		Color_Material(glm::vec4 _color);
		Color_Material(glm::vec3 _color);
		
		Color_Material& SetColor(int red,int green, int blue, int alpha);
		Color_Material& SetColor(int red,int green, int blue);
		Color_Material& SetColor(glm::vec4 _color);
		Color_Material& SetColor(glm::vec3 _color);

		glm::vec4 GetColor(){return color;}
		
		virtual ~Color_Material();
		virtual Color_Material& operator=(Color_Material& material);
		
		virtual Color_Material& Load();
		virtual Color_Material& Unload();
		virtual Color_Material& Reload();
		virtual bool IsLoaded()const;
		virtual const Color_Material& Use()const;

		virtual Upp::String GetCalculationCode(const Upp::String& CustomName ="")const;
		virtual Upp::String GetShaderNameStructure()const;
		virtual Upp::String GetShaderDataStructure()const; //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual Upp::String GetShaderMaterialFunction()const; //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual Upp::String GetShaderMaterialPrototypeFunction()const;//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual void SentToShader(Shader& shader,const Upp::String& CustomName = "")const; //This function MUST be rewritted in inherrited class, It will allow UOGL to send good data to a shader
};

class Texture2D : public Material{
	private:
		Upp::String Path = "";
		Upp::String SpecularPath = "";
		
		inline static int TextureCount =0;
		
		GLenum format=GL_RGB;
		TextureType type =TT_DIFFUSE;
		
		int width=-1,height=-1,nrChannels=-1;
		int TextureIterator =0; //Used to track the number of generated texture  (allow us to do some glActiveTexture(GL_TEXTURE1) where 1 depending on int Number of Generated Texture)
		bool MipMap = true, FlipOnLoad = true;
		
		unsigned int ID;
		unsigned int SpecularID;
		bool SpecularLoaded = false;
	
		Texture2D& LoadDefaultname();
		Upp::Array<TextureParameter> textureParameters; //Vector of parameter. Must be redifed to do custom texture loading
		Texture2D& LoadTextureParameter();
	public:
		Texture2D();
		Texture2D(const Upp::String& _path);
		Texture2D(const Upp::String& _path,const Upp::String& _Name);
		Texture2D(const Texture2D& texture);
		Texture2D& operator=(Texture2D& texture);
		virtual ~Texture2D();
		
		Texture2D& LoadDefaultTextureParameter();
		TextureParameter& AddTextureParameter(const TextureParameter& parameter);
		Upp::Array<TextureParameter>& GetTextureParameters();

		Texture2D& SetPath(const Upp::String& _path);
		Upp::String GetPath();
		Texture2D& SetType(TextureType _type);
		TextureType GetType();

		unsigned int GetId();

		bool IsMipMap(); //Return if GenerateMipMap is to true or false
		Texture2D& EnableMipMap();
		Texture2D& DisableMipMap();
		
		bool IsFlippedOnLoad();
		Texture2D& EnableFlipOnLoad();
		Texture2D& DisableFlipOnLoad();

		int GetTextureIterator();
		int GetWidth();
		int GetHeight();
		int GetNrChannels();
		
		Texture2D& SetSpecularPath(const Upp::String& _SpecularePath);
		Upp::String GetSpecularPath();
		
		virtual Texture2D& Load();
		virtual Texture2D& Unload();
		virtual Texture2D& Reload();
		virtual bool IsLoaded()const;
		virtual const Texture2D& Use()const;
		
		
		virtual Upp::String GetCalculationCode(const Upp::String& CustomName ="")const;
		virtual Upp::String GetShaderNameStructure()const;
		virtual Upp::String GetShaderDataStructure()const; //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual Upp::String GetShaderMaterialFunction()const; //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual Upp::String GetShaderMaterialPrototypeFunction()const;//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
		virtual void SentToShader(Shader& shader,const Upp::String& CustomName = "")const; //This function MUST be rewritted in inherrited class, It will allow UOGL to send good data to a shader
};
#endif
