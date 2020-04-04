#include "MatrixOpe.h"
namespace MLL
{
	class CART
	{
		struct twoSubData
		{
			Data left;
			Data right;
		};
		struct split
		{
			int bestIndex = 0; //��¼��õķ������ԣ�����Ҷ�ӽڵ�ʱ������ʾ���������±꣬����Ϊ-1����ʾΪҶ�ӽڵ���
			double value = 0; //��Ϊ���ѽڵ㣬���ʾ������ֵ������ΪҶ�ӽڵ㣬������¼Ҷ�ӽڵ�ľ�ֵ
		};
		typedef struct bitnode
		{
			struct bitnode *left = NULL; //С�ڵ�����ֵ��������
			struct bitnode *right = NULL; //������ֵ��������
			int feature = 0; //ֻ�з�Ҷ�ӽڵ���з������ԣ�Ҷ�ӽڵ�feature=-1
			double meanValue = 0; //�����Ҷ�ӽڵ�Ϊֵ��,��Ҷ�ӽڵ�Ϊ��ֵ
			Data data; //ÿ�������洢�˸����µ�����
			bitnode():left(NULL),right(NULL),feature(-1),meanValue(0),data() {};
		} bitnode,*bitree;

		public:	
			twoSubData binSplitDataSet(const Data &data,const int &axis,const double &value);

			double mean(const Data &data);

			double MeanErr(const Data &data);

			split chooseBestSplit(const Data &data,const double &minErr);

			int createBinTree(bitree &t,Data &data);

			int preorder(bitree &t);//�ݹ��������������

			int prune(bitree &t,const Data &testData);

			double predict(bitree t, const RowData &data);//bitree &t���������ã����ܸı�����

			CART(const std::string &file);

			typedef std::shared_ptr<CART> CARTPtr;
	};
}
