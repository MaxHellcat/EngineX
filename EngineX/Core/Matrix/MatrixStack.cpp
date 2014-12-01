//
//  MStack.cpp
//  EngineX
//
//  Created by Maxim Reshetey on 19.11.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

#include "MatrixStack.h"

#if !GLKIT
#include "cmath" // For M_PI, etc
#include "cstring" // For memcpy, etc
#endif

void
MStack::loadIdentity()
{
#if GLKIT
	GLKMatrixStackLoadMatrix4(_stack, GLKMatrix4Identity);
#else
	Matrix4 identityMatrix =
	{
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};
	_stack[_top] = identityMatrix;
#endif
}

#pragma mark - Push and Pop
#pragma mark -
void
MStack::push()
{
#if GLKIT
	GLKMatrixStackPush(_stack);
#else
	if (_top < kStackDepth-1) // Only if stack limit not reached
	{
		++_top; // Switch to the next matrix up the stack and duplicate
		memcpy(_stack[_top].m, _stack[_top-1].m, sizeof(float)*16);
	}
#endif
}

void
MStack::pop()
{
#if GLKIT
	GLKMatrixStackPop(_stack);
#else
	if (_top > 0)
	{
		--_top; // Just move the pointer to the previous matrix
	}
#endif
}

#pragma mark - Projections
#pragma mark -
void
MStack::setPerspective(float fovyDegrees, float aspect, float near, float far)
{
#if GLKIT
	_projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(fovyDegrees), aspect, near, far);
#else
	float fovyRadians = fovyDegrees * (M_PI / 180.0f);
	float cotan = 1.0f / tanf(fovyRadians / 2.0f);

	Matrix4 perspectiveMatrix =
	{
		cotan / aspect,		0.0f,	0.0f,									0.0f,
		0.0f,				cotan,	0.0f,									0.0f,
		0.0f,				0.0f,	(far + near) / (near - far),			-1.0f,
		0.0f,				0.0f,	(2.0f * far * near) / (near - far),		0.0f
	};

	// TODO: Here we must multiply, but for simplicity just replace current projection matrix
	_projectionMatrix = perspectiveMatrix;
#endif
}

void
MStack::setOrtho(float left, float right, float bottom, float top, float near, float far)
{
#if GLKIT
	_projectionMatrix = GLKMatrix4MakeOrtho(left, right, bottom, top, near, far);
#else
	float dx = right - left;
	float dy = top - bottom;
	float dz = far - near;
	float tx = - (right + left) / dx;
	float ty = - (top + bottom) / dy;
	float tz = - (far + near) / dz;

	Matrix4 orthographicMatrix =
	{
		2.0f/dx,	0.0f,		0.0f,		0.0f,
		0.0f,		2.0f/dy,	0.0f,		0.0f,
		0.0f,		0.0f,		-2.0f/dz,	0.0f,
		tx,			ty,			tz,			1.0f
	};

	// TODO: Here we must multiply, but for simplicity just replace current projection matrix
	_projectionMatrix = orthographicMatrix;
#endif
}

#pragma mark - Rotate and Translate
#pragma mark -
void
MStack::translate(float tx, float ty, float tz)
{
#if GLKIT
	GLKMatrixStackTranslate(_stack, tx, ty, tz);
#else
	Matrix4 translationMatrix =
	{
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		tx,		ty,		tz,		1.0f
	};

	Matrix4 & m = _stack[_top];
	m = mult(m, translationMatrix);
#endif
}

void
MStack::rotate(float radians, float x, float y, float z)
{
#if GLKIT
	GLKMatrixStackRotate(_stack, radians, x, y, z);
#else
	// Normalize vector
	float scale = 1.0f / (sqrtf(x*x + y*y + z*z));
	Vector3 v = { x*scale, y*scale, z*scale };

    // Actualy rotate
	float cos = cosf(radians);
    float cosp = 1.0f - cos;
    float sin = sinf(radians);

    Matrix4 rotationMatrix =
	{
		cos + cosp * v.v[0] * v.v[0],			cosp * v.v[0] * v.v[1] + v.v[2] * sin,	cosp * v.v[0] * v.v[2] - v.v[1] * sin,	0.0f,
		cosp * v.v[0] * v.v[1] - v.v[2] * sin,	cos + cosp * v.v[1] * v.v[1],			cosp * v.v[1] * v.v[2] + v.v[0] * sin,	0.0f,
		cosp * v.v[0] * v.v[2] + v.v[1] * sin,	cosp * v.v[1] * v.v[2] - v.v[0] * sin,	cos + cosp * v.v[2] * v.v[2],			0.0f,
		0.0f,									0.0f,									0.0f,									1.0f
	};

	Matrix4 & m = _stack[_top];
	m = mult(m, rotationMatrix);
#endif
}

#pragma mark - Matrices
#pragma mark -
Matrix4
MStack::modelViewMatrix()
{
#if GLKIT
	return GLKMatrixStackGetMatrix4(_stack);
#else
	return _stack[_top];
#endif
}

Matrix4
MStack::modelViewProjectionMatrix()
{
#if GLKIT
	return GLKMatrix4Multiply(_projectionMatrix, modelViewMatrix());
#else
	return mult(_projectionMatrix, modelViewMatrix());
#endif
}

Matrix3
MStack::normalMatrix()
{
#if GLKIT
	return GLKMatrixStackGetMatrix3InverseTranspose(_stack);
#else
	return matrixInverseTranspose(_stack[_top]);
#endif
}

#pragma mark - Matrix utils
#pragma mark -

//void MStack::loadMatrix(const Matrix4 & matrix)
//{
//#if GLKIT
//	GLKMatrixStackLoadMatrix4(_stack, matrix);
//#else
//	memcpy(_stack[_top].m, _stack[_top-1].m, sizeof(float)*16);
//#endif
//}

#if !GLKIT
Matrix4
MStack::mult(const Matrix4 & matrixLeft, const Matrix4 & matrixRight)
{
	Matrix4 m;

    m.m[0]  = matrixLeft.m[0] * matrixRight.m[0]  + matrixLeft.m[4] * matrixRight.m[1]  + matrixLeft.m[8] * matrixRight.m[2]   + matrixLeft.m[12] * matrixRight.m[3];
	m.m[4]  = matrixLeft.m[0] * matrixRight.m[4]  + matrixLeft.m[4] * matrixRight.m[5]  + matrixLeft.m[8] * matrixRight.m[6]   + matrixLeft.m[12] * matrixRight.m[7];
	m.m[8]  = matrixLeft.m[0] * matrixRight.m[8]  + matrixLeft.m[4] * matrixRight.m[9]  + matrixLeft.m[8] * matrixRight.m[10]  + matrixLeft.m[12] * matrixRight.m[11];
	m.m[12] = matrixLeft.m[0] * matrixRight.m[12] + matrixLeft.m[4] * matrixRight.m[13] + matrixLeft.m[8] * matrixRight.m[14]  + matrixLeft.m[12] * matrixRight.m[15];

	m.m[1]  = matrixLeft.m[1] * matrixRight.m[0]  + matrixLeft.m[5] * matrixRight.m[1]  + matrixLeft.m[9] * matrixRight.m[2]   + matrixLeft.m[13] * matrixRight.m[3];
	m.m[5]  = matrixLeft.m[1] * matrixRight.m[4]  + matrixLeft.m[5] * matrixRight.m[5]  + matrixLeft.m[9] * matrixRight.m[6]   + matrixLeft.m[13] * matrixRight.m[7];
	m.m[9]  = matrixLeft.m[1] * matrixRight.m[8]  + matrixLeft.m[5] * matrixRight.m[9]  + matrixLeft.m[9] * matrixRight.m[10]  + matrixLeft.m[13] * matrixRight.m[11];
	m.m[13] = matrixLeft.m[1] * matrixRight.m[12] + matrixLeft.m[5] * matrixRight.m[13] + matrixLeft.m[9] * matrixRight.m[14]  + matrixLeft.m[13] * matrixRight.m[15];

	m.m[2]  = matrixLeft.m[2] * matrixRight.m[0]  + matrixLeft.m[6] * matrixRight.m[1]  + matrixLeft.m[10] * matrixRight.m[2]  + matrixLeft.m[14] * matrixRight.m[3];
	m.m[6]  = matrixLeft.m[2] * matrixRight.m[4]  + matrixLeft.m[6] * matrixRight.m[5]  + matrixLeft.m[10] * matrixRight.m[6]  + matrixLeft.m[14] * matrixRight.m[7];
	m.m[10] = matrixLeft.m[2] * matrixRight.m[8]  + matrixLeft.m[6] * matrixRight.m[9]  + matrixLeft.m[10] * matrixRight.m[10] + matrixLeft.m[14] * matrixRight.m[11];
	m.m[14] = matrixLeft.m[2] * matrixRight.m[12] + matrixLeft.m[6] * matrixRight.m[13] + matrixLeft.m[10] * matrixRight.m[14] + matrixLeft.m[14] * matrixRight.m[15];

	m.m[3]  = matrixLeft.m[3] * matrixRight.m[0]  + matrixLeft.m[7] * matrixRight.m[1]  + matrixLeft.m[11] * matrixRight.m[2]  + matrixLeft.m[15] * matrixRight.m[3];
	m.m[7]  = matrixLeft.m[3] * matrixRight.m[4]  + matrixLeft.m[7] * matrixRight.m[5]  + matrixLeft.m[11] * matrixRight.m[6]  + matrixLeft.m[15] * matrixRight.m[7];
	m.m[11] = matrixLeft.m[3] * matrixRight.m[8]  + matrixLeft.m[7] * matrixRight.m[9]  + matrixLeft.m[11] * matrixRight.m[10] + matrixLeft.m[15] * matrixRight.m[11];
	m.m[15] = matrixLeft.m[3] * matrixRight.m[12] + matrixLeft.m[7] * matrixRight.m[13] + matrixLeft.m[11] * matrixRight.m[14] + matrixLeft.m[15] * matrixRight.m[15];

    return m;
}

Matrix3
MStack::matrixInverseTranspose(const Matrix4 & matrix)
{
	// Inverse matrix first (Cramer's rule)
	double det_1;
	double pos, neg, temp;

	// TODO: Sort out what m4 like when matrix is singular
	Matrix4 m4;

	float * mb = m4.m;
	const float * ma = matrix.m;

	// Calculate the determinant of submatrix A and determine if
	// the matrix is singular as limited by the double precision
	// floating-point data representation.
    pos = neg = 0.0;
    temp =  ma[ 0] * ma[ 5] * ma[10];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp =  ma[ 4] * ma[ 9] * ma[ 2];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp =  ma[ 8] * ma[ 1] * ma[ 6];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp = -ma[ 8] * ma[ 5] * ma[ 2];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp = -ma[ 4] * ma[ 1] * ma[10];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp = -ma[ 0] * ma[ 9] * ma[ 6];
    if (temp >= 0.0) pos += temp; else neg += temp;
    det_1 = pos + neg;

	// Is the submatrix A singular?
    if ((det_1 == 0.0) || (ABS(det_1 / (pos - neg)) < 1.0e-15))
	{
		// Matrix has no inverse, singular matrix
		say("WARNING: Matrix HAS NO inverse!");
//		return mb;
    }
	else
	{
        // Calculate inverse(A) = adj(A) / det(A)
		det_1 = 1.0 / det_1;
		mb[ 0] =   ( ma[ 5] * ma[10] - ma[ 9] * ma[ 6] ) * (float)det_1;
		mb[ 1] = - ( ma[ 1] * ma[10] - ma[ 9] * ma[ 2] ) * (float)det_1;
		mb[ 2] =   ( ma[ 1] * ma[ 6] - ma[ 5] * ma[ 2] ) * (float)det_1;
		mb[ 4] = - ( ma[ 4] * ma[10] - ma[ 8] * ma[ 6] ) * (float)det_1;
		mb[ 5] =   ( ma[ 0] * ma[10] - ma[ 8] * ma[ 2] ) * (float)det_1;
		mb[ 6] = - ( ma[ 0] * ma[ 6] - ma[ 4] * ma[ 2] ) * (float)det_1;
		mb[ 8] =   ( ma[ 4] * ma[ 9] - ma[ 8] * ma[ 5] ) * (float)det_1;
		mb[ 9] = - ( ma[ 0] * ma[ 9] - ma[ 8] * ma[ 1] ) * (float)det_1;
		mb[10] =   ( ma[ 0] * ma[ 5] - ma[ 4] * ma[ 1] ) * (float)det_1;

        // Calculate -C * inverse(A)
        mb[12] = - ( ma[12] * mb[ 0] + ma[13] * mb[ 4] + ma[14] * mb[ 8] );
        mb[13] = - ( ma[12] * mb[ 1] + ma[13] * mb[ 5] + ma[14] * mb[ 9] );
        mb[14] = - ( ma[12] * mb[ 2] + ma[13] * mb[ 6] + ma[14] * mb[10] );

        // Fill in last row
        mb[ 3] = 0.0f;
		mb[ 7] = 0.0f;
		mb[11] = 0.0f;
        mb[15] = 1.0f;
	}

	// Then transpose and return
	Matrix3 m =
	{
		m4.m[0], m4.m[1], m4.m[2],
		m4.m[4], m4.m[5], m4.m[6],
		m4.m[8], m4.m[9], m4.m[10]
	};

	return m;
}

#endif // #if !GLKIT
