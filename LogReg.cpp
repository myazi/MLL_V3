#include "LogReg.h"

namespace MLL {
                       
    /**
    �ݶ��½��㷨����Ҫ��ȷ�����ݶȷ��򣬲��������õ�����˼�����n��������
    ��Ŀ�꺯����͹�滮���⣬��ô�ֲ���Сֵ����ȫ����Сֵ

    **/
    int LogReg::gradAscent_Log()
    {
        if(_y.col!=1)
        {
            cout<<"logReg is two class"<<endl;
            return -1;
        }
        Matrix weights(_x.col,_y.col,0.1,"T");
        Matrix xT = _x.transposeMatrix();

        //float alpha=0.01;///��������
        float error=0;///��¼������
        int iter=0;
        int i,j;
        Matrix z(_y.row,_y.col,0,"T");//���ȷ������Ĵ�С
        Matrix grad(_x.col,_y.col,0,"T");
        for(iter=0; iter<_iter; iter++)
        {
            z = _x * weights;
            for(i = 0; i < z.row; i++)
            {
                z.data[i][0]=sigmoid(z.data[i][0]);
            }
            z = _y - z;
            error=0;
            for(i=0; i<_x.row; i++)///ͳ�ƴ�����
                error+=z.data[i][0];
            //cout<<"error="<<error<<endl;
            //if(error<x.row/100 && error>x.row/100)///���ô�����С��һ��ֵʱ�˳�����
               // break;
            grad = xT * z;///���㸺�ݶȷ���
            //grad = (1.0/x.row) * grad;
            for(i=0; i<grad.row; i++)
                grad.data[i][0]*= _alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
            weights = weights + grad;///�����ݶȷ�����һ������
        }

        /**
        ��֤�㷨����ȷ��
        **/
        int er1=0,er2=0;
        Matrix train=_x * weights;
        cout<<"test"<<endl;
        for(i=0; i<_y.row; i++)
        {
            if(train.data[i][0]>0)
            {
                cout<<1-_y.data[i][0]<<endl;
                er1+=(1-_y.data[i][0]);
            }
            else
            {
                cout<<0-_y.data[i][0]<<endl;
                er2-=(0-_y.data[i][0]);
            }
        }
        cout<<"er1="<<er1<<endl;
        cout<<"er2="<<er2<<endl;
    }
    /**
    ����ݶ��½����ݶ��½�����ͬ�����ڸ��ݶȷ����ȷ�����ݶ��½��Ǹ������е�������ȷ�����ݶȷ���
    ������ݶ��½�ÿ��ֻ��һ����������ȷ�����ݶȷ�����Ȼ����ȫ���ţ������ŵ����������ӣ�ͬ������

    **/
    int LogReg::stoGradAscent_Log()//����ݶ��½�ÿһ��ѡ��m�������������ݶ��½����򣬸ô�����ֻѡ��һ��������������ݶ��½���������ֵ
    {
        Matrix xOneRow(1,_x.col,0,"T");
        Matrix xOneRowT(_x.col,1,0,"T");

        Matrix weights(_x.col,_y.col,0.1,"T");
        Matrix z(1,_y.col,0,"T");//���ȷ������Ĵ�С
        Matrix grad(_x.col,_y.col,0,"T");

        double error;
        int i,j,c;
        for(c=0; c<1000; c++)
        {
            for(i=0; i<_x.row; i++)
            {
                xOneRow=_x.getOneRow(i);///���ѡ��һ�������㣬����û�������ѡ�񣬶��ǰ���ѡ��
                z = xOneRow * weights;
                z.data[0][0]=sigmoid(z.data[0][0]);
                z.data[0][0]=_y.data[i][0]-z.data[0][0];
                xOneRowT = xOneRow.transposeMatrix();
                grad = xOneRowT * z;///����һ��������Ԥ�������ȷ�����ݶȷ���
                for(j=0; j<grad.row; j++)
                    grad.data[j][0]*=_alpha;
                weights = weights + grad; ///����
            }
            }
        //��֤�㷨����ȷ��
        cout<<"test"<<endl;
        Matrix test = _x * weights;
        for(i=0; i<_y.row; i++)
        {
            if(test.data[i][0]>0)
            {
                cout<<1-_y.data[i][0]<<endl;
            }
            else
            {
                cout<<0-_y.data[i][0]<<endl;
            }
        }
    }
    LogReg::LogReg(const char *file, const string &model, const double alpha,const int iter)
    {
        cout<<"loadData:"<< file<<endl;
        cout<<"----------------------"<<endl;
        _x.LoadData(file);
        _x.print(); 
        _y=_x.getOneCol(_x.col-1);
        _x.deleteOneCol(_x.col-1);
        _x.print();
        _alpha = alpha;
        _iter = iter;
    }
}
