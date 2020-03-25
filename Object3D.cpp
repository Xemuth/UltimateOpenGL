#include "Object3D.h"
#include "Mesh.h"
#include "UltimateOpenGL.h"
#include <plugin/assimp/assimp.h>
Object3D::Object3D(){}
Object3D::Object3D(Mesh& _mesh){
	Mesh& m =meshes.Create<Mesh>(_mesh);
	transform.AddChildren(m.GetTransform());
}
Object3D::Object3D(Upp::Vector<Mesh>& _meshes){
	for(Mesh& m : _meshes){
		Mesh& m2 =meshes.Create<Mesh>(m);
		m2.SetObject3D(*this);
		transform.AddChildren(m2.GetTransform());
	}
}
Object3D::Object3D(Upp::Vector<float>& Vertices, ReaderParameters readerParameter, ReaderRoutine readerRoutine){ //way of reading a floating point vector
	ReadData(Vertices,readerParameter,readerRoutine);
}
/*Object3D::Object3D(const Upp::String& pathOfModel){//Path of model to load || METHOD ONLY since Model loading requiere to have a scene binded
	LoadModel(pathOfModel);
}*/
Object3D::Object3D(Object3D& _object){//Be carefull of setting the Scene
	*this = _object;
}
Object3D& Object3D::operator=(Object3D& _object){//Be carefull of setting the Scene
	for(Mesh& m : _object.meshes){
		Mesh& m2 =meshes.Create<Mesh>(m);
		m2.SetObject3D(*this);
		transform.AddChildren(m2.GetTransform());
	}
	directory= _object.directory;
	loaded = _object.loaded;
	return *this;
}
Object3D::~Object3D(){
	meshes.Clear();
}

Upp::Array<Mesh>& Object3D::GetMeshes(){//Return Array of Mesh
	return meshes;
}
Upp::String Object3D::GetModelDirectory(){ //Return Model directory if it have been loaded one time at least
	return directory;
}
bool Object3D::IsLoaded(){ //Return true if the object is loaded
	return loaded;
}

Object3D& Object3D::SetMaterialForMeshes(const Material* material){
	for(Mesh& m : meshes){
		m.SetMaterial( material);
	}
	return *this;
}
Object3D& Object3D::SetMaterialForMeshes(Material& material){
	for(Mesh& m : meshes){
		m.SetMaterial( material);
	}
	return *this;
}
Object3D& Object3D::SetDrawMethodForMeshes(unsigned int GL_DRAW_METHOD){
	for(Mesh& m : meshes){
		m.SetDrawMethod(GL_DRAW_METHOD);
	}
	return *this;
}

Object3D& Object3D::AssignShaderToMeshes(Shader& shader){
	if(shader.IsCompiled()){
		for(Mesh& m : meshes){
			m.GetShader() = shader;
		}
	}
	return *this;
}
Object3D& Object3D::UseSameShaderMeshes(int MeshToUse){
	if(meshes.GetCount() > MeshToUse &&  meshes[MeshToUse].GetShader().IsCompiled()){
		int cpt = 0;
		for(Mesh& m : meshes){
			if(MeshToUse != cpt) m.GetShader() = meshes[MeshToUse].GetShader();
			cpt++;
		}
	}
	return *this;
}
Object3D& Object3D::EnableSlowDraw(){SlowDraw = true;return *this;}
Object3D& Object3D::DisableSlowDraw(){SlowDraw = false;return *this;}
bool Object3D::IsSlowDraw(){return SlowDraw;}

Object3D& Object3D::LoadModel(const Upp::String& path){ //Used to load 3D Model
	Upp::String realPath =TransformFilePath(path);
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
        LOG("ERROR::ASSIMP:: " << Upp::String(importer.GetErrorString()));
        return *this;
    }
    // retrieve the directory path of the filepath
    directory = realPath.Left( realPath.ReverseFind('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
    return *this;
}
Object3D& Object3D::ReadData(Upp::Vector<float>& data ,ReaderParameters readerParameter,ReaderRoutine readerRoutine){ //Used to read vector<Float>
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
	if(readerParameter.colorPosition != -1 && readerParameter.colorType == CT_RGB) NumberOfFloatByLine+=3;
	else if(readerParameter.colorPosition != -1 && readerParameter.colorType == CT_RGBA)NumberOfFloatByLine+=4;
		
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
					m = &(meshes.Create<Mesh>(*this)); // Original line, Here happen a memory violation
				}
				created=true;
			}else{
				LOG("Warning : void Object3D::ReadData(...) No starting mesh defined in readerRoutine and allowCreation set to false... First Mesh is mesh selected by default");
				if(meshes.GetCount()>0) m = &meshes[0];
			}
		}
		if(m){
			if(dataBuffer.GetCount() > 0){
				if(m->ReadData(dataBuffer,readerParameter, readerRoutine.useMaterialColor)){
					m->SetObject3D(*this);
					transform.AddChildren(m->GetTransform());
				//LOG("Log : void Object3D::ReadData(...) Data have been readed succesfully !");
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
	return *this;
}

//Loading 3D Model using Assimp and custom loader
void Object3D::ProcessNode(aiNode *node, const aiScene *scene){//Used to load 3D Model
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
void Object3D::ProcessMesh(aiMesh *mesh, const aiScene *scene){
	// data to fill
    Upp::Vector<Vertex> vertices;
    Upp::Vector<unsigned int> indices;
    //Upp::Vector<Texture> textures;

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
    const Material* mat =nullptr;
    if(this->scene != nullptr){
        mat = CreateAndBindMaterial(material,aiTextureType_DIFFUSE);
	    CreateAndBindMaterial(material,aiTextureType_SPECULAR);
	    CreateAndBindMaterial(material,aiTextureType_HEIGHT);
	    CreateAndBindMaterial(material,aiTextureType_AMBIENT);
	    CreateAndBindMaterial(material,aiTextureType_NORMALS);
    }else{
        LOG("Warning : cant load and Bind texture since no context is linked to object3D : " + name );
    }
    // return a mesh object created from the extracted mesh data
    Mesh& m =  meshes.Create<Mesh>(vertices, indices); //, textures);
    m.SetObject3D(*this);
    if(mat)m.SetMaterial(mat);
	GetTransform().AddChildren(m.GetTransform());
}
const Material* Object3D::CreateAndBindMaterial(aiMaterial *mat, aiTextureType type){
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Upp::String name = Upp::String(str.C_Str());
		Upp::String NameClear = (name.Find(".") != -1)? name.Left(name.Find(".")) : name;
		if(GetScene().GetContext().IsMaterialExist(NameClear)){
			return &(GetScene().GetContext().GetAllMaterials().Get(NameClear));
		}else{
			if(type == aiTextureType_DIFFUSE)
				return &(GetScene().GetContext().CreateMaterial<Texture2D>(NameClear,directory+"/"+ name).DisableFlipOnLoad().Load());
			else if(type == aiTextureType_SPECULAR)
			    return nullptr;
			else if(type == aiTextureType_HEIGHT)
			    return nullptr;
			else if(type == aiTextureType_NORMALS)
				return nullptr;
		}
		//vectorToFile.Add(GetScene()->GetContext()->AddTexture(Upp::String(str.C_Str()),directory+"/"+ Upp::String(str.C_Str()),t,false,false));      
	}
	return nullptr;
}
//Override
Object3D* Object3D::Clone(){
	return new Object3D(*this);
}
void Object3D::Load(){
	if(!loaded){
		for(int i = 0 ; i < meshes.size(); i++){//Changement made by Iñaki
			meshes[i].Load(i);
		}
		loaded =true;
	}else{
		LOG("Error : void Object3D::Load() Object3D named : " + name + " is still loaded !");
	}
}
void Object3D::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera){
	if(onDraw != nullptr){
		onDraw(*this);
	}
	for(int i = 0; i < meshes.size(); i++){ // meshes.size(); i++)//Changement made by Iñaki
		bool DifferentShader = (i > 0 && meshes[i-1].GetShader().GetId() != meshes[i].GetShader().GetId());
		bool DifferentMaterial = (i > 0 && meshes[i-1].GetMaterial() && meshes[i-1].GetMaterial()->GetName() != meshes[i].GetMaterial()->GetName());
		meshes[i].Draw(i,model,view,projection,transform,DifferentShader,DifferentMaterial);
	}
}