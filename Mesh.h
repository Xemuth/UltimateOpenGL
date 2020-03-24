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
        
        unsigned int DrawMethod = GL_TRIANGLES;
        
        Shader shader;
        Transform transform;
        const Material* material = nullptr;
    public:
        Mesh();
        Mesh(Object3D& obj);
        Mesh(Upp::Vector<Vertex>& _vertices);
        Mesh(Upp::Vector<Vertex>& _vertices, Upp::Vector<unsigned int>& _indices);
        Mesh(Mesh& mesh);
        ~Mesh();
        Mesh& operator=(Mesh& mesh);
        
        Upp::Vector<Vertex>& GetVertices();
        Upp::Vector<unsigned int>& GetIndices();
        
        Object3D& GetObject3D();
        Mesh& SetObject3D(Object3D& obj);
        
        Shader& GetShader();
        Mesh& GenerateAutomaticShader();
        Mesh& SetDrawMethod(unsigned int GL_DRAW_METHOD);
        Transform& GetTransform();
        const Material* GetMaterial();
        Mesh& SetMaterial(const Material* _mat);
        Mesh& SetMaterial(Material& _mat);
        
        bool IsLoaded();
        
        bool ReadData(Upp::Vector<float>& data, ReaderParameters& readerParameter, bool UseMaterialColor = false);
        Mesh& Load(int MeshNumber);
        Mesh& LoadDefaultIndices();
        Mesh& Draw(int MeshNumber,glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4,bool DifferentShader = false, bool DifferentMaterial = false);
};
#endif
