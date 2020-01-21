#ifndef MatrixOpe_H
#define MatrixOpe_H
#include "Matrix.h"

namespace MLL
{
	Matrix operator+(const Matrix &matrix1, const Matrix &matrix2);

	Matrix operator-(const Matrix &matrix1, const Matrix &matrix2);

	Matrix operator*(const Matrix &matrix1, const Matrix &matrix2);

	Matrix operator*(const double &alpha, const Matrix &matrix1);

	Matrix operator/(const Matrix &matrix1 , const double &alpha);

	inline void sigmoid(float *z)
	{
		*z = 1.0 / (1.0 + exp(-*z));       
	}

	inline void sigmoid(Matrix *z)
	{
		for(size_t i=0;i<z->row;i++)
		{
			for(size_t j=0;j<z->col;j++)
			{
				z->data[i][j]=1.0 / (1.0 + exp(- z->data[i][j]));
			}
		}
	}

	Matrix one_hot(const Matrix &Y, const int &C);

	// Matrix operator<<
}
#endif // MatrixOpe_H
