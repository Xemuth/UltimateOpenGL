#include "UltimateOpenGL.h"
Mesh::Mesh(const Upp::Vector<Vertex>& vertices, Upp::Vector<unsigned int>& indices, Upp::Vector<Texture>& textures)
{
    this->vertices.Append(vertices);
    this->indices.Append(indices);
    this->textures.Append(textures);
    
    for(Texture& t : textures){
   		MaterialTexture& mt =  CreateMaterialTexture(t.GetName());
   		mt.diffuse = t.GetTextureIterator(); 
		mt.specular= t.GetTextureIterator();                                                                 
    }

   //Load();
}

Mesh::Mesh(Mesh& _mesh){
	object3D = _mesh.GetObject3D();
	VAO = _mesh.GetVAO();
	VBO = _mesh.GetVBO();
	EBO = _mesh.GetEBO();
        
    vertices.Append(_mesh.GetVertices());
    indices.Append(_mesh.GetIndices());
    textures.Append(_mesh.GetTextures());
    
    shader = _mesh.GetShader();
    
    transform = _mesh.GetTransform();
    
    Upp::VectorMap<Upp::String,MaterialColor>& mc = _mesh.GetMaterialColor();
    for(const Upp::String& str : mc.GetKeys()){
        materialsColor.Add(str, mc.Get(str));
    }
    Upp::VectorMap<Upp::String,MaterialTexture>& mt = _mesh.GetMaterialTextures();
    for(const Upp::String& str : mt.GetKeys()){
        materialsTexture.Add(str, mt.Get(str));
    }

	LightAffected = _mesh.IsLightAffected();
}

void Mesh::Load(){
	if(!shader.IsCompiled() && object3D != nullptr && object3D->GetScene() != nullptr){
		LOG("Shader not compiled !, Default shaders loaded !\n");
		GenerateAutoShader(object3D->GetScene() ->GetNumberOfDirLight(),  object3D->GetScene() ->GetNumberOfPointLight(),object3D->GetScene()->GetNumberOfSpotLight());
	}
	// create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}

void Mesh::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera){

	shader.Use();
	//Cout() << "x: " << Position.x << ", y: " << Position.y << ", z: " << Position.z << "\n";
	model = glm::translate(model,GetTransform().GetPosition())*glm::mat4_cast(GetTransform().GetQuaterion())*GetTransform().GetModelMatrixScaller();

	//if(ClearOnDraw)transformations.Clear();S
    shader.SetMat4("model",model);
    shader.SetMat4("transform", transform);
    shader.SetMat4("view",view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("viewPos",GetTransform().GetPosition());
    int cptColor= 0;
    for(const Upp::String &mcStr : materialsColor.GetKeys()){
        
		shader.SetMaterialColor("color"+ Upp::AsString(cptColor),materialsColor.Get(mcStr));
		cptColor++;
	}
	int cptTexture =0;

	
	for(const Upp::String &mtStr : materialsTexture.GetKeys()){
		object3D->GetScene()->GetContext()->GetTextures().Get(mtStr).Use();
		shader.SetMaterialTexture("texture"+ Upp::AsString(cptTexture),materialsTexture.Get(mtStr));
		cptTexture++;
	}
    for(const Upp::String& str : object3D->GetScene()->GetAllGameObject().GetKeys()){
		int cptObject = 0;
    	GameObject& obj =  object3D->GetScene()->GetAllGameObject().Get(str);
    	if(!obj.IsDrawableDuringThisFrame()){
			for(const Upp::String& dlStr : obj.GetDirLights().GetKeys()){
				shader.SetDirLight(dlStr,obj.GetDirLights().Get(dlStr),cptObject);
				cptObject++;
			}
			cptObject=0;
			for(const Upp::String& spStr : obj.GetSpotLights().GetKeys()){
				shader.SetSpotLight(spStr,obj.GetSpotLights().Get(spStr),cptObject);
				cptObject++;
			}
			cptObject=0;
			for(const Upp::String& poStr : obj.GetPointLights().GetKeys()){
				shader.SetPointLight(poStr,obj.GetPointLights().Get(poStr),cptObject);
				cptObject++;
			}
    	}
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
    shader.Unbind();
	/*
	// bind appropriate textures
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        Upp::String number;
        TextureType name = textures[i].GetType();
        if(name == DIFFUSE)
			number = std::to_string(diffuseNr++);
		else if(name == SPECULAR)
			number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if(name == NORMAL)
			number = std::to_string(normalNr++); // transfer unsigned int to stream
         else if(name == HEIGHT)
		    number = std::to_string(heightNr++); // transfer unsigned int to stream

												 // now set the sampler to the correct texture unit
       // glUniform1i(glGetUniformLocation(shader.GetId(), (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].GetId());
    }
    
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
    */
}


void Mesh::SetShader(Shader& _shader){
	shader = _shader;
}
Shader& Mesh::GetShader(){
	return shader;
}


unsigned int Mesh::GetVAO(){
	return VAO;
}
unsigned int Mesh::GetVBO(){
	return VBO;
}
unsigned int Mesh::GetEBO(){
	return EBO;
}

Upp::Vector<Vertex>& Mesh::GetVertices(){
	return vertices;
}
Upp::Vector<unsigned int>& Mesh::GetIndices(){
	return indices;
}

/* Texture Gestion */
Mesh& Mesh::BindTexture(const Upp::String& TextureName){
	if(object3D != nullptr && object3D->GetScene() != nullptr && object3D->GetScene()->GetContext() !=nullptr){
		Texture t =object3D->GetScene()->GetContext()->GetTexture("TextureName");
		if(t.IsLoaded()){
			textures.Add(t);
			return *this;
		}
		else
			LOG(TextureName + " texture don't existe in context, you must add it before getting it !");
	}
	LOG("Mesh is not bind to GameObject or GameObject is not bind to Scene or Scene is not bind to Context wich is carrying texture");
	return *this;
}
bool Mesh::RemoveTexture(const Texture& _texture){
	int cpt = 0;
	for(Texture& t : textures){
		if(t.GetId() == _texture.GetId()){
			textures.Remove(cpt);
			return true;
		}
		cpt++;
	}
	return false;
}
Upp::Vector<Texture>& Mesh::GetTextures(){
	return textures;
}

/* Transform */
Transform& Mesh::GetTransform(){
	return transform;
}
void Mesh::SetTransform(const Transform& _transform){
	transform = _transform;
}

/* Material Manager */
Upp::VectorMap<Upp::String,MaterialTexture>& Mesh::GetMaterialTextures(){
	return materialsTexture;
}
Upp::VectorMap<Upp::String,MaterialColor>& Mesh::GetMaterialColor(){
	return materialsColor;
}

MaterialTexture& Mesh::CreateMaterialTexture(const Upp::String& _name){
	if( materialsTexture.Find(_name) ==-1){
		return materialsTexture.Add(_name);
	}else{
		return materialsTexture.Get(_name);
	}
}
MaterialColor& Mesh::CreateMaterialColor(const Upp::String& _name){
	if( materialsColor.Find(_name) ==-1){
		return materialsColor.Add(_name);
	}else{
		return materialsColor.Get(_name);
	}
}
//Miscelnious
void Mesh::SetObject3D(Object3D* _object3D){
	object3D = _object3D;
}
Object3D* Mesh::GetObject3D(){
	return object3D;
}

void Mesh::SetLightAffected(bool _light){
	LightAffected = _light;
}
bool Mesh::IsLightAffected(){
	return LightAffected;
}

void Mesh::GenerateAutoShader(int NbLightDir,int NbLightPoint,int NbLightSpot){
	Upp::String vertexShader = BasicShaders.Get("Default_Vertex_Shader");
	Upp::String fragmentShader = BasicShaders.Get("Default_Fragment_Shader");
		
	Upp::String all_uniform="";
	Upp::String all_VecMulti="FragColor = ";
	
	Upp::String AllDirLights="";
	Upp::String AllPointLights="";
	Upp::String AllSpotLights="";	
	if(materialsTexture.GetCount()== 0 && materialsColor.GetCount() == 0){
		float r =Upp::Randomf() ;float g =Upp::Randomf() ;float b =Upp::Randomf() ;
		MaterialColor& m = CreateMaterialColor("defaultColor");
		m.SetDiffuse(glm::vec3( r,g,b));
		LOG("INFO : GenerateAutoShader(int,int,int) -> static color set for " + object3D->GetName() + " object !");	
	}
	if(materialsTexture.GetCount() > 0){
		//First we must had in Vertex Shader  all this thing : 
		vertexShader.Replace("//LAYOUT_TEXTURE_COORD","layout (location = 2) in vec2 aTexCoord;");
		vertexShader.Replace("//OUT_VECTOR_TEXCOORDS","out vec2 TexCoords;");
		vertexShader.Replace("//TEXTCOORDS_CALCULATION","TexCoords = aTexCoord;");
		
		//Then for fragment Shader :
		fragmentShader.Replace("//STRUCT_MATERIAL_TEXTURE","MATERIAL_TEXTURE_STRUCT()");
		fragmentShader.Replace("//OUT_FRAG_COLOR","out vec4 FragColor;");
		fragmentShader.Replace("//IN_VECTOR_TEXTURE_COORD","in vec2 TexCoords;");
		int cpt = 0;
		for(const Upp::String& key : materialsTexture.GetKeys()){
			Upp::String textureNameIdentifier = "texture" + Upp::AsString(cpt) ;
			all_uniform << "uniform MaterialTexture " << textureNameIdentifier << ";\n";
			all_VecMulti << ((cpt > 0)?"*":"") <<  "texture(" + textureNameIdentifier + ".diffuse,TexCoords)"; 
			if( NbLightDir > 0 && LightAffected){
				if(cpt == 0){
					AllDirLights << "for(int i = 0; i < " + Upp::AsString(NbLightDir) +"; i++){\n";
					AllDirLights << "\tresult += CalcTextureDirLight("+ textureNameIdentifier +",dirLights[i], norm, viewDir);\n";
				}else{
					AllDirLights << "\tresult += CalcTextureDirLight("+ textureNameIdentifier +",dirLights[i], norm, viewDir);\n";
				}
				if(cpt + 1 == NbLightDir){
					AllDirLights << "}";	
				}
			}
			if( NbLightPoint > 0 && LightAffected){
				if(cpt == 0){
					AllPointLights << "for(int i = 0; i < " + Upp::AsString(NbLightPoint) +"; i++){\n";
					AllPointLights << "\tresult += CalcTexturePointLight("+ textureNameIdentifier +",pointLights[i], norm, FragPos , viewDir);\n";
				}else{
					AllPointLights << "\tresult += CalcTexturePointLight("+ textureNameIdentifier +",pointLights[i], norm, FragPos , viewDir);\n";
				}
				if(cpt+ 1 == NbLightPoint){
					AllPointLights << "}";	
				}
			}
			if( NbLightSpot > 0 && LightAffected){
				if(cpt == 0){
					AllSpotLights << "for(int i = 0; i < " + Upp::AsString(NbLightSpot) +"; i++){\n";
					AllSpotLights << "\tresult += CalcTextureSpotLight("+ textureNameIdentifier +",spotLights[i], norm, FragPos , viewDir);\n";
				}else{
					AllSpotLights << "\tresult += CalcTextureSpotLight("+ textureNameIdentifier +",spotLights[i], norm, FragPos , viewDir);\n";
				}
				if(cpt+ 1 == NbLightSpot){
					AllSpotLights << "}";	
				}
			}
			cpt++;
		}
		if(all_uniform.GetCount()>0) fragmentShader.Replace("//UNIFORM_MATERIAL_TEXTURE_NAME",all_uniform);

		if( NbLightDir > 0 && LightAffected){
			fragmentShader.Replace("//STRUCT_DIR_LIGHT","LIGHT_DIR_STRUCT()");
			fragmentShader.Replace("//IMPORT_DIR_LIGHT_TEXTURE_PROTOTYPE","LIGHT_DIR_TEXTURE_PROTOTYPE()");
			fragmentShader.Replace("//DIR_LIGHT_TEXTURE_FUNCTION","LIGHT_DIR_TEXTURE_FUNCTION()");	
			fragmentShader.Replace("//DIR_LIGHTS_ARRAY","uniform DirLight dirLights["+ Upp::AsString(NbLightDir) +"];");
			fragmentShader.Replace("//DIRECTIONAL_LIGHTS",AllDirLights);
		}
		if( NbLightPoint > 0 && LightAffected){
			fragmentShader.Replace("//STRUCT_POINT_LIGHT","LIGHT_POINT_STRUCT()");
			fragmentShader.Replace("//IMPORT_POINT_LIGHT_TEXTURE_PROTOTYPE","LIGHT_POINT_TEXTURE_PROTOTYPE()");
			fragmentShader.Replace("//POINT_LIGHT_TEXTURE_FUNCTION","LIGHT_POINT_TEXTURE_FUNCTION()");	
			fragmentShader.Replace("//POINT_LIGHTS_ARRAY","uniform PointLight pointLights["+ Upp::AsString(NbLightPoint) +"];");
			fragmentShader.Replace("//POINT_LIGHTS",AllPointLights);
		}
		if( NbLightSpot > 0 && LightAffected){
			fragmentShader.Replace("//STRUCT_SPOT_LIGHT","LIGHT_SPOT_STRUCT()");
			fragmentShader.Replace("//IMPORT_SPOT_LIGHT_TEXTURE_PROTOTYPE","LIGHT_SPOT_TEXTURE_PROTOTYPE()");
			fragmentShader.Replace("//SPOT_LIGHT_TEXTURE_FUNCTION","LIGHT_SPOT_TEXTURE_FUNCTION()");	
			fragmentShader.Replace("//SPOT_LIGHTS_ARRAY","uniform SpotLight spotLights["+ Upp::AsString(NbLightSpot) +"];");
			fragmentShader.Replace("//SPOT_LIGHTS",AllSpotLights);
		}
		
		if(all_VecMulti.GetCount()>12){
			if((NbLightDir > 0 || NbLightPoint > 0 || NbLightSpot > 0) && LightAffected){
				fragmentShader.Replace("//FRAG_COLOR_CALCULATION","FragColor = vec4(result,1.0);");
			}else{
				all_VecMulti << ";\n";
				fragmentShader.Replace("//FRAG_COLOR_CALCULATION",all_VecMulti);
			}
		}
	}
	if(materialsColor.GetCount() > 0){
		fragmentShader.Replace("//STRUCT_MATERIAL_COLOR","MATERIAL_COLOR_STRUCT()");
		fragmentShader.Replace("//OUT_FRAG_COLOR","out vec4 FragColor;");
		
		int cpt = 0;
		for(const Upp::String& key : materialsColor.GetKeys()){
			Upp::String colorIdentifier = "color" + Upp::AsString(cpt) ;
			all_uniform << "uniform MaterialColor " <<  colorIdentifier << ";\n";
			all_VecMulti << ((cpt > 0)?"*":"") <<  "vec4(" + colorIdentifier + ".diffuse,1.0)"; 
			if( NbLightDir > 0 && LightAffected){
				if(cpt == 0){
					AllDirLights << "for(int i = 0; i < " + Upp::AsString(NbLightDir) +"; i++){\n";
					AllDirLights << "\tresult += CalcColorDirLight("+ colorIdentifier +",dirLights[i], norm, viewDir);\n";
				}else{
					AllDirLights << "\tresult += CalcColorDirLight("+ colorIdentifier +",dirLights[i], norm, viewDir);\n";
				}
				if(cpt + 1 == NbLightDir){
					AllDirLights << "}";	
				}
			}
			if( NbLightPoint > 0 && LightAffected){
				if(cpt == 0){
					AllPointLights << "for(int i = 0; i < " + Upp::AsString(NbLightPoint) +"; i++){\n";
					AllPointLights << "\tresult += CalcColorPointLight("+ colorIdentifier +",pointLights[i], norm, FragPos ,viewDir);\n";
				}else{
					AllPointLights << "\tresult += CalcColorPointLight("+ colorIdentifier +",pointLights[i], norm, FragPos ,viewDir);\n";
				}
				if(cpt+ 1 == NbLightPoint){
					AllPointLights << "}";	
				}
			}
			if( NbLightSpot > 0 && LightAffected){
				if(cpt == 0){
					AllSpotLights << "for(int i = 0; i < " + Upp::AsString(NbLightSpot) +"; i++){\n";
					AllSpotLights << "\tresult += CalcColorSpotLight("+ colorIdentifier +",spotLights[i], norm, FragPos , viewDir);\n";
				}else{
					AllSpotLights << "\tresult += CalcColorSpotLight("+ colorIdentifier +",spotLights[i], norm, FragPos , viewDir);\n";
				}
				if(cpt+ 1 == NbLightSpot){
					AllSpotLights << "}";	
				}
			}
			cpt++;
		}
		if(all_uniform.GetCount()>0) fragmentShader.Replace("//UNIFORM_MATERIAL_COLOR_NAME",all_uniform);
		
		if( NbLightDir > 0 && LightAffected){
			fragmentShader.Replace("//STRUCT_DIR_LIGHT","LIGHT_DIR_STRUCT()");
			fragmentShader.Replace("//IMPORT_DIR_LIGHT_COLOR_PROTOTYPE","LIGHT_DIR_COLOR_PROTOTYPE()");
			fragmentShader.Replace("//DIR_LIGHT_COLOR_FUNCTION","LIGHT_DIR_COLOR_FUNCTION()");	
			fragmentShader.Replace("//DIR_LIGHTS_ARRAY","uniform DirLight dirLights["+ Upp::AsString(NbLightDir) +"];");
			fragmentShader.Replace("//DIRECTIONAL_LIGHTS",AllDirLights);
		}
		if( NbLightPoint > 0&& LightAffected){
			fragmentShader.Replace("//STRUCT_POINT_LIGHT","LIGHT_POINT_STRUCT()");
			fragmentShader.Replace("//IMPORT_POINT_LIGHT_COLOR_PROTOTYPE","LIGHT_POINT_COLOR_PROTOTYPE()");
			fragmentShader.Replace("//POINT_LIGHT_COLOR_FUNCTION","LIGHT_POINT_COLOR_FUNCTION()");	
			fragmentShader.Replace("//POINT_LIGHTS_ARRAY","uniform PointLight pointLights["+ Upp::AsString(NbLightPoint) +"];");
			fragmentShader.Replace("//POINT_LIGHTS",AllPointLights);
		}
		if( NbLightSpot > 0&& LightAffected){
			fragmentShader.Replace("//STRUCT_SPOT_LIGHT","LIGHT_SPOT_STRUCT()");
			fragmentShader.Replace("//IMPORT_SPOT_LIGHT_COLOR_PROTOTYPE","LIGHT_SPOT_COLOR_PROTOTYPE()");
			fragmentShader.Replace("//SPOT_LIGHT_COLOR_FUNCTION","LIGHT_SPOT_COLOR_FUNCTION()");	
			fragmentShader.Replace("//SPOT_LIGHTS_ARRAY","uniform SpotLight spotLights["+ Upp::AsString(NbLightSpot) +"];");
			fragmentShader.Replace("//SPOT_LIGHTS",AllSpotLights);
		}
		
		if(all_VecMulti.GetCount()>12){
			if((NbLightDir > 0 || NbLightPoint > 0 || NbLightSpot > 0) && LightAffected){
				fragmentShader.Replace("//FRAG_COLOR_CALCULATION","FragColor = vec4(result,1.0);");
			}else{
				all_VecMulti << ";\n";
				fragmentShader.Replace("//FRAG_COLOR_CALCULATION",all_VecMulti);
			}
		}
	}	
	if( (NbLightDir > 0 || NbLightPoint > 0 || NbLightSpot > 0) && LightAffected){
		//HEre we ensure to put everything to start lightcalculation
		fragmentShader.Replace("//LIGHT_STARTING_DATA","vec3 norm = normalize(Normal);\nvec3 viewDir = normalize(viewPos - FragPos);\nvec3 result = vec3(0.0,0.0,0.0);\n");
	}
	if(!GetShader().AddShader(  "Vertex",VERTEX,vertexShader).AddShader("Fragment",FRAGMENT,fragmentShader).CompileShader(true)){
		LOG("ERROR : GenerateAutoShader(int,int,int) ->Shader failled to compilate !");	
	}
}
