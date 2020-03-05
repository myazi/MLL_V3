#include <MatrixOpe.h>
#include <stdlib.h>
namespace MLL
{
	Matrix operator+(const Matrix &matrix1, const Matrix &matrix2)
	{
		if(matrix1._col != matrix2._col || matrix1._row != matrix2._row)
		{
			std::cout << matrix1._row << "*" << matrix1._col << std::endl;
			std::cout << matrix2._row << "*" << matrix2._col << std::endl;
			std::cout << "addData data1 data2 is no" << std::endl;
			exit(-1);
		}
		RowData cda(matrix1._col);
		Data da(matrix1._row,cda);
		static Matrix add;
		add._data = da;
		add._row = matrix1._row;
		add._col = matrix1._col;
		unsigned int i,j;
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix1._col; j++)
			{
				add._data[i][j] = matrix1._data[i][j] + matrix2._data[i][j];
			}
		}
		return add;
	}
	Matrix operator-(const Matrix &matrix1, const Matrix &matrix2)
	{
		if(matrix1._col != matrix2._col || matrix1._row != matrix2._row)
		{
			std::cout << matrix1._row<< "*" << matrix1._col << std::endl;
			std::cout << matrix2._row<< "*" << matrix2._col << std::endl;
			std::cout << "addData data1 data2 is no" << std::endl;
			exit(-1);
		}
		RowData cda(matrix1._col);
		Data da(matrix1._row, cda);
		static Matrix sub;
		sub._data = da;
		sub._row = matrix1._row;
		sub._col = matrix1._col;
		unsigned int i,j;
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix1._col; j++)
			{
				sub._data[i][j] = matrix1._data[i][j] - matrix2._data[i][j];
			}
		}
		return sub;
	}
	Matrix operator*(const Matrix &matrix1, const Matrix &matrix2)
	{
		if(matrix1._col != matrix2._row)
		{
			std::cout << matrix1._row << "*" << matrix1._col << std::endl;
			std::cout << matrix2._row << "*" << matrix2._col << std::endl;
			std::cout << "multsData error" << std::endl;
			exit(-1);
		}
		unsigned int i,j,k;
		static Matrix mults;
		ColData cda(matrix2._col);
		Data da(matrix1._row, cda);
		mults._data = da;
		mults._row = matrix1._row;
		mults._col = matrix2._col;
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix2._col; j++)
			{
				mults._data[i][j] = 0;
			}
		}
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix2._col; j++)
			{
				for(k = 0; k < matrix1._col; k++)
				{
					mults._data[i][j] += matrix1._data[i][k] * matrix2._data[k][j];
				}
			}
		}
		return mults;
	}
	Matrix operator*(const double &alpha, const Matrix &matrix1)
	{
		unsigned int i,j;
		static Matrix mults;
		ColData cda(matrix1._col);
		Data da(matrix1._row, cda);
		mults._data = da;
		mults._row = matrix1._row;
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix1._col; j++)
			{
				mults._data[i][j] = alpha * matrix1._data[i][j];
			}
		}
		return mults;
	}
	Matrix operator/(const Matrix &matrix1 , const double &alpha)
	{
		unsigned int i,j;
		static Matrix mults;
		ColData cda(matrix1._col);
		Data da(matrix1._row,cda);
		mults._data = da;
		mults._row = matrix1._row;
		for(i = 0 ; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix1._col; j++)
			{
				mults._data[i][j] = matrix1._data[i][j] / alpha;
			}
		}
		return mults;
	}
	/*
	Matrix sigmoid(Matrix &z)
	{
		static Matrix zz;
		ColData cda(z._col);
		Data da(z._row,cda);
		zz._data = da;
		zz._row = z._row;
		zz._col = z._col;

		for(size_t i = 0; i < z._row; i++)
		{
			for(size_t j = 0; j < z._col; j++)
			{
				zz._data[i][j] = 1.0 / (1 + exp(-z._data[i][j]));
			}
		}
		return zz;
	}
	float sigmoid(const float &z)
	{
		return 1.0 / ( 1 + exp(-z));
	}
	*/
	Matrix one_hot(const Matrix &Y, const int &C)
	{
		if(Y._row > Y._col)
		{
			static Matrix one_hot_Y(Y._row, C, 0, "ss");
			int i = 0;
			for(i = 0; i < one_hot_Y._row; i++)
			{
				one_hot_Y._data[i][Y._data[i][0]] = 1;
			}
			return one_hot_Y;
		}
		else
		{
			static Matrix one_hot_Y(C, Y._col, 0, "ss");
			int j = 0;
			for(j = 0; j < one_hot_Y._col; j++)
			{
				one_hot_Y._data[Y._data[0][j]][j] = 1;
			}
			return one_hot_Y;
		}
	}
}
