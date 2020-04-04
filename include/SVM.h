#ifndef SVM_H
#define SVM_H
#include "MatrixOpe.h"
#define MAX 1000000
#define MIN -100000
//SMO�����ṹ��
namespace  MLL
{
//�˺����Ľṹ��
    struct kTup
    {
        int type = 0;//0,1
        double arg = 0;//�˺�������
    };
    class SVM
    {
        private:
            Matrix _x;
            Matrix _y;
            double _C;
            double _soft;
            int _iter;
            Matrix _alphas;
            double _b;
            Matrix _eCache;
            Matrix _kernel;
            bool _k;
            kTup _ktup;
        public:
            Matrix kernelTran(Matrix &xOneRow);
            double clipAlpha(const double &alpha, const double &L, const double &H);
            double calcEk(const int &k);
            void updataEk(const int &k);
            int selectJ(const int &i, const double &Ei);
            int innerL(const int &i);
            int smoP();
            //�ǳ�ֵ��ע�����svm��ѵ�������������У���ÿһ����һ�����������Ե���w��������
            SVM(const std::string &file, const double C, const double soft, const double b, const int iter,kTup ktup);
    typedef std::shared_ptr<SVM> SVMPtr;
};
}
#endif
