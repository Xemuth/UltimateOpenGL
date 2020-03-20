#ifndef _UltimateOpenGL_Object3D_h_
#define _UltimateOpenGL_Object3D_h_
#include "GameObject.h"

class Object3D : public GameObject
{
	protected:
		Upp::Array<Mesh> meshes;
		Upp::String directory="";
		bool loaded=false;
		
		bool SlowDraw = true; //SlowDraw mean , for each mesh, UOGL will take time to sent ALL data about material to the shader, wich meaan for a strong amount of data, it can hugly decrase amount of FPS
							  //By setting it to false, it will never refresh uniform data in
							  //the shader, wich make the draw routine fatest !

	    //Loading 3D Model using Assimp and custom loader
	    void ProcessNode(aiNode *node, const aiScene *scene);
	    void ProcessMesh(aiMesh *mesh, const aiScene *scene);
	    const Material* CreateAndBindMaterial(aiMaterial *mat, aiTextureType type);
    public:
        Object3D();
        Object3D(Mesh& _mesh);
        Object3D(Upp::Vector<Mesh>& _meshes);
        Object3D(Upp::Vector<float>& Vertices, ReaderParameters readerParameter = ReaderParameters(), ReaderRoutine readerRoutine = ReaderRoutine()); //way of reading a floating point vector
        //Object3D(const Upp::String& pathOfModel); //Path of model to load || METHOD ONLY
        //since Model loading requiere to have a scene binded
        
        Object3D(Object3D& _object);//Be carefull of setting the Scene
        Object3D& operator=(Object3D& _object);//Be carefull of setting the Scene
        virtual ~Object3D();
    
		Upp::Array<Mesh>& GetMeshes(); //Return Array of Mesh
		Upp::String GetModelDirectory(); //Return Model directory if it have been loaded one time at least
		bool IsLoaded(); //Return true if the object is loaded
	
		Object3D& EnableSlowDraw();
		Object3D& DisableSlowDraw();
		bool IsSlowDraw();
	
		Object3D& SetMaterialForMeshes(const Material* material);
		Object3D& SetMaterialForMeshes(Material& material);
		Object3D& SetDrawMethodForMeshes(unsigned int GL_DRAW_METHOD);
		
		Object3D& AssignShaderToMeshes(Shader& shader);
		Object3D& UseSameShaderMeshes(int MeshToUse = 0);
		
		Object3D& LoadModel(const Upp::String& path); //Used to load 3D Model
        Object3D& ReadData(Upp::Vector<float>& data ,ReaderParameters readerParameter = ReaderParameters(),ReaderRoutine readerRoutine = ReaderRoutine()); //Used to read vector<Float>
		
    //Override
        virtual void Load();
        virtual void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera);
};
#endif