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
#include <plugin/glm/ext/quaternion_trigonometric.hpp>
#include <plugin/glm/gtx/norm.hpp>
#include <plugin/glm/gtx/string_cast.hpp>
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

enum LightType{LT_DIRLIGHT,LT_POINTLIGHT,LT_SPOTLIGHT,LT_LIGHT};

enum CameraType{CT_PERSPECTIVE,CT_ORTHOGRAPHIC,CT_FRUSTUM}; //Type of camera rendering
/*
	Definition of all defaults class used in UOGL
*/
class UltimateOpenGL_Context;

struct TextureInformation;
struct Vertex;

class Camera;
class Material;
class Transform;
class Scene;
class Light;
class DirLight;
class PointLight;
class SpotLight;
class GameObject;
class Object3D;
class Mesh;

//typedef int TRANSLATION_TYPE;
typedef void (*GAMEOBJECT_FUNCTION)(GameObject& myObject);
typedef void (*CAMERA_FUNCTION)(Camera& myCamera);
typedef void (*LIGHT_FUNCTION)(Light& myLight);

//Both structures is used to read Vector of float used to show multiple shape
enum ColorFormat{CF_FLOAT,CF_INTEGER};
enum ColorType{CT_RGBA,CT_RGB};
struct ReaderParameters{
	int coordinatesPosition = -1; //Define position of coordinates in vector of float
	int normalPosition = -1; //Define position of normal in vector of float
	int textureCoordinatePosition = -1; //Define position of texture coordinate in vector of float
	int tangentPosition = -1; //Define position of tangent position in vector of float
	int bitangentPosition = -1; //Define position of bitangent position in vector of float
	int colorPosition = -1; //Define position of color in vector of float
	ColorFormat colorFormat = CF_INTEGER; //Define format of Color
	ColorType colorType = CT_RGB;//Define type of color(RGB = 3 Float or Int ||RGBA = 4 Float or int)
	ReaderParameters(int CoordinatesPosition=0,int NormalPosition=1,int TextureCoordinatePosition=2,int TangentPosition=-1,int BitangentPosition=-1, int ColorPosition=-1,ColorFormat _ColorFormat=CF_INTEGER,ColorType _ColorType=CT_RGB){
		coordinatesPosition=CoordinatesPosition;
		normalPosition=NormalPosition;
		textureCoordinatePosition=TextureCoordinatePosition;
		tangentPosition=TangentPosition;
		bitangentPosition=BitangentPosition;
		colorPosition=ColorPosition;
		colorFormat =_ColorFormat;
		colorType = _ColorType;
	}
	ReaderParameters& CoordinatesPosition(int CoordinatesPosition){coordinatesPosition = CoordinatesPosition;return *this;}
	ReaderParameters& NormalPosition(int NormalPosition){normalPosition = NormalPosition;return *this;}
	ReaderParameters& TextureCoordinatePosition(int TextureCoordinatePosition){textureCoordinatePosition = TextureCoordinatePosition;return *this;}
	ReaderParameters& TangentPosition(int TangentPosition){tangentPosition = TangentPosition;return *this;}
	ReaderParameters& BitangentPosition(int BitangentPosition){bitangentPosition = BitangentPosition;return *this;}
	ReaderParameters& ColorPosition(int ColorPosition){colorPosition = ColorPosition;return *this;}
	ReaderParameters& ColorFormat(ColorFormat _ColorFormat){colorFormat = _ColorFormat;return *this;}
	ReaderParameters& ColorType(ColorType _ColorType){colorType = _ColorType;return *this;}
};
struct ReaderRoutine{
	int verticesPerMesh = -1; //Define how many line mesh should  carry
	int startMesh = -1;	 //Define where readData should start is completion of Mesh
	bool allowCreation = true; //Define if creation of mesh is allowed in the object3D
	bool useMaterialColor = false; //Define if ReaderRoutine should force the usage of Material color instead of color retrieve in Vector<float>(only if ColorPosition have been set)
	ReaderRoutine(int VerticesPerMesh=-1, int StartMesh=-1, bool UseMaterialColor = false, bool AllowCreation=true){
		verticesPerMesh=VerticesPerMesh;
		startMesh=StartMesh;
		allowCreation=AllowCreation;
		useMaterialColor = UseMaterialColor;
	}
	ReaderRoutine& VerticesPerMesh(int VerticesPerMesh){verticesPerMesh = VerticesPerMesh; return *this;}
	ReaderRoutine& StartMesh(int StartMesh){startMesh = StartMesh; return *this;}
	ReaderRoutine& AllowCreation(bool AllowCreation){allowCreation = AllowCreation; return *this;}
	ReaderRoutine& UseMaterialColor(bool UseMaterialColor){useMaterialColor = UseMaterialColor;return *this;}
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
	},
	{"Simple_Vertex_Shader",
	Replace(
		#include "Simps/SimpleVertex.glsl"
		,Upp::Vector<Upp::String>{R"(@)"},Upp::Vector<Upp::String>{"//"})
	},
	{"Simple_Fragment_Shader",
	Replace(
		#include "Simps/SimpleFragment.glsl"
		,Upp::Vector<Upp::String>{R"(@)"},Upp::Vector<Upp::String>{"//"})
	}
};

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

struct Vertex : public Upp::Moveable<Vertex> {
    // position
    glm::vec3 Position=glm::vec3(0.0,0.0,0.0);
    // normal
    glm::vec3 Normal=glm::vec3(0.0,0.0,0.0);
    // texCoords
    glm::vec2 TexCoords=glm::vec2(0.0,0.0);
    // tangent
    glm::vec3 Tangent=glm::vec3(0.0,0.0,0.0);
    // bitangent
    glm::vec3 Bitangent=glm::vec3(0.0,0.0,0.0);
    //Color
    glm::vec4 Color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    //bool SaveColor =false;
    
    Vertex(){}
    Vertex& SetPosition(glm::vec3 _position){
        Position = _position;
        return *this;
    }
    Vertex& SetNormal(glm::vec3 _normal){
        Normal = _normal;
        return *this;
    }
    Vertex& SetTexCoords(glm::vec2 _texCoords){
        TexCoords = _texCoords;
        return *this;
    }
    Vertex& SetTangent(glm::vec3 _tangent){
        Tangent = _tangent;
        return *this;
    }
    Vertex& SetBitangent(glm::vec3 _bitangent){
        Bitangent = _bitangent;
        return *this;
    }
    Vertex& SetColor(glm::vec4 _Color){
        Color = _Color;
        return *this;
    }
    Vertex& SetColor(glm::vec3 _Color){
		Color = glm::vec4(_Color.x,_Color.y,_Color.z,1.0f);
		return *this;
    }
    Vertex(const Vertex& v){
		Position = v.Position;
		Normal = v.Normal;
		TexCoords = v.TexCoords;
		Tangent = v.Tangent;
		Bitangent = v.Bitangent;
		Color = v.Color;
    }
};

static void PrintVertex(Vertex& v){
	LOG("Position: " + Upp::String( glm::to_string( v.Position))
	+ " Normal: " +Upp::String( glm::to_string(v.Normal))
	+ " TextureCoords: " + Upp::String(glm::to_string(v.TexCoords))
	+ " Color: " + Upp::String(glm::to_string(v.Color)));
}

//****************Static part **************************
/*
	This part give some basic function to do
	convertion of String path or Color transformation
*/
static Upp::String TransformFilePath(const Upp::String& FilePath){
	Upp::String FilePathBuffer =FilePath;

	FilePathBuffer = Replace(FilePathBuffer,Upp::Vector<Upp::String>{"\\"},Upp::Vector<Upp::String>{"/"});

	return FilePathBuffer;
}
static int ColorUniformisation(int ColorRgb){ //Fonction très utile et très complexe
	if(ColorRgb> 255) ColorRgb=255;
	if(ColorRgb< 0) ColorRgb=0;
	return ColorRgb;
}
static float ColorUniformisation(float ColorFloat){
	if(ColorFloat> 1.0f) ColorFloat=1.0f;
	if(ColorFloat< 0.0f) ColorFloat=0.0f;
	return ColorFloat;
}
static Upp::Vector<int> TransformGlmColorToRGB(glm::vec3 FloatColor){
	return Upp::Vector<int>{ ColorUniformisation((int)(FloatColor.x*255)),ColorUniformisation((int)(FloatColor.y*255)), ColorUniformisation((int)(FloatColor.z*255)) };
}
static Upp::Vector<int> TransformFloatColorToRGB(float RedFloat,float GreenFloat,float BlueFloat){
	return Upp::Vector<int>{ ColorUniformisation((int)(RedFloat*255)),ColorUniformisation((int)(GreenFloat*255)), ColorUniformisation((int)(BlueFloat*255))};
}
static glm::vec3 TransformRGBToFloatColor(int Red,int Green,int Blue){
	return  glm::vec3( ((float)ColorUniformisation(Red))/255.0f ,((float)ColorUniformisation(Green))/255.0f,((float)ColorUniformisation(Blue))/255.0f);
}
static glm::vec3 TransformVectorToFloatColor(Upp::Vector<int> rgb){
	if(rgb.GetCount() < 3){
		LOG("Erreur TransformRGBToFloatColor(Vector<int>) : Vector incorrect !\n");
		return  glm::vec3(1.0);
	}
	return glm::vec3(((float)ColorUniformisation(rgb[0]))/255.0f ,((float)ColorUniformisation(rgb[1]))/255.0f,((float)ColorUniformisation(rgb[2]))/255.0f);
}
static glm::vec4 TransformRGBAToFloatColor(int Red,int Green,int Blue,int alpha){
	return  glm::vec4(((float)ColorUniformisation(Red))/255.0f ,((float)ColorUniformisation(Green))/255.0f,((float)ColorUniformisation(Blue))/255.0f,((float)ColorUniformisation(alpha))/255.0f);
}
static glm::vec4 TransformVectorToFloatColorAlpha(Upp::Vector<int> rgba){
	if(rgba.GetCount() < 4){
		LOG("Erreur TransformRGBToFloatColor(Vector<int>) : Vector incorrect !\n");
		return  glm::vec4(1.0);
	}
	return glm::vec4(((float)ColorUniformisation(rgba[0]))/255.0f ,((float)ColorUniformisation(rgba[1]))/255.0f,((float)ColorUniformisation(rgba[2]))/255.0f,((float)ColorUniformisation(rgba[3]))/255.0f);
}
#endif
