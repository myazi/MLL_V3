#include "Hash.h"

namespace MLL
{

	/***数据归一化处理，_data[i][j]-min[j]/range[j]**/
	int Hash::Normalize(Matrix &x)
	{
		int j = 0, i = 0;
		Matrix minVals(1, x._col, 0);
		Matrix maxVals(1, x._col, 0);
		Matrix ranges(1, x._col, 0);
		for(j = 0; j < x._col; j++)
		{
			minVals._data[0][j] = x._data[0][j];
			maxVals._data[0][j] = x._data[0][j];
		}
		for(i = 0; i < x._row; i++)
		{
			for(j = 0; j < x._col; j++)
			{
				if(x._data[i][j] < minVals._data[0][j])
					minVals._data[0][j] = x._data[i][j];
				if(x._data[i][j] > maxVals._data[0][j])
					maxVals._data[0][j] = x._data[i][j];
			}
		}
		for(j = 0; j < x._col; j++)
			ranges._data[0][j] = maxVals._data[0][j] - minVals._data[0][j];
		for(i = 0; i < x._row; i++)
		{
			for(j = 0; j < x._col; j++)
			{
				x._data[i][j] -= minVals._data[0][j];
				if(ranges._data[0][j] != 0)
					x._data[i][j] /= ranges._data[0][j];
			}
		}
		return 0;
	}
	void Hash::BCSH2()
	{
		Matrix tmpB1 = _B;
		Matrix tmpB0 = _B;

		Matrix PX1_B1;
		Matrix PX1_B0;

		Matrix logPX1_B1;
		Matrix logPX1_B0;

		Matrix logPB1;
		Matrix logPB0;
		Matrix B1_B0;
		Matrix PXB1;
		Matrix Fx;
		
		double sum_row = 0;

		for(int it = 0; it < _iter; it++)
		{
			for(size_t i = 0; i < _B._row; i++)
				for (size_t j = 0; j < _B._col; j++)
					tmpB1._data[i][j] = _B._data[i][j] > 0 ? 1 : 0; // _Bij = 1 作为一类放到矩阵tmpB1中

			for(size_t i = 0; i < _B._row; i++)
				for (size_t j = 0; j < _B._col; j++)
					tmpB0._data[i][j] = _B._data[i][j] < 0 ? 1 : 0; // _Bij = -1 作为一类放到矩阵tmpB0中
			
			PX1_B1 = tmpB1 * _X.transposeMatrix();//B=1下X=1的参数
			for(int i = 0; i < PX1_B1._row; i++)
			{
				sum_row = 0;
				for(int j = 0; j < PX1_B1._col; j++)
					sum_row += PX1_B1._data[i][j];
				for(int j = 0; j < PX1_B1._col; j++)
					PX1_B1._data[i][j] = (PX1_B1._data[i][j] + 1.0) / (sum_row + PX1_B1._col);
			}
			
			PX1_B0 = tmpB0 * _X.transposeMatrix();// B=0下X=1的参数
			for(int i = 0; i < PX1_B0._row; i++)
			{
				sum_row = 0;
				for(int j = 0; j < PX1_B0._col; j++)
					sum_row += PX1_B0._data[i][j];
				for(int j = 0; j < PX1_B0._col; j++)
					PX1_B0._data[i][j] = (PX1_B0._data[i][j] + 1.0) / (sum_row + PX1_B0._col);
			}
			
			//logPX1_B1 = np.log2(PX1_B1)
			//logPX1_B0 = np.log2(PX1_B0)
			
			logPB1 = PX1_B1 * _X;//贝叶斯参数PX1_B1乘上样本X
			logPB0 = PX1_B0 * _X;//贝叶斯参数PX1_B1乘上样本X

			B1_B0 = (logPB1 - logPB0);
			for(size_t i = 0; i < B1_B0._row; i++)
				for (size_t j = 0; j < B1_B0._col; j++)
					B1_B0._data[i][j] = B1_B0._data[i][j] > 32 ? 32 : B1_B0._data[i][j]; // 对B1/B2最大值做限制
			
			PXB1 = B1_B0;
			Fx = PXB1;
			for(size_t i = 0; i < B1_B0._row; i++)
				for(size_t j = 0; j < B1_B0._col; j++)
				{
					PXB1._data[i][j] = pow(2, B1_B0._data[i][j]);
					PXB1._data[i][j] = PXB1._data[i][j] / (1.0 + PXB1._data[i][j]);
					Fx._data[i][j] = PXB1._data[i][j] * 2 - 1;
				}
			//Y = update(_B)
			for(size_t i = 0; i < B1_B0._row; i++)
				for (size_t j = 0; j < B1_B0._col; j++)
					_B._data[i][j] = Fx._data[i][j] > 0 ? 1 : -1; // 对B1/B2最大值做限制
			_B.print();	
		}
	}
	void Hash::init_B()
	{
		Matrix tmp_B;
		tmp_B.init_by_random(_bits,_X._col);
		for(size_t i = 0; i < tmp_B._row; i++)
			for (size_t j = 0; j < tmp_B._col; j++)
				tmp_B._data[i][j] = tmp_B._data[i][j] > 0.5 ? 1 : -1;
		_B = tmp_B;
	}
	Hash::Hash(const std::string &file, const std::string &model, const int &bits, const double &lambda, const int &iter)
	{

		_X.init_by_data(file);
		_X.print();
		Normalize(_X);//一行为一个样本，按列归一化
		_X = _X.transposeMatrix();//转化为（n * m）n为维度，m为样本数
		_bits = bits;
		_iter = iter;
		_lambda = lambda;
		
		if("BCSH2" == model)
		{
			init_B();
			_B.print();
			BCSH2();
		}
		else
		{
			;
		}
	}

}
