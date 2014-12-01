//
//  Shape.cpp
//  EngineX
//
//  Created by Maxim Reshetey on 13.12.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

#include "Shape.h"
#include "RenderController.h"

// Vertex structure for simple geometry
typedef
struct
{
	float x, y, z; // Vertex position
} Vertex;

static const int kVertexSize = sizeof(Vertex);
static const int kVertexStride = kVertexSize/sizeof(float);

void Shape::setRect(float width, float height)
{
	if (!height)
	{
		height = width; // If height missing, make square
	}

	// Hint: When you are designing your vertex structure, align the beginning of each attribute
	// to an offset that is either a multiple of its component size or 4 bytes, whichever is larger.
	const float vertices[] = // Reversed Z
	{
		-width*0.5f,	-height*0.5f,	0.0f,	// 0.0f,	0.0f, 1.0f, // A
		width*0.5f,		-height*0.5f,	0.0f,	// 0.0f,	0.0f, 1.0f, // B
		-width*0.5f,	height*0.5f,	0.0f,	// 0.0f,	0.0f, 1.0f, // C
		width*0.5f,		height*0.5f,	0.0f,	// 0.0f,	0.0f, 1.0f, // D
	};
	Model::create(vertices, 4, kVertexSize);
//	RenderController::ref().addModel(this); // Add model into drawing list
}

void Shape::setTriangle(float base, float height)
{
	if (!height) // If height missing, make equilateral triangle
	{
		float sqrtf3 = 1.732050807568877f;
		height = sqrtf3*base*0.5f; // Formula of getting height in equilateral triangle
	}

	const float vertices[] = // Reversed Z
	{
		-base*0.5f,		-height*0.5f,	0.0f,	// 0.0f,	0.0f, 1.0f, // A
		base*0.5f,		-height*0.5f,	0.0f,	// 0.0f,	0.0f, 1.0f, // B
		-0.0f,			height*0.5f,	0.0f,	// 0.0f,	0.0f, 1.0f, // C
	};
	Model::create(vertices, 3, kVertexSize);
//	RenderController::ref().addModel(this);
}

void Shape::setCircle(float radius)
{
	const short vertexCount = 48; // Circle detalization
	float vertices[kVertexStride*vertexCount];

	for(short i=0; i<vertexCount; ++i)
	{
		float rads = (float)i * 2.0f*(float)(M_PI)/vertexCount;

		vertices[i*kVertexStride+0] = radius * cosf(rads); // x
		vertices[i*kVertexStride+1] = radius * sinf(rads); // y
		vertices[i*kVertexStride+2] = 0.0f; // z
	}
	Model::create(vertices, vertexCount, kVertexSize);
	_drawMode = GL_TRIANGLE_FAN; // Solid circle
//	RenderController::ref().addModel(this);
}
