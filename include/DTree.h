/***
决策树，ID3,C4.5,CART

考虑

1信息增益,信息增益率,基尼指数
2属性类型（string）
3预剪枝
**/

#include "MatrixOpe.h"
#define MAX_SIZE_OF_TRAINING_SET 1000

namespace MLL {

	class DTree
	{
		struct Tree
		{
			int id;
			int split_feat;//分裂特征
			string feat_value;//特征的值
			string label;//树的类别
			int size;
			int kind;
			int parent;
			Tree *next;
		};
		private:
			Tree tree[MAX_SIZE_OF_TRAINING_SET];//用作存储后序遍历生成树的序列
			stack<Tree> s;
			int node=0;//用作存储后序遍历生成树的序列数组的下标
			vector<int> feat_flag;//初始化为0,最后一个用于统计还剩多少特征未使用
			DataStr data;

		public:
			vector<string> getkindattr(const DataStr &data, const int &axis);//获得axis属性的所有不同取值

			double calcShannonEntOrGini(const DataStr &data, const string &type);

			DataStr splitDataSet(const DataStr &data,const int &axis, const string &value);

			int chooseBestFectureTosplit(const DataStr &data, const string &type, const double &epsilon, const int &minLen);

			Tree dataToTree(const DataStr &data,const string &type, const int &bbestFet);

			int createTree();

			DTree(const string &file, const string &type);
			
			typedef std::shared_ptr<DTree> DTreePtr;
	};
}
