#include "Texture.h"

//**********************Texture*****************************
//**********Struct TextureParameter******
TextureParameter::TextureParameter(){
	target=GL_TEXTURE_2D;
	pname=GL_TEXTURE_WRAP_S;
	params=GL_REPEAT;
}
TextureParameter::TextureParameter(GLenum _target,GLenum _pname,int _params){
	target=_target;
	pname=_pname;
	params=_params;
}
TextureParameter::TextureParameter(const TextureParameter& _textureParameter){
	target=_textureParameter.target;
	pname=_textureParameter.pname;
	params=_textureParameter.params;
}
TextureParameter& TextureParameter::operator=(const TextureParameter& _textureParameter){
	target=_textureParameter.target;
	pname=_textureParameter.pname;
	params=_textureParameter.params;
	return *this;
}
//****************************************
Texture::Texture(){}
Texture::Texture(Upp::String _path){
	if(FileExists(_path)){
		pathTexture = Upp::Replace(_path,Upp::Vector<Upp::String>{"\\"},Upp::Vector<Upp::String>{"/"});
		LoadDefaultname();
	}else{
		LOG(_path + "\nDo not exist. Don't try to load it Right now, rather Redifine a path via SetPath(String path)\n");
	}
}
Texture::Texture(Upp::String _path,TextureColorSample _color){
	if(FileExists(_path)){
		pathTexture = Replace(_path,Upp::Vector<Upp::String>{"\\"},Upp::Vector<Upp::String>{"/"});
		LoadDefaultname();
	}else{
		LOG( _path +"\nDo not exist. Don't try to load it Right now, rather Redifine a path via SetPath(String path)\nHowever colorSample have been set to your value !\n");
	}
	color = _color;
}
Texture::Texture(Upp::String _path,Upp::String _Name,TextureColorSample _color){
	if(FileExists(_path)){
		pathTexture = Replace(_path,Upp::Vector<Upp::String>{"\\"},Upp::Vector<Upp::String>{"/"});
	}else{
		LOG(_path + "\nDo not exist. Don't try to load it Right now, rather Redifine a path via SetPath(String path)\nHowever colorSample and TextureName have been set to your value !\n");
	}
	color = _color;
	textureName = _Name;
}
Texture::Texture(const Texture& texture){
	pathTexture = texture.GetPath();
	textureName = texture.GetName();
	color = texture.GetTextureColor();
	width = texture.GetWidth();
	height = texture.GetHeight();
	nrChannels = texture.GetNrChannels();
	GenerateMipMap = texture.IsMipMap();
	loaded = texture.IsLoaded();
	if(loaded)ID = texture.GetId();
	TextureIterator = texture.GetTextureIterator();
	textureparameters.Append(texture.GetTextureParameters());
}
Texture::~Texture(){
	if(loaded){
		glDeleteTextures(1, &ID);
	}
}

void Texture::LoadTextureParameter(){
	for(TextureParameter& param :textureparameters){
		glTexParameteri(param.target,param.pname,param.params);
	}
}
void Texture::LoadDefaultTextureParameter(){
	textureparameters.Clear();
	textureparameters.Append(Upp::Vector<TextureParameter>{
		TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,(int)GL_REPEAT),
		TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)GL_REPEAT),
		TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,(int) GL_LINEAR_MIPMAP_LINEAR),
		TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,(int) GL_NEAREST),
	});
	LOG("Default Texture parameter loaded for " + textureName);
	/*
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		    // set texture filtering parameters
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	*/
}
void Texture::ClearTextureParameter(){
	textureparameters.Clear();
}
void Texture::SetTextureParameter(Upp::Vector<TextureParameter>& parameters){
	textureparameters.Clear();
	textureparameters.Append(parameters);
}
TextureParameter& Texture::AddTextureParameter(TextureParameter parameter){
	return textureparameters.Add(parameter);
}
void Texture::RemoveTextureParameter(int number){
	if(number >-1 &&number< textureparameters.GetCount()){
		textureparameters.Remove(number);
	}
}
const Upp::Vector<TextureParameter>& Texture::GetTextureParameters()const{
	return textureparameters;
}


void Texture::LoadDefaultname(){
	textureName = pathTexture.Right(pathTexture.GetCount()- pathTexture.ReverseFind("/") -1);
	textureName = textureName.Left(textureName.Find("."));
	LOG("Default texture name loaded : " + textureName);
}
bool Texture::SetPath(Upp::String _path){
	if(FileExists(_path)){
		pathTexture = _path;
		return true;
	}else{
		LOG(_path + "\nDo not exist.\n");
		return false;
	}
}
void Texture::SetName(Upp::String _name){//By Default the name of the texture gonna be is name from loading files
	textureName = _name;
}
void Texture::SetColorSample(TextureColorSample _color){
	color = _color;
}
bool Texture::IsMipMap() const{//Return if GenerateMipMap is to true or false
	return GenerateMipMap;
}
void Texture::ActivateMipMapGeneration(bool _activate){
	if(loaded) LOG("Warning ActivateMipMapGeneration() : Your texture is still loaded ! To ensure/Desactivate MipMap generation you must Remove(Desalocate texture) then reload it!...\n");
	GenerateMipMap = _activate;
}
unsigned int Texture::GetId() const{
	if(!loaded) LOG("Warning GetId() : you're asking for Id while the texture is not Loaded ! It mean the return value is wrong...\n");
	return ID;
}
Upp::String Texture::GetName() const{
	return textureName;
}
Upp::String Texture::GetPath() const{
	return pathTexture;
}
int Texture::GetTextureIterator() const{
	return TextureIterator;
}
int Texture::GetWidth() const{
	return width;
}
int Texture::GetHeight() const{
	return height;
}
int Texture::GetNrChannels() const{
	return nrChannels;
}
bool Texture::IsLoaded() const{
	return loaded;
}
TextureColorSample Texture::GetTextureColor() const{
	return color;
}

Texture& Texture::operator=(const Texture& texture){
	pathTexture = texture.GetPath();
	textureName = texture.GetName();
	color = texture.GetTextureColor();
	
	width = texture.GetWidth();
	height = texture.GetHeight();
	nrChannels = texture.GetNrChannels();
	
	//unsigned char *data; //Data of texture, Its commented because I delete this at the end of Loading so.. Maybe in further we gonna uncomment it
	//Texture dont have "parameteri wrapper" atm
	GenerateMipMap = texture.IsMipMap();
	loaded = texture.IsLoaded();
	if(loaded)ID = texture.GetId();
	TextureIterator = texture.GetTextureIterator(); //Used
	textureparameters.Append(texture.GetTextureParameters());
	return *this;
}

bool Texture::Load(unsigned int ActiveIterator,bool loadDefault,bool flipLoad){
	if(FileExists(pathTexture)){
		if(!loaded){
			TextureIterator = ActiveIterator;
			glGenTextures(1, &ID);
			glActiveTexture(GL_TEXTURE0 + TextureIterator);
			glBindTexture(GL_TEXTURE_2D, ID);
			
			if(loadDefault||textureparameters.GetCount() ==0)LoadDefaultTextureParameter();
			LoadTextureParameter();
			
			/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		    // set texture filtering parameters
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/
		    
			stbi_set_flip_vertically_on_load(flipLoad);
			
			unsigned char *data=nullptr; //Comment this and un comment Class attribute Data* to load it in our unsigned char* buffer
			data = stbi_load(pathTexture.ToStd().c_str(), &width, &height, &nrChannels, 0);
			if(data){
				if(color == SAMPLE_RGB){
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				}else if(color == SAMPLE_RGBA){
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				}
				if(GenerateMipMap)glGenerateMipmap(GL_TEXTURE_2D);
					loaded =true;
			}else{
				Upp::Cout() << "Error during loading of texture N°" << ActiveIterator << " From path:\n"<< pathTexture <<"\n";
				loaded=false;
			}
			stbi_image_free(data); //If we manage to use class attribute, we must dont forget to clean it in class destructor ! it's very important to prevent memory leak
		}else{
			Upp::Cout() << "This texture is still loaded ! use Remove() to deload it then try to reload it !\n";
			return false;
		}
	}else{
		Upp::Cout() << "FilePath of texutre is undefined  !\n";
		loaded =false;
	}
	return loaded;
}
bool Texture::Use(){
	if(loaded){
		glActiveTexture(GL_TEXTURE0 +TextureIterator);
		glBindTexture(GL_TEXTURE_2D, ID);
		return true;
	}else{
		Upp::Cout() << "Warning Use(): Can't use an Unloaded texture !\n";
	}
	return false;
}
bool Texture::Remove(){
	if(loaded){
		 //If we manage to use class attribute unsigned char* data, we must dont forget to clean it Here too ! it's very important to prevent memory leak
		 //Even if in theory if you use this, it's to load another data so old data would be
		 //erase but here we do proper thing
		glDeleteTextures(1, &ID);
		return true;
	}else{
		Upp::Cout() << "Warning Remove(): Nothing to remove !\n";
	}
	return false;
}