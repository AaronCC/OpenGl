#include "ShaderManager.h"
#include <fstream>
ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}

GLuint ShaderManager::getProgram() const
{
	return program;
}

GLuint ShaderManager::loadShader(const char* file, GLenum shadeType)
{
	std::ifstream shFile;
	shFile.open(file, std::ios::binary);
	int length;
	if (shFile.is_open())
	{
		shFile.seekg(0, std::ios::end);
		length = (int)shFile.tellg();
		shFile.seekg(0, std::ios::beg);
	}

	char * code = new char[length + 1];
	shFile.read(code, length);
	code[length] = 0;

	shFile.close();
	GLchar const * files[] = { code };
	GLuint shader;
	shader = glCreateShader(shadeType);
	glShaderSource(shader, 1, files, 0);
	glCompileShader(shader);
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result != 0)
		return shader;
	else
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		GLchar log[1000];
		glGetShaderInfoLog(shader, length, 0, log);
		std::cout << log;

	}
	glDeleteShader(shader);
	return 0;
}

bool ShaderManager::loadShaders(const char* vertexFile, const char* fragmentFile)
{
	GLuint vIndex, fIndex;

	vIndex = loadShader(vertexFile, GL_VERTEX_SHADER);
	if(vIndex == 0)
		return false;
	fIndex = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (fIndex == 0)
		return false;

	program = glCreateProgram();

	glAttachShader(program, vIndex);
	glAttachShader(program, fIndex);

	glLinkProgram(program);

	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus != 0)
		return true;
	else
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar log[1000];
		glGetProgramInfoLog(program, length, 0, log);
		std::cout << log;
	}
	return false;
}
