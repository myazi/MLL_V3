#include "SVM.h"
namespace MLL
{
   /* ///���������ļ����浽����dtm�ľ���Ԫ����

    ///ͨ����������е�load������ʼ��������������������x,y
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
    ///����SMO�������SVMģ��
    cout<<"SVM"<<endl;
    SMOP smop;
    kTup ktup;//�˺����Ķ��壬����typeԪ��Ϊ0��ʾ�����ú˺�������0�ֱ��Ӧ��ͬ�ĺ˺���
    ktup.type=1;
    ktup.arg=1.0;
    smop.smoP(x,y,0.6,0.001,40,ktup);//
    return 0;
}*/

    /**
    �ṹ��OS�ĳ�ʼ�������ڱ�������SMO�㷨����Ҫ�õ��Ĳ���
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
    ���ݲ����������ɲ�ͬ�ĺ˺���
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
            //��˹��
        }
        return K.transposeMatrix();
    }
    /**
    �����޼�������
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
    ���ֵ�ļ���Ҳ�Ǹ�����ѧ���ʽ��ʵ�ֵ�

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
            f = f.multsMatrix(ayTx,xOneRowT);//ֵ����ȶ
        }
        else
        {
            Matrix kOneCol = _kernel.getOneCol(k);
            f = f.multsMatrix(ayT,kOneCol);//ֵ����ȶ
        }
        double fXK=f.data[0][0]+ _b - _y.data[k][0];
        return fXK;
    }
    ///����������
    void SVM::updataEk(int k)
    {
        double Ek;
        Ek=calcEk(k);//�������ֵ
        _eCache.data[k][0]=1;
        _eCache.data[k][1]=Ek;
    }
    /**
    ���ڵڶ���alpha��ѡ�����Ei-Ej�Ĵ�С���к�����ͬʱ��������һ�����������治���E�����ټ�����
    ѡ������ʵڶ���alpha2�����ڵ�һ�ν������������ֵE�ľ����Ȼû��һ����Ч�ģ����Բ����������
    ���ѡ�������ʵ��������ѡ�񣬾���û��ʵ�֣����ǲ��õȼ۷�ʽʵ�֣�����Ҫע������Խ������
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
            //���ѡ��һ��j, ������ֻ�е�һ��ʱ���е�ek��û�������������ѡһ������
            if(maxK==-1)
            {
                maxK=(i*2+1)%100;
                Ej=calcEk(maxK);
            }
        }
        return maxK;
    }

    /**
    �ڲ�ѭ��ʵ������alpha���Ż��������ѭ�������ṩalpha1�Ŀ�ѡ���ϣ���������������֧����������
    ���������ϵ�ÿһ��alpha����������ж��Ƿ�Υ��KKT���������Υ����ѡΪ��һ��alpha1��
    ͬ������selctJ����������ʽѡ��ʹ��alpha1�ı�����alpha2��
    ѡ�������ij֮�󣬻��ø��ݵ�ǰalpha1��alpha2��ȷ���Ż��������ޡ�
    Ȼ��ͨ��������ʽ��������alpha1��alpha2�������ʱ���ж��Ƿ���ú˷�����
    ���Լ��������������޼���������ȷ���Ż����alpha1��alpha2��ͬʱ�Ե�ǰ�Ż�������bҲ�����޸�
    ���ǵ�ÿһ��ѡ��ڶ���alpha2ʱ������Ҫ�Ƚ�һ�����ֵ����������ֵ��ÿ���Ż������ж�ֻ�ж�Ӧ�޸�alpha�����ֵ�б仯
    ���������ǲ���ģ�������һ��������������Ч�����ֵ��
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
        double temp[3];//ii��jj��ij
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
            _alphas.data[j][0]=clipAlpha(_alphas.data[j][0],L,H);//��alpha���м���
            updataEk(j);//�������ֵ
            if(fabs(_alphas.data[j][0]-alphaJOld)<0.00000000001)
            {
                cout<<"j not moving enough---------------"<<endl;
                return 0;
            }
            _alphas.data[i][0] += _y.data[i][0] * _y.data[j][0]*(alphaJOld - _alphas.data[j][0]);
            updataEk(i);//�������ֵ

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

    SMO�㷨����ں���������Ҫ�����ǳ�ʼ��SMO���еĲ������ṹ��OS
    ȷ������������־����������������֧��������ѭ����ѡ�����alpha1������innerȷ��alpha1��
    ���ѭ��ÿһ���ǵ����Ż�һ��alpha,�ڲ�ѭ����������ʵ�ֵ����Ż�һ��alpha
    ��󣬶�smo�㷨��ʵ�ֽ��м�飬ͨ��alpha����w��b������ѵ�������ϱȽ���Ԥ��ֵ��ʵ��ֵ�Ĳ��졣
    �ð취ֻ�ܼ��SMO�㷨ʵ�ֵ���ȷ�ԣ����ܼ��SVM�����ܡ�
    */

    int SVM::smoP()
    {
        int iter=0;
        int i;
        bool entireSet=true;//��־�����жϵ�ǰ�������������alpha���������0-C֮��Ĳ���alpha��������ʵ��һ��alpha������ʽѡ�񣬼�ѡ������֧�������ϵĵ�
        int alphaPairsChanged=0;//�ò����ж���ȷ����һ������֮���Ƿ���ѡ�������Ҫ��ĵڶ�alpha������ֵΪ1��ʾ�ܣ�0Ϊ����
        for(iter=0; iter< _iter&&(alphaPairsChanged>0||entireSet); iter++)
        {
            //ѭ��������־Ϊ���������ѵ�Ԥ��ֵ�������ǲ����ټ����Ż����������е�֧�������ϵĵ㶼�Ҳ����ڶ���alpha�Ե�һ��alpha�����Ż��������ٱ������еĵ�Ѱ�ҿ��Ż��Ĳ����ԣ�
            //�����Ҳ������ٴα���֧�������ϵĵ㣬��α�ȻҲ���Ҳ������Ž�������
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
                    if(_alphas.data[i][0]>0 && _alphas.data[i][0] < _C)//ֻѡ��֧�������ϵĵ�
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
        ///��SMO�㷨ʵ�ֵ���ȷ�Խ�����֤�����Ԥ��ֵ��ʵ��ֵ�Ĳ��ȫΪ0��ʾ��ѵ��������Ԥ��ȫ��
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
        ///��֤ѵ���������ݣ���֤SVMʵ�ֵ���ȷ��
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
