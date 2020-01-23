#ifndef _UltimateOpenGL_Definition_h_
#define _UltimateOpenGL_Definition_h_
#include <Core/Core.h>

#define STRINGIFY(...) #__VA_ARGS__
#define SHADER(version, shader) "#version " #version "\n" STRINGIFY(shader)

#include "stb_image.h"//Stb is used to easily load different image type
#ifndef flagNOUPPGL
	#include <GLCtrl/GLCtrl.h>
#endif
//Assimp package used to Load 3D Models
/*
	Special Thanks to Koldo
	https://github.com/izabala123
	https://www.ultimatepp.org/forums/index.php?t=usrinfo&id=648&
*/
#include <plugin/Assimp/assimp.h>
//GLM Package used to do a lot of mathematical function
#define GLM_ENABLE_EXPERIMENTAL
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>
#include <plugin/glm/gtc/type_ptr.hpp>
#include <plugin/glm/gtx/quaternion.hpp>
#include <plugin/glm/gtx/norm.hpp>
//Gestion of auto generated Shaders
#include "Simps/MaterialTexture.glsl"
#include "Simps/MaterialColor.glsl"
#include "Simps/PointLight.glsl"
#include "Simps/SpotLight.glsl"
#include "Simps/DirLight.glsl"
/*
	Multiples ENUM used in differentes class
*/
enum TranslationType{TT_TRANSLATION, TT_ROTATION,TT_SECOND_ROTATION, TT_SCALE};
enum Camera_Movement {CM_FORWARD,CM_BACKWARD,CM_LEFT,CM_RIGHT};// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum TextureColorSample{TCS_SAMPLE_RGB,TCS_SAMPLE_RGBA}; //Allow to define type of color the texture you want load is
enum TextureType{TT_DIFFUSE,TT_SPECULAR,TT_NORMAL,TT_HEIGHT,TT_AMBIENT};
enum ShaderType{ST_PROGRAMM,ST_VERTEX,ST_FRAGMENT}; //Define type of shader we want
enum DrawMethod{DM_POINTS,DM_LINES,DM_LINE_STRIP,DM_LINE_LOOP,DM_TRIANGLES,DM_TRIANGLE_STRIP,DM_TRIANGLE_FAN,DM_QUADS,DM_QUAD_STRIP,DM_POLYGON};
enum LightType{LT_DIRLIGHT,LT_SPOTLIGHT,LT_POINTLIGHT};
enum GameObjectType{GOT_SOUND,GOT_3D,GOT_SPOT_LIGHT,GOT_POINT_LIGHT,GOT_DIR_LIGHT,GOT_UNKNOW};
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
/*
	Definition of all defaults class used in UOGL
*/
class UltimateOpenGL_Context;

struct TextureInformation;
struct Vertex;

class Scene;
class GameObject;
class Light;
class DirLight;
class PointLight;
class SpotLight;
class Object3D;
class Mesh;

//typedef int TRANSLATION_TYPE;
typedef void (*ACTION_FUNCTION)(GameObject& myObject);

//Both structures is used to read Vector of float used to show multiple shape
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
//Used to read all file used to auto generate some kind of shader
inline Upp::VectorMap<Upp::String,Upp::String> BasicShaders{
	{"Default_Vertex_Shader",
		Replace(
		#include "Simps/DefaultVertexShader.glsl"
		,Upp::Vector<Upp::String>{R"(@)"},Upp::Vector<Upp::String>{"//"})
	},
	{"Default_Fragment_Shader",
	Replace(
		#include "Simps/DefaultFragmentShader.glsl"
		,Upp::Vector<Upp::String>{R"(@)"},Upp::Vector<Upp::String>{"//"})
	}
};
Upp::String& IncludeShader(Upp::String& shader){
	shader.Replace("MATERIAL_TEXTURE_STRUCT()",MATERIAL_TEXTURE_STRUCT());
	shader.Replace("MATERIAL_COLOR_STRUCT()",MATERIAL_COLOR_STRUCT());
	
	shader.Replace("LIGHT_POINT_STRUCT()",LIGHT_POINT_STRUCT());
	shader.Replace("LIGHT_POINT_TEXTURE_PROTOTYPE()",LIGHT_POINT_TEXTURE_PROTOTYPE());
	shader.Replace("LIGHT_POINT_TEXTURE_FUNCTION()",LIGHT_POINT_TEXTURE_FUNCTION());
	shader.Replace("LIGHT_POINT_COLOR_PROTOTYPE()",LIGHT_POINT_COLOR_PROTOTYPE());
	shader.Replace("LIGHT_POINT_COLOR_FUNCTION()",LIGHT_POINT_COLOR_FUNCTION());
	
	shader.Replace("LIGHT_SPOT_STRUCT()",LIGHT_SPOT_STRUCT());
	shader.Replace("LIGHT_SPOT_TEXTURE_PROTOTYPE()",LIGHT_SPOT_TEXTURE_PROTOTYPE());
	shader.Replace("LIGHT_SPOT_TEXTURE_FUNCTION()",LIGHT_SPOT_TEXTURE_FUNCTION());
	shader.Replace("LIGHT_SPOT_COLOR_PROTOTYPE()",LIGHT_SPOT_COLOR_PROTOTYPE());
	shader.Replace("LIGHT_SPOT_COLOR_FUNCTION()",LIGHT_SPOT_COLOR_FUNCTION());
	
	shader.Replace("LIGHT_DIR_STRUCT()",LIGHT_DIR_STRUCT());
	shader.Replace("LIGHT_DIR_TEXTURE_PROTOTYPE()",LIGHT_DIR_TEXTURE_PROTOTYPE());
	shader.Replace("LIGHT_DIR_TEXTURE_FUNCTION()",LIGHT_DIR_TEXTURE_FUNCTION());
	shader.Replace("LIGHT_DIR_COLOR_PROTOTYPE()",LIGHT_DIR_COLOR_PROTOTYPE());
	shader.Replace("LIGHT_DIR_COLOR_FUNCTION()",LIGHT_DIR_COLOR_FUNCTION());
	shader = Replace(shader,Upp::Vector<Upp::String>{"\""},Upp::Vector<Upp::String>{""});
	return shader;
}
/*
	Used to do custom texture loading. If you don't have some advanced texture usage to do, you
	will not have to be bothered with this
*/
struct TextureParameter{
	TextureParameter(){
		target=GL_TEXTURE_2D;
		pname=GL_TEXTURE_WRAP_S;
		params=GL_REPEAT;
	}
	TextureParameter(GLenum _target,GLenum _pname,int _params){
		target=_target;
		pname=_pname;
		params=_params;
	}
	TextureParameter(const TextureParameter& _textureParameter){
		target=_textureParameter.target;
		pname=_textureParameter.pname;
		params=_textureParameter.params;
	}
	TextureParameter& operator=(const TextureParameter& _textureParameter){
		target=_textureParameter.target;
		pname=_textureParameter.pname;
		params=_textureParameter.params;
		return *this;
	}
	GLenum target;
	GLenum pname;
	int params;
};
//Basic to throw exception
class UOGLException : public std::exception { //classe to managed every exception
	private:
	    int m_numero;               //Id of Error
	    Upp::String m_phrase;       //Error summaries
	    int m_niveau;               //level of Error
	    char* myChar=NULL;

	public:
	    UOGLException(int numero=0, Upp::String phrase="", int niveau=0){
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
		virtual ~UOGLException(){
			delete [] myChar;
		}
};
#endif
