#ifndef FOUN_MATRIX_H
#define FOUN_MATRIX_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

struct Vector2;

struct Matrix44
{
	f32 m[4][4];
};

void Matrix44Identity(Matrix44& matrix);
void Matrix44OrthographicProjection(Matrix44& matrix, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

struct Matrix33
{
	f32 m[3][3];
};

void Matrix33Identity(Matrix33& matrix);

struct Matrix32
{
	f32 m[3][2];
};

void Matrix32Identity(Matrix32& matrix);
void Matrix32Rotate(Matrix32& matrix, f32 rotations);
void Matrix32Translate(Matrix32& matrix, f32 translateX, f32 translateY);
void Matrix32RotateTranslate(Matrix32& matrix, f32 rotations, f32 translateX, f32 translateY);
void Matrix32ScaleRotateTranslate(Matrix32& matrix, f32 scaleX, f32 scaleY, f32 rotations, f32 translateX, f32 translateY);
void Matrix32Multiply(Matrix32& matrix, const Matrix32& matrixA, const Matrix32& matrixB);

void Matrix33FromMatrix32(Matrix33& matrix33, const Matrix32& matrix32);

struct Matrix22
{
	f32 m[2][2];
};

void Matrix22Rotate(Matrix22& matrix, f32 rotations);

void TransformVector2(Vector2& outVector, const Vector2& inVector, const Matrix22& matrix);

//namespace foun
}
//namespace lct
}

#endif//FOUN_MATRIX_H
