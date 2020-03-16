#ifndef _UltimateOpenGL_Mesh_h_
#define _UltimateOpenGL_Mesh_h_
#include "Object3D.h"

class Mesh {
	protected:
		Object3D* object3D =nullptr;
		
        /*  Render data  */
        unsigned int VAO, EBO, VBO;

        /*  Mesh Data  */
        Upp::Vector<Vertex> vertices;
        Upp::Vector<unsigned int> indices;
        Transform transform;
        Upp::Vector<Texture2D> textures;
        
        Shader shader;//When Object3D handle a shader, default behaviour of Mesh is looking if is own shader is compiled. If yes it mean
				      //he must use this shader.
					  
		DrawMethod drawMethod = DM_TRIANGLES; //even if Mesh have a default DrawMethod. It wont be used for render this mesh. The drawMethod used is the parent
		bool useDrawMethod = false;           //the object3D drawMethod. To force drawMethod on
										      //one particulare Shader, you must call
										      //SetDrawMethod() of the mesh to ensure it must
										      //use a custom drawMethod()
        
		Upp::One<Material> material; //Same as Draw method. If material is completed with texture or something else than default color then it will be used instead of Obect3D material
		bool useMaterial = false;
		bool useMaterialColorInsteadOfColorRetrieve = false;
		
		bool AlphaAffected = true;
		bool LightAffected = true;
    public:
        Mesh();
        Mesh(Object3D& obj);
        Mesh(Object3D& gameObject, Upp::Vector<Vertex>& vertices, Upp::Vector<unsigned int>& indices, Upp::Vector<Texture2D>& textures);
        Mesh(const Mesh& _mesh); //Dont forget to set z
        
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
		
		bool ReadData(Upp::Vector<float>& data, ReaderParameters& readerParameter, bool UseMaterialColor = false );
		
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
           
        void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera);
};
#endif
