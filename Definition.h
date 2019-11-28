#ifndef _UltimateOpenGL_V2_UltimateDefinition_h_
#define _UltimateOpenGL_V2_UltimateDefinition_h_
#include <Core/Core.h>

#define STRINGIFY(...) #__VA_ARGS__
#define SHADER(version, shader) "#version " #version "\n" STRINGIFY(shader)

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/norm.hpp>

#ifndef flagUPPGL
	#include <glad/glad.h> //Glad is used to init OpenGL
#else
	#include <GLCtrl/GLCtrl.h>
#endif
//YOu can use NO_GL_INIT flag to prevent glad to init it
//however glad is require to get GL def
#include "stb_image.h"//Stb is used to easily load different image type 

enum GameObjectType{LIGHT,SOUND,OBJECT3D,OBJECT2D,OBJECTUI};
enum TranslationType{TRANSLATION, ROTATION,SECOND_ROTATION, SCALE};
enum Camera_Movement {FORWARD,BACKWARD,LEFT,RIGHT};// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum TextureColorSample{SAMPLE_RGB,SAMPLE_RGBA}; //Allow to define type of color the texture you want load is
enum TextureType{DIFFUSE,SPECULAR,NORMAL,HEIGHT};
enum ShaderType{PROGRAMM,VERTEX,FRAGMENT}; //Define type of shader we want 


// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  5.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class UltimateOpenGL_Context;
class Scene;
class Camera;
class GameObject;
class Mesh;
class Object3D;
class Texture;
class Shader;
class Transform;

struct Vertex;
struct TextureParameter;
struct Material;
struct MaterialTexture;
struct MaterialColor;
struct Light;
struct DirLight;
struct PointLight;
struct SpotLight;

typedef int TRANSLATION_TYPE;
typedef void (*ACTION_FUNCTION)(GameObject& myObject);

class UGLException : public std::exception { //classe to managed every exception
	private:
	    int m_numero;               //Id of Error
	    Upp::String m_phrase;       //Error summaries
	    int m_niveau;               //level of Error
	    char* myChar=NULL;

	public:
	    UGLException(int numero=0, Upp::String phrase="", int niveau=0){
	        m_numero = numero;
	        m_phrase = phrase;
	        m_niveau = niveau;
			myChar =  new char[m_phrase.GetCount()+1];
	        strcpy(myChar,this->m_phrase.ToStd().c_str());
	    }
	    
	    virtual const char* what() const throw() {
			return  (const char *)  myChar;
	    }
	    int getNiveau() const throw(){
			return m_niveau;
	    }
		virtual ~UGLException(){
			delete [] myChar;
		}
};



#endif
