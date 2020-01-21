#ifndef _UltimateOpenGL_V3_Mesh_h_
#define _UltimateOpenGL_V3_Mesh_h_
#include "Definition.h"
#include "Shader.h"
#include "Texture.h"


struct Vertex : public Upp::Moveable<Vertex> {
    // position
    glm::vec3 Position=glm::vec3(0);
    // normal
    glm::vec3 Normal=glm::vec3(0);
    // texCoords
    glm::vec2 TexCoords=glm::vec2(0.0,0.0);
    // tangent
    glm::vec3 Tangent=glm::vec3(0);
    // bitangent
    glm::vec3 Bitangent=glm::vec3(0);
    
    Vertex(){}
    
    Vertex& SetPosition(glm::vec3 _position){
        Position = _position;
        return *this;
    }
    Vertex& SetNormal(glm::vec3 _normal){
        Normal = _normal;
        return *this;
    }
    Vertex& SetTexCoords(glm::vec2 _texCoords){
        TexCoords = _texCoords;
        return *this;
    }
    Vertex& SetTangent(glm::vec3 _tangent){
        Tangent = _tangent;
        return *this;
    }
    Vertex& SetBitangent(glm::vec3 _bitangent){
        Bitangent = _bitangent;
        Bitangent = _bitangent;
        return *this;
    }
    
    Vertex(const Vertex& v){
		Position = v.Position;
		Normal = v.Normal;
		TexCoords = v.TexCoords;
		Tangent = v.Tangent;
		Bitangent = v.Bitangent;
    }
    
};

class Mesh : public Upp::Moveable<Mesh> {
	protected:
		Object3D* object3D =nullptr;
		
        /*  Render data  */
        unsigned int VAO, VBO, EBO;

        /*  Mesh Data  */
        Upp::Vector<Vertex> vertices;
        Upp::Vector<unsigned int> indices;
        Transform transform;
        Upp::Vector<Texture> textures;
        
        Shader shader;//When Object3D handle a shader, default behaviour of Mesh is looking if is own shader is compiled. If yes it mean
				      //he must use this shader.
					  
		DrawMethod drawMethod = DM_TRIANGLES; //even if Mesh have a default DrawMethod. It wont be used for render this mesh. The drawMethod used is the parent
		bool useDrawMethod = false;           //the object3D drawMethod. To force drawMethod on
										      //one particulare Shader, you must call
										      //SetDrawMethod() of the mesh to ensure it must
										      //use a custom drawMethod()
        
		Material material; //Same as Draw method. If material is completed with texture or something else than default color then it will be used instead of Obect3D material
		bool useMaterial = false;
		
		
		bool AlphaAffected = true;
		bool LightAffected = true;
    public:
        
		unsigned int GetVAO();
		unsigned int GetVBO();
		unsigned int GetEBO();
		
		Upp::Vector<Vertex>& GetVertices();
		
		void SetIndices(Upp::Vector<unsigned int>& _indices);
		Upp::Vector<unsigned int>& GetIndices();

		/* Texture Gestion */
		Mesh& BindTexture(const Upp::String& TextureName,float mixValue = 1.0, float textureShininess = 0.64f,const Upp::String& TextureSpecularName ="", const Upp::String& NormalMappingTextureName="");
	/*	bool RemoveTexture(const Texture& _texture);
		Upp::Vector<Texture>& GetTextures();*/

		/* Transform */
		Transform& GetTransform();
		void SetTransform(Transform& _transform);
		
		/* Material Manager */
		Material& GetMaterial();
		Mesh& SetMaterial(Material& m);
		/*Upp::VectorMap<Upp::String,MaterialTexture>& GetMaterialTextures();
		Upp::VectorMap<Upp::String,MaterialColor>& GetMaterialColor();
		
		MaterialTexture& CreateMaterialTexture(const Upp::String& _name);
		MaterialColor& CreateMaterialColor(const Upp::String& _name);*/
		
		bool ReadData(Upp::Vector<float>& data, ReaderParameters readerParameter);
		
		Mesh& SetDrawMethod(DrawMethod dm);
		DrawMethod GetDrawMethod();
		
	//	bool BindTexture(Upp::String textureName,float textureShininess,Upp::String TextureSpecularName);
		
		//Miscelnious
		void SetObject3D(Object3D& _object3D);
		Object3D& GetObject3D();
		
		void SetShader(Shader& _shader);
		Shader& GetShader();
		
		void SetLightAffected(bool _light =false);
		bool IsLightAffected();
		
		void UseAlpha(bool _alpha =false);
		bool IsAlpha();

        /*  Functions  */
        void Load();
        void LoadDefaultIndices();
        void GenerateAutoShader(int NbLightDir,int NbLightPoint,int NbLightSpot);
        GLint ResolveDrawMethod();
        
        Mesh() = default;
        Mesh(GameObject& gameObject, const Upp::Vector<Vertex>& vertices, Upp::Vector<unsigned int>& indices, Upp::Vector<Texture>& textures);
        Mesh(const Mesh& _mesh); //Dont forget to set z
        void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera);
};
#include "TransformMesh.h"
#endif
