#include "Material.h"
Upp::String Material::GetName(){
	return name;
}
Material& Material::SetName(const Upp::String& _name){
	name = _name;
}
float Material::GetShininess(){
	return shininess;
}
Material& Material::SetShininess(float _Shininess){
	shininess = _Shininess;
	return *this;
}
float Material::GetDiffuse(){
	return diffuse;
}
Material& Material::SetDiffuse(float _Diffuse){
	diffuse = _Diffuse;
	return *this;
}
glm::vec3 Material::GetAmbient(){
	return ambient;
}
Material& Material::SetAmbient(glm::vec3 _Ambient){
	ambient = _Ambient;
	return *this;
}
glm::vec3 Material::GetSpecular(){
	return  specular;
}
Material& Material::SetSpecular(glm::vec3 _Specular){
	specular = _Specular;
	return *this;
}
//**********************Color*******************************
Color_Material::Color_Material(){}
Color_Material::Color_Material(int red,int green, int blue){
	color = glm::vec4(TransformRGBToFloatColor(red,green,blue),1.0f);
}
Color_Material::Color_Material(int red,int green, int blue, int alpha){
	color = TransformRGBAToFloatColor(red,green,blue,alpha);
}
Color_Material::Color_Material(glm::vec4 _color){
	color = _color;
}
Color_Material::Color_Material(glm::vec3 _color){
	color =glm::vec4(_color,1.0f);
}
Color_Material& Color_Material::operator=(Material& material){
	color = material.color;
	return *this;
}
Color_Material& Color_Material::SetColor(int red,int green, int blue, int alpha){
	color = TransformRGBAToFloatColor(red,green,blue,alpha);
}
Color_Material& Color_Material::SetColor(int red,int green, int blue){
	color = glm::vec4(TransformRGBToFloatColor(red,green,blue),1.0f);
}
Color_Material& Color_Material::SetColor(glm::vec4 _color){
	color = _color;
}
Color_Material& Color_Material::SetColor(glm::vec3 _color){
	color =glm::vec4(_color,1.0f);
}

Color_Material& Color_Material::Load(){
}
Color_Material& Color_Material::Unload(){
}
Color_Material& Color_Material::Reload(){
}
Color_Material& Color_Material::IsLoaded(){
	return true; //Color don't need loading
}
Color_Material& Color_Material::Use(){
	//since color don't need to be store in GPU their is not reason to bind anythings
}
const Upp::String& Color_Material::GetShaderDataStructure(){//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return MATERIAL_COLOR_STRUCT();
}
const Upp::String& Color_Material::GetShaderMaterialFunction(){//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return "";
}
const Upp::String& Color_Material::GetShaderMaterialPrototypeFunction(){//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return "";
}
void Color_Material::SentToShader(Shader& shader,const Upp::String& CustomName){//This function MUST be rewritted in inherrited class, It will allow UOGL to send good data to a shader
	if(shader.IsCompiled()){
		Upp::String nameToUse = (CustomName.GetCount()>0)? CustomName : name;
		shader.SetVec3(nameToUse +".ambient",ambient);
		shader.SetVec4(nameToUse +".diffuse",color);
		shader.SetVec3(nameToUse +".specular",specular);
		shader.SetFloat(nameToUse +".shininess",shininess);
	}
}
//***********************Texture2D**************************
Texture2D::Texture2D(){}
Texture2D::Texture2D(const Upp::String& _path){
	if(FileExists(_path)){
		Path = Upp::Replace(_path,Upp::Vector<Upp::String>{"\\"},Upp::Vector<Upp::String>{"/"});
		LoadDefaultname();
	}else{
		LOG("Error : Texture2D::Texture2D(...) FilePath : \""+ _path +"\" is incorrect !");
	}
}
Texture2D::Texture2D(const Upp::String& _path,const Upp::String& _Name){
	if(FileExists(_path)){
		Path = Upp::Replace(_path,Upp::Vector<Upp::String>{"\\"},Upp::Vector<Upp::String>{"/"});
	}else{
		LOG("Error : Texture2D::Texture2D(...) FilePath : \""+ _path +"\" is incorrect !");
	}
	name = _Name;
}
Texture2D::Texture2D(const Texture2D& texture){
	Path = texture.Path;
	name = texture.name;
	width = texture.width;
	height = texture.height;
	nrChannels = texture.nrChannels;
	MipMap = texture.MipMap;
	Loaded = texture.Loaded;
	if(Loaded)ID = texture.ID;
	TextureIterator = texture.TextureIterator;
	textureParameters.Append(texture.textureParameters);
}
Texture2D& Texture2D::operator=(Texture2D& texture){
	Path = texture.Path;
	SpecularPath = SpecularID;
	name = texture.name;
	width = texture.width;
	height = texture.height;
	nrChannels = texture.nrChannels;
	MipMap = texture.MipMap;
	Loaded = texture.loaded;
	if(loaded)ID = texture.ID;
	if(SpecularLoaded)SpecularID = texture.SpecularID;
	TextureIterator = texture.TextureIterator;
	textureParameters.Append(texture.textureParameters);
	return *this;
}
Texture2D::~Texture2D(){
	//This part is commented since Texture2D is working on Copying paradigme
	//So Getting a texture to retrieve ID would make it unload when the object is deleted
	//To Unload texture from graphic card, you must use Unload Function
	/*if(loaded){
		glDeleteTextures(1, &ID);
	}*/
}


Texture2D& Texture2D::LoadDefaultname(){
	name = Path.Right(Path.GetCount()- Path.ReverseFind("/") -1);
	name = name.Left(name.Find("."));
	LOG("Info : Texture2D::LoadDefaultname() default name have been setted : \""+ name +"\"");
	return *this;
}

Texture2D& Texture2D::LoadDefaultTextureParameter(){
	textureParameters.Clear();
	textureParameters.Append(Upp::Array<TextureParameter>{
		TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,(int)GL_REPEAT),
		TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)GL_REPEAT),
		TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,(int) GL_LINEAR_MIPMAP_LINEAR),
		TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,(int) GL_NEAREST),
	});
	LOG("Info : Texture::LoadDefaultTextureParameter() Default Texture parameter have been loaded for texture named : \""+ Name +"\"");
	return *this;
}
TextureParameter& Texture2D::AddTextureParameter(const TextureParameter& parameter){
	return textureParameters.Add(parameter);
}
Upp::Array<TextureParameter>& Texture2D::GetTextureParameters(){
	return textureParameters;
}

Texture2D& Texture2D::SetPath(const Upp::String& _path){
	if(FileExists(_path)){
		Path = _path;
	}else{
		LOG("Warning : Texture::SetPath(...) Texture file path is incorrect !");
	}
	return *this;
}
Upp::String Texture2D::GetPath(){
	return Path;
}
Texture2D& Texture2D::SetType(TextureType _type){
	type = _type;
}
TextureType Texture2D::GetType(){
	return type;
}

unsigned int Texture2D::GetId(){
	return ID;
}

bool Texture2D::IsMipMap(){ //Return if GenerateMipMap is to true or false
	return MipMap;
}
Texture2D& Texture2D::EnableMipMap(){
	MipMap = true;
	return  *this;
}
Texture2D& Texture2D::DisableMipMap(){
	MipMap = false;
	return  *this;
}

bool Texture2D::IsFlippedOnLoad(){
	return FlipOnLoad;
}
Texture2D& Texture2D::EnableFlipOnLoad(){
	FlipOnLoad = true;
	return  *this;
}
Texture2D& Texture2D::DisableFlipOnLoad(){
	FlipOnLoad = false;
	return  *this;
}

int Texture2D::GetTextureIterator(){
	return TextureIterator;
}
int Texture2D::GetWidth(){
	return width;
}
int Texture2D::GetHeight(){
	return height;
}
int Texture2D::GetNrChannels(){
	return nrChannels;
}

Texture2D& Texture2D::SetSpecularPath(Upp::String& _SpecularePath){
	if(FileExists(_SpecularePath)){
		SpecularPath = _SpecularePath;
	}else{
		LOG("Warning : Texture::SetSpecularPath(...) Texture file path is incorrect !");
	}
	return *this;
}
Upp::String Texture2D::GetSpecularPath(){
	return SpecularePath;
}

Texture2D& Texture2D::Load(){
	if(Upp::FileExists(Path)){
		if(!loaded){
			ID=0;
			glGenTextures(1, &ID);
			glActiveTexture(GL_TEXTURE0 + TextureIterator);
			glBindTexture(GL_TEXTURE_2D, ID);
			TextureIterator++;
			
			if(textureParameters.GetCount() ==0)LoadDefaultTextureParameter();
			LoadTextureParameter();
		
			stbi_set_flip_vertically_on_load(FlipOnLoad);
			unsigned char *data=nullptr; //Comment this and un comment Class attribute Data* to load it in our unsigned char* buffer
			data = stbi_load(Path.ToStd().c_str(), &width, &height, &nrChannels, 0);
			if(data){
		        if (nrChannels == 1)
		            format = GL_RED;
		        else if (nrChannels == 3)
		            format = GL_RGB;
		        else if (nrChannels == 4)
		            format = GL_RGBA;
		        if(format ==GL_RGBA){
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		        }
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				if(MipMap)glGenerateMipmap(GL_TEXTURE_2D);
					loaded =true;
			}else{
				LOG("Error : Texture2D::Load(...) Error on loading texture named : \""+ name +"\" !");
				loaded=false;
			}
			stbi_image_free(data); //If we manage to use class attribute, we must dont forget to clean it in class destructor ! it's very important to prevent memory leak
			if(SpecularPath.GetCount()> 0){
				SpecularID = 0;
				glGenTextures(1, &SpecularID);
				glActiveTexture(GL_TEXTURE0 + TextureIterator);
				glBindTexture(GL_TEXTURE_2D, SpecularID);
				TextureIterator++;
				data = stbi_load(SpecularPath.ToStd().c_str(), &width, &height, &nrChannels, 0);
				if(data){
					if (nrChannels == 1)
			            format = GL_RED;
			        else if (nrChannels == 3)
			            format = GL_RGB;
			        else if (nrChannels == 4)
			            format = GL_RGBA;
			        if(format ==GL_RGBA){
						glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			        }
					glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
					if(MipMap)glGenerateMipmap(GL_TEXTURE_2D);
						SpecularLoaded =true;
				}
				
			}else{
				LOG("Error : Texture2D::Load(...) Error on loading Speculare texture named : \""+ name +"\" !");
			}
			stbi_image_free(data);
		}else{
			LOG("Error : Texture2D::Load(...) Texture named : \""+ name +"\" is already loaded ! Use Unload() Function to Unload or Reload() to reload the texture");
			return false;
		}
	}else{
		LOG("Error : Texture2D::Load(...) FilePath : \""+ Path +"\" of texture named : \""+ name +"\" is incorrect !");
		Loaded =false;
	}
	return *this;
}
Texture2D& Texture2D::Unload(){
	if(loaded){
		glDeleteTextures(1, &ID);
		loaded = false;
		return true;
	}else{
		LOG("Warning : Texture2D::Unload() Nothing to remove !");
	}
	return *this;
}
Texture2D& Texture2D::Reload(){
	if(Loaded){
		glDeleteTextures(1, &ID);
		loaded = false;
		Load();
	}else{
		Load();
	}
	return *this;
}
bool Texture2D::IsLoaded(){
	return loaded;
}
Texture2D& Texture2D::Use(){
	if(Loaded){
		glActiveTexture(GL_TEXTURE0 +TextureIterator);
		glBindTexture(GL_TEXTURE_2D, ID);
		return *this;
	}else{
		LOG("Warning Use(): Can't use an Unloaded texture !");
	}
	return *this;
}

const Upp::String& Texture2D::GetShaderDataStructure(){ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return MATERIAL_TEXTURE_STRUCT();
}
const Upp::String& Texture2D::GetShaderMaterialFunction(){ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	//TODO
}
const Upp::String& Texture2D::GetShaderMaterialPrototypeFunction(){//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	//TODO
}
void Texture2D::SentToShader(Shader& shader,const Upp::String& CustomName){//This function MUST be rewritted in inherrited class, It will allow UOGL to send good data to a shader
	//TODO
}