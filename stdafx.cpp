#include "stdafx.h"

std::ofstream LogFile("log.txt");

bool IsPowerOfTwo(unsigned long x) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

bool CheckOpenglSupport() {
	//get opengl data (here was CRASH on suspisious notebook)
	std::string OpenGLVersion = (char*)glGetString(GL_VERSION);
	std::string OpenGLVendor = (char*)glGetString(GL_VENDOR);
	std::string OpenGLRenderer = (char*)glGetString(GL_RENDERER);
	std::string ShadersVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	LogFile<<"Window: OpenGL version: "<<OpenGLVersion.c_str()<<std::endl;
	LogFile<<"Window: OpenGL vendor: "<<OpenGLVendor.c_str() <<std::endl;
	LogFile<<"Window: OpenGL renderer: "<<OpenGLRenderer.c_str()<<std::endl;
	LogFile<<"Window: Shaders version: "<<ShadersVersion.c_str()<<std::endl;
	
	float VersionOGL, VersionSHD;
	sscanf_s(OpenGLVersion.c_str(), "%f", &VersionOGL);
	if(VersionOGL < 3.0f) {
		LogFile<<"Window: Old version of OpenGL. Sorry"<<std::endl;
		return false;
	}
	sscanf_s(ShadersVersion.c_str(), "%f", &VersionSHD);
	if(VersionSHD < 3.3f) {
		LogFile<<"Window: Old version of shaders. Sorry"<<std::endl;
		return false;
	}
	
	return true;
}
