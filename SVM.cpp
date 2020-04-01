#include "SVM.h"
namespace MLL
{

    /**
    �ṹ��OS�ĳ�ʼ�������ڱ�������SMO�㷨����Ҫ�õ��Ĳ���
    */
    SVM::SVM(const std::string &file, const double C, const double soft, const double b, const int iter,kTup ktup)
    {
        _x.init_by_data(file);
        _y=_x.getOneCol(_x._col-1);
        _x.deleteOneCol(_x._col-1);
		std::cout << "x:" << _x._row << "&" << _x._col << std::endl;
		std::cout << "y:" <<  _y._row << "&" << _y._col << std::endl;
        _eCache.initMatrix(_x._row,2,0);
        _alphas.initMatrix(_x._row,1,0);
        _kernel.initMatrix(_x._row,_x._row,0);
        _b=b;
        _C=C;
        _iter = iter;
        _soft=soft;
        if(ktup.type!=0)
        {
            int i=0, j=0;
            Matrix xOnerow(1,_x._col,0);
            Matrix kOnecol(_x._row,1,0);
            for(i=0; i<_x._row; i++)
            {
                xOnerow=_x.getOneRow(i);
                kOnecol= SVM::kernelTran(xOnerow);
                for(j=0; j<_x._row; j++)
                    _kernel._data[i][j]=kOnecol._data[0][j];
            }
            _k=true;
        }
        _k=false;
    }
    /**
    ���ݲ����������ɲ�ͬ�ĺ˺���
    */
    Matrix SVM::kernelTran(Matrix xOnerow)
    {
        Matrix K(_x._row,1,0);
        Matrix xOnerowT = xOnerow.transposeMatrix();
        if(_ktup.type==1)
        {
            K = K.multsMatrix(_x,xOnerowT);
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
        Matrix ay(_x._row,1,0);
        size_t i;
        for(i=0; i<ay._row; i++)
        {
            ay._data[i][0]=_alphas._data[i][0]*_y._data[i][0];
        }
        Matrix ayT =ay.transposeMatrix();
        Matrix f(1,1,0);
        if(!_k)
        {
            Matrix ayTx =ayTx.multsMatrix(ayT,_x);
            Matrix xOnerow = _x.getOneRow(k);
            Matrix xOnerowT = xOnerow.transposeMatrix();
            f = f.multsMatrix(ayTx,xOnerowT);//ֵ����ȶ
        }
        else
        {
            Matrix kOnecol = _kernel.getOneCol(k);
            f = f.multsMatrix(ayT,kOnecol);//ֵ����ȶ
        }
        double fXK=f._data[0][0]+ _b - _y._data[k][0];
        return fXK;
    }
    ///����������
    void SVM::updataEk(int k)
    {
        double Ek;
        Ek=calcEk(k);//�������ֵ
        _eCache._data[k][0]=1;
        _eCache._data[k][1]=Ek;
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
        for(k=0; k<_alphas._col; k++)
        {
            if(_eCache._data[k][0]==1&&k!=i)
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
        if(_y._data[i][0]*(Ei- _y._data[i][0] * _soft)<0 && _alphas._data[i][0]< _C || _y._data[i][0]*(Ei- _y._data[i][0] * _soft)>0 && _alphas._data[i][0]>0)
        {
            Ei=calcEk(i);
            j=selectJ(i,Ei);
            Ej=calcEk(j);
            alphaIOld= _alphas._data[i][0];
            alphaJOld= _alphas._data[j][0];
            if(_y._data[i][0]!= _y._data[j][0])
            {
                L=std::max<double>(0.0, _alphas._data[j][0] - _alphas._data[i][0]);
                H=std::min<double>(_C, _alphas._data[j][0] - _alphas._data[i][0] + _C);
            }
            else
            {
                L=std::max<double>(0.0, _alphas._data[j][0] + _alphas._data[i][0] - _C);
                H=std::min<double>(_C, _alphas._data[j][0] + _alphas._data[i][0]);
            }
            if(L==H)
            {
                std::cout<<"l=h------------------"<<std::endl;
                return 0;
            }
            if(!_k)
            {
                temp[0]=0;
                temp[1]=0;
                temp[2]=0;
                for(n=0; n<_x._row; n++)
                {
                    temp[0]+=pow(_x._data[i][n],2);
                    temp[1]+=pow(_x._data[j][n],2);
                    temp[2]+=_x._data[i][n] * _x._data[j][n];
                }
            }
            else
            {
                temp[0] = _kernel._data[i][i];
                temp[1] = _kernel._data[j][j];
                temp[2] = _kernel._data[i][j];
                eta=temp[0]+temp[1]-2*temp[2];
            }
            eta=temp[0]+temp[1]-2*temp[2];
            if(eta<0)
            {
                std::cout<<"eta<0-----------------"<<std::endl;
                return 0;
            }
            _alphas._data[j][0] += _y._data[j][0]*(Ei-Ej)/eta;
            _alphas._data[j][0]=clipAlpha(_alphas._data[j][0],L,H);//��alpha���м���
            updataEk(j);//�������ֵ
            if(fabs(_alphas._data[j][0]-alphaJOld)<0.00000000001)
            {
                std::cout<<"j not moving enough---------------"<<std::endl;
                return 0;
            }
            _alphas._data[i][0] += _y._data[i][0] * _y._data[j][0]*(alphaJOld - _alphas._data[j][0]);
            updataEk(i);//�������ֵ

            b1=_b-Ei - _y._data[i][0]*(_alphas._data[i][0]-alphaIOld)*temp[0] - _alphas._data[j][0]*(_alphas._data[j][0]-alphaJOld)*temp[2];
            b2=_b-Ej - _y._data[i][0]*(_alphas._data[i][0]-alphaIOld)*temp[2] - _alphas._data[j][0]*(_alphas._data[j][0]-alphaJOld)*temp[1];
            if(0 < _alphas._data[i][0] && _C > _alphas._data[i][0])
                _b=b1;
            else if(0 < _alphas._data[j][0] && _C > _alphas._data[j][0])
                _b=b2;
            else
                _b=(b1+b2)/2;
            return 1;
        }
        std::cout<<"kkt--------------------------"<<std::endl;
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
        int it = 0;
        int i = 0;
        bool entireSet=true;//��־�����жϵ�ǰ�������������alpha���������0-C֮��Ĳ���alpha��������ʵ��һ��alpha������ʽѡ�񣬼�ѡ������֧�������ϵĵ�
        int alphaPairsChanged=0;//�ò����ж���ȷ����һ������֮���Ƿ���ѡ�������Ҫ��ĵڶ�alpha������ֵΪ1��ʾ�ܣ�0Ϊ����
        for(it = 0; it < _iter && (alphaPairsChanged>0||entireSet); it++)
        {
            //ѭ��������־Ϊ���������ѵ�Ԥ��ֵ�������ǲ����ټ����Ż����������е�֧�������ϵĵ㶼�Ҳ����ڶ���alpha�Ե�һ��alpha�����Ż��������ٱ������еĵ�Ѱ�ҿ��Ż��Ĳ����ԣ�
            //�����Ҳ������ٴα���֧�������ϵĵ㣬��α�ȻҲ���Ҳ������Ž�������
            alphaPairsChanged=0;
            if(entireSet)
            {
                for(i=0; i<_x._row; i++)
                {
                    alphaPairsChanged+=innerL(i);
                    std::cout<<"it="<< it <<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<"  entireSet="<<entireSet<<std::endl;
                }
                it++;
            }
            else
            {
                for(i=0; i< _x._row; i++)
                {
                    if(_alphas._data[i][0]>0 && _alphas._data[i][0] < _C)//ֻѡ��֧�������ϵĵ�
                        continue;
                    alphaPairsChanged+=innerL(i);
                    std::cout<<"it="<<it<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<alphaPairsChanged<<"  entireSet="<<entireSet<<std::endl;
                }
                it++;
            }
            if(entireSet)
                entireSet=false;
            else if(alphaPairsChanged==0)
                entireSet=true;
        }
        ///��SMO�㷨ʵ�ֵ���ȷ�Խ�����֤�����Ԥ��ֵ��ʵ��ֵ�Ĳ��ȫΪ0��ʾ��ѵ��������Ԥ��ȫ��
        Matrix ay(_x._row,1,0);
        for(i=0; i<_x._row; i++)
        {
            ay._data[i][0] = _alphas._data[i][0] * _y._data[i][0];
        }

        Matrix xT = _x.transposeMatrix();

        Matrix w = w.multsMatrix(xT,ay);

        Matrix pre_label = pre_label.multsMatrix(_x,w);
        std::cout<<_b<<"  ";
        for(i=0; i < _x._col; i++)
        {
            std::cout<<w._data[i][0]<<"  ";
        }
        std::cout<<std::endl;
        std::cout<<"----���Խ��-------"<<std::endl;
        ///��֤ѵ���������ݣ���֤SVMʵ�ֵ���ȷ��
        for(i=0; i< _x._row; i++)
        {
            if((pre_label._data[i][0] + _b) > 0)
                std::cout<<"pre_label: 1" << "\t" << "fact_label: " << _y._data[i][0] <<std::endl;
            else
                std::cout<<"pre_label: -1" << "\t" << "fact_label: " << _y._data[i][0] <<std::endl;
        }
        return 0;
    }
}
