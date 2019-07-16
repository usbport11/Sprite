#include "../../stdafx.h"
#include "Shader.h"

MShader::MShader()
{
	VertexArrayId = 0;
	ProgramId = 0;

	VertexShaderFileName = 0;
	FragmentShaderFileName = 0;
	VertexShaderFileName = NULL;
	FragmentShaderFileName = NULL;
}

MShader::~MShader()
{
	VertexArrayId = 0;
	ProgramId = 0;
	
	VertexShaderFileName = 0;
	FragmentShaderFileName = 0;
	VertexShaderFileName = NULL;
	FragmentShaderFileName = NULL;
}
	
GLuint MShader::CreateShaderProgram(const char* inVertexShaderFileName, const char* inFragmentShaderFileName)
{
	if(!inVertexShaderFileName || !inFragmentShaderFileName)
	{
		LogFile<<"Shader: Some of shader file name is empty"<<std::endl;
		return 0;
	}
	
	//vertex array
	glGenVertexArrays(1, &VertexArrayId);
	glBindVertexArray(VertexArrayId);
	GLenum Error = glGetError();
	if(Error != GL_NO_ERROR)
	{
		LogFile<<"Game2: "<<(char*)gluErrorString(Error)<<" "<<Error<<std::endl;
		return false;
	}

	VertexShaderFileName = (char*)inVertexShaderFileName;
	FragmentShaderFileName = (char*)inFragmentShaderFileName;
	
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(VertexShaderFileName, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		LogFile<<"Shader(vertex): Can not open shader file"<<std::endl;
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(FragmentShaderFileName, std::ios::in);
	if(FragmentShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	LogFile<<"Shader(vertex): Compiling shader"<<std::endl;
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		LogFile<<"Shader(vertex): Vertex: "<<&VertexShaderErrorMessage[0]<<std::endl;
	}

	// Compile Fragment Shader
	LogFile<<"Shader(fragment): Compiling shader"<<std::endl;
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		LogFile<<"Shader: Fragment: "<<&FragmentShaderErrorMessage[0]<<std::endl;
	}
		
	// Link the program
	LogFile<<"Shader: Linking program"<<std::endl;
	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderID);
	glAttachShader(ProgramId, FragmentShaderID);
	glLinkProgram(ProgramId);

	// Check the program
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		LogFile<<&ProgramErrorMessage[0]<<std::endl;
	}

	glDetachShader(ProgramId, VertexShaderID);
	glDetachShader(ProgramId, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	
	VertexShaderCode.clear();
	FragmentShaderCode.clear();
	
	return ProgramId;
}


bool MShader::AddUnifrom(const char* Name, const char* SearchName) {
	if(!ProgramId) {
		LogFile<<"Shader: Program not loaded"<<std::endl;
		return false;
	}
	if(!Name || !SearchName) {
		LogFile<<"Shader: Uniform name or uniform search name is not set: "<<SearchName<<std::endl;
		return false;
	}
	std::map<const char*, GLuint, stStringCompare>::iterator it;
	it = Uniforms.find(Name);
	if(it == Uniforms.end()) {
		GLuint Id = glGetUniformLocation(ProgramId, SearchName);
		if(Id == -1) {
			LogFile<<"Shader: Can't get uniform: "<<SearchName<<std::endl;
			return false;
		}
		Uniforms.insert(std::pair<const char*, GLuint>(Name, Id));
	}
	else LogFile<<"Shader: Uniform alredy loaded"<<std::endl;
	
	return true;
}
	
void MShader::Close()
{
	Uniforms.clear();
	glDeleteProgram(ProgramId);
	glDeleteVertexArrays(1, &VertexArrayId);
}
