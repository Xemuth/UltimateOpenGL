#ifndef _UltimateOpenGL_V3_Object3D_h_
#define _UltimateOpenGL_V3_Object3D_h_
#include "Definition.h"

class Object3D : public GameObject, Upp::Moveable<Object3D>
{
	 private:	     
        /*  Model Data */
	    Upp::Vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	    Upp::Array<Mesh> meshes;
	    Upp::String directory="";
	    bool gammaCorrection;
        /*  Functions   */
        
        void ProcessNode(aiNode *node, const aiScene *scene);
        void ProcessMesh(aiMesh *mesh, const aiScene *scene);
        
     	void ManageTextures(Upp::Vector<Texture>& vectorToFile, aiMaterial *mat, aiTextureType type);
     	
        Upp::Vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,const Upp::String& typeName);
        
    public:
        /*  Functions   */
        Object3D();
        Object3D(Scene* _scene);
        Object3D(Mesh& _mesh);
        Object3D(Upp::Array<Mesh>& _meshes);
        Object3D(const Upp::String& pathOfModel);
        void LoadModel(const Upp::String& path);
        void Load();
        void Draw(glm::mat4 model,glm::mat4 view,glm::mat4 projection,glm::mat4 transform,Camera& camera);	
        
        Object3D& BindTexture(const Upp::String& TextureName,float mixValue = 1.0, float textureShininess = 0.64f,const Upp::String& TextureSpecularName ="", const Upp::String& NormalMappingTextureName="");
   
};


#endif
