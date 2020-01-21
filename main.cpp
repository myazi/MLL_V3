#include <iostream>
#include "LineReg.h"
#include "LogReg.h"
#include "SoftMaxReg.h"
#include "DNN.h"
#include "SVM.h"
#include "DTree.h"
#include "Adaboost.h"
#include "CART.h"
#include "RF.h"
#include "kMeans.h"
#include "KNN.h"
#include "Bayes.h"
#include "GMM.h"
/*#include "SVM.h"
#include "HMM.h"
#include "MEMM.h"
#include "CRF.h"
#include "MDP.h"
#include "DTree.h"
#include "CART.h"
#include "Adaboost.h"
#include "RF.h"
#include "kMeans.h"
#include "PCA.h"//
#include "SVD.h"//
#include "KNN.h"
#include "Hash.h"//
*/
int LogReg(const char *file = "logReg.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	std::string model = "gradAscent";
    double alpha = 0.001;
    double iter = 10000;
    MLL::LogReg::LogRegPtr logreg = std::make_shared<MLL::LogReg>(path_file,model,alpha,iter);
    if(model=="gradAscent")
        logreg->gradAscent_Log();
    if(model=="stoGradAscent")
       logreg->stoGradAscent_Log();

    return 0;
}
int SoftMaxReg(const char *file = "logReg.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	std::string model = "gradAscent";
    double alpha = 0.001;
    double iter = 100;
    MLL::SoftMaxReg::SoftMaxRegPtr softmaxreg = std::make_shared<MLL::SoftMaxReg>(path_file,model,alpha,iter);
    if(model=="gradAscent")
        softmaxreg->gradAscent_SoftMax();
    if(model=="stoGradAscent")
        softmaxreg->stoGradAscent_SoftMax();

    return 0;

}
int LineReg(const char *file = "lineReg.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	std::string model = "regression";
    double lamd = 0.001;
    double k = 100;
    MLL::LineReg::LineRegPtr linereg = std::make_shared<MLL::LineReg>(path_file,model,lamd,k);
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
int trainDNN(const char *file = "logReg.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
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
    MLL::DNN::DNNPtr dnn = std::make_shared<MLL::DNN>(path_file,"gd",0.1,"he",0.01,1,64,0.9, 0.999, 0.0000001, 500, true);
    //dnn->predict(dnn->_x,dnn->_y);
    return 0;
}
int SVM(const char *file = "logReg.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
    double C = 0.6;
    double soft = 0.001;
    double b = 0;
    int iter = 100;
    MLL::kTup ktup;//核函数的定义，其中type元素为0表示不适用核函数，非0分别对应不同的核函数
    ktup.type=1;
    ktup.arg=1.0;
    MLL::SVM::SVMPtr svm = std::make_shared<MLL::SVM>(path_file,C,soft,b,iter,ktup);//
    svm->smoP();
    return 0;
}
int Adaboost(const char *file = "adaboost.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	int numIt = 100;
	MLL::Adaboost::AdaboostPtr adaboost = std::make_shared<MLL::Adaboost>(path_file,numIt);
	adaboost->AdaboostTrainDS();
	return 0;
}
int DTree(const char *file = "id3.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	std::string type = "ID3";
	MLL::DTree::DTreePtr dtree = std::make_shared<MLL::DTree>(path_file,type);
	return 0;
}

int CART(const char *file = "cart.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	MLL::CART::CARTPtr cart = std::make_shared<MLL::CART>(path_file);
	return 0;
}
int RF(const char *file = "rf.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	int numIt = 50;
	int deep = 5;
	double epsilon = 0.01;
	MLL::RF::RFPtr rf = std::make_shared<MLL::RF>(path_file, numIt, deep, epsilon);
	return 0;
}
int KMeans(const char *file = "kmeans.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	MLL::KMeans::KMeansPtr kmeans = std::make_shared<MLL::KMeans>(path_file);
	return 0;

}
int KNN(const char *file = "knn.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	MLL::KNN::KNNPtr knn = std::make_shared<MLL::KNN>(path_file);
	return 0;
}

int Bayes(const char *file = "bayes.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	MLL::Bayes::BayesPtr bayes = std::make_shared<MLL::Bayes>(path_file);
	return 0;
}
int GMM(const char *file = "GMM.txt")
{
	std::string path_file = "data/";
    path_file.append(file);
	int K = 10;
	int Iter = 100;
	MLL::GMM::GMMPtr gmm = std::make_shared<MLL::GMM>(path_file,K,Iter);
	return 0;
}
int main(int argc, char* argv[])
{
    
	std::string models[13] ={"LineReg","LogReg","SoftMaxReg","DNN","SVM","DTree","CART","Adaboost","RF","KMeans","KNN","Bayes","GMM"};
	std::vector<std::string> models_pos(models, models + 13); 
    int (*models_list[])(const char *file) = {LineReg, LogReg,SoftMaxReg,trainDNN,SVM,DTree,CART,Adaboost,RF,KMeans,KNN,Bayes,GMM};
    int (*models_ptr) (const char *file) = LogReg;
    if(argc < 2)
    {
		std::cout<<"arg is error"<<std::endl;
        return -1;
    }
	std::string type = argv[1];
	char *file_name = argv[2];
	std::cout<< type  << std::endl;
    //int pos = 1;//logreg
	int ret = 0;
    for(int i = 0; i < models_pos.size(); i++)
    {
		std::cout<< models_pos[i] << std::endl;
        if(type == models_pos[i])
        {
            models_ptr = models_list[i];
			ret = models_ptr(file_name);
			return ret;
        }
    }
    //trainDNN("data/train.txt");
    //LineReg("data/lineReg.txt","regression",0.01,1);
    //LogReg("data/logReg.txt","gradAscent",0.01,1000);
    //LogReg("sample.new","gradAscent",0.001,5000);
    //LogReg("sample","gradAscent",0.01,1000);
    //SoftMaxReg("data/logReg.txt","gradAscent",0.01,1000);
    //ME();
    //trainDNN();
    //SVM("data/logReg.txt");

    //Bayes();
    //HMM();
    //HMM_CWS();
    //MEMM_CWS();
    //CRF_CWS();
    //MDP();

    //DTree("data/id3.txt");
	//CART("data/cart.txt");
    //Adaboost("sample",50);
    //Adaboost("data/adaboost.txt",50);
    //RF(50,5,0.01);

    //kmeans();
    //GMM(3,5);
    return 0;
}
