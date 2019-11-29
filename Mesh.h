#ifndef _UltimateOpenGL_V3_Mesh_h_
#define _UltimateOpenGL_V3_Mesh_h_
#include "Definition.h"

struct Vertex : public Upp::Moveable<Vertex> {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    
    Vertex(){}
    Vertex(const Vertex& v){
     	Position = v.Position;
     	Normal = v.Normal;
     	Tangent = v.Tangent;
     	Bitangent = v.Bitangent;   
    }
    
};

class Mesh : public Upp::Moveable<Mesh> {
	private:
			
		Object3D* object3D =nullptr;
		
        /*  Render data  */
        unsigned int VAO, VBO, EBO;
        
        
        /*  Mesh Data  */
        Upp::Vector<Vertex> vertices;
        Upp::Vector<unsigned int> indices;
        Upp::Vector<Texture> textures;
        
        Shader shader;
        
        Transform transform;
        
        Upp::VectorMap<Upp::String,MaterialColor> materialsColor; //Properties materialColor of the object
		Upp::VectorMap<Upp::String,MaterialTexture> materialsTexture; //Properties materialTexture of the object //Basicly you only have one of both Material to set
    
    	bool LightAffected = true;
    public:
		
		inline static Upp::VectorMap<Upp::String,Upp::String> BasicShaders{
			{"Default_Vertex_Shader",
				Replace(
				#include "Simps/DefaultVertexShader.glsl"
				,Upp::Vector<Upp::String>{R"(@)"},Upp::Vector<Upp::String>{"//"})
			},
			{"Default_Fragment_Shader",
			Replace(
				#include "Simps/DefaultFragmentShader.glsl"
				,Upp::Vector<Upp::String>{R"(@)"},Upp::Vector<Upp::String>{"//"})
			}
		};
	
		unsigned int GetVAO();
		unsigned int GetVBO();
		unsigned int GetEBO();
		
		Upp::Vector<Vertex>& GetVertices();
		Upp::Vector<unsigned int>& GetIndices();

		/* Texture Gestion */
		Mesh& BindTexture(const Upp::String& TextureName);
		bool RemoveTexture(const Texture& _texture);
		Upp::Vector<Texture>& GetTextures();

		/* Transform */
		Transform& GetTransform();
		void SetTransform(const Transform& _transform);
		
		/* Material Manager */
		Upp::VectorMap<Upp::String,MaterialTexture>& GetMaterialTextures();
		Upp::VectorMap<Upp::String,MaterialColor>& GetMaterialColor();
		
		MaterialTexture& CreateMaterialTexture(const Upp::String& _name);
		MaterialColor& CreateMaterialColor(const Upp::String& _name);
		
		
		//Miscelnious
		void SetObject3D(Object3D* _object3D);
		Object3D* GetObject3D();
		
		void SetShader(Shader& _shader);
		Shader& GetShader();
		
		void SetLightAffected(bool _light);
		bool IsLightAffected();

        /*  Functions  */
        void Load();
        void GenerateAutoShader(int NbLightDir,int NbLightPoint,int NbLightSpot);
        Mesh() = default;
        Mesh(const Upp::Vector<Vertex>& vertices, Upp::Vector<unsigned int>& indices, Upp::Vector<Texture>& textures);
        Mesh(Mesh& _mesh);
        void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera);
};
#endif
