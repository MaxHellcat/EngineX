//
//  Shader.cpp
//  EngineX
//
//  Created by Maxim Reshetey on 20.11.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

#include "Shader.h"
#include <fstream>
#include <sstream>

static const char * AttrNames[] =
{
	"attrPosition",
//	"attrNormalCoord",
	"attrPositionTex",
//	"attrTanCoord"
};

Shader::ShaderMap Shader::_map; // Init static map
std::string Shader::_bundlePath;

// TODO: Passing say 2 to attrCount is bogus, use last enum attr or smth smarter
Shader & Shader::createWithNameAndAttributeCount(const char * shaderName, short attrCount)
{
	std::string vShader = _bundlePath + '/' + shaderName + ".vsh"; // Shader full path
	std::string fShader = _bundlePath + '/' + shaderName + ".fsh";

	// Read shaders into strings
	std::ifstream vsf(vShader), fsf(fShader);
	std::string vs, fs;

	vsf.seekg(0, std::ios::end);
	vs.reserve(vsf.tellg());
	vsf.seekg(0, std::ios::beg);
	vs.assign((std::istreambuf_iterator<char>(vsf)), std::istreambuf_iterator<char>());

	fsf.seekg(0, std::ios::end);
	fs.reserve(vsf.tellg());
	fsf.seekg(0, std::ios::beg);
	fs.assign((std::istreambuf_iterator<char>(fsf)), std::istreambuf_iterator<char>());

//	TODO: Why not allocate in stack, its faster and safer
	Shader * shader = new Shader();

	shader->_name = shaderName;

	// Compile shaders
	if (shader->compile(shader->_vShader, GL_VERTEX_SHADER, vs.c_str()) == false)
	{
		glDeleteShader(shader->_vShader); shader->_vShader = 0;
//		return false;
	}

	if (shader->compile(shader->_fShader, GL_FRAGMENT_SHADER, fs.c_str()) == false)
	{
		glDeleteShader(shader->_fShader); shader->_fShader = 0;
//		return false;
	}

	// Create shader program and attach compiled shaders
	// Hint: This function returns 0 if an error occurs creating the program object.
	shader->_program = glCreateProgram();
	if (shader->_program == 0)
	{
//		return false;
	}

	glAttachShader(shader->_program, shader->_vShader);
	glAttachShader(shader->_program, shader->_fShader);

	// Bind attribute locations, this must be done prior to linking

	// TODO: Assert attributesCount <= eNumVertexAttribs
	// The approach I've taken is based on the assumption, that:
	// - Attributes must be sequential in order, i.e. if shader has texCoord, it must have position
	// - The order is: position, texCoord, TBD
	for (short i=0; i<attrCount; ++i) { glBindAttribLocation(shader->_program, i, AttrNames[i]); }

	return *shader;
}

// TODO: glShaderBinary
bool Shader::compile(GLuint & shader, GLenum type, const char * source)
{
	shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, NULL);

	glCompileShader(shader);

#if defined(DEBUG)
    GLint logLength = 1;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
	{
        GLchar * log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
		printf("Shader compile log:\n%s", log);
        free(log);
    }
#endif

	GLint status = GL_FALSE;

	// Hint: Returns GL_TRUE if the last compile operation on shader was successful, and GL_FALSE otherwise.
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	// TODO: Careful, should be used after the very last shader compilation
//	glReleaseShaderCompiler();

//	info("Shader compile status: %s\n", status?"TRUE":"FALSE");

	return status;
}

bool Shader::link()
{
	glLinkProgram(_program);

#if defined(DEBUG)
	GLint logLength = 1;
	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
	{
		GLchar * log = (GLchar *)malloc(logLength);
		glGetProgramInfoLog(_program, logLength, &logLength, log);
		printf("Program link log: %s", log);
		free(log);
    }
	else
	{
		info("Shader program linked OK\n");
	}
#endif

	GLint status = GL_FALSE;

	// Hint: Returns GL_TRUE if the last link operation on program was successful, and GL_FALSE otherwise.
	glGetProgramiv(_program, GL_LINK_STATUS, &status);

	return status;
}

// Supposed to be called in debug mode
bool Shader::validate()
{
	GLint logLength = 1;

    // Hint: If validation is successful, program is guaranteed to execute given the current state.
	// Otherwise, program is guaranteed to not execute.
	glValidateProgram(_program);

    glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
	{
        GLchar * log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(_program, logLength, &logLength, log);
        info("Program %s", log); // The format is proper here
        free(log);
    }
	else
	{
		info("Shader program validation PASSED");
	}

	GLint status = GL_FALSE;

	// Hint: Returns GL_TRUE if the last validation operation on program was successful, and GL_FALSE otherwise.
	glGetProgramiv(_program, GL_VALIDATE_STATUS, &status);

	return status;
}
