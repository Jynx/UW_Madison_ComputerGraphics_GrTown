// CS559 Utility Code: Shader Tools
// a collection of useful routines for making use of Shaders
//
// written November 2008 by Michael Gleicher
// with code borrowed from the tutorial by Cody Robson
//
// Update 2014: change GLEE to GLEW
// warning: this code assumes that you're using GLEW

#define GLEW_STATIC 1

#include <GL/glew.h>

#include "ShaderTools.H"
#include "stdlib.h"
#include "stdio.h"

#include <fstream>
#include <sstream>

using std::ifstream;
using std::string;
using std::vector;
using std::stringstream;


vector<string> shaderPaths;

// read in a file once you have the file name - called from the real routine
// that searches in a path
static bool readShaderFileI(const char* filename, stringstream *program)
{

	ifstream inFile(filename);
	if (!inFile.good()) return false;

	*program << inFile.rdbuf();

	inFile.close();

	return true;
}

// look for a shader file in the shaderPaths (try no path first)
bool readShaderFile(const char* filename, stringstream *program)
{
	// try no path first
	if (readShaderFileI(filename, program)) return true;

	// otherwise, try all the paths
	for (vector<string>::iterator i = shaderPaths.begin(); i != shaderPaths.end(); ++i)
	{
		string fullpath = *i + "/" + filename;
		if (readShaderFileI(fullpath.c_str(), program)) return true;
	}
	return false;
}

// see if the shader compiled correctly
// if it did not, return a true value, and print an error
bool checkShaderError(GLuint shader)
{
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled)
		return false;
	else {
		GLint length;
		GLchar* log;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		log = (GLchar*)malloc(length + 1);
		glGetShaderInfoLog(shader, length, &length, log);
		printf("Shader Compilation Log:\n%s\n", log);
		free(log);
		return true;
	}
}

// see if the program linked correctly
// if it did not, return a true value, and print an error
bool checkProgramError(GLuint program)
{
	GLint compiled;
	glGetProgramiv(program, GL_LINK_STATUS, &compiled);
	if (compiled)
		return false;
	else {
		GLint length;
		GLchar* log;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		log = (GLchar*)malloc(length + 1);
		glGetProgramInfoLog(program, length, &length, log);
		printf("Shader Compilation Log:\n%s\n", log);
		free(log);
		return true;
	}
}

// for reasons I don't understand, this program barfs if there is
// a C++ style comment - they're easy enough to get rid of, so rather than
// sorting it out, here's something easier
void ridCPPcomments(vector<string>& prog)
{
	for (vector<string>::iterator i = prog.begin(); i != prog.end(); ++i) {
		string::size_type f = i->find("//");
		if (f != string::npos)
			i->erase(i->begin() + f, i->end());
	}
}

// load in and compile a vertex and fragment shader into a "program"
// again, this is taken from Cody's tutorial
GLuint loadShader(const char* vertexFileName, const char* fragmentFileName, char*& error)
{
	error = 0;
	glewInit();
	// create shader program object and both a vertex and fragment object
	GLuint	program = glCreateProgram();
	GLuint	vertex = glCreateShader(GL_VERTEX_SHADER_ARB);
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER_ARB);

	// read shaders into strings

	stringstream vertData;
	if (!readShaderFile(vertexFileName, &vertData))
	{
		error = "Couldn't open vertex shader file!";
		return 0;
	}

	stringstream fragData;
	if (!readShaderFile(fragmentFileName, &fragData))
	{
		error = "Could not open fragment shader file!";
		return 0;
	}


	// give ogl the shader source string

	const string &vertString = vertData.str();
	const char *vertShader_c = vertString.c_str();
	GLuint vertShaderLen = vertString.size();

	// compile shaders
	glShaderSource(vertex,
		1,
		(const GLchar**)&vertShader_c,
		(GLint*)&vertShaderLen);

	glCompileShader(vertex);
	if (checkShaderError(vertex)) {
		error = "Could not compile vertex program";

		printf("%s\n", vertShader_c);

		return 0;
	}

	const string &fragString = fragData.str();
	const char *fragShader_c = fragString.c_str();
	GLuint fragShaderLen = fragString.size();

	// compile shaders
	glShaderSource(fragment,
		1,
		(const GLchar**)&fragShader_c,
		(GLint*)&fragShaderLen);

	glCompileShader(fragment);
	if (checkShaderError(fragment)) {
		error = "Could not compile fragment program";

		printf("%s\n", fragShader_c);

		return 0;
	}


	// attach shaders to program object and link
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	if (checkProgramError(program)) {
		error = "Shaders failed to link!";
		return 0; // bail out, display isn't going to happen at this point.
	}

	// display results
	GLint success;
	glGetProgramiv(program, GL_OBJECT_LINK_STATUS_ARB, &success);
	if (!success)
	{
		error = "Shaders failed to compile and link!";
		return 0; // bail out, display isn't going to happen at this point.
	}
	else
	{
		printf("%s and %s loaded and linked successfully.\n", vertexFileName, fragmentFileName);
	}
	return program;
}