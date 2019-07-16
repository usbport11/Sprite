#include "stdafx.h"
#include "classes/system/Shader.h"
#include "classes/system/Scene.h"
#include "classes/buffers/StaticBuffer.h"
#include "classes/image/SpriteLoader.h"

using namespace std;

double DeltaTime = 0;
bool Pause;
bool keys[1024] = {0};
int WindowWidth = 800, WindowHeight = 600;
bool EnableVsync = 1;
GLFWwindow* window;

glm::vec2 MouseSceneCoord;

MShader Shader;
MScene Scene;
MStaticBuffer StaticBuffer;
MSpriteLoader SpriteLoader;
MSprite* Sprite = NULL;

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void mousepos_callback(GLFWwindow* window, double x, double y) {
	MouseSceneCoord = Scene.WindowPosToWorldPos(x, y);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		keys[key] = 1;
	}
	else if(action == GLFW_RELEASE) {
		keys[key] = 0;
	}
}

bool InitApp() {
	LogFile<<"Starting application"<<endl;    
    glfwSetErrorCallback(error_callback);
    
    if(!glfwInit()) return false;
    window = glfwCreateWindow(WindowWidth, WindowHeight, "TestApp", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return false;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mousepos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwMakeContextCurrent(window);
    if(glfwExtensionSupported("WGL_EXT_swap_control")) {
    	LogFile<<"Window: V-Sync supported. V-Sync: "<<EnableVsync<<endl;
		glfwSwapInterval(EnableVsync);//0 - disable, 1 - enable
	}
	else LogFile<<"Window: V-Sync not supported"<<endl;
    LogFile<<"Window created: width: "<<WindowWidth<<" height: "<<WindowHeight<<endl;

	//glew
	GLenum Error = glewInit();
	if(GLEW_OK != Error) {
		LogFile<<"Window: GLEW Loader error: "<<glewGetErrorString(Error)<<endl;
		return false;
	}
	LogFile<<"GLEW initialized"<<endl;
	
	if(!CheckOpenglSupport()) return false;

	//shaders
	if(!Shader.CreateShaderProgram("shaders/main.vertexshader.glsl", "shaders/main.fragmentshader.glsl")) return false;
	if(!Shader.AddUnifrom("MVP", "MVP")) return false;
	LogFile<<"Shaders loaded"<<endl;

	//scene
	if(!Scene.Initialize(&WindowWidth, &WindowHeight)) return false;
	LogFile<<"Scene initialized"<<endl;

	//randomize
    srand((unsigned int)time(NULL));
    LogFile<<"Randomized"<<endl;
    
	//other initializations
	if (!SpriteLoader.Initialize()) {
		LogFile << "SpriteLoader initialize failed" << endl;
		return false;
	}
	SpriteLoader.SetRemoveColor(glm::vec4(1, 1, 1, 1));
	Sprite = SpriteLoader.GetSprite("textures/tex05.png");
	if (!Sprite) {
		LogFile << "Sprite loding failed" << endl;
		return false;
	}
	SpriteLoader.Close();

	if(!StaticBuffer.Initialize()) return false;
	StaticBuffer.SetPrimitiveType(GL_POINTS);
	StaticBuffer.AddVertexArray(Sprite->GetPoints(), Sprite->GetColors());
	if(!StaticBuffer.Dispose()) return false;
	
	//turn off pause
	Pause = false;
    
    return true;
}

void UpdateStep() {
}

void RenderStep() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(Shader.ProgramId);
	glUniformMatrix4fv(Shader.Uniforms["MVP"], 1, GL_FALSE, Scene.GetDynamicMVP());
	StaticBuffer.Begin();
		StaticBuffer.Draw();
	StaticBuffer.End();
}

void KeysProcessing() {
	if(keys[GLFW_KEY_ESCAPE]) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		return;
	}
}

void ClearApp() {
	if (Sprite) {
		Sprite->Clear();
		delete Sprite;
	}
	StaticBuffer.Close();

	memset(keys, 0, 1024);
	Shader.Close();
	LogFile<<"Application: closed"<<endl;
}

int main(int argc, char** argv) {
	const double LimitFPS = (double) 1.0 / 60.0;
	int Frames = 0;
	int Updates = 0;
	double LastTime = 0, NowTime = 0;
	double RenderTimer = 0;
	
	LogFile<<"Application: started"<<endl;
	if(!InitApp()) {
		ClearApp();
		glfwTerminate();
		LogFile.close();
		return 0;
	}
	
	while(!glfwWindowShouldClose(window)) {
		NowTime = glfwGetTime();
		DeltaTime = NowTime - LastTime;
		RenderTimer += DeltaTime;
		LastTime = NowTime;
		
		if(!Pause) UpdateStep();
		if(RenderTimer >= LimitFPS) {
			RenderStep();
			RenderTimer -= LimitFPS;
		}
		
        glfwSwapBuffers(window);
        glfwPollEvents();
        KeysProcessing();
	}
	ClearApp();
    glfwTerminate();
    LogFile.close();
}
