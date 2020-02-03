#ifndef SVM_H
#define SVM_H
#include "MatrixOpe.h"
#include <iostream>
#define MAX 1000000
#define MIN -100000
//SMO参数结构体
namespace  MLL
{
//核函数的结构体
    struct kTup
    {
        int type;//0,1
        double arg;//核函数参数
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
            //非常值得注意的是svm中训练样本按列排列，即每一列是一个样本，所以导致w是行向量
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
