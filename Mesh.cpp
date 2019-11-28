#include "UltimateOpenGL.h"
Mesh::Mesh(const Upp::Vector<Vertex>& vertices, Upp::Vector<unsigned int>& indices, Upp::Vector<Texture>& textures)
{
    this->vertices.Append(vertices);
    this->indices.Append(indices);
    this->textures.Append(textures);

    Load();
}

void Mesh::Load(){
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

void Mesh::Draw(){
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
}


void Mesh::SetShader(Shader& _shader){
	shader = _shader;
}
Shader& Mesh::GetShader(){
	return shader;
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

