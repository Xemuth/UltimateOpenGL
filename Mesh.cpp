#include "Mesh.h"
#include "Texture.h"
#include "Scene.h"
#include "UltimateOpenGL.h"
Mesh::Mesh(): transform(){}
Mesh::Mesh(Object3D& gameObject, Upp::Vector<Vertex>& vertices, Upp::Vector<unsigned int>& indices, Upp::Vector<Texture>& textures) : transform()
{
    this->vertices.Append(vertices);
    this->indices.Append(indices);
    
    // this->textures.Append(textures);
	
	Texture lastDiffuse;
	Texture lastHeight;
	Texture lastNormal;
	Texture lastAmbient;
	Texture lastSpecular;
	
	for(Texture& t :   textures){
		if(t.GetType() == TT_DIFFUSE && lastDiffuse.IsLoaded()){
			
			//m.SetDiffuse(lastDiffuse.GetTextureIterator());
			if(lastSpecular.IsLoaded())
				material.AddTexture(t,1.0f,lastSpecular);
			else
				material.AddTexture(t);
			//Normal is not implemented yet
		//	if(lastNormal.IsLoaded())
		//		m.SetNormal(lastNormal.GetId());//Changement made by Iñaki
			
			lastNormal =Texture();
			lastSpecular =Texture();
			lastDiffuse = Texture();
		}else{
			if(t.GetType() == TT_DIFFUSE){
				lastDiffuse = t;
			}else if(t.GetType() == TT_NORMAL){
				lastNormal = t;
			}else if(t.GetType() == TT_HEIGHT){
				lastHeight = t;
			}else if(t.GetType() == TT_AMBIENT){
				lastAmbient = t;
			}else if(t.GetType() == TT_SPECULAR){
				lastSpecular = t;
			}
		}
	}
	if(lastDiffuse.IsLoaded()){
	
	
		if(lastSpecular.IsLoaded())
			material.AddTexture(lastDiffuse,1.0f,lastSpecular);
		else
			material.AddTexture(lastDiffuse);
		//Normal not yet implemented
	//	if(lastNormal.IsLoaded())
		//	m.SetNormal(lastNormal.GetId());//Changement made by Iñaki
	}

	int count =  material.GetTextures().GetCount();
	for(TextureInformation& ti : material.GetTextures()){
		ti.SetMix((float) (1.0f / count));
	}

	
   //Load();
}
Mesh::Mesh(Object3D& obj): transform(){
	object3D = &obj;
}

Mesh::Mesh(const Mesh& _mesh){
	object3D = _mesh.object3D;
	VAO = _mesh.VAO;
	VertexVBO = _mesh.VertexVBO;
	EBO = _mesh.EBO;
        
    vertices.Append(_mesh.vertices);
    indices.Append(_mesh.indices);
 //   textures.Append(_mesh.GetTextures());
    
    shader = _mesh.shader;
    
    transform =_mesh.transform;
    //transform.SetMeshObject(*this);
    
    /*for(const Upp::String& str :_mesh.materialsColor.GetKeys()){
        materialsColor.Add(str, _mesh.materialsColor.Get(str));
    }
    for(const Upp::String& str : _mesh.materialsTexture.GetKeys()){
        materialsTexture.Add(str,_mesh.materialsTexture.Get(str));
    }*/
    material = _mesh.material;

	LightAffected = _mesh.LightAffected;
}
void Mesh::LoadDefaultIndices(){
    int cpt= vertices.GetCount();
    for(int r = 0; r < cpt; r++){
        indices.Add(r);
    }
}
Object3DBehaviour Mesh::GetBehaviour(){
	return behavior;
}
Mesh& Mesh::SetBehaviour(Object3DBehaviour _behaviour){
	behavior = _behaviour;
	return *this;
}
GLint Mesh::ResolveDrawMethod(){
	switch(drawMethod){
		case DM_POINTS:
			return GL_POINTS;
		break;
		case DM_LINES:
			return GL_LINES;
		break;
		case DM_LINE_STRIP:
			return GL_LINE_STRIP;
		break;
		case DM_LINE_LOOP:
			return GL_LINE_LOOP;
		break;
		case DM_TRIANGLES:
			return GL_TRIANGLES;
		break;
		case DM_TRIANGLE_STRIP:
			return GL_TRIANGLE_STRIP;
		break;
		case DM_TRIANGLE_FAN:
			return GL_TRIANGLE_FAN;
		break;
		case DM_QUADS:
			return GL_QUADS;
		break;
		case DM_QUAD_STRIP:
			return GL_QUAD_STRIP;
		break;
		case DM_POLYGON:
			return GL_POLYGON;
		break;
		default:
			return GL_TRIANGLES;
    }
}
void Mesh::Load(){
#ifdef flagUOGLV3
	if(indices.size() != 0 || vertices.size() != 0){
		/*if(textures.GetCount() > 0 && materialsTexture.GetCount() == 0){
			for(Texture& t : textures){
			//	Upp::Cout() << "Texture of mesh is " + t.GetName() + " with ID of " +  Upp::AsString(t.GetId()) + "\n";
				MaterialTexture& m =  materialsTexture.Add(t.GetName());
				m.SetDiffuse(t.GetTextureIterator());
			}
		}*/
		
		//Juste un test,ne doit pas rester en létat
		/*for(MaterialTexture& mt : materialsTexture){
			mt.SetMix((float) (1.0f / materialsTexture.GetCount()));
		}*/
		
		if(!shader.IsCompiled()){
			LOG("Shader not compiled !, Default shaders loaded !\n");
			//GenerateAutoShader(object3D->GetScene() ->GetNumberOfDirLight(),  object3D->GetScene() ->GetNumberOfPointLight(),object3D->GetScene()->GetNumberOfSpotLight());
			GetShader().AssignSimpleShader();
		}
		if(indices.size() == 0 && vertices.size() > 0){
            LOG("No indice defined ! Auto generation of incices !");
            LoadDefaultIndices();
        }


		// create buffers/arrays
	    glGenVertexArrays(1, &VAO);
	    glGenBuffers(1, &VertexVBO);
	    glGenBuffers(1, &EBO);
	
	    glBindVertexArray(VAO);
	    // load data into vertex buffers
	    glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
	    // A great thing about structs is that their memory layout is sequential for all its items.
	    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	    // again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	
	/*	Upp::Cout() << "SizeOFVertex : " << sizeof(Vertex) << "\n";
		float* buffer = nullptr;
		for(Vertex& v : vertices){
			buffer = (float*)&v;
			
			for (int e =0 ; e < 13 ; e++){
				Upp::Cout() <<Upp::AsString(*buffer) <<",";
				buffer++;
			}
			
			Upp::Cout() <<  Upp::EOL;
		}*/
	
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
	   // shader.Use();
	}else{
	    LOG("Mesh of object named " + object3D->GetName() + " are error ! or undefined ! nothing to load !");
	}
#else
	if(indices.size() != 0 || vertices.size() != 0){
		/*if(textures.GetCount() > 0 && materialsTexture.GetCount() == 0){
			for(Texture& t : textures){
			//	Upp::Cout() << "Texture of mesh is " + t.GetName() + " with ID of " +  Upp::AsString(t.GetId()) + "\n";
				MaterialTexture& m =  materialsTexture.Add(t.GetName());
				m.SetDiffuse(t.GetTextureIterator());
			}
		}*/
		
		//Juste un test,ne doit pas rester en létat
	/*	for(MaterialTexture& mt : materialsTexture){
			mt.SetMix((float) (1.0f / materialsTexture.GetCount()));
		}
	*/
	/*	if(!shader.IsCompiled() && object3D != nullptr ){
			LOG("Shader not compiled !, Default shaders loaded !\n");
			GenerateAutoShader(object3D->GetScene().GetNumberOfDirLight(),  object3D->GetScene().GetNumberOfPointLight(),object3D->GetScene().GetNumberOfSpotLight());
		}
		if(indices.size() == 0 && vertices.size() > 0){
            LOG("No indice defined ! Auto generation of incices !");
            LoadDefaultIndices();
        }*/


		// create buffers/arrays
	    glGenVertexArrays(1, &VAO);
	    glGenBuffers(1, &VertexVBO);
	    glGenBuffers(1, &EBO);
	
	    glBindVertexArray(VAO);
	    // load data into vertex buffers
	    glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
	    // A great thing about structs is that their memory layout is sequential for all its items.
	    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	    // again translates to 3/2 floats which translates to a byte array.
	    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	
	/*	Upp::Cout() << "SizeOFVertex : " << sizeof(Vertex) << "\n";
		float* buffer = nullptr;
		for(Vertex& v : vertices){
			buffer = (float*)&v;
			for (int e =0 ; e < 13 ; e++){
				Upp::Cout() <<Upp::AsString(*buffer) <<",";
				buffer++;
			}
			Upp::Cout() <<  Upp::EOL;
		}*/
	
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
	   // shader.Use();
	}else{
	    LOG("Mesh of object named " + object3D->GetName() + " are error ! or undefined ! nothing to load !");
	}
#endif
}
void Mesh::Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera){
#ifdef flagUOGLV3
	shader.Use();
	//Cout() << "x: " << Position.x << ", y: " << Position.y << ", z: " << Position.z << "\n";
	model = glm::translate(model,GetTransform().GetPosition())*glm::mat4_cast(GetTransform().GetQuaterion())*GetTransform().GetModelMatrixScaller();

	//if(ClearOnDraw)transformations.Clear();
    shader.SetMat4("model",model);
    shader.SetMat4("transform", transform);
    shader.SetMat4("view",view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("viewPos",GetTransform().GetPosition());
    int cptTexture =0;

	


    glBindVertexArray(VAO);
    //Draw method can be setted with SetDrawMethod
    switch(drawMethod){
		case DM_POINTS:
			glDrawElements(GL_POINTS, indices.GetCount(), GL_UNSIGNED_INT, 0);
		break;
		case DM_LINES:
			
			glDrawElements(GL_LINES, indices.GetCount(), GL_UNSIGNED_INT, 0);
		break;
		case DM_LINE_STRIP:
			glDrawElements(GL_LINE_STRIP, indices.GetCount(), GL_UNSIGNED_INT, 0);
		break;
		case DM_LINE_LOOP:
			glDrawElements(GL_LINE_LOOP, indices.GetCount(), GL_UNSIGNED_INT, 0);
		break;
		case DM_TRIANGLES:
			glDrawElements(GL_TRIANGLES, indices.GetCount(), GL_UNSIGNED_INT, 0);
		break;
		case DM_TRIANGLE_STRIP:
			glDrawElements(GL_TRIANGLE_STRIP, indices.GetCount(), GL_UNSIGNED_INT, 0);
		break;
		case DM_TRIANGLE_FAN:
			glDrawElements(GL_TRIANGLE_FAN, indices.GetCount(), GL_UNSIGNED_INT, 0);
		break;
		case DM_QUADS:
			glDrawElements(GL_QUADS, indices.GetCount(), GL_UNSIGNED_INT, 0);
		break;
		case DM_QUAD_STRIP:
			glDrawElements(GL_QUAD_STRIP, indices.GetCount(), GL_UNSIGNED_INT, 0);
		break;
		case DM_POLYGON:
			glDrawElements(GL_POLYGON, indices.GetCount(), GL_UNSIGNED_INT, 0);
		break;
		default:
			LOG("Error : void Mesh::Draw(...) DrawMethod is unknow !");
    }
    
    
    glBindVertexArray(0);
    // always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
    shader.Unbind();
#else
	shader.Use();
	//Cout() << "x: " << Position.x << ", y: " << Position.y << ", z: " << Position.z << "\n";
	model = glm::translate(model,GetTransform().GetPosition())*glm::mat4_cast(GetTransform().GetQuaterion())*GetTransform().GetModelMatrixScaller();

	//if(ClearOnDraw)transformations.Clear();
    shader.SetMat4("model",model);
    shader.SetMat4("transform", transform);
    shader.SetMat4("view",view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("viewPos",GetTransform().GetPosition());
    int cptTexture =0;
    
	/*for(const Upp::String &mtStr : materialsTexture.GetKeys()){
		materialsTexture.Get(mtStr).diffuse = object3D->GetScene().GetContext()->GetTextures().Get(mtStr).GetTextureIterator();
		materialsTexture.Get(mtStr).specular= object3D->GetScene().GetContext()->GetTextures().Get(mtStr).GetTextureIterator();
		object3D->GetScene()->GetContext()->GetTextures().Get(mtStr).Use();
		shader.SetMaterialTexture("texture"+ Upp::AsString(cptTexture),materialsTexture.Get(mtStr));
		cptTexture++;
	}*/
	/*
	if(cptTexture==0){
	    int cptColor= 0;
	    for(const Upp::String &mcStr : materialsColor.GetKeys()){
			shader.SetMaterialColor("color"+ Upp::AsString(cptColor),materialsColor.Get(mcStr));
			cptColor++;
		}
	}*/
	
/*	if(LightAffected &&  object3D ){
		for(Light* obj : object3D->GetScene().GetAllLights()){
			int cptDirLight=0;
			int cptPointLight=0;
			int cptSpotLight=0;
			if(obj.drawable){
				if(obj.lightType == LT_DIRLIGHT){
					shader.SetDirLight(obj->name,* static_cast<DirLight*>(obj),cptDirLight);
					cptDirLight++;
				}else if(obj->lightType == LT_POINTLIGHT){
					shader.SetPointLight(obj->name,* static_cast<PointLight*>(obj),cptPointLight);
					cptPointLight++;
				}else if(obj->lightType == LT_SPOTLIGHT){
					shader.SetSpotLight(obj->name,* static_cast<SpotLight*>(obj),cptSpotLight);
					cptSpotLight++;
				}
			}
		}
	}*/

   /* for(const Upp::String& str : object3D->GetScene()->GetAllGameObject().GetKeys()){
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
    }*/
  //	glLineWidth(2.5); //Allow user to change LineWidth
    glBindVertexArray(VAO);
    //Draw method can be setted with SetDrawMethod
    switch(drawMethod){
    	case DM_POINTS:
    		glDrawElements(GL_POINTS, indices.GetCount(), GL_UNSIGNED_INT, 0);
    	break;
    	case DM_LINES:
    		
    		glDrawElements(GL_LINES, indices.GetCount(), GL_UNSIGNED_INT, 0);
    	break;
    	case DM_LINE_STRIP:
    		glDrawElements(GL_LINE_STRIP, indices.GetCount(), GL_UNSIGNED_INT, 0);
    	break;
    	case DM_LINE_LOOP:
    		glDrawElements(GL_LINE_LOOP, indices.GetCount(), GL_UNSIGNED_INT, 0);
    	break;
    	case DM_TRIANGLES:
    		glDrawElements(GL_TRIANGLES, indices.GetCount(), GL_UNSIGNED_INT, 0);
    	break;
    	case DM_TRIANGLE_STRIP:
    		glDrawElements(GL_TRIANGLE_STRIP, indices.GetCount(), GL_UNSIGNED_INT, 0);
    	break;
    	case DM_TRIANGLE_FAN:
    		glDrawElements(GL_TRIANGLE_FAN, indices.GetCount(), GL_UNSIGNED_INT, 0);
    	break;
    	case DM_QUADS:
    		glDrawElements(GL_QUADS, indices.GetCount(), GL_UNSIGNED_INT, 0);
    	break;
    	case DM_QUAD_STRIP:
    		glDrawElements(GL_QUAD_STRIP, indices.GetCount(), GL_UNSIGNED_INT, 0);
    	break;
    	case DM_POLYGON:
    		glDrawElements(GL_POLYGON, indices.GetCount(), GL_UNSIGNED_INT, 0);
    	break;
    	default:
    		LOG("Error : void Mesh::Draw(...) DrawMethod is unknow !");
    }
    
    
    glBindVertexArray(0);
    // always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
    shader.Unbind();
#endif
}

/*
bool Mesh::BindTexture(Upp::String textureName,float textureShininess,Upp::String TextureSpecularName){
	if(object3D != nullptr && object3D->GetScene() != nullptr &&  textureName.GetCount() > 0 && object3D->GetScene()->GetContext()->GetTextures().Find(textureName) !=-1){
		if(TextureSpecularName.GetCount() > 0 && object3D->GetScene()->GetContext()->GetTextures().Find(TextureSpecularName)!=-1){
			materialsTexture.Add(textureName, MaterialTexture(textureShininess,true,object3D->GetScene()->GetContext()->GetTextures().Get(textureName).GetTextureIterator(),object3D->GetScene()->GetContext()->GetTextures().Get(TextureSpecularName).GetTextureIterator()));
		}else{
			materialsTexture.Add(textureName, MaterialTexture(textureShininess,false,object3D->GetScene()->GetContext()->GetTextures().Get(textureName).GetTextureIterator(),0 ));
		}
		return true;
	}else{
		LOG("Error : BindTexture(String,float,String) Invalide texture name ! he can't be empty or undefined !\n");
		return false;
	}
}*/

void Mesh::SetShader(Shader& _shader){
	shader = _shader;
}
Shader& Mesh::GetShader(){
	return shader;
}


unsigned int Mesh::GetVAO(){
	return VAO;
}
unsigned int Mesh::GetVertexVBO(){
	return VertexVBO;
}
unsigned int Mesh::GetMaterialVBO(){
	return MaterialVBO;
}
unsigned int Mesh::GetMatriceVBO(){
	return MatriceVBO;
}
unsigned int Mesh::GetEBO(){
	return EBO;
}

Upp::Vector<Vertex>& Mesh::GetVertices(){
	return vertices;
}
void Mesh::SetIndices(Upp::Vector<unsigned int>& _indices){
	indices.Clear();
	indices.Append(_indices);
}
Upp::Vector<unsigned int>& Mesh::GetIndices(){
	return indices;
}


bool Mesh::ReadData(Upp::Vector<float>& data,ReaderParameters& readerParameter,bool UseMaterialColor){
	//I thing this code is really bad.
	useMaterialColorInsteadOfColorRetrieve=true;
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
								useMaterialColorInsteadOfColorRetrieve =UseMaterialColor;
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
			v.SaveColor = !useMaterialColorInsteadOfColorRetrieve;
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

/* Texture Gestion */
Mesh& Mesh::BindTexture(const Upp::String& TextureName,float mixValue, float textureShininess,const Upp::String& TextureSpecularName, const Upp::String& NormalMappingTextureName){
	if(object3D != nullptr){
		Texture t =object3D->GetScene().GetContext().GetTexture(TextureName);
		if(t.IsLoaded()){
			TextureInformation& TI =  material.UseTextures().AddTexture(t,mixValue);
			material.SetShininess(textureShininess);

			if(TextureSpecularName.GetCount() > 0){
				Texture tSpeculare = GetObject3D().GetScene().GetContext().GetTexture(TextureName);
				if(tSpeculare.IsLoaded()){
					/*** Here I add the texture to material ***/
					TI.SetSpecular(tSpeculare);
					if(tSpeculare.GetType() != TT_SPECULAR)
						LOG("Class Mesh:(Warning) Mesh& Mesh::BindTexture(...) You are binding as speculare a texture wich is not a speculare type !");
				}else{
					LOG("Error : Mesh& Mesh::BindTexture(...) Specular texture of " + object3D->GetName() +" named " + tSpeculare.GetName() +" is not loaded !" );
				}
			}
			/*
			NORMAL IS NOT ALLOWED YET IN NEW ULTIMATE OPENGL
			if(NormalMappingTextureName.GetCount() > 0){
				Texture tNormal = GetObject3D().GetScene().GetContext().GetTexture(TextureName);
				if(tNormal.IsLoaded()){
					//Here I add the texture to material
					m.SetNormal(tNormal.GetId());
					if(tNormal.GetType() != TT_NORMAL)
						LOG("Class Mesh:(Warning) Mesh& Mesh::BindTexture(...) You are binding as Normal a texture wich is not a Normal type !");
				}else{
					LOG("Class Mesh:(Error) Mesh& Mesh::BindTexture(...) Normal texture of " + object3D->GetName() +" named " + tNormal.GetName() +" is not loaded !" );
				}
			}*/
			if(t.GetType() != TT_DIFFUSE)
				LOG("Class Mesh:(Warning) Mesh& Mesh::BindTexture(...) You are binding as Diffuse a texture wich is not a Diffuse type !");
			return *this;
		}
		else{
			LOG("Class Mesh:(Error) Mesh& Mesh::BindTexture(...) " + TextureName + " texture don't existe in context, you must add it before getting it !");
			return *this;
		}
	}
	LOG("Class Mesh:(Error) Mesh& Mesh::BindTexture(...) Mesh is not bind to GameObject or GameObject is not bind to Scene or Scene is not bind to Context wich is carrying texture");
	return *this;
}

/* Transform */
Transform& Mesh::GetTransform(){
	return transform;
}
void Mesh::SetTransform(Transform& _transform){
	transform = _transform;
}

/* Material Manager */
Material& Mesh::GetMaterial(){
	return material;
}
Mesh& Mesh::SetMaterial(Material& m){
	material = m;
	return *this;
}
/*
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
}*/
//Miscelnious
void Mesh::SetObject3D(Object3D& _object3D){
	object3D = &_object3D;
}
Object3D& Mesh::GetObject3D(){
	if(!object3D) ASSERT("MeshObject get Nullptr object");
	return *object3D;
}

void Mesh::SetLightAffected(bool _light){
	LightAffected = _light;
}
bool Mesh::IsLightAffected(){
	return LightAffected;
}

void Mesh::UseAlpha(bool _alpha ){
	AlphaAffected = _alpha;
}
bool Mesh::IsAlpha(){
	return AlphaAffected;
}

Mesh& Mesh::SetDrawMethod(DrawMethod dm){
	drawMethod = dm;
	return *this;
}
DrawMethod Mesh::GetDrawMethod(){
	return drawMethod;
}

void Mesh::GenerateAutoShader(int NbLightDir,int NbLightPoint,int NbLightSpot){
	
	/*
	Upp::String vertexShader = BasicShaders.Get("Default_Vertex_Shader");
	Upp::String fragmentShader = BasicShaders.Get("Default_Fragment_Shader");
		
	Upp::String all_uniform="";
	Upp::String all_VecMulti= "";
	if(AlphaAffected)
		all_VecMulti ="vec4 texColor =";
	else
		all_VecMulti= "FragColor = ";
	
	vertexShader.Replace("//UNIFORM_MODEL","uniform mat4 model;\n");
	vertexShader.Replace("//NORMAL_CALCULATION","Normal = mat3(transpose(inverse(model))) * aNormal;\n");

	vertexShader.Replace("//FRAGPOS_CALCULATION","FragPos = vec3(model * vec4(aPos, 1.0));\n");
	vertexShader.Replace("//POSITION_CALCULATION","gl_Position = projection * view * model * vec4(aPos, 1.0f);\n");
	
	vertexShader.Replace("//LAYOUT_POSITION","layout (location = 0) in vec3 aPos;");
	vertexShader.Replace("//LAYOUT_NORMAL","layout (location = 1) in vec3 aNormal;");
	vertexShader.Replace("//LAYOUT_TEXTURE_COORD","layout (location = 2) in vec2 aTexCoord;");
	vertexShader.Replace("//LAYOUT_TANGEANT","layout (location =3) in vec3 aTangent;");
	vertexShader.Replace("//LAYOUT_BITANGEANT","layout (location =4) in vec3 aBiTangent;");
	
	Upp::String AllDirLights="";
	Upp::String AllPointLights="";
	Upp::String AllSpotLights="";
	if(materialsTexture.GetCount()== 0 && materialsColor.GetCount() == 0){
		float r = float(Upp::Randomf());	float g = float(Upp::Randomf());	float b = float(Upp::Randomf()); //Changement made by Iñaki
		MaterialColor& m = CreateMaterialColor("defaultColor");
		m.SetDiffuse(glm::vec3( r,g,b));
		LOG("Class Mesh:(INFO) void Mesh::GenerateAutoShader(int,int,int) -> static color set for " + object3D->GetName() + " object !");
	}
	if(materialsTexture.GetCount() > 0){
		//First we must had in Vertex Shader  all this thing :
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
			cpt++;
		}
		if(all_uniform.GetCount()>0) fragmentShader.Replace("//UNIFORM_MATERIAL_TEXTURE_NAME",all_uniform);

		if( NbLightDir > 0 && LightAffected){
			cpt = 0;
			for(const Upp::String& key : materialsTexture.GetKeys()){
				Upp::String textureNameIdentifier = "texture" + Upp::AsString(cpt) ;
				if(cpt == 0){
					AllDirLights << "for(int i = 0; i < " + Upp::AsString(NbLightDir) +"; i++){\n";
					AllDirLights << "\tresult += CalcTextureDirLight("+ textureNameIdentifier +",dirLights[i], norm, viewDir);\n";
				}else{
					AllDirLights << "\tresult += CalcTextureDirLight("+ textureNameIdentifier +",dirLights[i], norm, viewDir);\n";
				}
				cpt++;
			}
			AllDirLights << "}";	
			fragmentShader.Replace("//STRUCT_DIR_LIGHT","LIGHT_DIR_STRUCT()");
			fragmentShader.Replace("//IMPORT_DIR_LIGHT_TEXTURE_PROTOTYPE","LIGHT_DIR_TEXTURE_PROTOTYPE()");
			fragmentShader.Replace("//DIR_LIGHT_TEXTURE_FUNCTION","LIGHT_DIR_TEXTURE_FUNCTION()");	
			fragmentShader.Replace("//DIR_LIGHTS_ARRAY","uniform DirLight dirLights["+ Upp::AsString(NbLightDir) +"];");
			fragmentShader.Replace("//DIRECTIONAL_LIGHTS",AllDirLights);
		}
		if( NbLightPoint > 0 && LightAffected){
			cpt = 0;
			for(const Upp::String& key : materialsTexture.GetKeys()){
				Upp::String textureNameIdentifier = "texture" + Upp::AsString(cpt) ;
				if(cpt == 0){
					AllPointLights << "for(int i = 0; i < " + Upp::AsString(NbLightPoint) +"; i++){\n";
					AllPointLights << "\tresult += CalcTexturePointLight("+ textureNameIdentifier +",pointLights[i], norm, FragPos , viewDir);\n";
				}else{
					AllPointLights << "\tresult += CalcTexturePointLight("+ textureNameIdentifier +",pointLights[i], norm, FragPos , viewDir);\n";
				}
				cpt++;
			}
			AllPointLights << "}";
			fragmentShader.Replace("//STRUCT_POINT_LIGHT","LIGHT_POINT_STRUCT()");
			fragmentShader.Replace("//IMPORT_POINT_LIGHT_TEXTURE_PROTOTYPE","LIGHT_POINT_TEXTURE_PROTOTYPE()");
			fragmentShader.Replace("//POINT_LIGHT_TEXTURE_FUNCTION","LIGHT_POINT_TEXTURE_FUNCTION()");
			fragmentShader.Replace("//POINT_LIGHTS_ARRAY","uniform PointLight pointLights["+ Upp::AsString(NbLightPoint) +"];");
			fragmentShader.Replace("//POINT_LIGHTS",AllPointLights);
		}
		if( NbLightSpot > 0 && LightAffected){
			cpt = 0;
			for(const Upp::String& key : materialsTexture.GetKeys()){
				Upp::String textureNameIdentifier = "texture" + Upp::AsString(cpt) ;
				if(cpt == 0){
					AllSpotLights << "for(int i = 0; i < " + Upp::AsString(NbLightSpot) +"; i++){\n";
					AllSpotLights << "\tresult += CalcTextureSpotLight("+ textureNameIdentifier +",spotLights[i], norm, FragPos , viewDir);\n";
				}else{
					AllSpotLights << "\tresult += CalcTextureSpotLight("+ textureNameIdentifier +",spotLights[i], norm, FragPos , viewDir);\n";
				}
				cpt++;
			}
			AllSpotLights << "}";
			fragmentShader.Replace("//STRUCT_SPOT_LIGHT","LIGHT_SPOT_STRUCT()");
			fragmentShader.Replace("//IMPORT_SPOT_LIGHT_TEXTURE_PROTOTYPE","LIGHT_SPOT_TEXTURE_PROTOTYPE()");
			fragmentShader.Replace("//SPOT_LIGHT_TEXTURE_FUNCTION","LIGHT_SPOT_TEXTURE_FUNCTION()");
			fragmentShader.Replace("//SPOT_LIGHTS_ARRAY","uniform SpotLight spotLights["+ Upp::AsString(NbLightSpot) +"];");
			fragmentShader.Replace("//SPOT_LIGHTS",AllSpotLights);
		}
		
		if(all_VecMulti.GetCount()>12){
			if((NbLightDir > 0 || NbLightPoint > 0 || NbLightSpot > 0) && LightAffected){
			//	fragmentShader.Replace("//FRAG_COLOR_CALCULATION","FragColor = vec4(result,1.0);");
				if(true)fragmentShader.Replace("//FRAG_COLOR_CALCULATION","vec4 texColor = texture(texture0.diffuse,TexCoords);\nif(texColor.a < 0.1)\n\tdiscard;\nFragColor =texColor;\n");
				else fragmentShader.Replace("//FRAG_COLOR_CALCULATION","FragColor =texture(texture0.diffuse,TexCoords);\n");
			}else{
				if(AlphaAffected)
					//all_VecMulti << ";\nif(texColor.a < 0.1)\n\tdiscard;\nFragColor=texColor;\n";
					all_VecMulti << ";\nFragColor=texColor;\n";
				else
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
		}
		
		if(all_VecMulti.GetCount()>12){
			//True represent "if alpha is enable" 
			if((NbLightDir > 0 || NbLightPoint > 0 || NbLightSpot > 0) && LightAffected){
				fragmentShader.Replace("//FRAG_COLOR_CALCULATION","FragColor = vec4(result,1.0);");
			}else{
				all_VecMulti << ";\nFragColor =texColor;\n";
				fragmentShader.Replace("//FRAG_COLOR_CALCULATION",all_VecMulti);
			}
		}
	}	
	if( (NbLightDir > 0 || NbLightPoint > 0 || NbLightSpot > 0) && LightAffected){
		//HEre we ensure to put everything to start lightcalculation
		fragmentShader.Replace("//LIGHT_STARTING_DATA","vec3 norm = normalize(Normal);\nvec3 viewDir = normalize(viewPos - FragPos);\nvec3 result = vec3(0.0,0.0,0.0);\n");
	}
	if(!GetShader().AddShader(  "Vertex",ST_VERTEX,vertexShader).AddShader("Fragment",ST_FRAGMENT,fragmentShader).CompileShader(true)){
		LOG("Class Mesh:(ERROR) void Mesh::GenerateAutoShader(int,int,int) ->Shader failled to compilate !");	
	}*/
}
