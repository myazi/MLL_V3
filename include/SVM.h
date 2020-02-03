#ifndef SVM_H
#define SVM_H
#include "MatrixOpe.h"
#include <iostream>
#define MAX 1000000
#define MIN -100000
//SMO�����ṹ��
namespace  MLL
{
//�˺����Ľṹ��
    struct kTup
    {
        int type;//0,1
        double arg;//�˺�������
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
            //�ǳ�ֵ��ע�����svm��ѵ�������������У���ÿһ����һ�����������Ե���w��������
            SVM(const std::string &file, const double C, const double soft, const double b, const int iter,kTup ktup);
            Matrix kernelTran(Matrix xOneRow);
            double clipAlpha(double alpha,double L,double H);
            double calcEk(int k);
            void updataEk(int k);
            int selectJ(int i,double Ei);
            int innerL(int i);
            int smoP();
    typedef std::shared_ptr<SVM> SVMPtr;
};
}
#endif
