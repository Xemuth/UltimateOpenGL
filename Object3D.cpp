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
        LOG("ERROR::ASSIMP:: " << Upp::String(importer.GetErrorString()));
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

unsigned int Object3D::GetVertexVBO(){
	return VertexVBO;
}
unsigned int Object3D::GetMaterialVBO(){
	return MaterialVBO;
}
unsigned int Object3D::GetMatriceVBO(){
	return MatriceVBO;
}
//Override
void Object3D::Load(){
	if(!loaded){
#ifdef flagUOGLV3
		for(int i =0 ; i < meshes.size(); i++){//Changement made by Iñaki
			if(shader.IsCompiled())meshes[i].SetShader(shader);
			meshes[i].Load();
		}
#else
		Upp::Array<glm::mat4> AllMatrices;
		Upp::Vector<Vertex> AllVertex;
		Upp::Vector<GlobalMaterialInformation> AllTextureInformation;
		
		int cptVertex = 0;
		int cptMesh = 0;
		for(Mesh& m : meshes){
			if(m.GetBehaviour() == OBJ_DYNAMIC) m.Load();
			else{
				for(Vertex& v :  m.GetVertices()){
					Vertex& buff = AllVertex.Add() = v;
					if(!buff.SaveColor){
						buff.Color = (m.GetMaterial().HaveBeenSetUp())? m.GetMaterial().GetColor(): (material.HaveBeenSetUp())?material.GetColor():buff.Color;
					}
					cptVertex++;
					NumberOfVertexToDraw++;
				}
				//AllTextureInformation.Add((m.GetMaterial().HaveBeenSetUp() )?m.GetMaterial().GetGlobalMaterialInformation():GetMaterial().GetGlobalMaterialInformation());
				GlobalMaterialInformation gmi = GetMaterial().GetGlobalMaterialInformation();
				AllTextureInformation.Add(gmi);
			}
			AllMatrices.Add(transform.GetModelMatrice());
			cptMesh++;
		}
		//After having setted up all the data into the global Object3D buffer I settup all
		//layout of the data sended to the graphical card
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		glGenBuffers(1,&VertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
		
		/*
		layout (location = 0) in vec3 aPos;\n
		layout (location = 1) in vec3 aNorm;\n
		layout (location = 2) in vec2 aTextCoords;\n
		layout (location = 3) in vec3 aTangents;\n
		layout (location = 4) in vec3 aBiTangents;\n
		layout (location = 5) in vec4 aColors;\n
		layout (location = 6) in int aUseTextures;\n
		layout (location = 7) in bool aUseColors;\n
		layout (location = 8) in int aTextures;\n
		layout (location = 9) in int aSpeculareTextures;\n
		layout (location = 10) in mat4 aMatricesModels;\n
		*/
		
		glBufferData(GL_ARRAY_BUFFER, cptVertex * sizeof(Vertex), &AllVertex[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //importing coordinate
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3))); //Importing Normalmap
		glEnableVertexAttribArray(1);
		
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3))); //Importing Texture coordinate
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2))); //Importing Tangents
		glEnableVertexAttribArray(3);
		
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(glm::vec3) + sizeof(glm::vec2))); //Importing BiTangents
		glEnableVertexAttribArray(4);
		
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4 * sizeof(glm::vec3) + sizeof(glm::vec2))); //Importing Color
		glEnableVertexAttribArray(5);
		
		//HEre I bind the model matrix
		
		glGenBuffers(1,&MatriceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, MatriceVBO);
		glBufferData(GL_ARRAY_BUFFER, cptMesh * sizeof(glm::mat4), &AllMatrices[0], GL_STATIC_DRAW);
		
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0); //Importing first row
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4))); //Importing second row
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4))); //Importing third row
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4))); //Importing fourth row
		glEnableVertexAttribArray(9);
		
		glVertexAttribDivisor(6, 1);
	    glVertexAttribDivisor(7, 1);
	    glVertexAttribDivisor(8, 1);
	    glVertexAttribDivisor(9, 1);
	
		//Here I must build a new VBO that will  carry texture and for each All mesh
		/*
		glGenBuffers(1,&MaterialVBO);
		glBindBuffer(GL_ARRAY_BUFFER, MaterialVBO);
		
		glBufferData(GL_ARRAY_BUFFER, cptMesh * sizeof(GlobalMaterialInformation), &AllTextureInformation[0], GL_STATIC_DRAW);
		glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)0); //importing UseTextures
		glEnableVertexAttribArray(6);
		
		glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(sizeof(float))); //importing UseColors
		glEnableVertexAttribArray(7);
		
		glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(2 * sizeof(float))); //importing Shininess
		glEnableVertexAttribArray(8);
		
		glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(3 * sizeof(float))); //importing Diffuse
		glEnableVertexAttribArray(9);
		
		glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)( 4 * sizeof(float))); //importing Ambient
		glEnableVertexAttribArray(10);
		
		glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(4 * sizeof(float) + sizeof(glm::vec3))); //importing Speculare
		glEnableVertexAttribArray(11);
		
		glVertexAttribPointer(12, 1, GL_INT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(4 * sizeof(float) + 2 * sizeof(glm::vec3))); //importing Texture1
		glEnableVertexAttribArray(12);
		
		glVertexAttribPointer(13, 1, GL_INT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(4 * sizeof(float) + 2 * sizeof(glm::vec3) + sizeof(int))); //importing TextureSpeculare1
		glEnableVertexAttribArray(13);
		
		glVertexAttribPointer(14, 1, GL_FLOAT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(4 * sizeof(float) + 2 * sizeof(glm::vec3) + 2 * sizeof(int))); //importing Mix1
		glEnableVertexAttribArray(14);
		
		glVertexAttribPointer(15, 1, GL_INT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(5 * sizeof(float) + 2 * sizeof(glm::vec3) + 2 * sizeof(int))); //importing Texture2
		glEnableVertexAttribArray(15);
		
		glVertexAttribPointer(16, 1, GL_INT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(5 * sizeof(float) + 2 * sizeof(glm::vec3) + 3 * sizeof(int))); //importing TextureSpeculare2
		glEnableVertexAttribArray(16);
		
		glVertexAttribPointer(17, 1, GL_FLOAT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(5 * sizeof(float) + 2 * sizeof(glm::vec3) + 4 * sizeof(int))); //importing Mix2
		glEnableVertexAttribArray(17);
		
		glVertexAttribPointer(18, 1, GL_INT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(6 * sizeof(float) + 2 * sizeof(glm::vec3) + 3 * sizeof(int))); //importing Texture3
		glEnableVertexAttribArray(18);
		
		glVertexAttribPointer(19, 1, GL_INT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(6 * sizeof(float) + 2 * sizeof(glm::vec3) + 4 * sizeof(int))); //importing TextureSpeculare3
		glEnableVertexAttribArray(19);
		
		glVertexAttribPointer(20, 1, GL_FLOAT, GL_FALSE, sizeof(GlobalMaterialInformation), (void*)(6 * sizeof(float) + 2 * sizeof(glm::vec3) + 5 * sizeof(int))); //importing MiX3
		glEnableVertexAttribArray(20);
		*/
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
#endif
		loaded = true;
	}
}
void Object3D::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera){
	if(loaded){
	#ifdef flagUOGLV3
		for(int i = 0; i < meshes.size(); i++) // meshes.size(); i++)//Changement made by Iñaki
			meshes[i].Draw(model,camera.GetTransform().GetViewMatrix(),projection,transform,camera);
	#else
		if(onDraw)onDraw(*this);
		shader.Use();
		shader.SetMat4("view",camera.GetTransform().GetViewMatrix());
		shader.SetMat4("projection",projection);
		//shader.SetMat4("model",GetTransform().GetModelMatrice());
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, NumberOfVertexToDraw);
		shader.Unbind();
	#endif
	}
}
