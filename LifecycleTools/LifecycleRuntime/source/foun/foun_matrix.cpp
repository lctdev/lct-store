#include <foun/foun_matrix.h>
#include <foun/foun_math.h>

namespace lct
{
namespace foun
{

void Matrix44Identity(Matrix44& matrix)
{
	matrix.m[0][0] = 1.0f;
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = 0.0f;

	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = 0.0f;

	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[2][3] = 0.0f;

	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;
}

void Matrix44OrthographicProjection(Matrix44& matrix, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = -(right + left) / (right - left);

	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = -(top + bottom) / (top - bottom);

	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = -2.0f / (far - near);
	matrix.m[2][3] = -(far + near) / (far - near);

	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;
}

void Matrix33Identity(Matrix33& matrix)
{
	matrix.m[0][0] = 1.0f;
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;

	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[1][2] = 0.0f;

	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = 1.0f;
}

void Matrix32Identity(Matrix32& matrix)
{
	matrix.m[0][0] = 1.0f;
	matrix.m[0][1] = 0.0f;

	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 1.0f;

	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
}

void Matrix32Rotate(Matrix32& matrix, f32 rotations)
{
	f32 rotateRadians = RadiansFromRotations(rotations);
	f32 sin = Sin(rotateRadians);
	f32 cos = Cos(rotateRadians);

	matrix.m[0][0] = cos;
	matrix.m[0][1] = sin;

	matrix.m[1][0] = -sin;
	matrix.m[1][1] = cos;

	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
}

void Matrix32Translate(Matrix32& matrix, f32 translateX, f32 translateY)
{
	matrix.m[0][0] = 1.0f;
	matrix.m[0][1] = 0.0f;

	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 1.0f;

	matrix.m[2][0] = translateX;
	matrix.m[2][1] = translateY;
}

void Matrix32RotateTranslate(Matrix32& matrix, f32 rotations, f32 translateX, f32 translateY)
{
	f32 rotateRadians = RadiansFromRotations(rotations);
	f32 sin = Sin(rotateRadians);
	f32 cos = Cos(rotateRadians);

	matrix.m[0][0] = cos;
	matrix.m[0][1] = sin;

	matrix.m[1][0] = -sin;
	matrix.m[1][1] = cos;

	matrix.m[2][0] = translateX;
	matrix.m[2][1] = translateY;
}

void Matrix32ScaleRotateTranslate(Matrix32& matrix, f32 scaleX, f32 scaleY, f32 rotations, f32 translateX, f32 translateY)
{
	f32 rotateRadians = RadiansFromRotations(rotations);
	f32 sin = Sin(rotateRadians);
	f32 cos = Cos(rotateRadians);

	matrix.m[0][0] = scaleX * cos;
	matrix.m[0][1] = scaleX * sin;

	matrix.m[1][0] = scaleY * -sin;
	matrix.m[1][1] = scaleY * cos;

	matrix.m[2][0] = translateX;
	matrix.m[2][1] = translateY;
}

void Matrix32Multiply(Matrix32& matrix, const Matrix32& matrixA, const Matrix32& matrixB)
{
	matrix.m[0][0] = matrixA.m[0][0] * matrixB.m[0][0] + matrixA.m[0][1] * matrixB.m[1][0]; // + matrixA[0][2](0.0f) * matrixB[2][0];
	matrix.m[0][1] = matrixA.m[0][0] * matrixB.m[0][1] + matrixA.m[0][1] * matrixB.m[1][1]; // + matrixA[0][2](0.0f) * matrixB[2][1];

	matrix.m[1][0] = matrixA.m[1][0] * matrixB.m[0][0] + matrixA.m[1][1] * matrixB.m[1][0]; // + matrixA[1][2](0.0f) * matrixB[2][0];
	matrix.m[1][1] = matrixA.m[1][0] * matrixB.m[0][1] + matrixA.m[1][1] * matrixB.m[1][1]; // + matrixA[1][2](0.0f) * matrixB[2][1];

	matrix.m[2][0] = matrixA.m[2][0] * matrixB.m[0][0] + matrixA.m[2][1] * matrixB.m[1][0] + matrixB.m[2][0]; // + matrixA[2][2](1.0f) * matrixB[2][0];
	matrix.m[2][1] = matrixA.m[2][0] * matrixB.m[0][1] + matrixA.m[2][1] * matrixB.m[1][1] + matrixB.m[2][1]; // + matrixA[2][2](1.0f) * matrixB[2][1];
}

void Matrix33FromMatrix32(Matrix33& matrix33, const Matrix32& matrix32)
{
	matrix33.m[0][0] = matrix32.m[0][0];
	matrix33.m[0][1] = matrix32.m[0][1];
	matrix33.m[0][2] = 0.0f;

	matrix33.m[1][0] = matrix32.m[1][0];
	matrix33.m[1][1] = matrix32.m[1][1];
	matrix33.m[1][2] = 0.0f;

	matrix33.m[2][0] = matrix32.m[2][0];
	matrix33.m[2][1] = matrix32.m[2][1];
	matrix33.m[2][2] = 1.0f;
}

//namespace foun
}
//namespace lct
}
