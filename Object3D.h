#ifndef _UltimateOpenGL_V3_Object3D_h_
#define _UltimateOpenGL_V3_Object3D_h_
#include "Definition.h"

class Object3D : public GameObject, Upp::Moveable<Object3D>
{
	 protected:	     
		
        /*  Model Data */
	    Upp::Vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	    Upp::Array<Mesh> meshes;
	    Upp::String directory="";
	    bool gammaCorrection;
	    
	    bool loaded=false;
	    
	    
	    /*
	    	This part allow High performance multipleMesh rendering.
	    */
	    bool HighPerfomanceDrawing =false; //Used to know if hight perfomance draw is should be enable or not
	    void GenerateHighPerfomanceShader(); //USed to generate hight performance shader. Ensure the boolean HighPerfomanceDrawing is set to true before loading
	    /*
	    glm::vec3* positionVector = nullptr; //Stock position of each mesh information
	    glm::vec3* normalVector = nullptr; //Stock normal if it existe
	    glm::vec2* textureCoordinateVector = nullptr; //Stock TC if it existe
	    glm::vec3* tangeant = nullptr;
	    glm::vec3* biTangeant =nullptr;
	    glm::mat4* modelMatrices =nullptr; //Used to stock all model we have to draw (used for
	    high perf)*/
	    Shader HighPerfShader;
	    //unsigned int HighPerformanceVAO;
	  //  unsigned int HighPerformanceVBO;
	    //unsigned int HighPerformanceEBO;
	    ///////////////////////////////////////////////////////////////////
	    
        /*  Functions   */
        
        void ProcessNode(aiNode *node, const aiScene *scene);
        void ProcessMesh(aiMesh *mesh, const aiScene *scene);
        
     	void ManageTextures(Upp::Vector<Texture>& vectorToFile, aiMaterial *mat, aiTextureType type);
     	
        Upp::Vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,const Upp::String& typeName);
		       
 		int MeshShaderToUse=-1;
 		Shader* ShaderToUse=nullptr;
 		
 		int MeshIndiceToUse=-1;
 		Upp::Vector<unsigned int> IndicesToUse;
 		
    public:
        /*  Functions   */
        Object3D();
        Object3D(Scene* _scene);
        Object3D(Mesh& _mesh);
        Object3D(Upp::Vector<float>& Vertices, ReaderParameters readerParameter = ReaderParameters(), ReaderRoutine readerRoutine = ReaderRoutine());
        Object3D(Upp::Array<Mesh>& _meshes);
        Object3D(const Upp::String& pathOfModel, Scene*_scenePtr = nullptr);
        Object3D(Object3D& obj);
        Object3D& operator=(Object3D& obj);
        ~Object3D();
        void LoadModel(const Upp::String& path);
        
        void ReadData(Upp::Vector<float>& data ,ReaderParameters readerParameter = ReaderParameters(),ReaderRoutine readerRoutine = ReaderRoutine());
        //void ReadData(Upp::Vector<float>& data);
        
        void Load();
        void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera);	
        
        Upp::Array<Mesh>& GetAllMeshes();
        
        
        Object3D& SetLightAffected(bool b = false);
        Object3D& SetAlaphaAffected(bool b = false);
        Object3D& BindTexture(const Upp::String& TextureName,float mixValue = 1.0, float textureShininess = 0.64f,const Upp::String& TextureSpecularName ="", const Upp::String& NormalMappingTextureName="");
   		Object3D& AddMaterialColor(const Upp::String& ColorName,MaterialColor materialColor = MaterialColor());
   		Object3D& SetDrawMethod(DrawMethod dm);
   		Object3D& UseOneShader(int _MeshShaderToUse =0); //Define if the object should use one shader to all is mesh
   		Object3D& UseOneShader(Shader* MeshShaderToUse); //Define if the object should use one shader to all is mesh
   		Object3D& UseSameIndices(int _MeshIndicestoUse =0);
		Object3D& UseSameIndices(const Upp::Vector<unsigned int>& _indicesToUse);
		Object3D& ActivateHighPerfomance();
		Object3D& DesactivateHighPerfomance();
};


#endif
