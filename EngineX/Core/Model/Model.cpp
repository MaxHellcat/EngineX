//
//  Model.cpp
//  EngineX
//
//  Created by Maxim Reshetey on 22.11.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

#include "Model.h"
#include "RenderController.h"

Model::~Model()
{
	// Don't release shader, as it can be shared across multiple models

	// Release VBO and VAO
	if (_vertexBuffer)
	{
		glDeleteBuffers(1, &_vertexBuffer);
		_vertexBuffer = 0;
	}
	if (_vertexArray)
	{
		glDeleteVertexArraysOES(1, &_vertexArray);
		_vertexArray = 0;
	}

	// Release vertices
	free(_clientData.vertices);
	_clientData.vertices = 0;

	info("Model with %i vertices deleted\n", _clientData.vertexCount);
};
