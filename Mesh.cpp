#include "Mesh.h"

Mesh::Mesh(){}
Mesh::Mesh(Object3D& obj){
	object3D = &obj;
}
Mesh::Mesh(Upp::Vector<Vertex>& vertices){
	vertices.Append(vertices);
	LoadDefaultIndices();
}
Mesh::Mesh(Upp::Vector<Vertex>& vertices, Upp::Vector<unsigned int>& indices){
	vertices.Append(vertices);
    indices.Append(indices);
}
Mesh::Mesh(Mesh& mesh){
	*this = mesh;
}
Mesh& Mesh::operator=(Mesh& mesh){
	object3D = mesh.object3D;
	loaded = mesh.loaded;
	
	vertices.Clear();
	vertices.Append(mesh.vertices);
	indices.Clear();
	indices.Append(mesh.indices);
	
	VAO = mesh.VAO;
	EBO = mesh.EBO;
	VBO = mesh.VBO;
	
	shader = mesh.shader;
	transform = mesh.transform;
	if( mesh.material.Get())
		material =  Upp::clone(~mesh.material);
	return *this;
}
Upp::Vector<Vertex>& Mesh::GetVertices(){
	return vertices;
}
Upp::Vector<unsigned int>& Mesh::GetIndices(){
	return indices;
}
Object3D& Mesh::GetObject3D(){
	ASSERT_(object3D,"Object3D of a mesh is nullptr, you must ensure that all your meshes have their object3D setted correctly by using SetObject3D(Object3D&) !");
	return *object3D;
}
Mesh& Mesh::SetObject3D(Object3D& obj){
	object3D = &obj;
	return *this;
}
Shader& Mesh::GetShader(){
	return shader;
}
Transform& Mesh::GetTransform(){
	return transform;
}
Material* Mesh::GetMaterial(){
	return material.Get();
}
bool Mesh::IsLoaded(){
	return loaded;
}
bool Mesh::ReadData(Upp::Vector<float>& data, ReaderParameters& readerParameter, bool UseMaterialColor ){
	//I thing this code is really bad.
	if(readerParameter.coordinatesPosition> -1){
		enum AllDataTypes{VERTICES,NORMAL,TEXTURE,TANGANT,BITANGANT,COLOR_INT_RGB,COLOR_FLOAT_RGB,COLOR_INT_RGBA,COLOR_FLOAT_RGBA};
		Upp::ArrayMap<int,AllDataTypes> map;
		int* iterateur =(int*)&readerParameter.coordinatesPosition;
		int cpt  = 0;
		for(int e=0;e< 6;iterateur++,e++){
			if(*iterateur != -1){
				for(int r = 0;r < 6; r++){
					if(*iterateur == r){
						switch(cpt){
							case 0:
								map.Add(*iterateur,VERTICES);
							break;
							case 1:
								map.Add(*iterateur,NORMAL);
							break;
							case 2:
								map.Add(*iterateur,TEXTURE);
							break;
							case 3:
								map.Add(*iterateur,TANGANT);
							break;
							case 4:
								map.Add(*iterateur,BITANGANT);
							break;
							case 5:
								if( readerParameter.colorType == CT_RGB){
									if(readerParameter.colorFormat == CF_INTEGER){
										map.Add(*iterateur,COLOR_INT_RGB);
									}else if(readerParameter.colorFormat == CF_FLOAT){
										map.Add(*iterateur,COLOR_FLOAT_RGB);
									}
								}else if(readerParameter.colorType == CT_RGBA){
									if(readerParameter.colorFormat == CF_INTEGER){
										map.Add(*iterateur,COLOR_INT_RGBA);
									}else if(readerParameter.colorFormat == CF_FLOAT){
										map.Add(*iterateur,COLOR_FLOAT_RGBA);
									}
								}
							break;
							default:
								LOG("Class Mesh:(Warning) bool Mesh::ReadData() strange iterator is passed over the map !");
						}
						break;
					}
				}
			}
			cpt++;
		}
		Upp::SortByKey(map,[](const int& a, const int& b) { return a<b; });
		float* fIterator =(float*)&data[0];
		int iteratorSize = 0;
		float float1 = 0.0f;
		float float2 = 0.0f;
		float float3 = 0.0f;
		float float4 = 0.0f;
		while(iteratorSize < data.GetCount()){
			Vertex& v =  vertices.Add();
			//v.SaveColor = !useMaterialColorInsteadOfColorRetrieve;
			for(const int& iterator  : map.GetKeys()){
				switch( map.Get(iterator)){
					case VERTICES:
						float1 =*fIterator++;
						float2 =*fIterator++;
						float3 =*fIterator++;
						v.Position = glm::vec3(float1,float2,float3);
						iteratorSize+=3;
					break;
					case NORMAL:
						float1 =*fIterator++;
						float2 =*fIterator++;
						float3 =*fIterator++;
						v.Normal = glm::vec3(float1,float2,float3);
						iteratorSize+=3;
					break;
					case TEXTURE:
						float1 =*fIterator++;
						float2 =*fIterator++;
						v.TexCoords = glm::vec2(float1,float2);
						iteratorSize+=2;
					break;
					case TANGANT:
						float1 =*fIterator++;
						float2 =*fIterator++;
						float3 =*fIterator++;
						v.Tangent = glm::vec3(float1,float2,float3);
						iteratorSize+=3;
					break;
					case BITANGANT:
						float1 =*fIterator++;
						float2 =*fIterator++;
						float3 =*fIterator++;
						v.Bitangent = glm::vec3(float1,float2,float3);
						iteratorSize+=3;
					break;
					case COLOR_INT_RGB:
						float1 =*fIterator++;
						float2 =*fIterator++;
						float3 =*fIterator++;
						v.SetColor(TransformRGBToFloatColor(float1,float2,float3));
						iteratorSize+=3;
					break;
					case COLOR_INT_RGBA:
						float1 =*fIterator++;
						float2 =*fIterator++;
						float3 =*fIterator++;
						float4 =*fIterator++;
						v.SetColor(TransformRGBAToFloatColor(float1,float2,float3,float4));
						iteratorSize+=4;
					break;
					case COLOR_FLOAT_RGB:
						float1 =*fIterator++;
						float2 =*fIterator++;
						float3 =*fIterator++;
						v.SetColor(glm::vec3(float1,float2,float3));
					break;
					case COLOR_FLOAT_RGBA:
						float1 =*fIterator++;
						float2 =*fIterator++;
						float3 =*fIterator++;
						float4 =*fIterator++;
						v.SetColor(glm::vec4(float1,float2,float3,float4));
					break;
					default:
						LOG("Class Mesh:(Warning) bool Mesh::ReadData() strange value spotted on the map used to readData !");
				}
			}
		}
		return true;
	}else{
		LOG("Class Mesh:(Warning) bool Mesh::ReadData() no position of vertices set in readerParameters !");
		return false;
	}
}
Mesh& Mesh::Load(){
	if(vertices.size() != 0){
		if(!shader.IsCompiled()){
			LOG("Shader not compiled !, Default shaders loaded !\n");
			//GenerateAutoShader(object3D->GetScene() ->GetNumberOfDirLight(),  object3D->GetScene() ->GetNumberOfPointLight(),object3D->GetScene()->GetNumberOfSpotLight());
			shader.AssignSimpleShader();
		}
		if(indices.size() == 0 && vertices.size() > 0){
            LOG("No indice defined ! Auto generation of incices !");
            LoadDefaultIndices();
        }
	    glGenVertexArrays(1, &VAO);
	    glGenBuffers(1, &VBO);
	    glGenBuffers(1, &EBO);
	    glBindVertexArray(VAO);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	    glEnableVertexAttribArray(2);
	    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	    glEnableVertexAttribArray(3);
	    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	    glEnableVertexAttribArray(4);
	    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		glEnableVertexAttribArray(5);
	    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	    glBindVertexArray(0);
	}else{
	    LOG("Mesh of object named " + GetObject3D().GetName() + " are error ! or undefined ! nothing to load !");
	}
	return *this;
}
Mesh& Mesh::LoadDefaultIndices(){
	int cpt= vertices.GetCount();
    for(int r = 0; r < cpt; r++){
        indices.Add(r);
    }
    return *this;
}
Mesh& Mesh::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera){
	shader.Use();
	model = GetTransform().GetModelMatrice(); // glm::translate(model,GetTransform().GetPosition())*glm::mat4_cast(GetTransform().GetQuaterion())*GetTransform().GetModelMatrixScaller();
    shader.SetMat4("model",model);
    shader.SetMat4("transform", transform);
    shader.SetMat4("view",view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("viewPos",GetTransform().GetPosition());
    if(material.Get()){
		material->SentToShader(shader);
    }
    //glLineWidth(2.5); //Allow user to change LineWidth
    glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.GetCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
    shader.Unbind();
    return *this;
}