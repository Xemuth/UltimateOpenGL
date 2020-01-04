#include "UltimateOpenGL.h"

Object3D::Object3D(){
}
Object3D::Object3D(Scene* _scene){
	scene = _scene;
}
Object3D::Object3D(Mesh& _mesh){
	Mesh& m =meshes.Create<Mesh>(_mesh);
    m.SetObject3D(this);
    transform.AddChild(&m.GetTransform());
}
Object3D::Object3D(Upp::Array<Mesh>& _meshes){
}

Object3D::Object3D(Upp::Vector<float>& Vertices, ReaderParameters readerParameter,ReaderRoutine readerRoutine){
	ReadData(Vertices,readerParameter,readerRoutine);
}

Object3D::Object3D(const Upp::String& pathOfModel, Scene*_scenePtr){
	if(_scenePtr) scene = _scenePtr;
	LoadModel(pathOfModel);
}

Object3D::Object3D(Object3D& obj) : GameObject(obj){
	textures_loaded.Append(obj.textures_loaded);	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    meshes.Append(obj.meshes);
    directory= obj.directory;
    gammaCorrection = obj.gammaCorrection; 
    loaded = obj.loaded; 
    HighPerfomanceDrawing = obj.HighPerfomanceDrawing;
    HighPerfShader = obj.HighPerfShader;
    
	transform = obj.transform;
	transform.ClearChilds();
    for(Mesh& m : meshes){
    	transform.AddChild(&m.GetTransform());
    }
    
	
	MeshShaderToUse=obj.MeshShaderToUse;
	ShaderToUse=obj.ShaderToUse;
	
	MeshIndiceToUse=obj.MeshIndiceToUse;
	IndicesToUse.Append(obj.IndicesToUse);
}
Object3D::~Object3D(){
	/*if(modelMatrices) delete modelMatrices;
	if(positionVector) delete positionVector; //Stock position of each mesh information
	if(normalVector) delete normalVector; //Stock normal if it existe
	if(textureCoordinateVector) delete textureCoordinateVector; //Stock TC if it existe
	if(tangeant) delete tangeant;
	if(biTangeant) delete biTangeant;*/
}
Object3D& Object3D::operator=(Object3D& obj){
	textures_loaded.Append(obj.textures_loaded);	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    meshes.Append(obj.meshes);
    directory= obj.directory;
    gammaCorrection = obj.gammaCorrection; 
    
    transform = obj.transform;
    transform.ClearChilds();
    for(Mesh& m : meshes){
    	transform.AddChild(&m.GetTransform());
    }
    
    loaded = obj.loaded; 
	HighPerfomanceDrawing = obj.HighPerfomanceDrawing;
    HighPerfShader = obj.HighPerfShader;   
       
	MeshShaderToUse=obj.MeshShaderToUse;
	ShaderToUse=obj.ShaderToUse;
	
	MeshIndiceToUse=obj.MeshIndiceToUse;
	IndicesToUse.Append(obj.IndicesToUse);
	return *this;
}

void Object3D::LoadModel(const Upp::String& path){
	Upp::String realPath =UltimateOpenGL_Context::TransformFilePath(path);
    // read file via ASSIMP
    Assimp::Importer importer;
  //  const aiScene* scene = importer.ReadFile(realPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |aiProcess_GenNormals );
    const aiScene* scene = importer.ReadFile(realPath,aiProcess_JoinIdenticalVertices |		// join identical vertices/ optimize indexing
		aiProcess_ValidateDataStructure |		// perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality |		// improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials |	// remove redundant materials
		aiProcess_GenUVCoords |					// convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords |			// pre-process UV transformations (scaling, translation ...)
		//aiProcess_FindInstances |				// search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights |			// limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes |				// join small meshes, if possible;
		//aiProcess_PreTransformVertices |
		aiProcess_GenSmoothNormals |			// generate smooth normal vectors if not existing
		aiProcess_SplitLargeMeshes |			// split large, unrenderable meshes into sub-meshes
		aiProcess_Triangulate |					// triangulate polygons with more than 3 edges
		aiProcess_ConvertToLeftHanded |			// convert everything to D3D left handed space
		aiProcess_SortByPType);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        Upp::Cout() << "ERROR::ASSIMP:: " << Upp::String(importer.GetErrorString()) << Upp::EOL;
        return;
    }
    // retrieve the directory path of the filepath
    directory = realPath.Left( realPath.ReverseFind('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}

void Object3D::ReadData(Upp::Vector<float>& data,ReaderParameters readerParameter,ReaderRoutine readerRoutine){
	Mesh* m =nullptr;
	int start = readerRoutine.startMesh;
	Upp::Vector<float> Copie;
	Copie.Append(data);
	Upp::Vector<float> dataBuffer;
	bool stop = false;
	bool created = false;	
	
	int NumberOfFloatByLine = 0;
	if(readerParameter.coordinatesPosition !=-1) NumberOfFloatByLine+=3;
	if(readerParameter.normalPosition != -1)NumberOfFloatByLine+=3;
	if(readerParameter.textureCoordinatePosition != -1)NumberOfFloatByLine+=2;
	if(readerParameter.tangentPosition != -1)NumberOfFloatByLine+=3;
	if(readerParameter.bitangentPosition != -1)	NumberOfFloatByLine+=3;
	
	while(!stop){
		if(readerRoutine.verticesPerMesh != -1){
			if(Copie.GetCount() >= NumberOfFloatByLine*readerRoutine.verticesPerMesh){
				for(int r = 0; r < NumberOfFloatByLine*readerRoutine.verticesPerMesh;r++){
					dataBuffer.Add(Copie[r]);
				}
				Copie.Remove(0,NumberOfFloatByLine*readerRoutine.verticesPerMesh);
			}
			if(dataBuffer.GetCount()<NumberOfFloatByLine*readerRoutine.verticesPerMesh)stop=true;
		}else{
			dataBuffer.Append(Copie);
			stop=true;
		}
		
		if(start != -1){
			for(int e = 0; e < meshes.GetCount();e++){
				if(e == readerRoutine.startMesh){
					m = &meshes[e];
					start++;					
					break;
				}
			}
		}else{
			if(readerRoutine.allowCreation){
				if(dataBuffer.GetCount() > 0){
					m = &meshes.Add();
				}
				created=true;
			}else{
				LOG("Warning : void Object3D::ReadData(...) No starting mesh defined in readerRoutine and allowCreation set to false... First Mesh is mesh selected by default");
				if(meshes.GetCount()>0) m = &meshes[0];
			}
		}
		if(m){
			if(dataBuffer.GetCount() > 0){
				if(m->ReadData(dataBuffer,readerParameter)){
					m->SetObject3D(this);
					transform.AddChild(&(m->GetTransform()));
				//	LOG("Log : void Object3D::ReadData(...) Data have been readed succesfully !");	
				}else{
					if(created){
						meshes.Remove(meshes.GetCount()-1);
						created=false;
					}
					LOG("Error : void Object3D::ReadData(...) Error during process of data !");
				}
				dataBuffer.Clear();
			}
			m = nullptr;
		}else if(created){
			//Useless
		}else{
			LOG("Error : void Object3D::ReadData(...) Error during resolution of mesh to affect !");
		}
	}
}

Object3D& Object3D::ActivateHighPerfomance(){
	if(!loaded) HighPerfomanceDrawing = true;
	else LOG("Error : Object3D& Object3D::ActivateHighPerfomance() Object3D is loaded so it can't be swap to high performance !");
	return *this;
}
Object3D& Object3D::DesactivateHighPerfomance(){
	if(!loaded) HighPerfomanceDrawing = false;
	else LOG("Error : Object3D& Object3D::DesactivateHighPerfomance() Object3D is loaded so it can't be swap off to high performance !");
	return *this;
}

void Object3D::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera){
	if(onDraw != nullptr){
		onDraw(*this);	
	}
	if(HighPerfomanceDrawing &&  (ShaderToUse ||MeshShaderToUse > -1)){
		if(HighPerfShader.IsCompiled()){
			HighPerfShader.Use();
		    HighPerfShader.SetMat4("view",view);
		    HighPerfShader.SetMat4("projection", projection);
		    HighPerfShader.SetMat4("model", glm::translate(model,GetTransform().GetPosition())*glm::mat4_cast(GetTransform().GetQuaterion())*GetTransform().GetModelMatrixScaller());
			//TODO
			//Here we must set everything relative to the uniform of the shader
			//HighPerfShader.SetMaterialColor()
			//meshes[200].SetDrawMethod(DM_QUADS);
			for(Mesh& mes : meshes){
				glBindVertexArray(mes.GetVAO());
		        glDrawElements(mes.ResolveDrawMethod(), mes.GetIndices().GetCount(), GL_UNSIGNED_INT, 0);
		        glBindVertexArray(0);
			}
		}
	}else{
		for(int i = 0; i < meshes.size(); i++) // meshes.size(); i++)//Changement made by Iñaki
        	meshes[i].Draw(model,view,projection,transform,camera);
	}
}
void Object3D::Load(){
	if(!loaded){
		bool loopStartZero =true;
		Shader* shad = nullptr;
		Upp::Vector<unsigned int> ind;
		
		if(HighPerfomanceDrawing){
			GenerateHighPerfomanceShader();
			if(HighPerfShader.IsCompiled()){
				if(IndicesToUse.GetCount()> 0){
					ind.Append(IndicesToUse);
				}else if(MeshIndiceToUse != -1){
					if(meshes.GetCount() > 0){
						if(meshes[0].GetIndices().GetCount() == 0){
							meshes[0].LoadDefaultIndices();
						}
						ind.Append(meshes[0].GetIndices());
					}
				}
				
				
				/*positionVector = new glm::vec3[meshes.size()];;
			    normalVector = new glm::vec3[meshes.size()];;
			    textureCoordinateVector = new glm::vec2[meshes.size()]; //Stock TC if it existe
			    tangeant = new glm::vec3[meshes.size()];;
			    biTangeant =new glm::vec3[meshes.size()];;
				modelMatrices = new glm::mat4[meshes.size()];*/
				
				for(int i = 0; i < meshes.size(); i++){//Changement made by Iñaki
					meshes[i].SetShader(HighPerfShader);
					if(ind.GetCount()>0) meshes[i].SetIndices(ind);
			    	meshes[i].Load(); 
			    	/*
			    	unsigned int buffer;
				    glGenBuffers(1, &buffer);
				    glBindBuffer(GL_ARRAY_BUFFER, buffer);
				    glBufferData(GL_ARRAY_BUFFER, meshes.size() * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
			    	
			    	//glBindVertexArray(meshes[i].GetVAO());
			    	// set attribute pointers for matrix (4 times vec4)
			        glEnableVertexAttribArray(5);
			        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			        glEnableVertexAttribArray(6);
			        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			        glEnableVertexAttribArray(7);
			        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			        glEnableVertexAttribArray(8);
			        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
			
			        glVertexAttribDivisor(5, 1);
			        glVertexAttribDivisor(6, 1);
			        glVertexAttribDivisor(7, 1);
			        glVertexAttribDivisor(8, 1);
			
			        glBindVertexArray(0);*/
			    	
				}
			}
			loaded =true;
		}else{
			if(ShaderToUse){
				shad = ShaderToUse;
			}else if(MeshShaderToUse != -1){
				if(meshes.GetCount() > 0){
					meshes[0].Load();
					shad = &(meshes[0].GetShader());
					loopStartZero =false;
				}
			}
			if(IndicesToUse.GetCount()> 0){
				ind.Append(IndicesToUse);
			}else if(MeshIndiceToUse != -1){
				if(meshes.GetCount() > 0){
					if(meshes[0].GetIndices().GetCount() == 0){
						meshes[0].LoadDefaultIndices();
					}
					ind.Append(meshes[0].GetIndices());
				}
			}
			for(int i = ((loopStartZero)?0:1) ; i < meshes.size(); i++){//Changement made by Iñaki
				if(shad)meshes[i].SetShader(*shad);
				if(ind.GetCount()>0) meshes[i].SetIndices(ind);
		    	meshes[i].Load();
			}
			loaded =true;
		}
	}else{
		LOG("Error : void Object3D::Load() Object3D named : " + name + " is still loaded !");	
	}
}

void Object3D::ProcessNode(aiNode *node, const aiScene *scene){
	// process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        ProcessMesh(mesh, scene);			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Object3D::ManageTextures(Upp::Vector<Texture>& vectorToFile, aiMaterial *mat, aiTextureType type){
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
    	aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        if(this->scene->GetContext()->GetTexture(Upp::String(str.C_Str())).IsLoaded()){
            vectorToFile.Add(this->scene->GetContext()->GetTexture(Upp::String(str.C_Str())));
        }else{
            TextureType t = TT_DIFFUSE;
            if(type == aiTextureType_DIFFUSE)
            	t=TT_DIFFUSE;
            else if(type == aiTextureType_SPECULAR)
                t=TT_SPECULAR;
            else if(type == aiTextureType_HEIGHT)
                t=TT_HEIGHT;
            else if(type == aiTextureType_NORMALS)
                t=TT_NORMAL;
            
            vectorToFile.Add(GetScene()->GetContext()->AddTexture(Upp::String(str.C_Str()),directory+"/"+ Upp::String(str.C_Str()),t,false,false));      
        }
    }
}

Object3D& Object3D::UseOneShader(int _MeshShaderToUse){
	MeshShaderToUse = _MeshShaderToUse;
	return *this;
} //Define if the object should use one shader to all is mesh
Object3D& Object3D::UseOneShader(Shader* MeshShaderToUse){ //Define if the object should use one shader to all is mesh
	ShaderToUse = MeshShaderToUse;
	return *this;
}

Object3D& Object3D::UseSameIndices(int _MeshIndicestoUse){
	MeshIndiceToUse = _MeshIndicestoUse;
	return *this;
}
Object3D& Object3D::UseSameIndices(const Upp::Vector<unsigned int>& _indicesToUse){
	IndicesToUse.Append(_indicesToUse);
	return *this;
}


Object3D& Object3D::BindTexture(const Upp::String& TextureName,float mixValue, float textureShininess,const Upp::String& TextureSpecularName, const Upp::String& NormalMappingTextureName){
	//you may ask "why the hell you dont just call Mesh.BindTexture Method   ?? symply because
	//I dont want to resolve same texture again and again on each mesh.
	if(GetScene() != nullptr && GetScene()->GetContext() !=nullptr){	
		Texture t =GetScene()->GetContext()->GetTexture(TextureName);
		if(t.IsLoaded()){
			MaterialTexture m;
			m.SetDiffuse(t.GetTextureIterator()).SetMix(mixValue).SetShininess(textureShininess);
			if(TextureSpecularName.GetCount() > 0){
				Texture tSpeculare =GetScene()->GetContext()->GetTexture(TextureName);
				if(tSpeculare.IsLoaded()){
					/*** Here I add the texture to material ***/
					m.SetSpecular(tSpeculare.GetId());
					if(tSpeculare.GetType() != TT_SPECULAR)
						LOG("Warning : Mesh& Mesh::BindTexture(...) You are binding as speculare a texture wich is not a speculare type !");
				}else{
					LOG("Error : Mesh& Mesh::BindTexture(...) Specular texture of " + name +" named " + tSpeculare.GetName() +" is not loaded !" );
				}
			}
			if(NormalMappingTextureName.GetCount() > 0){
				Texture tNormal = GetScene()->GetContext()->GetTexture(TextureName);
				if(tNormal.IsLoaded()){
					/**Here I add the texture to material ***/
					m.SetNormal(tNormal.GetId());
					if(tNormal.GetType() != TT_NORMAL)
						LOG("Warning : Mesh& Mesh::BindTexture(...) You are binding as Normal a texture wich is not a Normal type !");
				}else{
					LOG("Error : Mesh& Mesh::BindTexture(...) Normal texture of " + name +" named " + tNormal.GetName() +" is not loaded !" );
				}
			}
			if(t.GetType() != TT_DIFFUSE)
				LOG("Warning : Mesh& Mesh::BindTexture(...) You are binding as Diffuse a texture wich is not a Diffuse type !");

			for(Mesh& mes : meshes){
				mes.GetMaterialTextures().Put(t.GetName(),m);
			}
			return *this;
		}
		else{
			LOG("Error : " +TextureName + " texture don't existe in context, you must add it before getting it !");
			return *this;
		}
	}
	LOG("Error : Object3D" + name +" is not bind to Scene or Scene is not bind to Context wich is carrying texture");
	return *this;
}

Object3D& Object3D::AddMaterialColor(const Upp::String& ColorName,MaterialColor materialColor){
	for(Mesh& mes : meshes){
		if(mes.GetMaterialColor().Find(ColorName) ==-1){
			mes.GetMaterialColor().Add(ColorName,materialColor);
		}
	}
	return *this;
}

Object3D& Object3D::SetLightAffected(bool b){
	for(Mesh& mes : meshes){
		mes.SetLightAffected(b);
	}
	return *this;
}

Object3D& Object3D::SetAlaphaAffected(bool b){
	for(Mesh& mes : meshes){
		mes.UseAlpha(b);
	}
	return *this;
}

Object3D& Object3D::SetDrawMethod(DrawMethod dm){
	for(Mesh& mes : meshes){
		mes.SetDrawMethod(dm);
	}
	return *this;
}

Upp::Array<Mesh>& Object3D::GetAllMeshes(){
	return meshes;
}

void Object3D::ProcessMesh(aiMesh *mesh, const aiScene *scene){
	// data to fill
    Upp::Vector<Vertex> vertices;
    Upp::Vector<unsigned int> indices;
    Upp::Vector<Texture> textures;

    // Walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = float(mesh->mVertices[i].x);//Changement made by Iñaki
        vector.y = float(mesh->mVertices[i].y);//Changement made by Iñaki
        vector.z = float(mesh->mVertices[i].z);//Changement made by Iñaki
        vertex.Position = vector;
        // normals
        vector.x = float(mesh->mNormals[i].x);//Changement made by Iñaki
        vector.y = float(mesh->mNormals[i].y);//Changement made by Iñaki
        vector.z = float(mesh->mNormals[i].z);//Changement made by Iñaki
        vertex.Normal = vector;
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = float(mesh->mTextureCoords[0][i].x); //Changement made by Iñaki
            vec.y = float(mesh->mTextureCoords[0][i].y); //Changement made by Iñaki
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        // tangent
       	/*vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.Tangent = vector;
        // bitangent
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.Bitangent = vector;*/
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN	
    // normal: texture_normalN


    // 1. diffuse maps
    if(this->scene != nullptr && this->scene->GetContext() != nullptr){
     	ManageTextures(textures,material,aiTextureType_DIFFUSE);
     //	Upp::Cout() << "Nb diffuse " << Upp::AsString(textures.GetCount()) << Upp::EOL;
     	ManageTextures(textures,material,aiTextureType_SPECULAR);
     //	Upp::Cout() << "Nb speculare + diffuse " << Upp::AsString(textures.GetCount()) << Upp::EOL;
     	ManageTextures(textures,material,aiTextureType_HEIGHT);
     //	Upp::Cout() << "Nb Height +  speculare + diffuse " << Upp::AsString(textures.GetCount()) << Upp::EOL;
     	ManageTextures(textures,material,aiTextureType_AMBIENT);
    // 	Upp::Cout() << "Nb  AMBIENT + Height +  speculare + diffuse " << Upp::AsString(textures.GetCount()) << Upp::EOL;
     	ManageTextures(textures,material,aiTextureType_NORMALS);
     //	Upp::Cout() << "Nb  NORMAL  + AMBIENT + Height +  speculare + diffuse " << Upp::AsString(textures.GetCount()) << Upp::EOL;
    }else{
        LOG("Warning : cant load and Bind texture since no context is linked to object3D : " + name );
    }
    
    // return a mesh object created from the extracted mesh data
    Mesh& m =  meshes.Create<Mesh>(vertices, indices, textures);
    m.SetObject3D(this);
   	GetTransform().AddChild(&m.GetTransform());
}


void Object3D::GenerateHighPerfomanceShader(){
	Upp::String vertexShader = BasicShaders.Get("Default_Vertex_Shader");
	Upp::String fragmentShader = BasicShaders.Get("Default_Fragment_Shader");
	
	Upp::String all_uniform="";
	Upp::String all_VecMulti= "";
	all_VecMulti= "FragColor = ";
	
	Upp::String AllDirLights="";
	Upp::String AllPointLights="";
	Upp::String AllSpotLights="";	
	

	vertexShader.Replace("//LAYOUT_POSITION","layout (location = 0) in vec3 aPos;");
	vertexShader.Replace("//LAYOUT_NORMAL","layout (location = 1) in vec3 aNormal;");	
	vertexShader.Replace("//LAYOUT_TEXTURE_COORD","layout (location = 2) in vec2 aTexCoord;");
	vertexShader.Replace("//LAYOUT_TANGEANT","layout (location =3) in vec3 aTangent;");	
	vertexShader.Replace("//LAYOUT_BITANGEANT","layout (location =4) in vec3 aBiTangent;");	
	vertexShader.Replace("//LAYOUT_MODEL_MATRIX","layout (location = 5) in mat4 aInstanceMatrix;\n");
	
//	vertexShader.Replace("//NORMAL_CALCULATION","Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;\n");
	vertexShader.Replace("//NORMAL_CALCULATION","Normal = mat3(transpose(inverse(model))) * aNormal;\n");
	//vertexShader.Replace("//FRAGPOS_CALCULATION","FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));\n");
	vertexShader.Replace("//FRAGPOS_CALCULATION","FragPos = vec3(model * vec4(aPos, 1.0));\n");
	//vertexShader.Replace("//POSITION_CALCULATION","gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0f);\n");
	vertexShader.Replace("//POSITION_CALCULATION","gl_Position = projection * view * model * vec4(aPos, 1.0f);\n");
	vertexShader.Replace("//UNIFORM_MODEL","uniform mat4 model;\n");
	if(true){//materialsColor.GetCount() > 0){
		//Vertex shader generation
		fragmentShader.Replace("//STRUCT_MATERIAL_COLOR","MATERIAL_COLOR_STRUCT()");

		fragmentShader.Replace("//STRUCT_MATERIAL_COLOR","MATERIAL_COLOR_STRUCT()");
		fragmentShader.Replace("//OUT_FRAG_COLOR","out vec4 FragColor;");
		 /*
		int cpt = 0;
		for(const Upp::String& key : materialsColor.GetKeys()){
			Upp::String colorIdentifier = "color" + Upp::AsString(cpt) ;
			all_uniform << "uniform MaterialColor " <<  colorIdentifier << ";\n";
			all_VecMulti << ((cpt > 0)?"*":"") <<  "vec4(" + colorIdentifier + ".diffuse,1.0)"; 
			cpt++;
		}
		if(all_uniform.GetCount()>0) fragmentShader.Replace("//UNIFORM_MATERIAL_COLOR_NAME",all_uniform);
		
		if( NbLightDir > 0 && LightAffected){
			cpt = 0;
			for(const Upp::String& key : materialsColor.GetKeys()){
				Upp::String colorIdentifier = "color" + Upp::AsString(cpt) ;
				if(cpt == 0){
					AllDirLights << "for(int i = 0; i < " + Upp::AsString(NbLightDir) +"; i++){\n";
					AllDirLights << "\tresult += CalcColorDirLight("+ colorIdentifier +",dirLights[i], norm, viewDir);\n";
				}else{
					AllDirLights << "\tresult += CalcColorDirLight("+ colorIdentifier +",dirLights[i], norm, viewDir);\n";
				}
			}
			AllDirLights << "}";
			fragmentShader.Replace("//STRUCT_DIR_LIGHT","LIGHT_DIR_STRUCT()");
			fragmentShader.Replace("//IMPORT_DIR_LIGHT_COLOR_PROTOTYPE","LIGHT_DIR_COLOR_PROTOTYPE()");
			fragmentShader.Replace("//DIR_LIGHT_COLOR_FUNCTION","LIGHT_DIR_COLOR_FUNCTION()");	
			fragmentShader.Replace("//DIR_LIGHTS_ARRAY","uniform DirLight dirLights["+ Upp::AsString(NbLightDir) +"];");
			fragmentShader.Replace("//DIRECTIONAL_LIGHTS",AllDirLights);
		}
		if( NbLightPoint > 0&& LightAffected){
			cpt = 0;
			for(const Upp::String& key : materialsColor.GetKeys()){
				Upp::String colorIdentifier = "color" + Upp::AsString(cpt) ;
				if(cpt == 0){
					AllPointLights << "for(int i = 0; i < " + Upp::AsString(NbLightPoint) +"; i++){\n";
					AllPointLights << "\tresult += CalcColorPointLight("+ colorIdentifier +",pointLights[i], norm, FragPos ,viewDir);\n";
				}else{
					AllPointLights << "\tresult += CalcColorPointLight("+ colorIdentifier +",pointLights[i], norm, FragPos ,viewDir);\n";
				}
			}
			AllPointLights << "}";
			fragmentShader.Replace("//STRUCT_POINT_LIGHT","LIGHT_POINT_STRUCT()");
			fragmentShader.Replace("//IMPORT_POINT_LIGHT_COLOR_PROTOTYPE","LIGHT_POINT_COLOR_PROTOTYPE()");
			fragmentShader.Replace("//POINT_LIGHT_COLOR_FUNCTION","LIGHT_POINT_COLOR_FUNCTION()");	
			fragmentShader.Replace("//POINT_LIGHTS_ARRAY","uniform PointLight pointLights["+ Upp::AsString(NbLightPoint) +"];");
			fragmentShader.Replace("//POINT_LIGHTS",AllPointLights);
		}
		if( NbLightSpot > 0&& LightAffected){
			cpt = 0;
			for(const Upp::String& key : materialsColor.GetKeys()){
				Upp::String colorIdentifier = "color" + Upp::AsString(cpt) ;
				if(cpt == 0){
					AllSpotLights << "for(int i = 0; i < " + Upp::AsString(NbLightSpot) +"; i++){\n";
					AllSpotLights << "\tresult += CalcColorSpotLight("+ colorIdentifier +",spotLights[i], norm, FragPos , viewDir);\n";
				}else{
					AllSpotLights << "\tresult += CalcColorSpotLight("+ colorIdentifier +",spotLights[i], norm, FragPos , viewDir);\n";
				}
			} 
			AllSpotLights << "}";
			fragmentShader.Replace("//STRUCT_SPOT_LIGHT","LIGHT_SPOT_STRUCT()");
			fragmentShader.Replace("//IMPORT_SPOT_LIGHT_COLOR_PROTOTYPE","LIGHT_SPOT_COLOR_PROTOTYPE()");
			fragmentShader.Replace("//SPOT_LIGHT_COLOR_FUNCTION","LIGHT_SPOT_COLOR_FUNCTION()");	
			fragmentShader.Replace("//SPOT_LIGHTS_ARRAY","uniform SpotLight spotLights["+ Upp::AsString(NbLightSpot) +"];");
			fragmentShader.Replace("//SPOT_LIGHTS",AllSpotLights);
		}*/
			fragmentShader.Replace("//FRAG_COLOR_CALCULATION","FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n");
		
		if(all_VecMulti.GetCount()>12){
			all_VecMulti << ";\nFragColor =texColor;\n";
			fragmentShader.Replace("//FRAG_COLOR_CALCULATION",all_VecMulti);
		}
		
	}
	/*
	if( (NbLightDir > 0 || NbLightPoint > 0 || NbLightSpot > 0) && LightAffected){
		//HEre we ensure to put everything to start lightcalculation
		fragmentShader.Replace("//LIGHT_STARTING_DATA","vec3 norm = normalize(Normal);\nvec3 viewDir = normalize(viewPos - FragPos);\nvec3 result = vec3(0.0,0.0,0.0);\n");
	} */
	if(!HighPerfShader.AddShader(  "Vertex",ST_VERTEX,vertexShader).AddShader("Fragment",ST_FRAGMENT,fragmentShader).CompileShader(true)){
		LOG("Class Mesh:(ERROR) void Mesh::GenerateAutoShader(int,int,int) ->Shader failled to compilate !");	
	}
}