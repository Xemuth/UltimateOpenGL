#ifndef _UltimateOpenGL_Object3D_h_
#define _UltimateOpenGL_Object3D_h_
#include "GameObject.h"

class Object3D : public GameObject
{
	protected:
		Upp::Array<Mesh> meshes;
		Upp::String directory="";
		bool loaded=false;
		
	    //Loading 3D Model using Assimp and custom loader
	    void ProcessNode(aiNode *node, const aiScene *scene);
	    void ProcessMesh(aiMesh *mesh, const aiScene *scene);
    public:
        Object3D();
        Object3D(Mesh& _mesh);
        Object3D(Upp::Vector<Mesh>& _meshes);
        Object3D(Upp::Vector<float>& Vertices, ReaderParameters readerParameter = ReaderParameters(), ReaderRoutine readerRoutine = ReaderRoutine()); //way of reading a floating point vector
        Object3D(const Upp::String& pathOfModel); //Path of model to load
        
        Object3D(Object3D& _object);//Be carefull of setting the Scene
        Object3D& operator=(Object3D& _object);//Be carefull of setting the Scene
        virtual ~Object3D();
    
		Upp::Array<Mesh>& GetMeshes(); //Return Array of Mesh
		Upp::String GetModelDirectory(); //Return Model directory if it have been loaded one time at least
		bool IsLoaded(); //Return true if the object is loaded
		
		Object3D& SetMaterialForMeshes(Material& material);
		
		void LoadModel(const Upp::String& path); //Used to load 3D Model
        void ReadData(Upp::Vector<float>& data ,ReaderParameters readerParameter = ReaderParameters(),ReaderRoutine readerRoutine = ReaderRoutine()); //Used to read vector<Float>
		
    //Override
        virtual void Load();
        virtual void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera);
};
#endif