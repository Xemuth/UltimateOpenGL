#include "Shader.h"
#include "Material.h"
#include "Light.h"
Upp::String IncludeShader(Upp::String& shader){
	shader.Replace("MATERIAL_TEXTURE_STRUCT()",MATERIAL_TEXTURE_STRUCT());
	shader.Replace("MATERIAL_COLOR_STRUCT()",MATERIAL_COLOR_STRUCT());
	
	shader.Replace("LIGHT_POINT_STRUCT()",LIGHT_POINT_STRUCT());
	shader.Replace("LIGHT_POINT_TEXTURE_PROTOTYPE()",LIGHT_POINT_TEXTURE_PROTOTYPE());
	shader.Replace("LIGHT_POINT_TEXTURE_FUNCTION()",LIGHT_POINT_TEXTURE_FUNCTION());
	shader.Replace("LIGHT_POINT_COLOR_PROTOTYPE()",LIGHT_POINT_COLOR_PROTOTYPE());
	shader.Replace("LIGHT_POINT_COLOR_FUNCTION()",LIGHT_POINT_COLOR_FUNCTION());
	
	shader.Replace("LIGHT_SPOT_STRUCT()",LIGHT_SPOT_STRUCT());
	shader.Replace("LIGHT_SPOT_TEXTURE_PROTOTYPE()",LIGHT_SPOT_TEXTURE_PROTOTYPE());
	shader.Replace("LIGHT_SPOT_TEXTURE_FUNCTION()",LIGHT_SPOT_TEXTURE_FUNCTION());
	shader.Replace("LIGHT_SPOT_COLOR_PROTOTYPE()",LIGHT_SPOT_COLOR_PROTOTYPE());
	shader.Replace("LIGHT_SPOT_COLOR_FUNCTION()",LIGHT_SPOT_COLOR_FUNCTION());
	
	shader.Replace("LIGHT_DIR_STRUCT()",LIGHT_DIR_STRUCT());
	shader.Replace("LIGHT_DIR_TEXTURE_PROTOTYPE()",LIGHT_DIR_TEXTURE_PROTOTYPE());
	shader.Replace("LIGHT_DIR_TEXTURE_FUNCTION()",LIGHT_DIR_TEXTURE_FUNCTION());
	shader.Replace("LIGHT_DIR_COLOR_PROTOTYPE()",LIGHT_DIR_COLOR_PROTOTYPE());
	shader.Replace("LIGHT_DIR_COLOR_FUNCTION()",LIGHT_DIR_COLOR_FUNCTION());
	shader = Replace(shader,Upp::Vector<Upp::String>{"\""},Upp::Vector<Upp::String>{""});
	return shader;
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
Upp::VectorMap<Upp::String,Shader::shader_Data>& Shader::GetShaders(){
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
					#ifdef flagDEBUG
					Upp::FileOut out("./fragment.txt");
					if(out){
						out <<shaders.Get(n).shader << Upp::EOL;
						out.Close();
					}
					#endif
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
					#ifdef flagDEBUG
						Upp::FileOut out("./vertex.txt");
						if(out){
							out <<shaders.Get(n).shader << Upp::EOL;
							out.Close();
						}
					#endif
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
	if(IsCompiled())
		glUseProgram(ID);
	else
		LOG("Warning : Shader& Shader::Use() -> Shader not compiled");
    return *this;
}
Shader& Shader::Unbind(){
	glUseProgram(0);
	return *this;
}
Shader& Shader::SetBool(Upp::String name, bool value){
	if(IsCompiled())
		glUniform1i(glGetUniformLocation(ID, name.ToStd().c_str()), (int)value);
    return *this;
}
Shader& Shader::SetInt(Upp::String name, int value){
	if(IsCompiled())
		glUniform1i(glGetUniformLocation(ID, name.ToStd().c_str()), value);
    return *this;
}
Shader& Shader::SetFloat(Upp::String name, float value){
    if(IsCompiled())
		glUniform1f(glGetUniformLocation(ID, name.ToStd().c_str()), value);
    return *this;
}
Shader& Shader::SetVec2(Upp::String name, const glm::vec2 &value){
	if(IsCompiled())
		glUniform2fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, &value[0]);
    return *this;
}
Shader& Shader::SetVec2(Upp::String name, float x, float y){
	if(IsCompiled())
		glUniform2f(glGetUniformLocation(ID, name.ToStd().c_str()), x, y);
    return *this;
}
Shader& Shader::SetVec3(Upp::String name, const glm::vec3 &value){
	if(IsCompiled())
		glUniform3fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, &value[0]);
    return *this;
}
Shader& Shader::SetVec3(Upp::String name, float x, float y, float z){
	if(IsCompiled())
		glUniform3f(glGetUniformLocation(ID, name.ToStd().c_str()), x, y, z);
    return *this;
}
Shader& Shader::SetVec4(Upp::String name, const glm::vec4 &value){
	if(IsCompiled())
		glUniform4fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, &value[0]);
    return *this;
}
Shader& Shader::SetVec4(Upp::String name, float x, float y, float z, float w){
	if(IsCompiled())
		glUniform4f(glGetUniformLocation(ID, name.ToStd().c_str()), x, y, z, w);
    return *this;
}
Shader& Shader::SetMat2(Upp::String name, const glm::mat2 &mat){
	if(IsCompiled())
		glUniformMatrix2fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this;
}
Shader& Shader::SetMat3(Upp::String name, const glm::mat3 &mat){
    if(IsCompiled())
		glUniformMatrix3fv(glGetUniformLocation(ID, name.ToStd().c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this;
}
Shader& Shader::SetMat4(Upp::String name, const glm::mat4 &mat){
	if(IsCompiled())
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
            LOG("ERROR::"<< buffer <<"_SHADER_COMPILATION_ERROR :\n" << infoLog );
            return false;
        }
        return true;
    } else if(type == ST_PROGRAMM) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
	        LOG("ERROR::PROGRAM_LINKING_ERROR :\n" << infoLog);
			return false;
        }
        return true;
    }
    return false;
}
bool Shader::Unload(){
	if(Compiled){
		glDeleteProgram(ID);
		glUseProgram(0);
		Compiled = false;
		return true;
	}
	return false;
}

void Shader::IncreasePositionVariable(int position,int lenght){
	if(LayoutPosition >= position)LayoutPosition += lenght;
	if(InPosition >= position)InPosition += lenght;
	if(StructurePosition >= position)StructurePosition += lenght;
	if(OutPosition >= position)OutPosition += lenght;
	if(UniformPosition >= position)UniformPosition += lenght;
	if(MainPosition >= position)MainPosition += lenght;
	if(FunctionPrototypePosition >= position)FunctionPrototypePosition += lenght;
	if(FunctionDeclarationPosition >= position)FunctionDeclarationPosition += lenght;
}

int Shader::InsertInShader(Upp::String& shader, int position,const Upp::String& s){
	if(position != -1){
		shader.Insert(position,s);
		IncreasePositionVariable(position,s.GetCount());
		return s.GetCount();
	}
	return 0;
}
int Shader::InsertIfNotFind(Upp::String& shader,int position,const Upp::String& s){ //return -1 if not inserted
	if(shader.Find(s) == -1){
		return InsertInShader(shader,position,s);
	}
	return -1;
}
int Shader::RemoveInsertInShader(Upp::String& shader, const Upp::String& remove,const Upp::String& s, int position){
	if(shader.Find(remove,position) != -1){
		int pos = shader.Find(remove,position);
		shader.Remove(pos,remove.GetCount());
		shader.Insert(pos,s);
		IncreasePositionVariable(pos,(-remove.GetCount() + s.GetCount()));
		return s.GetCount();
	}
	return 0;
}

Upp::String Shader::GenerateVertexShader(unsigned int VertexGenerationOption,const Material* material,const Upp::Vector<Upp::String>* Layout,const Upp::Vector<Upp::String>* Out,const Upp::Vector<Upp::String>* Uniform,const Upp::Vector<Upp::String>* Structure,const Upp::Vector<Upp::String>* Function,const Upp::String* Main){
	Upp::String ShadV = BasicShaders.Get("Simple_Vertex_Shader");
	if(VertexGenerationOption){
		LayoutPosition = ShadV.FindAfter("//LAYOUT_VARIABLE\n");
		StructurePosition = ShadV.FindAfter("//STRUCTURE\n");
		OutPosition = ShadV.FindAfter("//OUT_VARIABLE\n");
		UniformPosition = ShadV.FindAfter("//UNIFORM\n");
		MainPosition = ShadV.FindAfter("//CUSTOM_MAIN\n");
		FunctionPrototypePosition = ShadV.FindAfter("//FUNCTION_PROTOTYPE\n");
		FunctionDeclarationPosition = ShadV.FindAfter("//FUNCTION_DEFINITION\n");
		
		
		if(LayoutPosition != -1 && StructurePosition != -1 && OutPosition != -1 && FunctionPrototypePosition != -1 && FunctionDeclarationPosition != -1 && UniformPosition != -1 && MainPosition != -1){
			if(VertexGenerationOption  & VGO_DoBasics){
				/*
					Doing Basic mean Assign All necessary layout (Position Normal TextureCoord
					Tangeant / BiTangeant etc adding all class out like FragPos  and each important
					Uniform (viex projection and model)
				*/
				InsertInShader(ShadV, LayoutPosition,"layout (location = 0) in vec3 aPos;\n");
				InsertInShader(ShadV, LayoutPosition,"layout (location = 1) in vec3 aNorm;\n");
				InsertInShader(ShadV, LayoutPosition,"layout (location = 2) in vec2 aTextCoords;\n");
				InsertInShader(ShadV, LayoutPosition,"layout (location = 3) in vec3 aTangents;\n");
				InsertInShader(ShadV, LayoutPosition,"layout (location = 4) in vec3 aBiTangents;\n");
				InsertInShader(ShadV, LayoutPosition,"layout (location = 5) in vec4 aColors;\n");
				
				InsertInShader(ShadV, OutPosition,"out vec3 FragPos;\n");
				InsertInShader(ShadV, OutPosition,"out vec4 Colors;\n");
				InsertInShader(ShadV, OutPosition,"out vec3 normal;\n");
				
				InsertInShader(ShadV, UniformPosition,"uniform mat4 view;\n");
				InsertInShader(ShadV, UniformPosition,"uniform mat4 projection;\n");
				InsertInShader(ShadV, UniformPosition,"uniform mat4 model;\n");
				
				InsertInShader(ShadV, MainPosition,"FragPos = vec3(model * vec4(aPos, 1.0));\n");
				InsertInShader(ShadV, MainPosition,"Colors = aColors;\n");
				InsertInShader(ShadV, MainPosition,"normal = aNorm;\n");
				InsertInShader(ShadV, MainPosition,"gl_Position = projection * view * model * vec4(aPos, 1.0f);\n");
			}
			if(VertexGenerationOption & VGO_UseMaterialObject && material){
				/*
					Prepare necessary things relative to Material object. If it use texture then
					prepare necassary out (texture coordinate etc)
				*/
				InsertInShader(ShadV, MainPosition,"TextureCoordinate = aTextCoords;\n");
				InsertInShader(ShadV, OutPosition, "out vec2 TextureCoordinate;\n");
			}
			if(VertexGenerationOption & VGO_CustomLayout && Layout){
				/*
					Will ensure layout vector is not nullptr, if not will add different custom
					layout next to existing
				*/
			}
			if(VertexGenerationOption & VGO_CustomOut && Out){
				/*
					Will ensure out vector is not nullptr, if not will add different custom
					out next to existing
				*/
			}
			if(VertexGenerationOption & VGO_CustomUniform && Uniform){
				/*
					Will ensure uniform vector is not nullptr, if not will add different custom
					uniform next to existing
				*/
			}
			if(VertexGenerationOption & VGO_CustomStructure && Structure ){
				/*
					Will ensure structure vector is not nullptr, if not will add different custom
					structure next to existing
				*/
			}
			if(VertexGenerationOption & VGO_CustomFunction && Function){
				/*
					Will ensure function vector is not nullptr, if not will add different custom
					function prototype (determined by your function declaration (make sure your
					Function declaration is well formed to allow UOGL to determine the prototype)
					next to existing and will also add your function déclaration at the end of the
					shader
				*/
			}
			if(VertexGenerationOption & VGO_CustomMain && Main){
				/*
					If defined, will copy paste main String to the main function.
					You must not declare main into the string :
						Good things :
						Upp::String main = "FragColor = texture(cube.diffuse, TextureCoordinate);"
						Bad things :
						Upp::String main = "void main()\n
											{\n
												FragColor = texture(cube.diffuse, TextureCoordinate);
											}\n"
				*/
			}
		}
	}
	return ShadV;
}
Upp::String Shader::GenerateFragmentShader(unsigned int FragmentGenerationOption,const Material* material,const Upp::ArrayMap<Upp::String, Light>* AllSceneLights,const Upp::Vector<Upp::String>* In,const Upp::Vector<Upp::String>* Out,const Upp::Vector<Upp::String>* Uniform,const Upp::Vector<Upp::String>* Structure,const Upp::Vector<Upp::String>* Function,const Upp::String* Main){
	Upp::String ShadF = BasicShaders.Get("Simple_Fragment_Shader");
	if(FragmentGenerationOption){
		InPosition = ShadF.FindAfter("//IN_VARIABLE\n");
		StructurePosition = ShadF.FindAfter("//STRUCTURE\n");
		OutPosition = ShadF.FindAfter("//OUT_VARIABLE\n");
		UniformPosition = ShadF.FindAfter("//UNIFORM\n");
		MainPosition = ShadF.FindAfter("//CUSTOM_MAIN\n");
		FunctionPrototypePosition = ShadF.FindAfter("//FUNCTION_PROTOTYPE\n");
		FunctionDeclarationPosition = ShadF.FindAfter("//FUNCTION_DEFINITION\n");
		
		if(InPosition != -1 && StructurePosition != -1 && OutPosition != -1 && FunctionPrototypePosition != -1 && FunctionDeclarationPosition != -1 && UniformPosition != -1 && MainPosition != -1){
			if(FragmentGenerationOption  & FGO_DoBasics){
				/*
					Doing Basic mean Assign All necessary layout (Position Normal TextureCoord
					Tangeant / BiTangeant etc adding all class out like FragPos  and each important
					Uniform (viex projection and model)
				*/
				InsertInShader(ShadF, InPosition,"in vec3 FragPos;\n");
				InsertInShader(ShadF, InPosition,"in vec4 Colors;\n");
				
				InsertInShader(ShadF, OutPosition,"out vec4 FragColor;\n");
				InsertInShader(ShadF, MainPosition,"FragColor = Colors;\n");
			}
			if((FragmentGenerationOption & FGO_UseMaterialObject) && material){
				/*
					Prepare necessary things relative to Material object. If it use texture then
					prepare necassary out (texture coordinate etc)
				*/
				InsertInShader(ShadF, StructurePosition, material->GetShaderDataStructure());
				InsertInShader(ShadF, InPosition, "in vec2 TextureCoordinate;\n");
				InsertInShader(ShadF, UniformPosition,"uniform " + material->GetShaderNameStructure() + " " + material->GetName()+";\n");
				RemoveInsertInShader(ShadF,"FragColor = Colors;\n",material->GetCalculationCode(),ShadF.FindAfter("//CUSTOM_MAIN\n"));
			}
			if((FragmentGenerationOption & FGO_LoadLight) && AllSceneLights){
				/*
					Prepare necessary things relative to Light object (Function / Structure)
				*/
				InsertInShader(ShadF, UniformPosition,"uniform vec3 viewPos;\n");
				bool useMaterial = false;
				if(FGO_UseMaterialObject && material){
					if(typeid(*material) == typeid(Texture2D)){
						useMaterial =true;
					}
				}
				RemoveInsertInShader(ShadF,"FragColor = Colors;\n","");
				RemoveInsertInShader(ShadF,material->GetCalculationCode(),"vec3 norm = normalize(normal);\nvec3 viewDir = normalize(viewPos - FragPos);\nvec4 result;\n");
				
				for(const Upp::String& str : AllSceneLights->GetKeys()){
					const Light& t = AllSceneLights->Get(str);

					InsertIfNotFind(ShadF,StructurePosition,t.GetShaderDataStructure());
					
					InsertIfNotFind(ShadF,FunctionPrototypePosition,((useMaterial)? (t.GetShaderTexturePrototypeFunction()):(t.GetShaderColorPrototypeFunction())));
					InsertIfNotFind(ShadF,FunctionDeclarationPosition,((useMaterial)?(t.GetShaderTextureCalculationFunction()):(t.GetShaderColorCalculationFunction())));
					InsertInShader(ShadF, InPosition, "in vec3 normal;\n");
					InsertInShader(ShadF,UniformPosition,"uniform " + t.GetShaderNameStructure() + " " + t.GetName()+";\n");
					
					if(material && useMaterial) InsertInShader(ShadF, MainPosition,"result += CalcTextureDirLight(" + material->GetName() +","+ t.GetName() +", norm, viewDir);\n");
					else if(material)	InsertInShader(ShadF, MainPosition,"result += CalcColorDirLight(" + material->GetName() +","+ t.GetName() +", norm, viewDir);\n");
				}
				InsertInShader(ShadF, MainPosition,"FragColor = result;\n");
			}
			if((FragmentGenerationOption & FGO_CustomIn) && In){
				/*
					Will ensure in vector is not nullptr, if not will add different custom
					in next to existing
				*/
			}
			if((FragmentGenerationOption & FGO_CustomOut) && Out){
				/*
					Will ensure out vector is not nullptr, if not will add different custom
					out next to existing
				*/
			}
			if((FragmentGenerationOption & FGO_CustomUniform) && Uniform){
				/*
					Will ensure uniform vector is not nullptr, if not will add different custom
					uniform next to existing
				*/
			}
			if((FragmentGenerationOption & FGO_CustomStructure) && Structure ){
				/*
					Will ensure structure vector is not nullptr, if not will add different custom
					structure next to existing
				*/
			}
			if((FragmentGenerationOption & FGO_CustomFunction) && Function){
				/*
					Will ensure function vector is not nullptr, if not will add different custom
					function prototype (determined by your function declaration (make sure your
					Function declaration is well formed to allow UOGL to determine the prototype)
					next to existing and will also add your function déclaration at the end of the
					shader
				*/
			}
			if((FragmentGenerationOption & FGO_CustomMain) && Main){
				/*
					If defined, will copy paste main String to the main function.
					You must not declare main into the string :
						Good things :
						Upp::String main = "FragColor = texture(cube.diffuse, TextureCoordinate);"
						Bad things :
						Upp::String main = "void main()\n
											{\n
												FragColor = texture(cube.diffuse, TextureCoordinate);
											}\n"
				*/
			}
		}
	}
	return ShadF;
}
	
bool Shader::AssignSimpleShader(){
	Unload();
	shaders.Clear();
	AddShader("FRAGMENT",ST_FRAGMENT, GenerateFragmentShader(FGO_DoBasics));
	AddShader("VERTEX",ST_VERTEX, GenerateVertexShader(VGO_DoBasics));
	return Load(true);
}
bool Shader::AssignSimpleShaderMaterial(const Material* material){ //Craete and assign a simple shader wich can show texture
	Unload();
	shaders.Clear();
	AddShader("FRAGMENT",ST_FRAGMENT, GenerateFragmentShader(FGO_DoBasics | FGO_UseMaterialObject,material));
	AddShader("VERTEX",ST_VERTEX, GenerateVertexShader(VGO_DoBasics| VGO_UseMaterialObject,material));
	return Load(true);
}
bool Shader::AssignSimpleShaderMaterialLights(const Upp::ArrayMap<Upp::String, Light>& AllSceneLights,const Material* material){
	Unload();
	shaders.Clear();
	AddShader("VERTEX",ST_VERTEX, GenerateVertexShader(VGO_DoBasics| VGO_UseMaterialObject,material));
	AddShader("FRAGMENT",ST_FRAGMENT, GenerateFragmentShader(FGO_DoBasics | FGO_UseMaterialObject | FGO_LoadLight,material,&AllSceneLights));
	return Load(true);
}
bool Shader::Reload(bool autoGenerated){
	Unload();
	return Load(autoGenerated);
}