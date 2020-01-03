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

void Object3D::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera){
	if(onDraw != nullptr){
		onDraw(*this);	
	}
	for(int i = 0; i < meshes.size(); i++) // meshes.size(); i++)//Changement made by Iñaki
        meshes[i].Draw(model,view,projection,transform,camera);
}
void Object3D::Load(){
	bool loopStartZero =true;
	Shader* shad = nullptr;
	Upp::Vector<unsigned int> ind;
	
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
            TextureType t = DIFFUSE;
            if(type == aiTextureType_DIFFUSE)
            	t=DIFFUSE;
            else if(type == aiTextureType_SPECULAR)
                t=SPECULAR;
            else if(type == aiTextureType_HEIGHT)
                t=HEIGHT;
            else if(type == aiTextureType_NORMALS)
                t=NORMAL;
            
            vectorToFile.Add(GetScene()->GetContext()->AddTexture(Upp::String(str.C_Str()),directory+"/"+ Upp::String(str.C_Str()),t,false,false));      
        }
    }
}

Object3D& Object3D::UseOneShader(int _MeshShaderToUse){
	MeshShaderToUse = _MeshShaderToUse;
} //Define if the object should use one shader to all is mesh
Object3D& Object3D::UseOneShader(Shader* MeshShaderToUse){ //Define if the object should use one shader to all is mesh
	ShaderToUse = MeshShaderToUse;
}

Object3D& Object3D::UseSameIndices(int _MeshIndicestoUse){
	MeshIndiceToUse = _MeshIndicestoUse;
}
Object3D& Object3D::UseSameIndices(const Upp::Vector<unsigned int>& _indicesToUse){
	IndicesToUse.Append(_indicesToUse);
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
					if(tSpeculare.GetType() != SPECULAR)
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
					if(tNormal.GetType() != NORMAL)
						LOG("Warning : Mesh& Mesh::BindTexture(...) You are binding as Normal a texture wich is not a Normal type !");
				}else{
					LOG("Error : Mesh& Mesh::BindTexture(...) Normal texture of " + name +" named " + tNormal.GetName() +" is not loaded !" );
				}
			}
			if(t.GetType() != DIFFUSE)
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