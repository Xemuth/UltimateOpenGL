#include "Object3D.h"

Object3D::Object3D(const Upp::String& pathOfModel){
	
}
void Object3D::LoadModel(const Upp::String& path){
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        Upp::Cout << "ERROR::ASSIMP:: " << Upp::String(importer.GetErrorString()) << Upp::EOL;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.Mid(0, path.ReverseFind('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}
void Object3D::Draw(Shader shader){
	for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
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
            
            vectorToFile.Add(GetScene()->GetContext()->AddTexture(Upp::String(str.C_Str()),directory+"/"+ Upp::String(str.C_Str()),t));      
        }
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
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        // tangent
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.Tangent = vector;
        // bitangent
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.Bitangent = vector;
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
     	ManageTextures(textures,material,aiTextureType_SPECULAR);
     	ManageTextures(textures,material,aiTextureType_HEIGHT);
     	ManageTextures(textures,material,aiTextureType_AMBIENT);
     
    }else{
        LOG("Warning : cant load and Bind texture since no context is linked to object3D : " + name );
    }
    
    // return a mesh object created from the extracted mesh data
    meshes.Create(vertices, indices, textures);
}