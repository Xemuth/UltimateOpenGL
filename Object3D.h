#ifndef _UltimateOpenGL_V3_Object3D_h_
#define _UltimateOpenGL_V3_Object3D_h_
#include "Definition.h"
#include "GameObject.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh;
class GameObject;

class Object3D : public GameObject, Upp::Moveable<Object3D>
{
	 private:
        /*  Model Data */
	    Upp::Vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	    Upp::Vector<Mesh> meshes;
	    Upp::String directory="";
	    bool gammaCorrection;
        /*  Functions   */
        
        void ProcessNode(aiNode *node, const aiScene *scene);
        void ProcessMesh(aiMesh *mesh, const aiScene *scene);
        
     	void ManageTextures(Upp::Vector<Texture>& vectorToFile, aiMaterial *mat, aiTextureType type);
     	
        Upp::Vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,const Upp::String& typeName);
        
    public:
        /*  Functions   */
        Object3D(const Upp::String& pathOfModel);
        void LoadModel(const Upp::String& path);
        void Draw(Shader shader);	
   
};


#endif
