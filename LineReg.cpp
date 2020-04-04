#include "LineReg.h"

namespace MLL{

    /**

    ���Իع麯����ʵ�֣�����һ������Իع飬��Сƽ������Ϊ��ʧ��������Ŀ�꺯����һ����Լ����͹���ι滮���⣬
    ��͹���ι滮����ļ�Сֵ�ڵ���Ϊ0��ȡ�����Ҽ�СֵΪȫ����Сֵ�����б�ʽ�⡣������ѧ���ʽʵ�־���֮���������ò���w��
    **/
    int LineReg::regression()
    {
        Matrix xT = _x.transposeMatrix();
        Matrix xTx = xTx.multsMatrix(xT,_x);
        Matrix xTx_1 = xTx.niMatrix();
        Matrix xTx_1xT = xTx_1xT.multsMatrix(xTx_1,xT);
        Matrix ws;
        ws=ws.multsMatrix(xTx_1xT,_y);
        std::cout<<"ws"<<std::endl;
        ws.print();
		Matrix pre_y = _x * ws; 
		for(int i = 0; i < _y._row; i++)
		{
			std::cout << "pre_y:" << pre_y._data[i][0] << "\t" << "fact_y:" << _y._data[i][0]<< "\t" << "error=" << pre_y._data[i][0] - _y._data[i][0] << std::endl;
		}
        return 0;
    }
    
	/**
    �������ع麯��ֻ����һ������Իع麯���Ļ������ڶԽ�������������ĸ�������н�����󲻿�������ԣ�ͬ��Ҳ���������Ŀ�ģ�
    ���ó��õĶ������͵õ���ֱ������lam����ʽ��
    **/
    int LineReg::ridgeRegres()
    {
        Matrix xT= _x.transposeMatrix();
        Matrix xTx=xTx.multsMatrix(xT,_x);
		Matrix denom(xTx._row,xTx._col,_lamd,"diag");
        xTx=xTx.addMatrix(xTx,denom);
        Matrix xTx_1=xTx.niMatrix();
        Matrix xTx_1xT=xTx_1xT.multsMatrix(xTx_1,xT);
        Matrix ws=ws.multsMatrix(xTx_1xT,_y);
        std::cout<<"ws"<<std::endl;
        ws.print();
        return 0;
    }

    /**
    �ֲ���Ȩ���Իع��������Իع�Ļ����϶�ÿһ������������ѵ����ʱ�����ÿһ��ѵ���������������е���������һ����Ȩ��ϣ�
    Ȩ�ص�ȷ������ͨ��һ���������㣬���õ��и�˹�ˣ����������Խ����Ȩ��Խ�󣬷�֮ԽС����������ÿһ�����������͵õ��˲�һ����
    Ȩ���������������ó���������߲��������Ե��ˣ����������ӵ�ģ�͵ĸ��Ӷ������õ���Ϸ��������ݡ�

    **/
    //��Ҫע����Ǿֲ���Ȩ���Իع��Ƕ�ÿһ����������Ȩ�ؼ��㣬���Զ���ÿһ����������һ��Ȩ��w����������ĺ���ֻ�Ǿֲ����Իع��һ����Ҫ��������
    Matrix LineReg::locWeightLineReg(Matrix &test)
    {
        Matrix w(_x._row,_x._row,0);
        double temp=0;
        int i = 0, j = 0;

        /**
        ���ݲ��������������������ľ����Ѿ�ѡ��ĺ�ȷ���ֲ���Ȩ���󣬲��öԽ�����Ϊ�ֲ���Ȩֵ
        **/
        for(i = 0;i < _x._row;i++)
        {
            temp = 0;
            for(j = 0;j < _x._col;j++)
            {
                temp += (test._data[0][j] - _x._data[i][j]) * (test._data[0][j] - _x._data[i][j]);
            }
            w._data[i][i] = exp(temp / -2.0 *_k * _k);
        }
        Matrix xT = _x.transposeMatrix();
        Matrix wx = wx.multsMatrix(w,_x);
        Matrix xTwx;
        xTwx = xTwx.multsMatrix(xT,wx);
        Matrix xTwx_1;
        xTwx_1 = xTwx.niMatrix();
        Matrix xTwx_1xT;
        xTwx_1xT = xTwx_1xT.multsMatrix(xTwx_1,xT);
        Matrix xTwx_1xTw;
        xTwx_1xTw = xTwx_1xTw.multsMatrix(xTwx_1xT,w);
        Matrix ws = xTwx_1xTw * _y;
        return ws;
    }
    /**
    ͨ����������ľֲ���Ȩ���Իع麯��ʵ�ֶ�ÿһ����������ȷ�����Ĳ�����ͬ��������������ʽ�洢����n��������������n�У���m��������һ��m�У�
    **/

    int LineReg::lwlrtest()
    {
        int i,j;
        Matrix test;
        Matrix wsOne;
        Matrix ws;
        ws = _x.copyMatrix();
        /**
        ÿһ�������������þֲ���Ȩ���Իع�
        **/
        for(i=0; i < _x._row;i++)///ws����Ϊһ��Ȩ�ز���������m�б�ʾm������
        {
            test = _x.getOneRow(i);
            wsOne = locWeightLineReg(test);
            for(j = 0; j < _x._col; j++)
            {
                ws._data[i][j] = wsOne._data[0][j];
            }
        }
        for(i = 0; i < ws._row; i++)
        {
            for(j = 0; j < ws._col; j++)
            {
                std::cout<<ws._data[j][i]<<"  ";
            }
            std::cout<<std::endl;
        }

        /**��֤�ֲ���Ȩ���Իع����ȷ��*/
        double yy;
        for(i = 0; i < ws._row;i++)
        {
            yy = 0;
            for(j = 0; j < ws._col; j++)
            {
                yy += ws._data[j][i] * _x._data[i][j];
            }
            std::cout<<"y="<<yy<<std::endl;
        }
        return 0;
    }

    LineReg::LineReg(const std::string &file, const std::string &model, const double &lamd, const double &k)
    {
        _x.init_by_data(file);
        _y = _x.getOneCol(_x._col-1);
        _x.deleteOneCol(_x._col-1);
		std::cout << "x:" << _x._row << "&" << _x._col << std::endl;
		std::cout << "y:" <<  _y._row << "&" << _y._col << std::endl;
        _lamd = lamd;
        _k = k; 
    }
}
