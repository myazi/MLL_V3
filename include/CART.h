/***

回归树 CART

考虑

1最小均方差 ,拟合函数采用均值函数， 拟合函数采用线性模型
2特征阈值的选择是从逐个样本中选择，不是按步长选择
2属性类型（string）连续值
3预剪枝，后剪枝
**/

#include "MatrixOpe.h"
#include <memory>
namespace MLL
{
	class CART
	{
		struct twoSubData_C
		{
			Data left;
			Data right;
		};
		struct split_C
		{
			int bestIndex;//表示最好的分类属性，当非叶子节点时，即表示分裂属性下标，否则为-1，表示为叶子节点标记
			double value;//若为分裂节点，则表示分裂阈值，否则为叶子节点，用来记录叶子节点的均值
		};
		typedef struct bitnode
		{
			struct bitnode *left;//小于等于阈值的左子树
			struct bitnode *right;//大于阈值的右子树
			int feature;//只有非叶子节点才有分裂属性，叶子节点feature=-1
			double meanValue;//如果是叶子节点为值型,非叶子节点为阈值
			Data data;//每颗树都存储了该树下的数据
			bitnode():left(NULL),right(NULL),feature(-1),meanValue(0),data() {};
		} bitnode,*bitree;
		public:	
			twoSubData_C binSplitDataSet(const Data &data,const int &axis,const double &value);

			double mean_C(const Data &data);

			double MeanErr_C(const Data &data);

			split_C chooseBestSplit(const Data &data,const double &minErr);

			int createBinTree(bitree &t,Data &data);

			int preorder(bitree &t);//递归先序遍历二叉树

			int prune(bitree &t,const Data &testData);

			double predict(bitree t, const RowData &data);//bitree &t，不能引用，不能改变树根

			CART(const std::string &file);

			typedef std::shared_ptr<CART> CARTPtr;
	};
}
