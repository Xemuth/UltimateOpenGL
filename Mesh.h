#ifndef _UltimateOpenGL_Mesh_h_
#define _UltimateOpenGL_Mesh_h_
#include "Object3D.h"
#include "Definition.h"
class Mesh{
	protected:
		Object3D* object3D =nullptr;
		bool loaded = false;
		
		Upp::Vector<Vertex> vertices;
		Upp::Vector<unsigned int> indices;
		
        unsigned int VAO = 0, EBO = 0, VBO = 0;
        
        Shader shader;
        Transform transform;
        Upp::One<Material> material;
    public:
        Mesh();
        Mesh(Object3D& obj);
        Mesh(Upp::Vector<Vertex>& vertices);
        Mesh(Upp::Vector<Vertex>& vertices, Upp::Vector<unsigned int>& indices);
        Mesh(Mesh& mesh);
        Mesh& operator=(Mesh& mesh);
        
        Upp::Vector<Vertex>& GetVertices();
        Upp::Vector<unsigned int>& GetIndices();
        
        Object3D& GetObject3D();
        Mesh& SetObject3D(Object3D& obj);
        
        Shader& GetShader();
        Transform& GetTransform();
        Material* GetMaterial();
        
        bool IsLoaded();
        
        bool ReadData(Upp::Vector<float>& data, ReaderParameters& readerParameter, bool UseMaterialColor = false);
        Mesh& Load();
        Mesh& LoadDefaultIndices();
        Mesh& Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera);
};
#endif
