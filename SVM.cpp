#include "SVM.h"
namespace MLL
{
   /* ///加载数据文件保存到对象dtm的矩阵元素中

    ///通过矩阵对象中的load函数初始化样本的特征和类别矩阵x,y
    //char file[20]="data\\svm.txt";
    Matrix x;
    //x.LoadData(file);

    //char file[20]="data\\test.txt";
    char file[20]="sample";
    x.LoadData(file);
    //x = x.transposeMatrix();

    Matrix y;
    y=x.getOneCol(x.col-1);
    x.deleteOneCol(x.col-1);
    //for(int i=x.row-1;i>100;i--)
    //x.deleteOneCol(x.col-1);
    //    x.deleteOneRow(i);
    cout<<x.row<<"*"<<x.col<<endl;
    cout<<y.row<<"*"<<y.col<<endl;
    ///调用SMO函数求解SVM模型
    cout<<"SVM"<<endl;
    SMOP smop;
    kTup ktup;//核函数的定义，其中type元素为0表示不适用核函数，非0分别对应不同的核函数
    ktup.type=1;
    ktup.arg=1.0;
    smop.smoP(x,y,0.6,0.001,40,ktup);//
    return 0;
}*/

    /**
    结构体OS的初始化，用于保存所以SMO算法中需要用到的参数
    */
    SVM::SVM(const char *file, const double C, const double soft, const double b, const int iter,kTup ktup)
    {
        cout<<"loadData:"<< file<<endl;
        cout<<"----------------------"<<endl;
        _x.LoadData(file);
        _x.print(); 
        _y=_x.getOneCol(_x.col-1);
        _x.deleteOneCol(_x.col-1);
        cout<<"xxxxxxx"<<endl; 
        _eCache.initMatrix(_x.row,2,0,"ss");
        _alphas.initMatrix(_x.row,1,0,"ss");
        _kernel.initMatrix(_x.row,_x.row,0,"s");

        _b=b;
        _C=C;
        _iter = iter;
        _soft=soft;
        cout<<"dd"<<endl;
        if(ktup.type!=0)
        {
            int i=0,j=0;
            Matrix xOneRow(1,_x.col,0,"ss");
            Matrix kOneCol(_x.row,1,0,"ss");
            for(i=0; i<_x.row; i++)
            {
                xOneRow=_x.getOneRow(i);
                kOneCol= SVM::kernelTran(xOneRow);
                for(j=0; j<_x.row; j++)
                    _kernel.data[i][j]=kOneCol.data[0][j];
            }
            _k=true;
        }
        cout<<"aaa"<<endl;
        _k=false;
    }
    /**
    根据参数，来生成不同的核函数
    */
    Matrix SVM::kernelTran(Matrix xOneRow)
    {
        Matrix K(_x.row,1,0,"ss");
        Matrix xOneRowT = xOneRow.transposeMatrix();
        if(_ktup.type==1)
        {
            K = K.multsMatrix(_x,xOneRowT);
        }
        if(_ktup.type==2)
        {
            //高斯核
        }
        return K.transposeMatrix();
    }
    /**
    上下限剪辑函数
    */
    double SVM::clipAlpha(double alpha,double L,double H)
    {
        if(alpha>H)
            return H;
        if(alpha<L)
            return L;
        return alpha;
    }
    /**
    误差值的计算也是根据数学表达式来实现的

    **/
    double SVM::calcEk(int k)
    {
        Matrix ay(_x.row,1,0,"ss");
        size_t i;
        for(i=0; i<ay.row; i++)
        {
            ay.data[i][0]=_alphas.data[i][0]*_y.data[i][0];
        }
        Matrix ayT =ay.transposeMatrix();
        Matrix f(1,1,0,"ss");
        if(!_k)
        {
            Matrix ayTx =ayTx.multsMatrix(ayT,_x);
            Matrix xOneRow = _x.getOneRow(k);
            Matrix xOneRowT = xOneRow.transposeMatrix();
            f = f.multsMatrix(ayTx,xOneRowT);//值得商榷
        }
        else
        {
            Matrix kOneCol = _kernel.getOneCol(k);
            f = f.multsMatrix(ayT,kOneCol);//值得商榷
        }
        double fXK=f.data[0][0]+ _b - _y.data[k][0];
        return fXK;
    }
    ///更新误差矩阵
    void SVM::updataEk(int k)
    {
        double Ek;
        Ek=calcEk(k);//计算误差值
        _eCache.data[k][0]=1;
        _eCache.data[k][1]=Ek;
    }
    /**
    对于第二个alpha的选择采用Ei-Ej的大小进行衡量，同时这里引用一个矩阵来保存不变的E，减少计算量
    选择出合适第二个alpha2，对于第一次进来，保存误差值E的矩阵必然没有一个有效的，所以采用随机策略
    随机选择策略其实可以任意选择，具体没有实现，而是采用等价方式实现，但需要注意数据越界问题
    **/
    int SVM::selectJ(int i,double Ei)
    {
        int maxK=-1;
        double Ek;
        double Ej;
        double deltaE;
        double maxDeltaE=0;
        size_t k;
        for(k=0; k<_alphas.col; k++)
        {
            if(_eCache.data[k][0]==1&&k!=i)
            {
                Ek=calcEk(k);
                deltaE=fabs(Ei-Ek);
                if(deltaE>maxDeltaE)
                {
                    maxK=k;
                    maxDeltaE=deltaE;
                    Ej=Ek;
                }
            }
            //随机选择一个j, 理论上只有第一次时所有的ek都没计算过，所以任选一个即可
            if(maxK==-1)
            {
                maxK=(i*2+1)%100;
                Ej=calcEk(maxK);
            }
        }
        return maxK;
    }

    /**
    内层循环实现两个alpha的优化，由外层循环函数提供alpha1的可选集合（所有样本或者是支持向量），
    遍历集合上的每一个alpha，对其进行判断是否违反KKT条件，如果违反则选为第一个alpha1，
    同样调用selctJ函数来启发式选择使得alpha1改变最大的alpha2，
    选择出合适ij之后，还得根据当前alpha1和alpha2来确定优化的上下限。
    然后通过迭代公式，来计算alpha1和alpha2，计算的时候判断是否采用核方法。
    并对计算结果进行上下限剪辑，最终确定优化后的alpha1和alpha2，同时对当前优化后结果对b也进行修改
    考虑到每一次选择第二个alpha2时，都需要比较一个误差值，而这个误差值在每次优化过程中都只有对应修改alpha的误差值有变化
    而其他的是不变的，所以用一个矩阵来保存有效的误差值。
    */
    int SVM::innerL(int i)
    {
        double Ei=0;
        double Ej=0;
        int j;
        double alphaIOld;
        double alphaJOld;
        double L;
        double H;
        double eta;
        int n;
        double temp[3];//ii，jj，ij
        double b1,b2;
        if(_y.data[i][0]*(Ei- _y.data[i][0] * _soft)<0 && _alphas.data[i][0]< _C || _y.data[i][0]*(Ei- _y.data[i][0] * _soft)>0 && _alphas.data[i][0]>0)
        {
            Ei=calcEk(i);
            j=selectJ(i,Ei);
            Ej=calcEk(j);
            alphaIOld= _alphas.data[i][0];
            alphaJOld= _alphas.data[j][0];
            if(_y.data[i][0]!= _y.data[j][0])
            {
                L=max<double>(0.0, _alphas.data[j][0] - _alphas.data[i][0]);
                H=min<double>(_C, _alphas.data[j][0] - _alphas.data[i][0] + _C);
            }
            else
            {
                L=max<double>(0.0, _alphas.data[j][0] + _alphas.data[i][0] - _C);
                H=min<double>(_C, _alphas.data[j][0] + _alphas.data[i][0]);
            }
            if(L==H)
            {
                cout<<"l=h------------------"<<endl;
                return 0;
            }
            if(!_k)
            {
                temp[0]=0;
                temp[1]=0;
                temp[2]=0;
                for(n=0; n<_x.row; n++)
                {
                    temp[0]+=pow(_x.data[i][n],2);
                    temp[1]+=pow(_x.data[j][n],2);
                    temp[2]+=_x.data[i][n] * _x.data[j][n];
                }
            }
            else
            {
                temp[0] = _kernel.data[i][i];
                temp[1] = _kernel.data[j][j];
                temp[2] = _kernel.data[i][j];
                eta=temp[0]+temp[1]-2*temp[2];
            }
            eta=temp[0]+temp[1]-2*temp[2];
            if(eta<0)
            {
                cout<<"eta<0-----------------"<<endl;
                return 0;
            }
            _alphas.data[j][0] += _y.data[j][0]*(Ei-Ej)/eta;
            _alphas.data[j][0]=clipAlpha(_alphas.data[j][0],L,H);//对alpha进行剪辑
            updataEk(j);//更新误差值
            if(fabs(_alphas.data[j][0]-alphaJOld)<0.00000000001)
            {
                cout<<"j not moving enough---------------"<<endl;
                return 0;
            }
            _alphas.data[i][0] += _y.data[i][0] * _y.data[j][0]*(alphaJOld - _alphas.data[j][0]);
            updataEk(i);//更新误差值

            b1=_b-Ei - _y.data[i][0]*(_alphas.data[i][0]-alphaIOld)*temp[0] - _alphas.data[j][0]*(_alphas.data[j][0]-alphaJOld)*temp[2];
            b2=_b-Ej - _y.data[i][0]*(_alphas.data[i][0]-alphaIOld)*temp[2] - _alphas.data[j][0]*(_alphas.data[j][0]-alphaJOld)*temp[1];
            if(0 < _alphas.data[i][0] && _C > _alphas.data[i][0])
                _b=b1;
            else if(0 < _alphas.data[j][0] && _C > _alphas.data[j][0])
                _b=b2;
            else
                _b=(b1+b2)/2;
            return 1;
        }
        cout<<"kkt--------------------------"<<endl;
        return 0;
    }

    /**

    SMO算法的入口函数，其主要功能是初始化SMO所有的参数到结构体OS
    确定迭代结束标志，并在所有样本和支持向量上循环的选择合适alpha1，调用inner确定alpha1，
    外层循环每一次是迭代优化一对alpha,内层循环才是正真实现迭代优化一对alpha
    最后，对smo算法的实现进行检查，通过alpha求解出w，b，并在训练样本上比较其预测值与实际值的差异。
    该办法只能检查SMO算法实现的正确性，不能检查SVM的性能。
    */

    int SVM::smoP()
    {
        int iter=0;
        int i;
        bool entireSet=true;//标志用于判断当前迭代是针对所有alpha，还是针对0-C之间的部分alpha，这里其实第一个alpha的启发式选择，即选择落在支持向量上的点
        int alphaPairsChanged=0;//该参数判断在确定第一个参数之后，是否能选择出符合要求的第二alpha，返回值为1表示能，0为不能
        for(iter=0; iter< _iter&&(alphaPairsChanged>0||entireSet); iter++)
        {
            //循环结束标志为迭代次数已到预设值，或者是不能再继续优化（对于所有的支持向量上的点都找不到第二个alpha对第一个alpha进行优化后，重新再遍历所有的点寻找可优化的参数对）
            //还是找不到则再次遍历支持向量上的点，这次必然也是找不到，才结束迭代
            alphaPairsChanged=0;
            if(entireSet)
            {
                for(i=0; i<_x.row; i++)
                {
                    alphaPairsChanged+=innerL(i);
                    cout<<"iter="<<iter<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<"  entireSet="<<entireSet<<endl;
                }
                iter++;
            }
            else
            {
                for(i=0; i< _x.row; i++)
                {
                    if(_alphas.data[i][0]>0 && _alphas.data[i][0] < _C)//只选择支持向量上的点
                        continue;
                    alphaPairsChanged+=innerL(i);
                    cout<<"iter="<<iter<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<alphaPairsChanged<<"  entireSet="<<entireSet<<endl;
                }
                iter++;
            }
            if(entireSet)
                entireSet=false;
            else if(alphaPairsChanged==0)
                entireSet=true;
        }
        ///对SMO算法实现的正确性进行验证，输出预测值与实际值的差别，全为0表示在训练样本上预测全对
        Matrix ay(_x.row,1,0,"ss");
        for(i=0; i<_x.row; i++)
        {
            ay.data[i][0] = _alphas.data[i][0] * _y.data[i][0];
        }

        Matrix xT = _x.transposeMatrix();

        Matrix w = w.multsMatrix(xT,ay);

        Matrix label = label.multsMatrix(_x,w);
        cout<<_b<<"  ";
        for(i=0; i < _x.col; i++)
        {
            cout<<w.data[i][0]<<"  ";
        }
        cout<<endl;
        cout<<"-----------"<<endl;
        ///验证训练样本数据，验证SVM实现的正确性
        for(i=0; i< _x.row; i++)
        {
            if((label.data[i][0] + _b)>0)
                cout<<1 - _y.data[i][0]<<"  ";
            else
                cout<<0- _y.data[i][0]<<"  ";
        }
        return 0;
    }
}
