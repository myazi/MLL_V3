#include "SVD.h"

namespace MLL{

	void SVD::init_uv()
	{
		int m = _A._row;
		int n = _A._col;
		int d = _d;
		_u.init_by_random(m, d);
		_v.init_by_random(d, n);
		_u.print();	
		_v.print();	
	}
	void SVD::init_uv_sparse(const int &row, const int &col)
	{
		int m = row;
		int n = col;
		int d = _d;
		_u.init_by_random(m, d);
		_v.init_by_random(d, n);
		_u.print();	
		_v.print();	

	}
	int SVD::Sparse_MF()//稀疏矩阵分解 _A必须为三元组表示(i,j,value)
	{
		double error = 0;
		double loss = 0;
		int it = 0;
		int i = 0, j = 0, k = 0;

		Matrix ui;//行向量
        Matrix uiT;
		Matrix vj;//列向量
        Matrix vjT;
		Matrix Aij;//1 * 1矩阵
        Matrix grad_ui;
        Matrix grad_vj;
		/*
		 *稀疏矩阵分解 S = UV
		 */
		for(it = 0; it < _iter; it++)
		{
			error = 0;
			for(k = 0; k < _A._row; k++)
			{
				i = _A._data[k][0];//三元组i行
				j = _A._data[k][1];//三元组j列
				ui = _u.getOneRow(i);
				vj = _v.getOneCol(j);
				uiT = ui.transposeMatrix();//u矩阵第i行转置
				vjT = vj.transposeMatrix();//v矩阵第j列转置
				Aij = ui * vj;
				
				loss = _A._data[k][2] - Aij._data[0][0];//(_A - Aij)^2对Aij负梯度方向，也是损失
				grad_ui = _alpha * loss * vjT;//步长 * (_A - Aij)^2对ui负梯度
				grad_vj = _alpha * loss * uiT;//步长 * (_A - Aij)^2对vj负梯度
				
				for(int jj = 0; jj < _u._col; jj++)
					_u._data[i][jj] += grad_ui._data[0][jj];
				for(int ii = 0; ii < _v._row; ii++)
					_v._data[ii][j] += grad_vj._data[ii][0];
				error += (loss > 0 ? loss : -loss);
			}
			std::cout << "error" << error << std::endl;
		}
		std::cout << "=======A=========" << std::endl;
		_A.print();
		std::cout << "=======u=========" << std::endl;
		_u.print();
		std::cout << "=======v=========" << std::endl;
		_v.print();
		std::cout << "=======u * v=========" << std::endl;
		Matrix A_bar = _u * _v;
		A_bar.print();
	}

	int SVD::MF()
    {
		float error = 0;///记录错误率
        int it = 0;
        int i = 0, j = 0;

        Matrix grad_u;
        Matrix grad_v;
        Matrix A_bar;
        Matrix Loss_A;
        Matrix uT;
        Matrix vT;
        
		/*
		 *矩阵分解 S = UV
		 */
		for(it = 0; it < _iter; it++)
        {
			vT = _v.transposeMatrix();
			uT = _u.transposeMatrix();
			A_bar = _u * _v;
			Loss_A = (_A - A_bar);//(_A - A_bar)^2对A_bar负梯度方向
            
			grad_u = Loss_A * vT;//计算矩阵u负梯度方向
			grad_u = _alpha * grad_u;
			
			grad_v = uT * Loss_A;//计算矩阵v负梯度方向
			grad_v = _alpha * grad_v;

            _u = _u + grad_u;///往负梯度方向走一个步长
            _v = _v + grad_v;///往负梯度方向走一个步长
			error = 0;
			for(i = 0; i < Loss_A._row; i++)
				for(j = 0; j < Loss_A._col; j++)
					error += (Loss_A._data[i][j] > 0 ? Loss_A._data[i][j] : -Loss_A._data[i][j]);
			std::cout << "error" << error << std::endl;
        }

		std::cout << "=======A=========" << std::endl;
		_A.print();
		std::cout << "=======u=========" << std::endl;
		_u.print();
		std::cout << "=======v=========" << std::endl;
		_v.print();
		std::cout << "=======u * v=========" << std::endl;
		A_bar.print();

    }
	
	int SVD::NMF()//非负矩阵分解
    {
		float error = 0;///记录错误率
        int it = 0;
        int i = 0, j = 0;
        Matrix uT;
        Matrix vT;
        Matrix A_bar;
        Matrix Loss_A;

		/*
		 *非负矩阵分解 S = UV
		 */
		for(it = 0; it < _iter; it++)
        {
			vT = _v.transposeMatrix();
			uT = _u.transposeMatrix();
			
			for(i = 0; i < _u._row; i++)
				for(j = 0; j < _u._col; j++)
					_u._data[i][j] = _u._data[i][j] * (_A * vT)._data[i][j] / (_u * _v * vT)._data[i][j];
			
			for(i = 0; i < _v._row; i++)
				for(j = 0; j < _v._col; j++)
					_v._data[i][j] = _v._data[i][j] * (uT * _A)._data[i][j] / (uT * _u * _v)._data[i][j];

            error = 0;
			A_bar = _u * _v;
			Loss_A = (_A - A_bar);
			for(i = 0; i < Loss_A._row; i++)
				for(j = 0; j < Loss_A._col; j++)
					error += (Loss_A._data[i][j] > 0 ? Loss_A._data[i][j] : -Loss_A._data[i][j]);
			std::cout << "error" << error << std::endl;
        }

		std::cout << "=======A=========" << std::endl;
		_A.print();
		std::cout << "=======u=========" << std::endl;
		_u.print();
		std::cout << "=======v=========" << std::endl;
		_v.print();
		std::cout << "=======u * v=========" << std::endl;
		A_bar.print();

    }
	SVD::SVD(const std::string &file, const std::string &model, const int &d,  const double &alpha, const int &iter)
	{
		_A.init_by_data(file);
		//_A.print();
		_d = d;
		_alpha = alpha;
		_iter = iter;
		if("normal" == model)
		{
			init_uv();
			MF();
			init_uv();
			NMF();
		}
		else
		{
			int row = 1183;
			int col = 1183;
			init_uv_sparse(row, col);
			Sparse_MF();
		}
	}
}
