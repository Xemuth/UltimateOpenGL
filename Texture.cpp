#include "Texture.h"
//**********************Material****************************
TextureInformation& TextureInformation::SetTexture(Texture& _texture){
	if(_texture.IsLoaded()){
		texture = _texture;
	}
	return *this;
}
TextureInformation& TextureInformation::SetSpecular(Texture& _specular){
	if(_specular.IsLoaded()){
		Specular = _specular;
	}
	return *this;
}
TextureInformation& TextureInformation::SetMix(float _mix){
	mix = ((_mix > 1.0f)? 1.0f:_mix <0.0f)? 0.0f:_mix;
	return *this;
}

Material::Material(const Material& material){
	shininess = material.shininess; //Brilliance en français
	diffuse = material.diffuse; //Valeur de diffusion
	
	ambient = material.ambient;
	specular = material.specular;
	whatToUse = material.whatToUse;// what to use.
	
//	TextureToUse = ;
	TextureToUse.Append(material.TextureToUse);	 //the float is used to setUp the MixValue of texture. If you only have one texture set it to One.
												//If you have 2 textures and want a 50%/50% set it to 0.5f to boath texture.
								  
	defaultColor = material.defaultColor; //used to know if this material have been modified and should be use.
	color = material.color; //Color with Alpha
}
Material& Material::operator=(const Material& material){
	shininess = material.shininess; //Brilliance en français
	diffuse = material.diffuse; //Valeur de diffusion
	
	ambient = material.ambient;
	specular = material.specular;
	whatToUse = material.whatToUse;// what to use.
	
	TextureToUse.Append(material.TextureToUse); //the float is used to setUp the MixValue of texture. If you only have one texture set it to One.
										 //If you have 2 textures and want a 50%/50% set it to 0.5f to boath texture.
								  
	defaultColor = material.defaultColor; //used to know if this material have been modified and should be use.
	color = material.color; //Color with Alpha
	
	return *this;
}
Material& Material::UseTextures(){
	whatToUse = TEXTURE;
	return *this;
}
Material& Material::UseColor(){
	whatToUse = COLOR;
	return *this;
}
Material& Material::UseBoth(){
	whatToUse = BOTH;
	return *this;
}
Material& Material::SetColor(int red,int green, int blue, int alpha){
	glm::vec3 buffer = TransformRGBToFloatColor(red,green,blue);
	alpha = ((alpha<0)?0:alpha> 255)?255:0;
	color = glm::vec4(buffer.x,buffer.y,buffer.z,(float(alpha)/255.0f));
	defaultColor=false;
	return *this;
}
Material& Material::SetColor(glm::vec4 _color){
	color = _color;
	return *this;
}
Material& Material::SetColor(glm::vec3 _color){
	color = glm::vec4(_color.x,_color.y,_color.z,1.0f);
	return *this;
}
TextureInformation& Material::AddTexture(Texture& t1, float MixValueInPourcentage, Texture tSpeculare ){
	if(t1.IsLoaded()){
		TextureInformation& t = TextureToUse.Create<TextureInformation>().SetTexture(t1).SetMix(MixValueInPourcentage);
		if(tSpeculare.IsLoaded())
			t.SetSpecular(tSpeculare);
		else
			LOG("Warning : Material& Material::AddTexture(...) Texture speculare named " + tSpeculare.GetName() + " is not loaded yet and has been discarded !");
		return t;
	}else{
		LOG("Warning : Material& Material::AddTexture(...) Texture have not been added because it's not loaded yet !");
	}
	throw UOGLException();
}
Material& Material::RemoveTexture(int iterator){
	if(iterator < TextureToUse.GetCount() &&  iterator >= 0) TextureToUse.Remove(iterator,1);
	return *this;
}
Upp::Array<TextureInformation>& Material::GetTextures(){
	return TextureToUse;
}

bool Material::HaveBeenSetUp(){//Used to know if this Material have been setting up (to know if UOGL should use it).
	if(!defaultColor)return true;
	if(TextureToUse.GetCount()> 0)return true;
	return false;
}

Material& Material::SetDiffuse(float _diffuse){
	diffuse = _diffuse;
	return *this;
}
Material& Material::SetShininess(float _shininess){
	shininess = _shininess;
	return *this;
}
Material& Material::SetAmbient(glm::vec3 _ambient){
	ambient = _ambient;
	return *this;
}
Material& Material::SetSpecular(glm::vec3 _specular){
	specular = _specular;
	return *this;	
}
glm::vec4 Material::GetColor(){
	return color;
}
glm::vec3 Material::GetAmbient(){
	return ambient;
}
glm::vec3 Material::GetSpecular(){
	return specular;
}
float Material::GetShininess(){
	return shininess;
}
float Material::GetDiffuse(){
	return diffuse;
}
///////////////////////////////////////////////////////////////////////////////
////////////////////////////     Texture   ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
Texture::Texture(){}
Texture::Texture(const Upp::String& _path){
	if(FileExists(_path)){
		Path = Upp::Replace(_path,Upp::Vector<Upp::String>{"\\"},Upp::Vector<Upp::String>{"/"});
		LoadDefaultname();
	}else{
		LOG("Error : Texture::Texture(...) FilePath : \""+ _path +"\" is incorrect !");
	}
}
Texture::Texture(const Upp::String& _path,const Upp::String& _Name){
	if(FileExists(_path)){
		Path = Replace(_path,Upp::Vector<Upp::String>{"\\"},Upp::Vector<Upp::String>{"/"});
	}else{
		LOG("Error : Texture::Texture(...) FilePath : \""+ _path +"\" is incorrect !");
	}
	Name = _Name;
}
Texture::Texture(const Texture& texture){
	Path = texture.Path;
	Name = texture.Name;
	width = texture.width;
	height = texture.height;
	nrChannels = texture.nrChannels;
	MipMap = texture.MipMap;
	Loaded = texture.Loaded;
	if(Loaded)ID = texture.ID;
	TextureIterator = texture.TextureIterator;
	textureParameters.Append(texture.textureParameters);
}
Texture& Texture::operator=(const Texture& texture){
	Path = texture.Path;
	Name = texture.Name;
	width = texture.width;
	height = texture.height;
	nrChannels = texture.nrChannels;
	MipMap = texture.MipMap;
	Loaded = texture.Loaded;
	if(Loaded)ID = texture.ID;
	TextureIterator = texture.TextureIterator;
	textureParameters.Append(texture.textureParameters);
	return *this;
}
Texture::~Texture(){
	//This part is commented since Texture is working on Copying paradigme
	//So Getting a texture to retrieve ID would make it unload when the object is deleted
	//To Unload texture from graphic card, you must use Unload Function
	/*if(loaded){
		glDeleteTextures(1, &ID);
	}*/
}
Texture& Texture::LoadDefaultTextureParameter(){
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
TextureParameter& Texture::AddTextureParameter(const TextureParameter& parameter){
	return textureParameters.Add(parameter);
}
Upp::Array<TextureParameter>& Texture::GetTextureParameters(){
	return textureParameters;
}
unsigned int Texture::GetId(){
	return ID;
}
Texture& Texture::LoadDefaultname(){
	Name = Path.Right(Path.GetCount()- Path.ReverseFind("/") -1);
	Name = Name.Left(Name.Find("."));
	LOG("Info : Texture::LoadDefaultname() default name have been setted : \""+ Name +"\"");
	return *this;
}
Texture&  Texture::SetPath(const Upp::String& _path){
	if(FileExists(_path)){
		Path = _path;
	}else{
		LOG("Warning : Texture::SetPath(...) Texture file path is incorrect !");
	}
	return *this;
}
Upp::String Texture::GetPath(){
	return Path;
}
Texture& Texture::SetName(const Upp::String& _name){
	Name = _name;
	return *this;
}
Upp::String Texture::GetName(){
	return Name;
}
Texture& Texture::SetType(TextureType _type){
	type = _type;
	return *this;
}
TextureType Texture::GetType(){
	return type;
}
bool Texture::IsMipMap(){
	return MipMap;
}
Texture& Texture::EnableMipMap(){
	MipMap = true;
	return *this;
}
Texture& Texture::DisableMipMap(){
	MipMap = false;
	return *this;
}
bool Texture::IsFlippedOnLoad(){
	return FlipOnLoad;
}
Texture& Texture::EnableFlipOnLoad(){
	FlipOnLoad = true;
	return *this;
}
Texture& Texture::DisableFlipOnLoad(){
	FlipOnLoad = false;
	return *this;
}
int Texture::GetTextureIterator(){
	return TextureIterator;
}
int Texture::GetWidth(){
	return width;
}
int Texture::GetHeight(){
	return height;
}
int Texture::GetNrChannels(){
	return nrChannels;
}
bool Texture::IsLoaded(){
	return Loaded;
}
Texture& Texture::LoadTextureParameter(){
	for(TextureParameter& param :textureParameters){
		glTexParameteri(param.target,param.pname,param.params);
	}
	return *this;
}
bool Texture::Reload(unsigned int ActiveIterator){
	Unload();
	return Load(ActiveIterator);
}
bool Texture::Load(unsigned int ActiveIterator){
	if(Upp::FileExists(Path)){
		if(!Loaded){
			TextureIterator = ActiveIterator;
			ID=0;
			glGenTextures(1, &ID);
			glActiveTexture(GL_TEXTURE0 + TextureIterator);
			glBindTexture(GL_TEXTURE_2D, ID);
			
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
					Loaded =true;
			}else{
				LOG("Error : Texture::Load(...) Error on loading texture named : \""+ Name +"\" !");
				Loaded=false;
			}
			stbi_image_free(data); //If we manage to use class attribute, we must dont forget to clean it in class destructor ! it's very important to prevent memory leak
		}else{
			LOG("Error : Texture::Load(...) Texture named : \""+ Name +"\" is already loaded ! Use Unload() Function to Unload or Reload() to reload the texture");
			return false;
		}
	}else{
		LOG("Error : Texture::Load(...) FilePath : \""+ Path +"\" of texture named : \""+ Name +"\" is incorrect !");
		Loaded =false;
	}
	return Loaded;
}
bool Texture::Unload(){
	if(Loaded){
		glDeleteTextures(1, &ID);
		Loaded = false;
		return true;
	}else{
		LOG("Warning : Texture::Unload() Nothing to remove !");
	}
	return false;
}
bool Texture::Use(){
	if(Loaded){
		//Upp::Cout() << "Using texture ID " << Upp::AsString(ID) << " to the iterator number " <<Upp::AsString(TextureIterator) << "\n";
		glActiveTexture(GL_TEXTURE0 +TextureIterator);
		glBindTexture(GL_TEXTURE_2D, ID);
		return true;
	}else{
		LOG("Warning Use(): Can't use an Unloaded texture !");
	}
	return false;
}