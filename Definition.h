#ifndef _UltimateOpenGL_V3_Definition_h_
#define _UltimateOpenGL_V3_Definition_h_
#include <Core/Core.h>

#define STRINGIFY(...) #__VA_ARGS__
#define SHADER(version, shader) "#version " #version "\n" STRINGIFY(shader)

#include "stb_image.h"//Stb is used to easily load different image type 
#ifndef flagNOUPPGL
	#include <GLCtrl/GLCtrl.h>
#endif
/*z
#ifndef flagUPPGL
//	#include <glad/glad.h> //Glad is used to init OpenGL
	//glad's no longer in UOGL. IMO the window manager should load open
#else
	#include <GLCtrl/GLCtrl.h>
#endif*/
#define GLM_ENABLE_EXPERIMENTAL
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>
#include <plugin/glm/gtc/type_ptr.hpp>
#include <plugin/glm/gtx/quaternion.hpp>
#include <plugin/glm/gtx/norm.hpp>

#include <plugin/Assimp/assimp.h>


enum GameObjectType{LIGHT,SOUND,OBJECT3D,OBJECT2D,OBJECTUI};
enum TranslationType{TRANSLATION, ROTATION,SECOND_ROTATION, SCALE};
enum Camera_Movement {UOGL_FORWARD,UOGL_BACKWARD,UOGL_LEFT,UOGL_RIGHT};// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum TextureColorSample{SAMPLE_RGB,SAMPLE_RGBA}; //Allow to define type of color the texture you want load is
enum TextureType{DIFFUSE,SPECULAR,NORMAL,HEIGHT, AMBIENT};
enum ShaderType{PROGRAMM,VERTEX,FRAGMENT}; //Define type of shader we want 
enum DrawMethod{UOGL_POINTS,UOGL_LINES,UOGL_LINE_STRIP,UOGL_LINE_LOOP,UOGL_TRIANGLES,UOGL_TRIANGLE_STRIP,UOGL_TRIANGLE_FAN,UOGL_QUADS,UOGL_QUAD_STRIP,UOGL_POLYGON};

/*
PROGRAMM -> GL_POINTS
Treats each vertex as a single point. Vertex n defines point n. N points are drawn.
UOGL_LINES -> GL_LINES
Treats each pair of vertices as an independent line segment. Vertices 2 ⁢ n - 1 and 2 ⁢ n define line n. N 2 lines are drawn.
UOGL_LINE_STRIP - > GL_LINE_STRIP
Draws a connected group of line segments from the first vertex to the last. Vertices n and n + 1 define line n. N - 1 lines are drawn.
UOGL_LINE_LOOP -> GL_LINE_LOOP
Draws a connected group of line segments from the first vertex to the last, then back to the first. Vertices n and n + 1 define line n. The last line, however, is defined by vertices N and 1 . N lines are drawn.
UOGL_TRIANGLE -> GL_TRIANGLES (Default USED)
Treats each triplet of vertices as an independent triangle. Vertices 3 ⁢ n - 2 , 3 ⁢ n - 1 , and 3 ⁢ n define triangle n. N 3 triangles are drawn.
UOGL_TRIANGLE_STRIP -> GL_TRIANGLE_STRIP
Draws a connected group of triangles. One triangle is defined for each vertex presented after the first two vertices. For odd n, vertices n, n + 1 , and n + 2 define triangle n. For even n, vertices n + 1 , n, and n + 2 define triangle n. N - 2 triangles are drawn.
UOGL_TRIANGLE_FAN -> GL_TRIANGLE_FAN
Draws a connected group of triangles. One triangle is defined for each vertex presented after the first two vertices. Vertices 1 , n + 1 , and n + 2 define triangle n. N - 2 triangles are drawn.
UOGL_QUADS -> GL_QUADS
Treats each group of four vertices as an independent quadrilateral. Vertices 4 ⁢ n - 3 , 4 ⁢ n - 2 , 4 ⁢ n - 1 , and 4 ⁢ n define quadrilateral n. N 4 quadrilaterals are drawn.
UOGL_QUAD_STRIP -> GL_QUAD_STRIP
Draws a connected group of quadrilaterals. One quadrilateral is defined for each pair of vertices presented after the first pair. Vertices 2 ⁢ n - 1 , 2 ⁢ n , 2 ⁢ n + 2 , and 2 ⁢ n + 1 define quadrilateral n. N 2 - 1 quadrilaterals are drawn. Note that the order in which vertices are used to construct a quadrilateral from strip data is different from that used with independent data.
UOGL_POLYGON -> GL_POLYGON
Draws a single, convex polygon. Vertices 1 through N define this polygon
*/


// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       = 15.0f;
const float SENSITIVITY =  0.1f;
const float FOV        =  90.0f;

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

struct ReaderParameters{
	int coordinatesPosition = -1; //Define position of coordinates in vector of float
	int normalPosition = -1; //Define position of normal in vector of float
	int textureCoordinatePosition = -1; //Define position of texture coordinate in vector of float
	int tangentPosition = -1; //Define position of tangent position in vector of float
	int bitangentPosition = -1; //Define position of bitangent position in vector of float

	ReaderParameters(int vp=0,int np=1,int tcp=2,int tp=-1,int bp=-1){
		coordinatesPosition=vp;
		normalPosition=np;
		textureCoordinatePosition=tcp;
		tangentPosition=tp;
		bitangentPosition=bp;
	}

	ReaderParameters& CoordinatesPosition(int vp){coordinatesPosition = vp;return *this;}
	ReaderParameters& NormalPosition(int np){normalPosition = np;return *this;}
	ReaderParameters& TextureCoordinatePosition(int tcp){textureCoordinatePosition = tcp;return *this;}
	ReaderParameters& TangentPosition(int tp){tangentPosition = tp;return *this;}
	ReaderParameters& BitangentPosition(int bp){bitangentPosition = bp;return *this;}
};

struct ReaderRoutine{
	int verticesPerMesh = -1; //Define how many line mesh should  carry
	int startMesh = -1;	 //Define where readData should start is completion of Mesh
	bool allowCreation = true; //Define if creation of mesh is allowed in the object3D
	
	ReaderRoutine(int vpm=-1, int sm=-1, int ac=true){
		verticesPerMesh=vpm;
		startMesh=sm;
		allowCreation=ac;
	}
	 
	ReaderRoutine& VerticesPerMesh(int vpm){verticesPerMesh = vpm; return *this;}
	ReaderRoutine& StartMesh(int sm){startMesh = sm; return *this;}
	ReaderRoutine& AllowCreation(bool ac){allowCreation = ac; return *this;}
};

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
