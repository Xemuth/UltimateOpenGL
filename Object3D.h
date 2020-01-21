#ifndef _UltimateOpenGL_Object3D_h_
#define _UltimateOpenGL_Object3D_h_
#include "GameObject.h"
#include "Texture.h"
#include "Shader.h"
class Object3D : public GameObject
{
	protected:
	    //Upp::Vector<Texture> textures_loaded;// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	    Upp::Array<Mesh> meshes;
	    Upp::String directory="";
	    bool loaded=false;
	    
		Shader shader; //When scene handle a shader, default behaviour of Object3D is looking if is own shader is compiled. If yes it mean
					   //he must use this shader.
					   
		DrawMethod drawMethod = DM_TRIANGLES;
		
		bool alphaAffected = true;
		bool lightAffected = true;
		
		Material material; //This material must be Set. To overLoad on a mesh this material, you must set the mesh material
        
        //Loading 3D Model using Assimp and custom loader
        void ProcessNode(aiNode *node, const aiScene *scene);
        void ProcessMesh(aiMesh *mesh, const aiScene *scene);
        void ManageTextures(Upp::Vector<Texture>& vectorToFile, aiMaterial *mat, aiTextureType type);
        Upp::Vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,const Upp::String& typeName);
    public:
        
        Object3D(Scene& _scene);
        Object3D(Scene& _scene, Mesh& _mesh);
        Object3D(Scene& _scene, Upp::Array<Mesh>& _meshes);
        Object3D(Scene& _scene, Upp::Vector<float>& Vertices, ReaderParameters readerParameter = ReaderParameters(), ReaderRoutine readerRoutine = ReaderRoutine());
        Object3D(Scene& _scene, const Upp::String& pathOfModel);
        Object3D(Object3D& _object);//Be carefull of setting the Scene
        Object3D& operator=(Object3D& _object);//Be carefull of setting the Scene

        void LoadModel(const Upp::String& path); //Used to load 3D Model
        void ReadData(Upp::Vector<float>& data ,ReaderParameters readerParameter = ReaderParameters(),ReaderRoutine readerRoutine = ReaderRoutine()); //Used to read vector<Float>
        
        Upp::Array<Mesh>& GetMeshes();
        
        Object3D& EnableLightCalculation();
        Object3D& DisableLightCalculation();
        bool IsLightCalculationEnable();
        
        Object3D& EnableAlpha();
        Object3D& DisableAlpha();
        bool IsAlphaEnable();

        Material& GetMaterial();
		
		Object3D& SetDrawMethod(DrawMethod dm);
		DrawMethod GetDrawMethod();
		
		Object3D& SetShader(Shader& _shader);
		Shader& GetShader();
		
		//Override
        void Load();
        void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera);
};
#endif