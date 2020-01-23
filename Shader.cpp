#include "Shader.h"
#include "UltimateOpenGL.h"
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
	glm::vec3 buffer = UltimateOpenGL_Context::TransformRGBToFloatColor(red,green,blue);
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

//**********************Shader******************************
//**********************Struct shader_Data
Shader::shader_Data::shader_Data(){}
Shader::shader_Data::shader_Data(Upp::String _name,ShaderType _type,Upp::String _shader){
	name = _name;
	type = _type;
	shader = _shader;
}
Shader::shader_Data& Shader::shader_Data::operator=(const shader_Data& data){
	name= data.name;
	shader = data.shader;
	type = data.type;
	return *this;
}
//**End Struct shader_Data
Shader::Shader(){}
Shader::Shader(Upp::String _name,ShaderType _type,Upp::String _shader){
	shaders.Add(_name,shader_Data(_name,_type, _shader));
}
Shader::Shader(const Shader& _shader){
	shaders.Clear();
	for(const Upp::String &key : _shader.shaders.GetKeys()){
		shaders.Add(key,_shader.shaders.Get(key));
	}
	AutoGenerated = _shader.AutoGenerated;
	Compiled = _shader.Compiled;
	if(Compiled)ID = _shader.ID;
}
Shader& Shader::operator=(const Shader& _shader){
	shaders.Clear();
	for(const Upp::String &key : _shader.shaders.GetKeys()){
		shaders.Add(key,_shader.shaders.Get(key));
	}
	AutoGenerated = _shader.AutoGenerated;
	Compiled = _shader.Compiled;
	if(Compiled)ID = _shader.ID;
	return *this;
}
Shader::~Shader(){
}
Upp::ArrayMap<Upp::String,Shader::shader_Data>& Shader::GetShaders(){
	return shaders;
}
Shader& Shader::AddShader(Upp::String _name,ShaderType _type,Upp::String _shader){
	shaders.Add(_name,shader_Data(_name,_type,IncludeShader(_shader)));
	return *this;
}
unsigned int Shader::GetId()const{
	if(!Compiled) LOG("Warning : Shader::GetId() Shader is not compiled !");
	return ID;
}
bool Shader::IsCompiled()const{
	return Compiled;
}
bool Shader::IsAutoGenerated() const{
	return AutoGenerated;
}
bool Shader::Load(bool isAutoGenerated){
	if(!Compiled){
		if( shaders.GetCount() ){
			Upp::Vector<unsigned int> RdyToCompile;
			for (const Upp::String &n : shaders.GetKeys()){
				unsigned int buff;
				if(shaders.Get(n).type == ST_FRAGMENT){
					buff = glCreateShader(GL_FRAGMENT_SHADER);
					Upp::Buffer<char> str(shaders.Get(n).shader.GetCount()+1); //Changement made by Iñaki
					char * ptr =str;
					/*
					Upp::FileOut out("./fragment.txt");
					if(out){
						out << 	shaders.Get(n).shader << Upp::EOL;
						out.Close();
					}
					*/
					strcpy(ptr,shaders.Get(n).shader.ToStd().c_str());
				    glShaderSource(buff, 1, const_cast<const char**>(&ptr), NULL);
				    glCompileShader(buff);
				    if(CheckCompileErrors(buff, ST_FRAGMENT)){
				        RdyToCompile.Add(buff);
					}
				}else if(shaders.Get(n).type == ST_VERTEX){
					buff = glCreateShader(GL_VERTEX_SHADER);
					Upp::Buffer<char> str(shaders.Get(n).shader.GetCount()+1);//Changement made by Iñaki
					char * ptr =str;
					/*
					Upp::FileOut out("./vertex.txt");
					if(out){
						out << 	shaders.Get(n).shader << Upp::EOL;
						out.Close();
					}*/
					strcpy(ptr,shaders.Get(n).shader.ToStd().c_str());
				    glShaderSource(buff, 1, const_cast<const char**>(&ptr), NULL);
				    glCompileShader(buff);
				    if(CheckCompileErrors(buff, ST_VERTEX)){
				        RdyToCompile.Add(buff);
				    }
				}
			}
			ID = glCreateProgram();
			for(unsigned int &i : RdyToCompile){
				glAttachShader(ID, i);
			}
		    glLinkProgram(ID);
			for(unsigned int &i : RdyToCompile){
				glDeleteShader(i);
			}
		    RdyToCompile.Clear();
		    if( CheckCompileErrors(ID, ST_PROGRAMM)){
				Compiled = true;
				if(isAutoGenerated) AutoGenerated=true;
				return  true;
		    }
		    return false;
		}
		return false;
	}else{
		return true;
	}
}
Shader& Shader::Use(){
    glUseProgram(ID);
    return *this;
}
Shader& Shader::Unbind(){
	glUseProgram(0);
	return *this;
}
Shader& Shader::SetBool(Upp::String name, bool value){
    glUniform1i(glGetUniformLocation(ID, name.ToStd().c_str()), (int)value);
    return *this;
}
Shader& Shader::SetInt(Upp::String name, int value){
    glUniform1i(glGetUniformLocation(ID, name.ToStd().c_str()), value);
    return *this;
}
Shader& Shader::SetFloat(Upp::String name, float value){
    glUniform1f(glGetUniformLocation(ID, name.ToStd().c_str()), value);
    return *this;
}
Shader& Shader::SetVec2(Upp::String name, const glm::vec2 &value){
    glUniform2fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, &value[0]);
    return *this;
}
Shader& Shader::SetVec2(Upp::String name, float x, float y){
    glUniform2f(glGetUniformLocation(ID, name.ToStd().c_str()), x, y);
    return *this;
}
Shader& Shader::SetVec3(Upp::String name, const glm::vec3 &value){
    glUniform3fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, &value[0]);
    return *this;
}
Shader& Shader::SetVec3(Upp::String name, float x, float y, float z){
    glUniform3f(glGetUniformLocation(ID, name.ToStd().c_str()), x, y, z);
    return *this;
}
Shader& Shader::SetVec4(Upp::String name, const glm::vec4 &value){
    glUniform4fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, &value[0]);
    return *this;
}
Shader& Shader::SetVec4(Upp::String name, float x, float y, float z, float w){
    glUniform4f(glGetUniformLocation(ID, name.ToStd().c_str()), x, y, z, w);
    return *this;
}
Shader& Shader::SetMat2(Upp::String name, const glm::mat2 &mat){
    glUniformMatrix2fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this;
}
Shader& Shader::SetMat3(Upp::String name, const glm::mat3 &mat){
    glUniformMatrix3fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this;
}
Shader& Shader::SetMat4(Upp::String name, const glm::mat4 &mat){
    glUniformMatrix4fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this;
}
/*
Shader& Shader::SetPointLight(Upp::String name, const PointLight &pointLight,int num) const{
	if(autoGenerated && num!= -1 ){
		name ="pointLights["+ Upp::AsString(num)+"]";
	}
	SetVec3(name+".position",pointLight.GetPosition());
	SetVec3(name+".ambient", pointLight.GetAmbient());
	SetVec3(name+".diffuse", pointLight.GetDiffuse());
	SetVec3(name+".specular", pointLight.GetSpecular());
	SetFloat(name+".constant",pointLight.GetConstant());
	SetFloat(name+".linear",pointLight.GetLinear());
	SetFloat(name+".quadratic",pointLight.GetQuadratic());
	return *this;
}
Shader& Shader::SetDirLight(Upp::String name, const DirLight &dirLight,int num) const{
	if(autoGenerated && num!= -1 ){
		name ="dirLights["+ Upp::AsString(num)+"]";
	}
	SetVec3(name+".direction", dirLight.GetDirection());
	SetVec3(name+".ambient", dirLight.GetAmbient());
	SetVec3(name+".diffuse", dirLight.GetDiffuse());
	SetVec3(name+".specular", dirLight.GetSpecular());
	return *this;
}
Shader& Shader::SetSpotLight(Upp::String name, const SpotLight &spotLight,int num) const{
	if(autoGenerated && num!= -1 ){
		name ="spotLights["+ Upp::AsString(num)+"]";
	}
	SetVec3(name+".direction", spotLight.GetDirection());
	SetVec3(name+".position", spotLight.GetPosition());
	SetVec3(name+".ambient", spotLight.GetAmbient());
	SetVec3(name+".diffuse", spotLight.GetDiffuse());
	SetVec3(name+".specular", spotLight.GetSpecular());
	SetFloat(name+".constant",spotLight.GetConstant());
	SetFloat(name+".linear",spotLight.GetLinear());
	SetFloat(name+".quadratic",spotLight.GetQuadratic());
	SetFloat(name+".cutOff",spotLight.GetCutOff());
	SetFloat(name+".outerCutOff",spotLight.GetOuterCutOff());
	return *this;
}
Shader& Shader::SetMaterialColor(Upp::String name, Material &materialColor) const{
	SetVec3(name+".ambient",materialColor.GetAmbient());
	SetVec4(name+".diffuse",materialColor.GetColor());
	SetFloat(name+".shininess",materialColor.GetShininess());
	SetVec3(name+".specular",materialColor.GetSpecular());
	return *this;
}
Shader& Shader::SetMaterialTexture(Upp::String name, Material &materialTexture,bool CheckSpecular) const{
	for(TextureInformation& t : materialTexture.GetTextures()){
		if(t.texture.IsLoaded()){
			name = t.texture.GetName();
			SetInt(name+".diffuse",t.texture.GetTextureIterator());
			SetFloat(name+".shininess",materialTexture.GetShininess());
			if(t.Specular.IsLoaded()){
				SetInt(name+".useSpecular",1);
				SetInt(name+".specular",t.Specular.GetTextureIterator());
			}
			else{
				SetInt(name+".useSpecular",0);
				SetInt(name+".specular",0);
			}
		}
	}
	return *this;
}*/
bool Shader::CheckCompileErrors(unsigned int shader, ShaderType type)
{
    int success;
    char infoLog[1024];
    if (type == ST_VERTEX || type == ST_FRAGMENT) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            Upp::String buffer;
            if(type == ST_VERTEX) {
                buffer ="VERTEX";
            } else if(type == ST_FRAGMENT) {
                  buffer ="FRAGMENT";
            } else {
                buffer ="UNKNOW";
            }
            Upp::Cout() << "ERROR::"<< buffer <<"_SHADER_COMPILATION_ERROR :\n" << infoLog << "\n";
            return false;
        }
        return true;
    } else if(type == ST_PROGRAMM) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
	        Upp::Cout() <<  "ERROR::PROGRAM_LINKING_ERROR :\n" << infoLog << "\n";
			return false;
        }
        return true;
    }
    return false;
}
bool Shader::Unload(){
	if(Compiled){
		glUseProgram(0);
		glDeleteProgram(ID);
		Compiled = false;
		return true;
	}
	return false;
}
bool Shader::Reload(bool autoGenerated){
	Unload();
	return Load(autoGenerated);
}