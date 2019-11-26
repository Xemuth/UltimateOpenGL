#ifndef _UltimateOpenGL_V3_Mesh_h_
#define _UltimateOpenGL_V3_Mesh_h_

#include "Definition.h"

struct Vertex {
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
};

class Mesh {
	private:
		
		Object3D* object3D =nullptr;
		
        /*  Render data  */
        unsigned int VAO, VBO, EBO;
        /*  Functions    */
        void Load();
        
        /*  Mesh Data  */
        Upp::Vector<Vertex> vertices;
        Upp::Vector<unsigned int> indices;
        Upp::Vector<Texture> textures;
        
        Shader shaders;
        
        Transform transform;
        
        Upp::VectorMap<Upp::String,MaterialColor> materialsColor; //Properties materialColor of the object
		Upp::VectorMap<Upp::String,MaterialTexture> materialsTexture; //Properties materialTexture of the object //Basicly you only have one of both Material to set
    
    	bool LightAffected = true;
    public:

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
		MaterialColor& CreateMaterialColor(Uconst Upp::String& _name);
		
		
		//Miscelnious
		void SetObject3D(Object3D* _object3D);
		Object3D* GetObject3D();
		
		void SetLightAffected(bool _light);
		bool IsLightAffected();

        /*  Functions  */
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void Draw(Shader shader);
};
#endif
