#ifndef stdafxH
#define stdafxH

//opengl
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gl/gl.h>
#include <gl/glu.h>
//dos
#include <time.h>
#include <stdio.h>
#include <math.h>
//strings
#include <string>
//stl
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//global log file variable - in stdafx.cpp
extern std::ofstream LogFile;

//help functions
bool IsPowerOfTwo(unsigned long x);
bool CheckOpenglSupport();

//IMPORTANT!
#define SHR_LAYOUT_VERTEX 0
#define SHR_LAYOUT_COLOR 1

#endif
