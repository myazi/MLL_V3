#include "SoftMaxReg.h"

namespace MLL {
    /**
    �ݶ��½��㷨����Ҫ��ȷ�����ݶȷ��򣬲��������õ�����˼�����n��������
    ��Ŀ�꺯����͹�滮���⣬��ô�ֲ���Сֵ����ȫ����Сֵ

    **/
    int SoftMaxReg::gradAscent_SoftMax()
    {
        Matrix weights(_x._col,_y._col,0.1);
        Matrix xT = _x.transposeMatrix();

        //float alpha=0.001;///��������
        float error=0;///��¼������
        int iter=0;
        int i,j;
        Matrix z(_y._row,_y._col,0);//���ȷ������Ĵ�С
        Matrix z_rowSum(z._row,1,0);//����ά����ͣ���һ��Ϊ������ʽ
        Matrix grad(_x._col,_y._col,0);
        for(iter=0; iter<_iter; iter++)
        {
            z = _x * weights;
            for(i=0; i<z._row; i++)
            {
                z_rowSum._data[i][0]=0;
                for(j=0;j<z._col;j++)
                {
					MLL::sigmoid(z._data[i][j]);
                    z_rowSum._data[i][0]+=z._data[i][j];//���
                }
                for(j=0;j<z._col;j++)
                {
                    z._data[i][j]/=z_rowSum._data[i][0];//��һ��
                    //if(iter%5000==0)
                        //std::cout<<z._data[i][j] <<"  ";
                }
                //if(iter%5000==0)
                    //std::cout<<std::endl;
            }
            z = _y - z;

            grad = xT * z;///���㸺�ݶȷ���
            //grad = (1.0/x._row) * grad;
            for(i=0; i<grad._row; i++)
            {
                for(j=0;j<grad._col; j++)
                {
                    grad._data[i][j]*= _alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
                }
            }
            weights = weights + grad;///�����ݶȷ�����һ������
            //weights.print();
        }
        /**
        ��֤�㷨����ȷ��
        **/
        Matrix test=_x * weights;
        std::cout<<"test"<<std::endl;
        for(i=0; i<_y._row; i++)
        {
            //for(j=0; j<y._col; j++)
            if(test._data[i][0]>test._data[i][1])
                std::cout<<0-_y._data[i][1]<<"  ";
            else
                std::cout<<1-_y._data[i][1]<<"  ";
            std::cout<<std::endl;
        }
    }
    /**
    ����ݶ��½����ݶ��½�����ͬ�����ڸ��ݶȷ����ȷ�����ݶ��½��Ǹ������е�������ȷ�����ݶȷ���
    ������ݶ��½�ÿ��ֻ��һ����������ȷ�����ݶȷ�����Ȼ����ȫ���ţ������ŵ����������ӣ�ͬ������

    **/
    int SoftMaxReg::stoGradAscent_SoftMax()//����ݶ��½�ÿһ��ѡ��m�������������ݶ��½����򣬸ô�����ֻѡ��һ��������������ݶ��½���������ֵ
    {
        Matrix xOne_row(1,_x._col,0);
        Matrix xOne_rowT(_x._col,1,0);

        Matrix weights(_x._col,_y._col,0.1);
        Matrix z(1,_y._col,0);//���ȷ������Ĵ�С
        Matrix grad(_x._col,_y._col,0);
        double z_rowSum=0;
        //double alpha=0.001;///����
        double error;
        int i,j,k,iter;
        for(iter=0; iter<_iter; iter++)
        {
            for(i=0; i<_x._row; i++)
            {
                xOne_row=_x.getOneRow(i);///���ѡ��һ�������㣬����û�������ѡ�񣬶��ǰ���ѡ��
                z = xOne_row * weights;
                z_rowSum=0;
                for(j=0;j<z._col;j++)
                {
                    sigmoid(z._data[0][j]);
                    z_rowSum+=z._data[0][j];//���
                }
                for(j=0;j<z._col;j++)
                {
                    z._data[0][j]/=z_rowSum;//��һ��
                    if(iter%1000==0)
                        std::cout<<z._data[0][j] <<" s ";
                }
                if(iter%1000==0)
                    std::cout<<std::endl;
                for(j=0;j<_y._col;j++)
                {
                    z._data[0][j]=_y._data[i][j]-z._data[0][j];
                }
                xOne_rowT = xOne_row.transposeMatrix();
                grad = xOne_rowT * z;///����һ��������Ԥ�������ȷ�����ݶȷ���
                for(k=0; k<grad._row;k++)
                {
                    for(j=0;j<grad._col; j++)
                    {
                        grad._data[k][j]*= _alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
                    }
                }
                weights = weights + grad; ///����
            }
        }
        //��֤�㷨����ȷ��
        /**
        ��֤�㷨����ȷ��
        **/
        Matrix test = _x * weights;
        std::cout<<"test"<<std::endl;
        for(i=0; i < _y._row; i++)
        {
            if(test._data[i][0]>test._data[i][1])
                std::cout<<0 - _y._data[i][1]<<"  ";
            else
                std::cout<<1 - _y._data[i][1]<<"  ";
            std::cout<<std::endl;
        }
    }

    SoftMaxReg::SoftMaxReg(const std::string &file, const std::string &model, const double &alpha, const int &iter)
    {
        std::cout<<"load_data"<<std::endl;
        std::cout<<"----------------------"<<std::endl;
        _x.init_by_data(file);
        _y=_x.getOneCol(_x._col-1);
		one_hot(_y,2);
        //_y.print();
		_x.deleteOneCol(_x._col-1);
        _alpha = alpha;
        _iter = iter;
    }
}
