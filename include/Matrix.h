#ifndef Matrix_H
#define Matrix_H
#include "LoadData.h"
namespace MLL
{
	class Matrix
	{
	public:
		Data data;
		unsigned int row;
		unsigned int col;
	public:
		Matrix();

		Matrix(const unsigned int &row, const unsigned int &col, const float &init_val, const std::string &type);

		void initMatrix(const unsigned int &row, const unsigned int &col, const float &init_val, const std::string &type);

		void LoadData(const std::string &filename);

		void LoadData_spare(const std::string &filename, const unsigned int &row, const unsigned int &col);
		
		void print();

		Matrix& copyMatrix();

		Matrix& getOneRow(const unsigned int &iRow) const;

		Matrix& getOneCol(const unsigned int &jCol) const;

		void deleteOneRow(const unsigned int &iRow);

		void deleteOneCol(const unsigned int &iCol);

		Matrix& transposeMatrix();//矩阵形式的转置

		Matrix& addMatrix(const Matrix &matrix1,const Matrix &matrix2);

		Matrix& subMatrix(const Matrix &matrix1,const Matrix &matrix2);

		Matrix& multsMatrix(const Matrix &matrix1, const Matrix &matrix2);//矩阵形式的相乘

		Matrix& dotmultsMatrix(const Matrix &matrix1, const Matrix &matrix2);//矩阵形式的相乘

		double detMatrix();//行列式

		Matrix& niMatrix();//高斯消元矩阵求逆,特别注意，LU分解不能进行行列式变换
	};
}
#endif // Data_H
