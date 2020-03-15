/***

**/

#include "Adaboost.h"
//#define MAX_SIZE_OF_TRAINING_SET 100
//#define MAX_NUMIT 100
//#define ATTR_NUM 22
#define MAX 1000000
#define MIN -100000

namespace MLL
{
	double* Adaboost::rangeSize(const int &axis)
	{
		double rangemin=MAX;
		double rangemax=MIN;
		double *range=(double *)malloc(sizeof(double)*2);
		for(size_t i=0; i<data.size(); i++)
		{
			if(data[i][axis]>rangemax)
				rangemax=data[i][axis];
			if(data[i][axis]<rangemin)
				rangemin=data[i][axis];
		}
		range[0]=rangemin;
		range[1]=rangemax;
		return range;
	}
	std::vector<int> Adaboost::Classify(const int &axis, const double &threshVal,const  std::string &threshIneq)
	{
		std::vector<int> label;
		for(size_t i=0; i<data.size(); i++)
		{
			label.push_back(1);
			if(threshIneq=="lt")
			{
				if(data[i][axis]<=threshVal)
					label[i]=-1;
					//label[i]=0;
			}
			else
			{
				if(data[i][axis]>threshVal)
					label[i]=-1;
					//label[i]=0;
			}
		}
		return label;
	}
	Adaboost::Stump Adaboost::buildStump(std::vector<double> &weight)
	{
		Stump stump;
		int i,j,k,l;
		double *errArr=(double *)malloc(sizeof(double)*data.size());
		double minErr=MAX;
		double weightError=0;
		double *range=(double *)malloc(sizeof(double)*2);
		double rangemin,rangemax;
		double stepSize;
		int numSteps = 2;
		//int numSteps = data.size()/10;
		double threshVal;
		int label_index=data[0].size()-1;
		std::vector<int> label;
		std::string threshIneq[2]= {"lt","gt"};
		for(i=0; i<data[0].size()-1; i++) //属性
		{
			range=rangeSize(i);
			rangemin=range[0];
			rangemax=range[1];
			stepSize=(rangemax-rangemin)/numSteps;
			for(j=0; j<=numSteps; j++) //其实是numstep+1步
			{
				threshVal=rangemin+stepSize*j;
				for(k=0; k<2; k++) //大于小于
				{
					label=Classify(i,threshVal,threshIneq[k]);
					weightError=0;
					for(l=0; l<data.size(); l++)
					{
						errArr[l]=label[l]-data[l][label_index] > 0 ? (label[l]-data[l][label_index])/2:-(label[l]-data[l][label_index])/2;
						//errArr[l]=label[l]-data[l][label_index] > 0 ? (label[l]-data[l][label_index]):-(label[l]-data[l][label_index]);
						weightError+=errArr[l] * weight[l];
					}
					if(weightError<minErr)
					{
						minErr=weightError;
						for(l=0; l<data.size(); l++)
						{
							if(label[l]>0)
								stump.twosubdata.left.push_back(data[l]);
							else
								stump.twosubdata.right.push_back(data[l]);
						}
						stump.label=label;
						stump.bestIndex=i;
						stump.minErr=minErr;
						stump.threshVal=threshVal;
						stump.ltOrgt=threshIneq[k];
						std::cout<<"minErr="<<minErr<<std::endl;
						stump.alpha=0.5*log((1-minErr)/minErr);
					}
				}
			}
		}
		return stump;
	}
	int Adaboost::AdaboostTrainDS()
	{
		int i,j,k;
		int label_index=data[0].size()-1;
		std::vector<double> weight;
		double weightSum=0;
		std::vector<double> aggErr;
		double sumErr=0;
		std::vector<Stump> stump(numIt);
		for(i=0; i<data.size(); i++)
		{
			weight.push_back(1.0/data.size());
			aggErr.push_back(0);
		}
		for(i=0; i<numIt; i++)
		{
			stump[i] = buildStump(weight);//写成赋值语句结构体中的结构体复制出现问题
			std::cout<<"alpha===="<<stump[i].alpha<<std::endl;
			weightSum=0;
			for(j=0; j<data.size(); j++)
			{
				weight[j]=weight[j]*(exp(-1*stump[i].alpha*(data[i][label_index]*stump[i].label[j])));
				weightSum+=weight[j];
			}
			for(j=0; j<data.size(); j++)
			{
				weight[j]/=weightSum;
			}
			for(j=0; j<data.size(); j++)
			{
				for(k=0; k<i; k++)
				{
					aggErr[j]+=stump[k].alpha*(stump[k].label[j]);
				}
				if(aggErr[j]>0)
					aggErr[j]=1;
				else
					aggErr[j]=-1;
			}
			sumErr=0;
			for(j=0; j<data.size(); j++)
			{
				sumErr+=(aggErr[j]-data[j][label_index])>0?(aggErr[j]-data[j][label_index])/2:-(aggErr[j]-data[j][label_index])/2;
			}
			//if(sumErr/data.size()<0.01)
			{
				std::cout<<sumErr<<std::endl;
				//break;
			}
		}
		for(i=0; i<numIt; i++) //50为投票器个数，但不一定是50，只会小于等于50
		{
			std::cout<<stump[i].alpha<<"   "<<stump[i].bestIndex<<"   "<<stump[i].threshVal<<"   "<<stump[i].ltOrgt<<"   "<<stump[i].minErr<<std::endl;
		}
		return 0;
	}
	Adaboost::Adaboost(const std::string &file,const int &numIt)
	{
		this->numIt = numIt;
		LoadData(data,file);
		int label_index=data[0].size()-1;
		for(int l=0; l<data.size(); l++)
		{
			if(data[l][label_index] == 0)
			{
				data[l][label_index] = -1;
			}
		}
	}
}
