#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <vector>
#include <iostream>
class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	GLuint getProgram() const;
	bool loadShaders(const char* vertexFile, const char* fragmentFile);
private:
	GLuint program;
	GLuint loadShader(const char* file, GLenum shadeType);
};

