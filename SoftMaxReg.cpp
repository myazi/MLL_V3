#include "SoftMaxReg.h"

namespace MLL {
    /**
    �ݶ��½��㷨����Ҫ��ȷ�����ݶȷ��򣬲��������õ�����˼�����n��������
    ��Ŀ�꺯����͹�滮���⣬��ô�ֲ���Сֵ����ȫ����Сֵ

    **/
    int SoftMaxReg::gradAscent_SoftMax()
    {
        Matrix weights(_x.col,_y.col,0.1,"T");
        Matrix xT = _x.transposeMatrix();

        //float alpha=0.001;///��������
        float error=0;///��¼������
        int iter=0;
        int i,j;
        Matrix z(_y.row,_y.col,0,"T");//���ȷ������Ĵ�С
        Matrix zRowSum(z.row,1,0,"ss");//����ά����ͣ���һ��Ϊ������ʽ
        Matrix grad(_x.col,_y.col,0,"T");
        for(iter=0; iter<_iter; iter++)
        {
            z = _x * weights;
            for(i=0; i<z.row; i++)
            {
                zRowSum.data[i][0]=0;
                for(j=0;j<z.col;j++)
                {
                    sigmoid(&z.data[i][j]);
                    zRowSum.data[i][0]+=z.data[i][j];//���
                }
                for(j=0;j<z.col;j++)
                {
                    z.data[i][j]/=zRowSum.data[i][0];//��һ��
                    //if(iter%5000==0)
                        //cout<<z.data[i][j] <<"  ";
                }
                //if(iter%5000==0)
                    //cout<<endl;
            }
            z = _y - z;

            grad = xT * z;///���㸺�ݶȷ���
            //grad = (1.0/x.row) * grad;
            for(i=0; i<grad.row; i++)
            {
                for(j=0;j<grad.col; j++)
                {
                    grad.data[i][j]*= _alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
                }
            }
            weights = weights + grad;///�����ݶȷ�����һ������
            //weights.print();
        }
        /**
        ��֤�㷨����ȷ��
        **/
        Matrix test=_x * weights;
        cout<<"test"<<endl;
        for(i=0; i<_y.row; i++)
        {
            //for(j=0; j<y.col; j++)
            if(test.data[i][0]>test.data[i][1])
                cout<<0-_y.data[i][1]<<"  ";
            else
                cout<<1-_y.data[i][1]<<"  ";
            cout<<endl;
        }
    }
    /**
    ����ݶ��½����ݶ��½�����ͬ�����ڸ��ݶȷ����ȷ�����ݶ��½��Ǹ������е�������ȷ�����ݶȷ���
    ������ݶ��½�ÿ��ֻ��һ����������ȷ�����ݶȷ�����Ȼ����ȫ���ţ������ŵ����������ӣ�ͬ������

    **/
    int SoftMaxReg::stoGradAscent_SoftMax()//����ݶ��½�ÿһ��ѡ��m�������������ݶ��½����򣬸ô�����ֻѡ��һ��������������ݶ��½���������ֵ
    {
        Matrix xOneRow(1,_x.col,0,"T");
        Matrix xOneRowT(_x.col,1,0,"T");

        Matrix weights(_x.col,_y.col,0.1,"T");
        Matrix z(1,_y.col,0,"T");//���ȷ������Ĵ�С
        Matrix grad(_x.col,_y.col,0,"T");
        double zRowSum=0;
        //double alpha=0.001;///����
        double error;
        int i,j,k,iter;
        for(iter=0; iter<_iter; iter++)
        {
            for(i=0; i<_x.row; i++)
            {
                xOneRow=_x.getOneRow(i);///���ѡ��һ�������㣬����û�������ѡ�񣬶��ǰ���ѡ��
                z = xOneRow * weights;
                zRowSum=0;
                for(j=0;j<z.col;j++)
                {
                    sigmoid(&z.data[0][j]);
                    zRowSum+=z.data[0][j];//���
                }
                for(j=0;j<z.col;j++)
                {
                    z.data[0][j]/=zRowSum;//��һ��
                    if(iter%1000==0)
                        cout<<z.data[0][j] <<" s ";
                }
                if(iter%1000==0)
                    cout<<endl;
                for(j=0;j<_y.col;j++)
                {
                    z.data[0][j]=_y.data[i][j]-z.data[0][j];
                }
                xOneRowT = xOneRow.transposeMatrix();
                grad = xOneRowT * z;///����һ��������Ԥ�������ȷ�����ݶȷ���
                for(k=0; k<grad.row;k++)
                {
                    for(j=0;j<grad.col; j++)
                    {
                        grad.data[k][j]*= _alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
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
        cout<<"test"<<endl;
        for(i=0; i < _y.row; i++)
        {
            if(test.data[i][0]>test.data[i][1])
                cout<<0 - _y.data[i][1]<<"  ";
            else
                cout<<1 - _y.data[i][1]<<"  ";
            cout<<endl;
        }
    }

    SoftMaxReg::SoftMaxReg(const string &file, const string &model, const double &alpha, const int &iter)
    {
        cout<<"loadData"<<endl;
        cout<<"----------------------"<<endl;
        _x.LoadData(file);
        _y=_x.getOneCol(_x.col-1);
        _y=one_hot(_y,2);
        _x.deleteOneCol(_x.col-1);
        _alpha = alpha;
        _iter = iter;
    }
}
