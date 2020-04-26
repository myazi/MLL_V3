#ifndef Matrix_H
#define Matrix_H
#include "LoadData.h"
namespace MLL
{
	class Matrix
	{
	public:
		Data _data;
		size_t _row;
		size_t _col;
	public:
		Matrix();
		
		~Matrix();

		Matrix(const Matrix &rhs);
		
		Matrix(const size_t &row, const size_t &col, const float &init_val);
		
		Matrix(const size_t &row, const size_t &col, const float &init_val, const std::string &type);

		void initMatrix(const size_t &row, const size_t &col, const float &init_val);

		void init_by_random(const size_t &row, const size_t &col);
		
		void init_by_data(const std::string &filename);

		void init_by_spare(const std::string &filename, const size_t &row, const size_t &col);
		
		void print() const;
	
		Matrix copyMatrix() const;

		Matrix getOneRow(const size_t &iRow) const;

		Matrix getOneCol(const size_t &jCol) const;

		void deleteOneRow(const size_t &iRow);

		void deleteOneCol(const size_t &iCol);

		Matrix transposeMatrix();//������ʽ��ת��

		Matrix addMatrix(const Matrix &matrix1,const Matrix &matrix2);

		Matrix subMatrix(const Matrix &matrix1,const Matrix &matrix2);

		Matrix multsMatrix(const Matrix &matrix1, const Matrix &matrix2);//������ʽ�����

		Matrix dotmultsMatrix(const Matrix &matrix1, const Matrix &matrix2);//������ʽ�����

		double detMatrix();//����ʽ

		Matrix niMatrix();//��˹��Ԫ��������,�ر�ע�⣬LU�ֽⲻ�ܽ�������ʽ�任
	};
}
#endif // Data_H
