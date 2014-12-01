 //
//  Model.h
//  EngineX
//
//  Created by Maxim Reshetey on 22.11.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

#ifndef __EngineX__Model__
#define __EngineX__Model__

#include "MatrixStack.h"
#include "Shader.h"

#include "list" // std::list of Models

#include <OpenGLES/ES2/gl.h> // For GL types
#include <OpenGLES/ES2/glext.h>


// Client vertex data
// Each Model class has own vertex data so instances can be cloned
typedef
struct
{
	float * vertices;
	int vertexCount;
} VertexData;


/*
//
// Tree of EngineX inheritance
//
           Model
		  /  |  \
		 /   |   \
	 Shape   |   Sprite
	   |     |     |
       |   ModelX  |
       |   /   \   |
       |  /     \  |
      ShapeX   SpriteX
*/

// Abstract class representing one scene object
// Base class for all other model kinds
// Use it only as prototype
class Model
{
	friend class EXScene; // So EXScene can access private draw()

protected: // Ctor
	Model() : // TODO: Rearrange according to declaration order
	_vertexArray(0),
	_vertexBuffer(0),
	_clientData{0, 0},
	_position{0.0f, 0.0f, 0.0f},
	_angle(0.0f),
	_visible(true),
	_shader(0),
	_drawMode(GL_TRIANGLE_STRIP)
	{
		_shader = &Shader::ref("Shader");
	}

	// TODO: Learn how to implement move Ctor
	Model(const Model & model) :
	_vertexArray(0), // Must call create() to set it
	_vertexBuffer(0), // Must call create() to set it
	_clientData{0, 0}, // Must call create() to set it
	_position(model._position),
	_angle(model._angle),
	_visible(model._visible),
	_drawMode(model._drawMode)
	{
//		info("Enter Model Copy Ctor\n");
	}

public: // Dtor
	virtual ~Model();

public: // Methods
	virtual float posX() const { return _position.x; }
	virtual void setPosX(float x) { _position.x = x; }

	virtual float posY() const { return _position.y; }
	virtual void setPosY(float y) { _position.y = y; }

	virtual void setPos(float x, float y) { _position = {x, y}; }

	virtual float angle() const { return _angle; }
	virtual void setAngle(float radians) { _angle = radians; }

//	inline bool visible() const { return _visible; }
//	void setVisible(bool visible) { _visible = visible; }

protected: // Methods
	inline bool create(const float * vertices, int count, int vertexSize)
	{
		// Allocate own set of raw vertex data so models can be copied
		_clientData.vertexCount = count;
		_clientData.vertices = (float *)malloc(_clientData.vertexCount*vertexSize);
		memcpy(_clientData.vertices, vertices, _clientData.vertexCount*vertexSize);

		// Create and bind the Vertex Array Object
		glGenVertexArraysOES(1, &_vertexArray);
		glBindVertexArrayOES(_vertexArray);

		// Create and bind VBO, configure the attributes in the VAO
		glGenBuffers(1, &_vertexBuffer); // Never forget to unbind when done

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, _clientData.vertexCount*vertexSize, _clientData.vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(eVertexAttrPosition, 3, GL_FLOAT, GL_FALSE, vertexSize, (void *)(0));
		glEnableVertexAttribArray(eVertexAttrPosition);

//		if (vertexSize == 5*sizeof(float)) // Three vertex corrdinates + 2 texture ones
//		{
//			glVertexAttribPointer(eVertexAttrTexPosition, 2, GL_FLOAT, GL_FALSE, vertexSize, (void *)(3*sizeof(float)));
//			glEnableVertexAttribArray(eVertexAttrTexPosition);
//		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArrayOES(0);

		return true;
	}

private: // Methods
	virtual void setUniforms() = 0; // Descendants must override to supply own set of uniforms
	inline void draw()
	{
		if (!_visible)
			return;

		// Static vars ARE big evil!
		// Remember that method static var is the one across ALL class instances
		MStack & stack = MStack::ref(); // Note, never ever make it static

		stack.push();

		// TODO: Translate only relevant coords (which are dirty, etc)
		stack.translate(posX(), posY(), 0.0f);

		// TODO: Rotate only if dirty
		stack.rotateZ(angle());

		setUniforms();

		_shader->use();

		// Docs: For best performance, your models should be submitted as a single unindexed triangle strip
		// using glDrawArrays with as few duplicated vertices as possible.
		glBindVertexArrayOES(_vertexArray);
		glDrawArrays(_drawMode, 0, _clientData.vertexCount);
		glBindVertexArrayOES(0);

		stack.pop();
	}

private: // Vars
	GLuint _vertexArray; // Vertex Array Object
	GLuint _vertexBuffer; // Vertex Buffer Object
	VertexData _clientData; // Model's raw vertex data, used to regenerate VBO/VAO when copying objects

	bool _visible;

	// TODO: Position and angle vars are not used by ModelX and its descendants
	EngineX::Point _position; // Model's position (Z coord is a real OpenGL depth)
	float _angle; // Model's angle, in radians

protected: // Vars
	Shader * _shader;
	int _drawMode; // TODO: Do we need it in Model? Probably in Shape only (and kids)
};

#endif /* defined(__EngineX__Model__) */
