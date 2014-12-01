//
//  MStack.h
//  EngineX
//
//  Created by Maxim Reshetey on 19.11.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

#ifndef __EngineX__MStack__
#define __EngineX__MStack__

// #define GLKIT (__IPHONE_OS_VERSION_MAX_ALLOWED >= 50000)
#define GLKIT 1


#if GLKIT
#include "common.h"
#import <GLKit/GLKMath.h>

typedef GLKVector3 Vector3;
typedef GLKMatrix3 Matrix3;
typedef GLKMatrix4 Matrix4;
typedef GLKMatrixStackRef StackRef;

#else

#include "common.h"

typedef
struct
{
    float v[3];
} Vector3;

typedef
struct
{
    float m[9];
} Matrix3;

struct _Matrix4
{
    float m[16];
} __attribute__((aligned(16)));
typedef struct _Matrix4 Matrix4;

#define ABS(A)	({ __typeof__(A) __a = (A); __a < 0 ? -__a : __a; }) // Used in matrix inversion

static const short kStackDepth = 8;

typedef Matrix4 (StackRef)[kStackDepth]; // For non-GLKit case, stackRef is just a pointer to an array

#endif


// Class to manage matrix stack
// For speed, preload kStackDepth matrices upon initialization and then just change current index
// All operations are row-major
class MStack
{
private:
	MStack()
	{
#if GLKIT
//		_stack = GLKMatrixStackCreate(NULL);
//		CFRetain(_ref);
#endif
	};
	~MStack()
	{
//		info("Matrix stack deleted");
//#if GLKIT
//		CFRelease(_stack); _stack = 0;
//#endif
	};
	MStack(const MStack &) = delete; // Not implemented
	MStack & operator=(const MStack &) = delete; // Not implemented

public: // Methods
	static MStack & ref() { static MStack inst;	return inst; }

public: // Methods
//	const int size() const { return GLKMatrixStackSize(_ref); }
	void loadIdentity();

	void push();
	void pop();

	void setPerspective(float fovyDegrees, float aspect, float near, float far);
	void setOrtho(float left, float right, float bottom, float top, float near, float far);

//	void setLookAt(float centerX, float centerY)
//	{
//#if GLKIT
////		GLKMatrixStackTranslate(_stack, tx, ty, tz);
////#else
//		Matrix4 translationMatrix =
//		{
//			1.0f,	0.0f,	0.0f,	0.0f,
//			0.0f,	1.0f,	0.0f,	0.0f,
//			0.0f,	0.0f,	1.0f,	0.0f,
//			centerX, centerY, 0.0f,		1.0f
//		};
//
//		// TODO: Since we don't do any trasnformations with proj matrix so far, just replace it.
//		// Otherwise you must multiply
////		_projectionMatrix = translationMatrix;
//		_projectionMatrix = GLKMatrix4Multiply(_projectionMatrix, translationMatrix);
//#else
//		err("setLookAt() NOT implemented!\n");
//#endif
//	}

	void translate(float tx, float ty, float tz);
	void rotate(float radians, float x, float y, float z);

	// TODO: Extend to non-GLKit usage
//	void scale(float sx, float sy)
//	{
//#if GLKIT
//		GLKMatrixStackScale(_stack, sx, sy, 1.0f);
//#else
//		err("GLKMatrixStackScale() NOT implemented!\n");
//#endif
//	}

	// TODO: Extend to non-GLKit usage
	void rotateZ(float radians)
	{
#if GLKIT
		GLKMatrixStackRotateZ(_stack, radians);
#else
		err("rotateZ() NOT implemented!\n");
#endif
	}

//	void loadMatrix(const Matrix4 & matrix);

//	const Matrix4 & projectionMatrix() const { return _projectionMatrix; }
	Matrix4 modelViewMatrix();
	Matrix4 modelViewProjectionMatrix();
	Matrix3 normalMatrix();

	static void init()
	{
		auto & myself = MStack::ref();
		
		myself._stack = GLKMatrixStackCreate(NULL);
//		CFRetain(_ref);
	}

	static void deinit()
	{
		auto & myself = MStack::ref();
	
#if GLKIT
		CFRelease(myself._stack); myself._stack = 0;
#endif

	info("Matrix Stack de-initialized\n");

	}

#if !GLKIT
private: // Methods
	short _top; // Index of top (active) matrix

	Matrix4 mult(const Matrix4 & matrixLeft, const Matrix4 & matrixRight);
	Matrix3 matrixInverseTranspose(const Matrix4 & matrix);
#endif

private: // Vars
	Matrix4 _projectionMatrix; // Only one for now
	StackRef _stack;
};

#endif /* defined(__EngineX__MStack__) */
