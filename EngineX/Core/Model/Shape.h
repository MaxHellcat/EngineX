//
//  Shape.h
//  EngineX
//
//  Created by Maxim Reshetey on 13.12.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

#ifndef __EngineX__Shape__
#define __EngineX__Shape__

#include "Model.h"

class Shape : public virtual Model
{
public:
	Shape() : _color{1.0f, 1.0f, 1.0f, 1.0f} // Default white color
	{
//		info("Enter Shape Ctor\n");
	};

	Shape(const Shape & shape) : _color(shape._color)
	{
//		info("Enter Shape Copy Ctor\n");
//		Model::create(shape._clientData.vertices, shape._clientData.vertexCount, 3);
	};

	virtual ~Shape()
	{
//		info("Enter Shape Dtor\n");
	}

public: // Methods
	void setRect(float width, float height=0.0f);
	void setTriangle(float base, float height=0.0f);
	void setCircle(float radius);

	virtual Shape & setColor(float r, float g, float b, float a=1.0f) { _color = {r, g, b, a}; return *this; }

	// TODO: What about complex shapes?
	static Shape & createCircle(float radius, float posX=0.0f, float posY=0.0f)
	{
		// Approach 1st - create in heap and shadow copy to RenderController model list
		// Approach 2nd - create in stack and deep copy
		// TODO: You decide which is more efficient
		Shape * shape = new Shape();
		shape->setCircle(radius);

		if (posX) shape->setPosX(posX);
		if (posY) shape->setPosY(posY);

		// The 1st solution is to release in RenderController::clear()
		return *shape; // TODO: 100% leak here as we never release
	}
	static Shape & createRect(float width, float height=0.0f, float posX=0.0f, float posY=0.0f)
	{
		Shape * shape = new Shape();
		shape->setRect(width, height);

		if (posX) shape->setPosX(posX);
		if (posY) shape->setPosY(posY);

		return *shape; // TODO: 100% leak here as we never release
	}
	static Shape & createTriangle(float base, float height=0.0f, float posX=0.0f, float posY=0.0f)
	{
		Shape * shape = new Shape();
		shape->setTriangle(base, height);

		if (posX) shape->setPosX(posX);
		if (posY) shape->setPosY(posY);

		return *shape; // TODO: 100% leak here as we never release
	}

private: // Methods
	virtual void setUniforms() override final
	{
		MStack & stack = MStack::ref();

		// TODO: Implement conditional uniform set (isDirty)
		glUniformMatrix4fv(_shader->uniform(uniModelViewProjectionMatrix), 1, 0, stack.modelViewProjectionMatrix().m); // 10ms, iPhone4

		// TODO: Implement conditional uniform set, approx. 10ms one call
		glUniform4f(_shader->uniform(uniColor), _color.r, _color.g, _color.b, _color.a);
	}

private: // Vars
	EngineX::ColorRGBA _color; // Shape's color
};

#endif /* defined(__EngineX__Shape__) */
