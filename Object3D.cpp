#include "Object3D.h"
#include "Mesh.h"
Object3D::Object3D(Scene& _scene) : GameObject(_scene,GOT_3D){}
Object3D::Object3D(Scene& _scene, Mesh& _mesh) : GameObject(_scene,GOT_3D){
	Mesh& m =meshes.Create<Mesh>(_mesh);
	transform.AddChildren(m.GetTransform());
}
Object3D::Object3D(Scene& _scene, Upp::Vector<Mesh>& _meshes) : GameObject(_scene,GOT_3D){
	for(Mesh& m : _meshes){
		Mesh& m2 =meshes.Create<Mesh>(m);
		transform.AddChildren(m2.GetTransform());
	}
}
Object3D::Object3D(Scene& _scene, Upp::Vector<float>& Vertices, ReaderParameters readerParameter, ReaderRoutine readerRoutine) : GameObject(_scene,GOT_3D){
	ReadData(Vertices,readerParameter,readerRoutine);
}
Object3D::Object3D(Scene& _scene, const Upp::String& pathOfModel) : GameObject(_scene,GOT_3D){
	LoadModel(pathOfModel);
}
Object3D::Object3D(Scene& _scene, Upp::String _name, Upp::Vector<Mesh>& _meshes) : GameObject(_scene,GOT_3D){
	name = _name;
	for(Mesh& m : _meshes){
		Mesh& m2 =meshes.Create<Mesh>(m);
		transform.AddChildren(m2.GetTransform());
	}
}
Object3D::Object3D(Scene& _scene, Upp::String _name, Upp::Vector<float>& Vertices, ReaderParameters readerParameter, ReaderRoutine readerRoutine) : GameObject(_scene,GOT_3D){
	name = _name;
	ReadData(Vertices,readerParameter,readerRoutine);
}
Object3D::Object3D(Scene& _scene, Upp::String _name, const Upp::String& pathOfModel) : GameObject(_scene,GOT_3D){
	name = _name;
	LoadModel(pathOfModel);
}
Object3D::Object3D(Object3D& _object) : GameObject(_object){//Be carefull of setting the Scene
	meshes.Append(_object.meshes);
	directory= _object.directory;
	loaded = _object.loaded;
	shader = _object.shader;
	drawMethod = _object.drawMethod;
	material = _object.material;
    for(Mesh& m : meshes){
		transform.AddChildren(m.GetTransform());
    }
}
Object3D::~Object3D(){
	meshes.Clear();
}
Object3D& Object3D::operator=(Object3D& _object){//Be carefull of setting the Scene
	*static_cast<GameObject*>(this)=_object;
	meshes.Append(_object.meshes);
	directory= _object.directory;
	loaded = _object.loaded;
	shader = _object.shader;
	drawMethod = _object.drawMethod;
	material = _object.material;
    for(Mesh& m : meshes){
		transform.AddChildren(m.GetTransform());
    }
    return *this;
}
//Loading 3D Model using Assimp and custom loader
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
    if(this->scene != nullptr){
        /*
	    ManageTextures(textures,material,aiTextureType_DIFFUSE);
	     //	Upp::Cout() << "Nb diffuse " << Upp::AsString(textures.GetCount()) << Upp::EOL;
	    ManageTextures(textures,material,aiTextureType_SPECULAR);
	     //	Upp::Cout() << "Nb speculare + diffuse " << Upp::AsString(textures.GetCount()) << Upp::EOL;
	    ManageTextures(textures,material,aiTextureType_HEIGHT);
	    //Upp::Cout() << "Nb Height +  speculare + diffuse " << Upp::AsString(textures.GetCount()) << Upp::EOL;
	    ManageTextures(textures,material,aiTextureType_AMBIENT);
	    //Upp::Cout() << "Nb  AMBIENT + Height +  speculare + diffuse " << Upp::AsString(textures.GetCount()) << Upp::EOL;
	    ManageTextures(textures,material,aiTextureType_NORMALS);
	     //	Upp::Cout() << "Nb  NORMAL  + AMBIENT + Height +  speculare + diffuse " << Upp::AsString(textures.GetCount()) << Upp::EOL;
	     */
    }else{
        LOG("Warning : cant load and Bind texture since no context is linked to object3D : " + name );
    }
    
    // return a mesh object created from the extracted mesh data
    Mesh& m =  meshes.Create<Mesh>(*this ,vertices, indices, textures);
    m.SetObject3D(*this);
	GetTransform().AddChildren(m.GetTransform());
}
void Object3D::LoadModel(const Upp::String& path){//Used to load 3D Model
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
        Upp::Cout() << "ERROR::ASSIMP:: " << Upp::String(importer.GetErrorString()) << Upp::EOL;
        return;
    }
    // retrieve the directory path of the filepath
    directory = realPath.Left( realPath.ReverseFind('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}
void Object3D::ReadData(Upp::Vector<float>& data ,ReaderParameters readerParameter ,ReaderRoutine readerRoutine){//Used to read vector<Float>
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
					
					m = &(meshes.Create<Mesh>(*this)); // Original line, Here happen a memory violation
				//	meshes.Create<Mesh>(*this); //Same memory violation happen
				//	meshes.Add(); //Occure the same error
				//	Mesh(*this); //Everythings is ok
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
}
Upp::Array<Mesh>& Object3D::GetMeshes(){
	return meshes;
}
Object3D& Object3D::EnableLightCalculation(){
	lightAffected = true;
	return *this;
}
Object3D& Object3D::DisableLightCalculation(){
	lightAffected = false;
	return *this;
}
bool Object3D::IsLightCalculationEnable(){
	return lightAffected;
}
Object3D& Object3D::EnableAlpha(){
	alphaAffected = true;
	return *this;
}
Object3D& Object3D::DisableAlpha(){
	alphaAffected = false;
	return *this;
}
bool Object3D::IsAlphaEnable(){
	return alphaAffected;
}
Material& Object3D::GetMaterial(){
	return material;
}
Object3D& Object3D::SetDrawMethod(DrawMethod dm){
	drawMethod = dm;
	return *this;
}
DrawMethod Object3D::GetDrawMethod(){
	return drawMethod;
}
Object3D& Object3D::SetShader(Shader& _shader){
	shader = _shader;
	return *this;
}
Shader& Object3D::GetShader(){
	return shader;
}
Object3DBehaviour Object3D::GetBehaviour(){
	return behavior;
}
Object3D& Object3D::SetBehaviour(Object3DBehaviour _behaviour){
	behavior = _behaviour;
	return *this;
}
//Override
void Object3D::Load(){
	
		/*if(behavior == OBJ_STATIC){
			
		}else if(behavior == OBJ_DYNAMIC){
			
		}*/
		
		//here you can have a look of every layout an object will have
		Upp::Array<glm::vec3> Positions;
		Upp::Vector<float> Normals;
		Upp::Vector<float> TextCoords;
		Upp::Vector<float> Tangents;
		Upp::Vector<float> BiTangents;
		Upp::Vector<float> Colors;
		Upp::Vector<int> UseTextures; //True to use texture, -1 it use color
		Upp::Vector<int> Textures; //UnusedTexture must be set to -1
		Upp::Vector<int> SpeculareTextures; //unusedSpeculare must be set to -1
		Upp::Array<glm::mat4> MatricesModels;
		
		glm::mat4 model(1.0f);
		//Upp::Cout()<< GetTransform().GetPosition().x << " | " << GetTransform().GetPosition().y << " | " << GetTransform().GetPosition().z << Upp::EOL;
		model = glm::translate(model,GetTransform().GetPosition())*glm::mat4_cast(GetTransform().GetQuaterion())*GetTransform().GetModelMatrixScaller();
		for(Mesh& m : meshes){
			if(m.GetBehaviour() == OBJ_DYNAMIC) m.Load();
			else{
				for(Vertex& v :  m.GetVertices()){
					Positions.Add(v.Position);
					/*Normals.Add(v.Normal.x);
					Normals.Add(v.Normal.y);
					Normals.Add(v.Normal.z);
					TextCoords.Add(v.TexCoords.x);
					TextCoords.Add(v.TexCoords.y);
					Tangents.Add(v.Tangent.x);
					Tangents.Add(v.Tangent.y);
					Tangents.Add(v.Tangent.z);
					BiTangents.Add(v.Bitangent.x);
					BiTangents.Add(v.Bitangent.y);
					BiTangents.Add(v.Bitangent.z);
					Colors.Add(m.GetMaterial().GetColor().x);
					Colors.Add(m.GetMaterial().GetColor().y);
					Colors.Add(m.GetMaterial().GetColor().z);
					Colors.Add(m.GetMaterial().GetColor().w);
					UseTextures.Add(-1); //For the test, only color will be able
					Textures.Add(-1);
					SpeculareTextures.Add(-1);*/
					MatricesModels.Add(model);
				}
				//Here I Add All data of the mesh to the global Object3D buffer.
			}
		}
		//After having setted up all the data into the global Object3D buffer I settup all
		//layout of the data sended to the graphical card
		
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(10,VBO);
		
		//Binding Position :
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, Positions.GetCount() * sizeof(glm::vec3), &Positions[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribDivisor(0, 0);
		/*
		//Binding Normals :
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, Normals.GetCount() * sizeof(float), &Normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1, 1);
		//Binding TextureCoordinate :
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, TextCoords.GetCount() * sizeof(float), &TextCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1);
		//Binding Tangeant :
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, Tangents.GetCount() * sizeof(float), &Tangents[0], GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);
		glVertexAttribDivisor(3, 1);
		//Binding BiTangeant :
		glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
		glBufferData(GL_ARRAY_BUFFER, BiTangents.GetCount() * sizeof(float), &BiTangents[0], GL_STATIC_DRAW);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(4);
		glVertexAttribDivisor(4, 1);
		//Binding Color :
		glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
		glBufferData(GL_ARRAY_BUFFER, Colors.GetCount() * sizeof(float), &Colors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(5);
		glVertexAttribDivisor(5, 1);
		//Binding UseTexture :
		glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
		glBufferData(GL_ARRAY_BUFFER, UseTextures.GetCount() * sizeof(float), &UseTextures[0], GL_STATIC_DRAW);
		glVertexAttribPointer(6, 1, GL_INT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(6);
		glVertexAttribDivisor(6, 1);
		//Binding Texture :
		glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
		glBufferData(GL_ARRAY_BUFFER, Textures.GetCount() * sizeof(float), &Textures[0], GL_STATIC_DRAW);
		glVertexAttribPointer(7, 1, GL_INT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(7);
		glVertexAttribDivisor(7, 1);
		//Binding SpeculareTexture :
		glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
		glBufferData(GL_ARRAY_BUFFER, SpeculareTextures.GetCount() * sizeof(float), &SpeculareTextures[0], GL_STATIC_DRAW);
		glVertexAttribPointer(8, 1, GL_INT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(8);
		glVertexAttribDivisor(8, 1);
		*/
		//Binding ModelMatrice :
			/*
		glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
		glBufferData(GL_ARRAY_BUFFER, MatricesModels.GetCount() * sizeof(glm::mat4), &MatricesModels[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1);*/
		// set attribute pointers for matrix (4 times vec4)
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, MatricesModels.GetCount() * sizeof(glm::mat4), &MatricesModels[0], GL_STATIC_DRAW);
		
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        /*glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);*/
		
		//Unbind the vertex array
		glBindVertexArray(0);
		loaded = true;
		/*
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
	
	    glBindVertexArray(0);*/
}
void Object3D::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera){
	/* Bind shader blablab blab
		etc.
	*/
	
	
	shader.Use();
    shader.SetMat4("view",view);
    shader.SetMat4("projection", projection);
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 11 ); //12 is static value , CHANGE TODO
	//glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0, 2);
	//glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 12);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	shader.Unbind();
	/*
	for(Mesh& m : meshes){
		//if(m.GetBehaviour() == OBJ_DYNAMIC) m.Draw();
	}*/
}
