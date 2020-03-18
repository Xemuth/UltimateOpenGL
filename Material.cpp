#include "Material.h"
#include "Shader.h"

Material::~Material(){}
Material& Material::operator=(Material& material){
	name = material.name;
	
	shininess = material.shininess; //Brilliance en français
	diffuse = material.diffuse; //Valeur de diffusion

	ambient = material.ambient;
    specular = material.specular;
    
    loaded = material.loaded;
    return  *this;
}
Upp::String Material::GetName()const{
	return name;
}
Material& Material::SetName(const Upp::String& _name){
	name = _name;
	return *this;
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
Color_Material::~Color_Material(){}
Color_Material& Color_Material::operator=(Color_Material& material){
	color = material.color;
	return *this;
}
Color_Material& Color_Material::SetColor(int red,int green, int blue, int alpha){
	color = TransformRGBAToFloatColor(red,green,blue,alpha);
	return *this;
}
Color_Material& Color_Material::SetColor(int red,int green, int blue){
	color = glm::vec4(TransformRGBToFloatColor(red,green,blue),1.0f);
	return *this;
}
Color_Material& Color_Material::SetColor(glm::vec4 _color){
	color = _color;
	return *this;
}
Color_Material& Color_Material::SetColor(glm::vec3 _color){
	color =glm::vec4(_color,1.0f);
	return *this;
}

Color_Material& Color_Material::Load(){
	
	return *this;
}
Color_Material& Color_Material::Unload(){
	
	return *this;
}
Color_Material& Color_Material::Reload(){
	
	return *this;
}
bool Color_Material::IsLoaded()const{
	return true; //Color don't need loading
}
const Color_Material& Color_Material::Use()const{
	//since color don't need to be store in GPU their is not reason to bind anythings
	return  *this;
}

Upp::String Color_Material::GetNameOfStructure()const{
	return "MaterialColor";
}
Upp::String Color_Material::GetCalculationCode(const Upp::String& CustomName)const{
	Upp::String nameToUse = (CustomName.GetCount()>0)? CustomName : name;
	return "FragColor = " + nameToUse + ".diffuse;";
}
Upp::String Color_Material::GetShaderDataStructure()const{//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return MATERIAL_COLOR_STRUCT();
}
Upp::String Color_Material::GetShaderMaterialFunction()const{//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return "";
}
Upp::String Color_Material::GetShaderMaterialPrototypeFunction()const{//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return "";
}
void Color_Material::SentToShader(Shader& shader,const Upp::String& CustomName)const{//This function MUST be rewritted in inherrited class, It will allow UOGL to send good data to a shader
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
	loaded = texture.loaded;
	if(loaded)ID = texture.ID;
	TextureIterator = texture.TextureIterator;
	textureParameters.Append(texture.textureParameters);
}
Texture2D& Texture2D::operator=(Texture2D& texture){
	Path = texture.Path;
	SpecularPath = texture.SpecularPath;
	name = texture.name;
	width = texture.width;
	height = texture.height;
	nrChannels = texture.nrChannels;
	MipMap = texture.MipMap;
	loaded = texture.loaded;
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
	LOG("Info : Texture::LoadDefaultTextureParameter() Default Texture parameter have been loaded for texture named : \""+ name +"\"");
	return *this;
}
Texture2D& Texture2D::LoadTextureParameter(){
	for(TextureParameter& param :textureParameters){
		glTexParameteri(param.target,param.pname,param.params);
	}
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
	return  *this;
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

Texture2D& Texture2D::SetSpecularPath(const Upp::String& _SpecularePath){
	if(FileExists(_SpecularePath)){
		SpecularPath = _SpecularePath;
	}else{
		LOG("Warning : Texture::SetSpecularPath(...) Texture file path is incorrect !");
	}
	return *this;
}
Upp::String Texture2D::GetSpecularPath(){
	return SpecularPath;
}

Texture2D& Texture2D::Load(){
	if(Upp::FileExists(Path)){
		if(!loaded){
			ID=0;
			
			glGenTextures(1, &ID);
			TextureIterator = TextureCount;
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
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				if(MipMap)glGenerateMipmap(GL_TEXTURE_2D);
				loaded =true;
				TextureCount++;
			}else{
				LOG("Error : Texture2D::Load(...) Error on loading texture named : \""+ name +"\" !");
				loaded=false;
			}
			stbi_image_free(data); //If we manage to use class attribute, we must dont forget to clean it in class destructor ! it's very important to prevent memory leak
			if(SpecularPath.GetCount() > 0){
				SpecularID = 0;
				glGenTextures(1, &SpecularID);
				TextureIterator = TextureCount;
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
					TextureCount++;
				}else{
					LOG("Error : Texture2D::Load(...) Error on loading Speculare texture named : \""+ name +"\" !");
				}
				stbi_image_free(data);
			}
		}else{
			LOG("Error : Texture2D::Load(...) Texture named : \""+ name +"\" is already loaded ! Use Unload() Function to Unload or Reload() to reload the texture");
			return *this;
		}
	}else{
		LOG("Error : Texture2D::Load(...) FilePath : \""+ Path +"\" of texture named : \""+ name +"\" is incorrect !");
		loaded =false;
	}
	return *this;
}
Texture2D& Texture2D::Unload(){
	if(loaded){
		glDeleteTextures(1, &ID);
		TextureCount--;
		loaded = false;
	}else{
		LOG("Warning : Texture2D::Unload() Nothing to remove !");
	}
	return *this;
}
Texture2D& Texture2D::Reload(){
	if(loaded){
		glDeleteTextures(1, &ID);
		TextureCount--;
		loaded = false;
		Load();
	}else{
		Load();
	}
	return *this;
}
bool Texture2D::IsLoaded()const{
	return loaded;
}
const Texture2D& Texture2D::Use()const{
	if(loaded){
		glActiveTexture(GL_TEXTURE0 +TextureIterator);
		glBindTexture(GL_TEXTURE_2D, ID);
		if(SpecularLoaded){
			glActiveTexture(GL_TEXTURE0 +TextureIterator+1);
			glBindTexture(GL_TEXTURE_2D, SpecularID);
		}
		return *this;
	}else{
		LOG("Warning Use(): Can't use an Unloaded texture !");
	}
	return *this;
}
Upp::String Texture2D::GetCalculationCode(const Upp::String& CustomName)const{
	Upp::String nameToUse = (CustomName.GetCount()>0)? CustomName : name;
	return "FragColor = texture(" + nameToUse + ".diffuse, TextureCoordinate);";
}
Upp::String Texture2D::GetNameOfStructure()const{
	return "MaterialTexture";
}
Upp::String Texture2D::GetShaderDataStructure()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	return MATERIAL_TEXTURE_STRUCT();
}
Upp::String Texture2D::GetShaderMaterialFunction()const{ //This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	//TODO
	return "";
}
Upp::String Texture2D::GetShaderMaterialPrototypeFunction()const{//This one must be rewritted to sent the custom data structure defined by the user (See every .GLSL files in UOGL)
	//TODO
	return "";
}
void Texture2D::SentToShader(Shader& shader,const Upp::String& CustomName)const{//This function MUST be rewritted in inherrited class, It will allow UOGL to send good data to a shader
	if(shader.IsCompiled()){
		Upp::String nameToUse = (CustomName.GetCount()>0)? CustomName : name;
		shader.SetInt(nameToUse +".diffuse",TextureIterator);
		shader.SetInt(nameToUse +".specular",TextureIterator+1);
		shader.SetFloat(nameToUse +".shininess",shininess);
		shader.SetInt(nameToUse +".useSpecular",(SpecularLoaded)?1:0);
	}
}