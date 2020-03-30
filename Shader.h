#ifndef _UltimateOpenGL_Shader_h_
#define _UltimateOpenGL_Shader_h_
#include "Definition.h"

class Material;
class Shader {
	private:
		struct shader_Data : Upp::Moveable<shader_Data>{
			shader_Data();
			shader_Data(Upp::String _name,ShaderType _type,Upp::String _shader);
			shader_Data& operator=(const shader_Data& data);
			
			Upp::String name;
			ShaderType type;
			Upp::String shader;
		};
	// utility function for checking shader compilation/linking errors.
		Upp::VectorMap<Upp::String,shader_Data> shaders;
		unsigned int ID;
		bool Compiled = false;
		bool AutoGenerated = false;
		bool CheckCompileErrors(unsigned int shader, ShaderType type);
		
		int LayoutPosition = -1;
		int InPosition = -1;
		int StructurePosition = -1;
		int OutPosition = -1;
		int UniformPosition = -1;
		int MainPosition = -1;
		int FunctionPrototypePosition = -1;
		int FunctionDeclarationPosition = -1;
		
		
		void IncreasePositionVariable(int position,int lenght);
		int InsertInShader(Upp::String& shader, int position,const Upp::String& s);
		int InsertIfNotFind(Upp::String& shader,int position,const Upp::String& s); //return -1 if not inserted
		int RemoveInsertInShader(Upp::String& shader, const Upp::String& remove,const Upp::String& s, int position =0);
	public:
	    Shader();
	    Shader(Upp::String _name,ShaderType _type,Upp::String _shader);
	    Shader(const Shader& _shader);
	    Shader& operator=(const Shader& data);
		~Shader();
	
	    Shader& AddShader(Upp::String _name,ShaderType _type,Upp::String _shader);
	    Upp::VectorMap<Upp::String,Shader::shader_Data>& GetShaders();
	
	    bool IsCompiled() const;
	    bool IsAutoGenerated() const;
	    unsigned int GetId() const;
	    
	
	    Shader& SetBool(Upp::String name, bool value);
	    Shader& SetInt(Upp::String name, int value);
	    Shader& SetFloat(Upp::String name, float value);
	
		Shader& SetVec2(Upp::String name, const glm::vec2 &value);
		Shader& SetVec2(Upp::String name, float x, float y);
		Shader& SetVec3(Upp::String name, const glm::vec3 &value);
		Shader& SetVec3(Upp::String name, float x, float y, float z);
		Shader& SetVec4(Upp::String name, const glm::vec4 &value);
		Shader& SetVec4(Upp::String name, float x, float y, float z, float w);
		Shader& SetMat2(Upp::String name, const glm::mat2 &mat);
		Shader& SetMat3(Upp::String name, const glm::mat3 &mat);
		Shader& SetMat4(Upp::String name, const glm::mat4 &mat);
	
		bool Load(bool autoGenerated =false);
		bool Reload(bool autoGenerated =false);
		bool Unload();
		
		bool AssignSimpleShader(); //Assign a simple shader wich just get model matrix and view matrix
		bool AssignSimpleShaderMaterial(const Material* material); //Craete and assign a simple shader wich can show texture
		bool AssignSimpleShaderMaterialLights(const Upp::ArrayMap<Upp::String, Light>& AllSceneLights,const Material* material);
		
		Shader& Use();
		Shader& Unbind();
		
		enum VertexGenerationOption{
			VGO_DoBasics = 0x1,
			
			VGO_UseMaterialObject = 0x2,
			
			VGO_CustomLayout = 0x4,
			VGO_CustomOut = 0x8,
			VGO_CustomUniform = 0x10,
			
			VGO_CustomStructure = 0x20,
			VGO_CustomFunction = 0x40,
			
			VGO_CustomMain = 0x80
		};
		Upp::String GenerateVertexShader(unsigned int VertexGenerationOption,const Material* material = nullptr,const Upp::Vector<Upp::String>* Layout = nullptr,const Upp::Vector<Upp::String>* Out = nullptr,const Upp::Vector<Upp::String>* Uniform = nullptr,const Upp::Vector<Upp::String>* Structure = nullptr,const Upp::Vector<Upp::String>* Function = nullptr,const Upp::String* Main = nullptr);
		
		enum FragmentGenerationOption{
			FGO_DoBasics = 0x1,
			
			FGO_UseMaterialObject = 0x10,
			FGO_LoadLight = 0x20,
			
			FGO_CustomIn = 0x2,
			FGO_CustomOut = 0x4,
			FGO_CustomUniform = 0x8,
			
			FGO_CustomStructure = 0x80,
			FGO_CustomFunction = 0x100,
			
			FGO_CustomMain = 0x200
		};
		Upp::String GenerateFragmentShader(unsigned int FragmentGenerationOption,const Material* material = nullptr,const Upp::ArrayMap<Upp::String, Light>* AllSceneLights = nullptr,const Upp::Vector<Upp::String>* In = nullptr,const Upp::Vector<Upp::String>* Out = nullptr,const Upp::Vector<Upp::String>* Uniform = nullptr,const Upp::Vector<Upp::String>* Structure = nullptr,const Upp::Vector<Upp::String>* Function = nullptr,const Upp::String* Main = nullptr);
		
		
};
#endif
