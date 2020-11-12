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
		std::cout << "(const A& rhs)" << std::endl;
	}
	Matrix::Matrix(const size_t  &row, const size_t  &col, const float &init_val)
	{
		if(row == 0 || col == 0)
		{
			std::cout<<"Structor is zero"<<std::endl;
			return ;
		}
		RowData cda(col);
		Data da(row,cda);
		this->_data = da;
		this->_row = row;
		this->_col = col;
		size_t  irow = 0, jcol = 0;
		for(irow = 0; irow < _row; irow++)
		{
			for(jcol = 0; jcol < _col; jcol++)
				this->_data[irow][jcol] = init_val;
		}
	}
	Matrix::Matrix(const size_t  &row, const size_t  &col, const float &init_val, const std::string &type)
	{
		if(row == 0 || col == 0)
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
		size_t  index = 0;
		// ��ʼ��diag����
		for(index = 0; index < _row; index++)
		{
			this->_data[index][index] = init_val;//����
		}
	}
	void Matrix::initMatrix(const size_t  &row, const size_t  &col, const float &init_val)
	{
		if(row == 0 || col == 0)
		{
			std::cout<<" Structor no zero"<<std::endl;
			return ;
		}
		RowData cda(col);
		Data da(row,cda);
		this->_data = da;
		this->_row = row;
		this->_col = col;
		size_t  irow = 0, jcol = 0;
		for(irow = 0; irow < _row; irow++)
		{
			for(jcol = 0; jcol < _col; jcol++)
				this->_data[irow][jcol] = init_val;
		}
	}
	void Matrix::init_by_random(const size_t &row, const size_t &col)
	{
		RowData cda(col);
		Data da(row,cda);
		this->_data = da;
		this->_row = row;
		this->_col = col;
		size_t  irow = 0, jcol = 0;
		double random = 0.0;
		for(irow = 0; irow < _row; irow++)
		{
			for(jcol = 0; jcol < _col; jcol++)
			{
				random = (rand() % 100) / 100.0;
				_data[irow][jcol] = random;
			}
		}
	}

	void Matrix::init_by_spare(const std::string &filename, const size_t  &row, const size_t  &col)
	{
		//initMatrix(row, col, 0)//����initMatrix������ʼ��
		RowData cda(col);
		Data da(row,cda);
		this->_data = da;
		this->_row = row;
		this->_col = col;
		size_t  irow = 0, jcol = 0;
		for(irow = 0; irow < this->_row; irow++)
		{
			for(jcol = 0; jcol < this->_col; jcol++)
			{
				this->_data[irow][jcol] = 0;
			}
		}
		LoadData_spare(this->_data, filename);
	}
	void Matrix::init_by_data(const std::string &filename)
	{
		LoadData(this->_data,filename);
		_row = this->_data.size();//data���ݵ�����
		_col = this->_data[0].size();//data��������

		size_t  irow = 0;
		for (irow = 0; irow < _row; irow++)
		{
			if(_col > _data[irow].size()) //���ݶ�������
				std::cout << "loaddata is error col" << std::endl;
		}
	}
	void Matrix::print() const
	{
		std::cout<< "matrix size:" << _row<<"*"<< _col<<std::endl;
		size_t  irow = 0, jcol = 0;
		for(irow = 0; irow < _row; irow++)
		{
			for(jcol = 0; jcol < _col; jcol++)
			{
				//if(jcol == irow){std::cout<< _data[irow][jcol] <<"  ";}
				std::cout<< _data[irow][jcol] <<"  ";
			}
			std::cout<<std::endl;
		}
	}
	Matrix Matrix::copyMatrix() const
	{
		Matrix cp(_row, _col, 0);
		size_t  irow = 0, jcol = 0;
		for(irow = 0; irow < this->_row; irow++)
		{
			for(jcol = 0; jcol < this->_col; jcol++)
			{
				cp._data[irow][jcol] = this->_data[irow][jcol];
            }
		}
		return cp;
    }
	Matrix Matrix::getOneRow (const size_t  &iRow) const
	{
		size_t  jcol = 0;
		Matrix one_row_matrix(1, _col,0);
		for(jcol = 0; jcol < _col; jcol++)
		{
			one_row_matrix._data[0][jcol] = this->_data[iRow][jcol];
		}
		return one_row_matrix;
	}
	Matrix Matrix::getOneCol (const size_t  &jCol) const 
	{
		size_t  irow = 0;
		Matrix one_col_matrix(_row,1,0);
		for(irow = 0; irow < _row; irow++)
		{
			one_col_matrix._data[irow][0] = this->_data[irow][jCol];
		}
		return one_col_matrix;
	}
	void Matrix::deleteOneRow(const size_t  &iRow)
	{
		//������� data���ݲ��ͷţ�ֻrow--,�����ھ���copyʱ����ʼ��ʱ��
		Matrix cp = this->copyMatrix();
		//Matrix cp = *this;
		this->_row--;
		size_t  irow = 0, jcol = 0;
		for(Data::iterator it = cp._data.begin(); it != cp._data.end(); it++, irow++)
		{
			if(irow < iRow)
			{
				for(std::vector<float>::iterator itRow = it->begin(); itRow != it->end(); itRow++, jcol++)
				{
					this->_data[irow][jcol] = *itRow;
				}
			}
			if(irow > iRow)
			{
				for(std::vector<float>::iterator itRow = it->begin(); itRow != it->end(); itRow++, jcol++)
				{
					this->_data[irow-1][jcol] = *itRow;
				}
			}
		}
	}
	/*Matrix Matrix::deleteOneRow(const size_t  &iRow)
	{
		//������� data���ݲ��ͷţ�ֻrow--,�����ھ���copyʱ����ʼ��ʱ��
		//Matrix cp = this->copyMatrix();
		Matrix cp = *this;
		this->_row--;
		size_t  irow = 0, jcol = 0;
		for(Data::iterator it = cp._data.begin(); it != cp._data.end(); it++, irow++)
		{
			if(irow < iRow)
			{
				for(std::vector<float>::iterator itRow = it->begin(); itRow != it->end(); itRow++, jcol++)
				{
					this->_data[irow][jcol] = *itRow;
				}
			}
			if(irow > iRow)
			{
				for(std::vector<float>::iterator itRow = it->begin(); itRow != it->end(); itRow++, jcol++)
				{
					this->_data[irow-1][jcol] = *itRow;
				}
			}
		}
	}*/
	void Matrix::deleteOneCol(const size_t  &jCol)
	{

		//Matrix cp = this->copyMatrix();
		Matrix cp = *this;
		this->_col--;

		size_t  irow = 0, jcol = 0;
		for(Data::iterator it = cp._data.begin(); it != cp._data.end(); it++, irow++)
		{
			jcol = 0;
			for(std::vector<float>::iterator itRow = it->begin(); itRow != it->end(); itRow++, jcol++)
			{
				if(jcol < jCol)
				{
					this->_data[irow][jcol] = *itRow;
				}
				if(jcol > jCol)
				{
					this->_data[irow][jcol-1] = *itRow;
				}
			}

		}
	}
	Matrix Matrix::transposeMatrix()//������ʽ��ת��
	{
		Matrix matrixT(_col,_row,0);
		size_t  irow = 0, jcol = 0;
		for(irow = 0; irow < _row; irow++)
		{
			for(jcol = 0; jcol < _col; jcol++)
			{
				matrixT._data[jcol][irow] = this->_data[irow][jcol];
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
		size_t  irow = 0, jcol = 0;
		Matrix add(matrix1._row, matrix1._col,0);
		for(irow = 0; irow < matrix1._row; irow++)
		{
			for(jcol = 0; jcol < matrix1._col; jcol++)
			{
				add._data[irow][jcol] = matrix1._data[irow][jcol] + matrix2._data[irow][jcol];
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
		size_t  irow = 0, jcol = 0;
		Matrix sub(matrix1._row, matrix1._col,0);
		for(irow = 0; irow < matrix1._row; irow++)
		{
			for(jcol = 0; jcol < matrix1._col; jcol++)
			{
				sub._data[irow][jcol] = matrix1._data[irow][jcol] - matrix2._data[irow][jcol];
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
		size_t  irow = 0, jcol = 0, k = 0;
		Matrix mults(matrix1._row,matrix2._col,0);
		for(irow = 0; irow < matrix1._row; irow++)
		{
			for(jcol = 0; jcol < matrix2._col; jcol++)
			{
				mults._data[irow][jcol] = 0;
			}
		}
		for(irow = 0; irow < matrix1._row; irow++)
		{
			for(jcol = 0; jcol < matrix2._col; jcol++)
			{
				for( k = 0; k < matrix1._col; k++)
				{
					mults._data[irow][jcol] += matrix1._data[irow][k] * matrix2._data[k][jcol];
				}
			}
		}
		return mults;
	}

	Matrix Matrix::dotmultsMatrix(const Matrix &matrix1, const Matrix &matrix2)//�����Ӧ��ˣ��������
	{
		if(matrix1._row != matrix2._row || matrix1._col != matrix2._col)
		{
			std::cout<<matrix1._row<<"*"<<matrix1._col<<std::endl;
			std::cout<<matrix2._row<<"*"<<matrix2._col<<std::endl;
			std::cout<<"multsData error"<<std::endl;
			exit(-1);
		}
		size_t  irow = 0, jcol = 0;
		Matrix dotmults(matrix1._row, matrix2._col, 0);
		for(irow = 0; irow < matrix1._row; irow++)
		{
			for(jcol = 0; jcol < matrix2._col; jcol++)
			{
				dotmults._data[irow][jcol] = matrix1._data[irow][jcol] * matrix2._data[irow][jcol];
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
		size_t  i = 0, j = 0, k = 0;
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
			std::cout << "Data ni is no" << "row * col:" << _row << "*" << _col << std::endl;
			exit(-1);
		}
		if(this->detMatrix() == 0)//�������������ʽ����������Ԫ��ȥ�ı��˲���������δ��ݲ��ı���һ������
		{
			std::cout << "Data det is no so ni is no" << std::endl;
			exit(-1);
		}
		size_t  i = 0, j = 0, k = 0;//�������-1���������ض���Ϊint��
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
		//UMatrix = *this;
		for(j = 0; j < _col; j++)
		{
			for(i = j+1; i <_row; i++)
			{
				temp = 0;
				for(k = 0; k < j; k++)
				{
					temp = LMatrix._data[i][k] * UMatrix._data[k][j];
				}
				if(UMatrix._data[j][j] == 0) {UMatrix._data[j][j] = 0.000000001; std::cout << "yingwenjie" << j<<std::endl;}//��ֹ�Գƾ������Խ�Ԫ��Ϊ0�����
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
		//mults = *this;
		mults = mults.multsMatrix(LMatrix,UMatrix);
		std::cout << "Lmatrix" <<std::endl;
		LMatrix.print();
		std::cout << "Umatrix" <<std::endl;
		UMatrix.print();
		Matrix LU = mults;
		std::cout << "lu" <<std::endl;
		mults.print();

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
		//*this = mCopy;
		Matrix I;// = *this;
		I = I.multsMatrix(LU,mults);
		std::cout<<"LU"<<"*"<<"LUni"<<std::endl;
		I.print();
		return mults;
	}
	/*size_t  LDL(Data x)//�����LDL�ֽ⣬��֪���������ھ�������ֵ�������������
	{
		Data l;
		l.initData(&l,x._col,x._row);
		Data d;
		d.initData(&d,x._col,x._row);
		size_t  i,j,k;
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

