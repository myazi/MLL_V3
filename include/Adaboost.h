/***

**/

#ifndef Adaboost_H
#define Adaboost_H
#include "MatrixOpe.h"
/*
struct twoSubData_A;

struct Stump;

double* rangeSize(const Data &data,int axis);

vector<int> Classify(const Data data, int axis, double threshVal, string threshIneq);

Stump buildStump(const Data &data,vector<double> weight);

int adaBoostTrainDS(const Data &data,const int &numIt);

int Adaboost(const int &numIt);

*/
namespace MLL 
{
	class Adaboost
	{
		struct twoSubData_A
		{
			Data left;
			Data right;
		};
		struct Stump
		{
			double minErr;
			int bestIndex;//表示最好的分类属性，当非叶子节点时，即表示分裂属性下标，否则为-1，表示为叶子节点标记
			string ltOrgt;//大于阈值的类别为-1，还是小于阈值的类别为-1
			double threshVal;
			vector<int> label;
			double alpha;//基学习器投票权重
			twoSubData_A twosubdata;//树桩包括两个分支的数据
		};
		private:
			vector<Stump> stump;
			Data data;
			int numIt;
		public:
			double* rangeSize(const int &axis);
			vector<int> Classify(const int &axis, const double &threshVal, const string &threshIneq);
			Stump buildStump(vector<double> &weight);
			int AdaboostTrainDS();
			Adaboost(const string &filename, const int &numIt);
			typedef std::shared_ptr<Adaboost> AdaboostPtr;
	};
}
#endif
