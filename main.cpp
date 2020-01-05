#include "LineReg.h"
#include "LogReg.h"
#include "SoftMaxReg.h"
//#include "ME.h"//
#include "DNN.h"
#include "SVM.h"
/*#include "SVM.h"
#include "Bayes.h"
#include "HMM.h"
#include "MEMM.h"
#include "CRF.h"
#include "MDP.h"
#include "DTree.h"
#include "CART.h"
#include "Adaboost.h"
#include "RF.h"
#include "kMeans.h"
#include "GMM.h"
#include "PCA.h"//
#include "SVD.h"//
#include "KNN.h"
#include "Hash.h"//
*/
#include <iostream>
int LogReg(const char *file, const std::string &model, const double &alpha, const int &iter)
{
    /**
    file: data/logReg.txt
    *file: d
    **/
    ///test
    MLL::LogReg::LogRegPtr logreg = std::make_shared<MLL::LogReg>(file,model,alpha,iter);
    //MLL::LogReg::LogRegPtr logreg(new MLL::LogReg(file,model,alpha,iter));
    if(model=="gradAscent")
        logreg->gradAscent_Log();
    if(model=="stoGradAscent")
        logreg->stoGradAscent_Log();

    return 0;
}
int SoftMaxReg(const char *file, const std::string &model,const double &alpha, const int &iter)
{
    MLL::SoftMaxReg::SoftMaxRegPtr softmaxreg = std::make_shared<MLL::SoftMaxReg>(file,model,alpha,iter);
    //MLL::LogReg::LogRegPtr logreg(new MLL::LogReg(file,model,alpha,iter));
    if(model=="gradAscent")
        softmaxreg->gradAscent_SoftMax();
    if(model=="stoGradAscent")
        softmaxreg->stoGradAscent_SoftMax();

    return 0;

}
int LineReg(const char *file, const string &model, const double &lamd, const double &k)
{
    MLL::LineReg::LineRegPtr linereg = std::make_shared<MLL::LineReg>(file,model,lamd,k);
    if(model=="regression")
    {
        linereg->regression();
    }
    if(model=="ridgeRegres")
    {
        linereg->ridgeRegres();
    }
    if(model=="lwlrtest")
    {
        linereg->lwlrtest();
    }
    return 0;
}
int trainDNN(const char *file){
    const char *initialization="he";
    double learn_rateing=0.1;
    int iter=1000;
    double lambd=0.1;
    double keep_prob=0.5;
    bool print_cost=true;
    const char *optimizer="gd";
    int mini_batch_size=64;
    double beta1=0.9;
    double beta2=0.999;
    double epsilon=0.00000001;

    //MLL::DNN::DNNPtr dnn = std::make_shared<MLL::DNN>(DNN(file,optimizer="gd",learn_rateing=0.001,initialization="he",lambd=0.001,keep_prob = 1,mini_batch_size=64,beta1=0.9, beta2=0.999, epsilon=0.00000001, iter=5000, print_cost=true);
    MLL::DNN::DNNPtr dnn = std::make_shared<MLL::DNN>(file,"gd",0.001,"he",0.001,1,64,0.9, 0.999, 0.00000001, 5000, true);
    //dnn->predict(dnn->_x,dnn->_y);
    return 0;
}
int SVM(const char *file)
{
    double C = 0.6;
    double soft = 0.001;
    double b = 0;
    int iter = 100;
    MLL::kTup ktup;//核函数的定义，其中type元素为0表示不适用核函数，非0分别对应不同的核函数
    ktup.type=1;
    ktup.arg=1.0;
    MLL::SVM::SVMPtr svm = std::make_shared<MLL::SVM>(file,C,soft,b,iter,ktup);//
    cout<<"sss"<<endl;
    svm->smoP();
    cout<<"00"<<endl;
    return 0;
}
int main()
{
    
    int i;
    trainDNN("data/train.txt");
    char file[20]="data/test.txt";
    //LineReg("data/lineReg.txt","regression",0.01,1);
    //LogReg("data/logReg.txt","gradAscent",0.01,1000);
    //SoftMaxReg("data/logReg.txt","gradAscent",0.01,1000);
    //ME();
    //trainDNN();
    //SVM(file);

    //Bayes();
    //HMM();
    //HMM_CWS();
    //MEMM_CWS();
    //CRF_CWS();
    //MDP();

    //DTree("ID3");
    //CART();
    //Adaboost(20);
    //RF(50,5,0.01);

    //kmeans();
    //GMM(3,5);

    //PCA();
    //SVD();

    //KNN();
    //Hash();

    return 0;
}
