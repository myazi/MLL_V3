#include "Matrix.h"
namespace MLL
{
	Matrix::Matrix()
	{
		//std::cout << "Structor" << std::endl;
	}
	Matrix::~Matrix()
	{
		//std::cout << "Destructor Matrix"<< std::endl;
	}
	Matrix::Matrix(const Matrix &rhs): _data(rhs._data), _row(rhs._row), _col(rhs._col)
	{
		//std::cout << "(const A& rhs)" << std::endl;
	}
	Matrix::Matrix(const unsigned int &row, const unsigned int &col, const float &init_val)
	{
		if(row==0 || col==0)
		{
			std::cout<<"Structor is zero"<<std::endl;
			return ;
		}
		RowData cda(col);
		Data da(row,cda);
		this->_data = da;
		this->_row = row;
		this->_col = col;
		unsigned int i = 0, j = 0;
		for(i = 0; i < _row; i++)
		{
			for(j = 0; j < _col; j++)
				this->_data[i][j] = init_val;
		}
	}
	Matrix::Matrix(const unsigned int &row, const unsigned int &col, const float &init_val, const std::string &type)
	{
		if(row==0 || col==0)
		{
			std::cout << " Structor is zero" << std::endl;
			return ;
		}
		if(row !=  col)
		{
			std::cout << "diag row != col" << std::endl;
			return ;
		}
		RowData cda(col);
		Data da(row,cda);
		this->_data = da;
		this->_row = row;
		this->_col = col;
		unsigned int i = 0, j = 0;
		// ��ʼ��diag����
		for(i=0; i<_row; i++)
		{
			this->_data[i][i] = init_val;
		}
	}
	void Matrix::initMatrix(const unsigned int &row, const unsigned int &col, const float &init_val)
	{
		if(row==0 || col==0)
		{
			std::cout<<" Structor no zero"<<std::endl;
			return ;
		}
		RowData cda(col);
		Data da(row,cda);
		this->_data = da;
		this->_row = row;
		this->_col = col;
		unsigned int i = 0, j = 0;
		for(i=0; i<_row; i++)
		{
			for(j=0; j<_col; j++)
				this->_data[i][j] = init_val;
		}
	}
	void Matrix::init_by_spare(const std::string &filename, const unsigned int &row, const unsigned int &col)
	{
		unsigned int i = 0, j = 0;
		RowData cda(col);
		Data da(row,cda);
		this->_data = da;
		this->_row = row;
		this->_col = col;
		for(i=0; i<this->_row; i++)
		{
			for(j=0; j<this->_col; j++)
			{
				this->_data[i][j] = 0;
			}
		}
		LoadData_spare(this->_data, filename);
	}
	void Matrix::init_by_data(const std::string &filename)
	{
		unsigned int i = 0, j = 0;
		LoadData(this->_data,filename);
		_row=this->_data.size();
		_col=this->_data[0].size();
		for (i = 0; i < _row; i++)
		{
			if(_col > _data[i].size()) //���ݶ�������
				std::cout << "loaddata is error col" << std::endl;
		}
	}
	void Matrix::print() const
	{
		std::cout<<"matrix size:" << _row<<"**"<< _col<<std::endl;
		unsigned int i = 0, j = 0;
		for(i = 0; i < _row; i++)
		{
			for(j = 0; j < _col; j++)
			{
				std::cout<< _data[i][j] <<"  ";
			}
			std::cout<<std::endl;
		}
	}
	Matrix Matrix::copyMatrix() const
	{
		unsigned int i = 0, j = 0;
		Matrix cp(_row, _col, 0);
		for(i = 0; i < this->_row; i++)
		{
			for(j = 0; j < this->_col; j++)
			{
				cp._data[i][j] = this->_data[i][j];
			}
		}
		return cp;
	}
	Matrix Matrix::getOneRow (const unsigned int &iRow) const
	{
		unsigned int j = 0;
		Matrix one_row_matrix(1,_col,0);
		for(j = 0; j < this->_data[iRow].size(); j++)
		{
			one_row_matrix._data[0][j] = this->_data[iRow][j];
		}
		return one_row_matrix;
	}
	Matrix Matrix::getOneCol (const unsigned int &jCol) const 
	{
		unsigned int i = 0;
		Matrix one_col_matrix(_row,1,0);
		for(i = 0; i < this->_data.size(); i++)
		{
			one_col_matrix._data[i][0] = this->_data[i][jCol];
		}
		return one_col_matrix;
	}
	void Matrix::deleteOneRow(const unsigned int &iRow)
	{

		unsigned int i = 0, j = 0;
		Matrix cp = this->copyMatrix();
		this->_row--;
		for(Data::iterator it = cp._data.begin(); it != cp._data.end(); it++, i++)
		{
			if(i < iRow)
			{
				for(std::vector<float>::iterator itRow = it->begin(); itRow != it->end(); itRow++)
				{
					this->_data[i][j] = *itRow;
				}
			}
			if(i > iRow)
			{
				for(std::vector<float>::iterator itRow = it->begin(); itRow != it->end(); itRow++)
				{
					this->_data[i-1][j] = *itRow;
				}
			}
		}
	}
	void Matrix::deleteOneCol(const unsigned int &iCol)
	{

		unsigned int i=0, j=0;
		Matrix cp = this->copyMatrix();
		this->_col--;
		//this->_data.clear();
		//RowData cda(this->_col);
		//Data da(this->_row,cda);
		//this->_data=da;

		for(Data::iterator it = cp._data.begin(); it != cp._data.end(); it++, i++)
		{
			j = 0;
			for(std::vector<float>::iterator itRow = it->begin(); itRow != it->end(); itRow++, j++)
			{
				if(j < iCol)
				{
					this->_data[i][j] = *itRow;
				}
				if(j > iCol)
				{
					this->_data[i][j-1] = *itRow;
				}
			}

		}
	}
	Matrix Matrix::transposeMatrix()//������ʽ��ת��
	{
		unsigned int i = 0, j = 0;
		Matrix matrixT(_col,_row,0);
		for(i = 0; i < _col; i++)
		{
			for(j = 0; j < _row; j++)
			{
				matrixT._data[i][j] = this->_data[j][i];
			}
		}
		return matrixT;
	}
	Matrix Matrix::addMatrix(const Matrix &matrix1, const Matrix &matrix2)
	{
		if(matrix1._col != matrix2._col || matrix1._row != matrix2._row)
		{
			std::cout<<matrix1._row<<"*"<<matrix1._col<<std::endl;
			std::cout<<matrix2._row<<"*"<<matrix2._col<<std::endl;
			std::cout<<"addData data1 data2 is no"<<std::endl;
			exit(-1);
		}
		unsigned int i = 0, j = 0;
		Matrix add(matrix1._row, matrix1._col,0);
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix1._col; j++)
			{
				add._data[i][j] = matrix1._data[i][j] + matrix2._data[i][j];
			}
		}
		return add;
	}

	Matrix Matrix::subMatrix(const Matrix &matrix1,const Matrix &matrix2)
	{
		if(matrix1._col != matrix2._col || matrix1._row != matrix2._row)
		{
			std::cout << matrix1._row << "*" << matrix1._col << std::endl;
			std::cout << matrix2._row << "*" << matrix2._col << std::endl;
			std::cout << "subData data1 data2 is no" << std::endl;
			exit(-1);
		}
		unsigned int i,j;
		Matrix sub(matrix1._row, matrix1._col,0);
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix1._col; j++)
			{
				sub._data[i][j] = matrix1._data[i][j] - matrix2._data[i][j];
			}
		}
		return sub;
	}

	Matrix Matrix::multsMatrix(const Matrix &matrix1, const Matrix &matrix2)//������ʽ�����
	{
		if(matrix1._col != matrix2._row)
		{
			std::cout << matrix1._row << "*" << matrix1._col << std::endl;
			std::cout << matrix2._row << "*" << matrix2._col << std::endl;
			std::cout << "multsData error" << std::endl;
			exit(-1);
		}
		unsigned int i = 0, j =0, k = 0;
		Matrix mults(matrix1._row,matrix2._col,0);
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
				for( k = 0; k < matrix1._col; k++)
				{
					mults._data[i][j] += matrix1._data[i][k] * matrix2._data[k][j];
				}
			}
		}
		return mults;
	}

	Matrix Matrix::dotmultsMatrix(const Matrix &matrix1, const Matrix &matrix2)//�����Ӧ���
	{
		if(matrix1._row != matrix2._row || matrix1._col != matrix2._col)
		{
			std::cout<<matrix1._row<<"*"<<matrix1._col<<std::endl;
			std::cout<<matrix2._row<<"*"<<matrix2._col<<std::endl;
			std::cout<<"multsData error"<<std::endl;
			exit(-1);
		}
		unsigned int i = 0, j = 0;
		Matrix dotmults(matrix1._row, matrix2._col, 0);
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix2._col; j++)
			{
				dotmults._data[i][j] = matrix1._data[i][j] * matrix2._data[i][j];
			}
		}
		return dotmults;
	}

	//����ʽ
	double Matrix::detMatrix()
	{
		if(_row!=_col)
		{
			std::cout<<"Data det is no"<<std::endl;
			exit(-1);
		}
		Matrix mCopy = this->copyMatrix();
		double det = 1;
		unsigned int i = 0, j = 0, k = 0;
		double max = -9999999;
		int swap = -1;
		double temp;
		ColData cda(_col);
		Data aij(_row,cda);
		for(k = 0; k < mCopy._col-1; k++)//k��ʾ��k����Ԫ��һ����Ҫn-1��
		{
			for(i = 0; i < mCopy._row; i++)
			{
				if(mCopy._data[i][k] > max)//ÿһ����Ԫ���ǱȽϵ�k�е�Ԫ�أ�ѡ����k��������һ��
				{
					swap = i;
				}
			}//�ҵ���k������Ԫ��ȥ������е��±�
			if(swap == -1 || mCopy._data[swap][k] == 0)
				return -1;//�����ԪΪ0
			for(j = 0; j < mCopy._col; j++)
			{
				temp = mCopy._data[k][j];
				mCopy._data[k][j] = mCopy._data[swap][j];
				mCopy._data[swap][j] = temp;
			}//��k����Ԫ��ѡ������һ����swap�У����k�н���
			for(i = k+1; i < mCopy._row; i++)
			{
				aij[i][k] = mCopy._data[i][k] / mCopy._data[k][k];// ��k����Ԫ����Ԫ��Ϊ��k�е�k�У��ѵ�k�����µ��ж�������Ԫ
				for(j = k; j < mCopy._col; j++)//����k�����µ�ÿһ�е�ÿһ��Ԫ�ض���ȥ��������Ԫ���ӵĳ˻�
				{
					mCopy._data[i][j] -= aij[i][k] * mCopy._data[k][j];
				}
			}
		}
		for(i = 0; i < mCopy._row; i++)
		{
			det *= mCopy._data[i][i];
		}
		//cout<<"det="<<det<<endl;
		return det;
	}
	//��˹��Ԫ��������,�ر�ע�⣬LU�ֽⲻ�ܽ�������ʽ�任
	Matrix Matrix::niMatrix()
	{
		if(_row != _col)
		{
			std::cout << "Data ni is no" << std::endl;
			exit(-1);
		}
		if(this->detMatrix() == 0)//�������������ʽ����������Ԫ��ȥ�ı��˲���������δ��ݲ��ı���һ������
		{
			std::cout << "Data det is no so ni is no" << std::endl;
			exit(-1);
		}
		unsigned int i = 0, j = 0, k = 0;//�������-1���������ض���Ϊint��
		double temp;
		Matrix mCopy = this->copyMatrix();
		Matrix UMatrix = this->copyMatrix();
		Matrix LMatrix = this->copyMatrix();
		Matrix UniMatrix = this->copyMatrix();
		Matrix LniMatrix = this->copyMatrix();
		ColData cda(_col);
		Data aij(_row,cda);
		for(k = 0; k < _col-1; k++)//k��ʾ��k����Ԫ��һ����Ҫn-1��
		{
			for(i = k+1; i < _row; i++)
			{
				aij[i][k] = _data[i][k] / _data[k][k];// ��k����Ԫ����Ԫ��Ϊ��k�е�k�У��ѵ�k�����µ��ж�������Ԫ
				for(j = k; j < _col; j++)//����k�����µ�ÿһ�е�ÿһ��Ԫ�ض���ȥ��������Ԫ���ӵĳ˻�
				{
					_data[i][j] -= aij[i][k] * _data[k][j];
				}
			}
		}
		UMatrix = *this;
		for(j = 0; j < _col; j++)
		{
			for(i = j+1; i <_row; i++)
			{
				temp = 0;
				for(k = 0; k < j; k++)
				{
					temp = LMatrix._data[i][k] * UMatrix._data[k][j];
				}
				LMatrix._data[i][j] = 1.0 / UMatrix._data[j][j] * (mCopy._data[i][j]-temp);
			}
		}
		for(i = 0; i < _row; i++)
		{
			for(j = 0; j < _col; j++)
			{
				if(i == j)
					LMatrix._data[i][j] = 1;
				if(j>i)
					LMatrix._data[i][j] = 0;
			}
		}
		Matrix mults;
		mults = *this;
		mults = mults.multsMatrix(LMatrix,UMatrix);
		Matrix LU = mults;
		//cout << "lu" << endl;
		//mults.print();

		//����u��
		for(j = 0; j < _col; j++)
		{
			for(i = j; (int) i >= 0; i--)
			{
				if(i == j)
					UniMatrix._data[i][j] = 1.0 / UMatrix._data[i][j];
				else
				{
					temp = 0;
					for(k = j; k > i; k--)
					{
						temp += UMatrix._data[i][k] * UniMatrix._data[k][j];
					}
					UniMatrix._data[i][j] = -1.0 / UMatrix._data[i][i]*temp;
				}
			}
			///�ؼ���������������
			for(i = j+1; i < _row; i++)
				UniMatrix._data[i][j] = 0;
		}
		//����l��
		for(j = 0; j < _col; j++)
		{
			for(i = 0; i < _row; i++)
			{
				if(j == i)
					LniMatrix._data[i][j] = 1;
				else
				{
					temp = 0;
					for(k = j; k < i; k++)
					{
						temp += (LMatrix._data[i][k] * LniMatrix._data[k][j]);
					}
					LniMatrix._data[i][j] = -temp;
				}
			}
		}

		mults = mults.multsMatrix(UniMatrix,LniMatrix);
		*this = mCopy;
		Matrix I = *this;
		I = I.multsMatrix(LU,mults);
		//cout<<"LU"<<"*"<<"LUni"<<endl;
		//I.print();
		return mults;
	}
	/*unsigned int LDL(Data x)//�����LDL�ֽ⣬��֪���������ھ�������ֵ�������������
	{
		Data l;
		l.initData(&l,x._col,x._row);
		Data d;
		d.initData(&d,x._col,x._row);
		unsigned int i,j,k;
		Data temp;
		temp.initData(&temp,x._col,x._row);
		for(i=0;i<x._col;i++)
		{
			l.mat[i][i]=1;
			for(j=0;j<i;j++)
			{
				for(k=0;k<j;k++)
				{
					temp.mat[i][k]=l.mat[i][k]*d.mat[k][k];
					temp.mat[i][j]-=temp.mat[i][k]*l.mat[j][k];
				}
				temp.mat[i][j]=temp.mat[i][j]+x.mat[i][j];
				l.mat[i][j]=temp.mat[i][j]/d.mat[j][j];
			}
			d.mat[i][i]=x.mat[i][i];
			for(k=0;k<i;k++)
			{
				d.mat[i][i]-=temp.mat[i][k]*l.mat[i][k];
			}
		}
		for(i=0;i<x._col;i++)
		{
			for(j=0;j<x._row;j++)
			{
				std::cout<<l.mat[i][j]<<"  ";
			}
			std::cout<<std::endl;
		}
		for(i=0;i<x._col;i++)
		{
			for(j=0;j<x._row;j++)
			{
			std::cout<<d.mat[i][j]<<"  ";
			}
			std::cout<<std::endl;
		}
	}*/
}

