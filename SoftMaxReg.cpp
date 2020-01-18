#include "SoftMaxReg.h"

namespace MLL {
    /**
    梯度下降算法，主要是确定负梯度方向，步长，采用迭代的思想迭代n至收敛，
    当目标函数是凸规划问题，那么局部最小值就是全局最小值

    **/
    int SoftMaxReg::gradAscent_SoftMax()
    {
        Matrix weights(_x.col,_y.col,0.1,"T");
        Matrix xT = _x.transposeMatrix();

        //float alpha=0.001;///迭代步长
        float error=0;///记录错误率
        int iter=0;
        int i,j;
        Matrix z(_y.row,_y.col,0,"T");//最好确定矩阵的大小
        Matrix zRowSum(z.row,1,0,"ss");//各个维度求和，归一化为概率形式
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
                    zRowSum.data[i][0]+=z.data[i][j];//求和
                }
                for(j=0;j<z.col;j++)
                {
                    z.data[i][j]/=zRowSum.data[i][0];//归一化
                    //if(iter%5000==0)
                        //cout<<z.data[i][j] <<"  ";
                }
                //if(iter%5000==0)
                    //cout<<endl;
            }
            z = _y - z;

            grad = xT * z;///计算负梯度方向
            //grad = (1.0/x.row) * grad;
            for(i=0; i<grad.row; i++)
            {
                for(j=0;j<grad.col; j++)
                {
                    grad.data[i][j]*= _alpha;///负梯度方向与步长的乘积确定迭代值
                }
            }
            weights = weights + grad;///往负梯度方向走一个步长
            //weights.print();
        }
        /**
        验证算法的正确性
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
    随机梯度下降与梯度下降法不同的是在负梯度方向的确定，梯度下降是根据所有的样本来确定负梯度方向，
    而随机梯度下降每次只看一个样本点来确定负梯度方向，虽然不完全可信，但随着迭代次数增加，同样收敛

    **/
    int SoftMaxReg::stoGradAscent_SoftMax()//随机梯度下降每一次选择m个样本进行求梯度下降方向，该代码中只选择一个样本进行求解梯度下降方向与数值
    {
        Matrix xOneRow(1,_x.col,0,"T");
        Matrix xOneRowT(_x.col,1,0,"T");

        Matrix weights(_x.col,_y.col,0.1,"T");
        Matrix z(1,_y.col,0,"T");//最好确定矩阵的大小
        Matrix grad(_x.col,_y.col,0,"T");
        double zRowSum=0;
        //double alpha=0.001;///步长
        double error;
        int i,j,k,iter;
        for(iter=0; iter<_iter; iter++)
        {
            for(i=0; i<_x.row; i++)
            {
                xOneRow=_x.getOneRow(i);///随机选择一个样本点，这里没有作随机选择，而是按序选择
                z = xOneRow * weights;
                zRowSum=0;
                for(j=0;j<z.col;j++)
                {
                    sigmoid(&z.data[0][j]);
                    zRowSum+=z.data[0][j];//求和
                }
                for(j=0;j<z.col;j++)
                {
                    z.data[0][j]/=zRowSum;//归一化
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
                grad = xOneRowT * z;///根据一样样本的预测误差来确定负梯度方向
                for(k=0; k<grad.row;k++)
                {
                    for(j=0;j<grad.col; j++)
                    {
                        grad.data[k][j]*= _alpha;///负梯度方向与步长的乘积确定迭代值
                    }
                }
                weights = weights + grad; ///迭代
            }
        }
        //验证算法的正确性
        /**
        验证算法的正确性
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
