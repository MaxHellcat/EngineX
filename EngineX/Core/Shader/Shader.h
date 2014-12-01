//
//  Shader.h
//  EngineX
//
//  Created by Maxim Reshetey on 20.11.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

#ifndef __EngineX__Shader__
#define __EngineX__Shader__

#include <OpenGLES/ES2/gl.h> // For types

#include "map"
#include "string" // TODO: Remove later, only for bundle path trick

#include "common.h"

//	typedef std::pair<std::string, GLuint> Pair;
//	typedef std::map<std::string, GLuint> Map;

//typedef std::map<std::string, GLbyte> Uniforms;
//typedef std::pair<std::string, GLbyte> UniPair;

typedef enum
{
	eVertexAttrPosition = 0,
	eVertexAttrTexPosition = 1,
	eNumVertexAttribs
} eVertexAttributes;

typedef enum
{
	uniModelViewProjectionMatrix = 0,
//	uniNormalMatrix,
	uniColor,
	eNumUniforms
} eShaderUniforms;


// Multiton
class Shader
{
	typedef std::pair<std::string, Shader *> Pair;
	typedef std::map<std::string, Shader *> ShaderMap;

	Shader(const Shader &) = delete; // Copying not allowed
	Shader & operator=(const Shader &) = delete; // Assigning not allowed

	Shader() : _program(0), _vShader(0), _fShader(0), _name("") {}
	~Shader()
	{
		// Delete program, shaders are already deleted whilst creating
		if (_program)
		{
			glDeleteProgram(_program);
			_program = 0;

			info("Shader \"%s\" deleted\n", _name.c_str());
		}
		else
		{
			err("Failed to delete shader \"%s\"\n", _name.c_str());
		}
	}

public: // Methods
	void use() const
	{
		// Conditional use is as >4 times as fast
		GLint pid = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &pid); // Approx 5ms, iPhone4
		if (pid != _program)
		{
			glUseProgram(_program); // Approx 22ms!, iPhone4
		}
	}

	GLint uniform(short index) const { return uniforms[index]; }

	// Here we assume shader Must have been created already, no lazy creation (as it's non-trivial flow)
	// The slowest place in Shader logic, ref is looked up by string search
	static Shader & ref(const char * shaderName)
	{
		info("Searching shader \"%s\" in ShaderMap... ", shaderName);

		ShaderMap::iterator it = _map.find(shaderName);

		if (it != _map.end())
		{
			say("FOUND\n");
			return *it->second;
        }

		// This comment saved me one debugging day
		say("NOT FOUND, returning empty object! (this generally means you forgot to create this shader earlier)\n");

		// TODO: We leak here!
		return *(new Shader());
	}

	// TODO: I don't like this, we'd better use retain/release approach, i.e. the one who
	// obtained (retained) shader ref, must finally release it. Thus, unused shader will get
	// deleted when ref count becomes zero
	static void deinit()
	{
		// Remove kids
//		for (ShaderMap::iterator it=_map.begin(); it!=_map.end(); ++it)
		for (auto & pair : _map)
		{
			delete pair.second; pair.second = 0;
//			delete it->second; it->second = 0;
		}
		_map.clear();

		info("Shader singleton de-initialized\n");
	}

	// Compile shaders and create shader program
	static Shader & createWithNameAndAttributeCount(const char * shaderName, short attrCount);

	// Link shader program, set uniforms (inc. tex samplers) and add self into static shaders map
	void finalizeWithTextureUnitCount(short unitsCount = 0)
	{
		// Link shader program
		if (link() == false)
		{
			if (_vShader)
			{
				glDeleteShader(_vShader);
				_vShader = 0;
			}
			if (_fShader)
			{
				glDeleteShader(_fShader);
				_fShader = 0;
			}
			if (_program)
			{
				glDeleteProgram(_program);
				_program = 0;
			}
//			return false;
		}

		// Finally release shaders as shader program owns copies (or retains refs)
		if (_vShader)
		{
			glDetachShader(_program, _vShader);
			glDeleteShader(_vShader);
			_vShader = 0;
		}
		if (_fShader)
		{
			glDetachShader(_program, _fShader);
			glDeleteShader(_fShader);
			_fShader = 0;
		}

		// Setup uniforms
		// TODO: Good old question - how do we efficiently sort out different set of uniforms for different shader?
		// Hint: The actual locations assigned to uniform variables are not known until the program object is linked successfully.
		uniforms[uniModelViewProjectionMatrix] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
	//	uniforms[uniNormalMatrix] = glGetUniformLocation(_program, "normalMatrix");
		uniforms[uniColor] = glGetUniformLocation(_program, "uniColor");

		// For shaders that work with textures only
		for (short i=0; i<unitsCount; ++i)
		{
			// TODO: Construct proper texUnitN string, when time comes to use multi-texturing
			glUniform1i(glGetUniformLocation(_program, "texUnit0"), i);
		}

		_map.insert(Pair(_name, this));

#ifdef DEBUG
		// Validate created shader program
		validate();
#endif
		
		info("Added shader \"%s\"\n", _name.c_str());
	}

	// TODO: Temporary solution
	static void setBundlePath(const char * path) { _bundlePath = path; }

private: // Methods
	bool compile(GLuint & shader, GLenum type, const char * source);
	bool link();
	bool validate();

private: // Vars
	GLuint _program, _vShader, _fShader;
	GLint uniforms[eNumUniforms];

	std::string _name; // Shader filename excluding extension

	static ShaderMap _map; // Map of all shaders loaded so far

	// TODO: Temporary solution
	static std::string _bundlePath;
};

#endif /* defined(__EngineX__Shader__) */
