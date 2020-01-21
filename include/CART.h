/***

�ع��� CART

����

1��С������ ,��Ϻ������þ�ֵ������ ��Ϻ�����������ģ��
2������ֵ��ѡ���Ǵ����������ѡ�񣬲��ǰ�����ѡ��
2�������ͣ�string������ֵ
3Ԥ��֦�����֦
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
			int bestIndex;//��ʾ��õķ������ԣ�����Ҷ�ӽڵ�ʱ������ʾ���������±꣬����Ϊ-1����ʾΪҶ�ӽڵ���
			double value;//��Ϊ���ѽڵ㣬���ʾ������ֵ������ΪҶ�ӽڵ㣬������¼Ҷ�ӽڵ�ľ�ֵ
		};
		typedef struct bitnode
		{
			struct bitnode *left;//С�ڵ�����ֵ��������
			struct bitnode *right;//������ֵ��������
			int feature;//ֻ�з�Ҷ�ӽڵ���з������ԣ�Ҷ�ӽڵ�feature=-1
			double meanValue;//�����Ҷ�ӽڵ�Ϊֵ��,��Ҷ�ӽڵ�Ϊ��ֵ
			Data data;//ÿ�������洢�˸����µ�����
			bitnode():left(NULL),right(NULL),feature(-1),meanValue(0),data() {};
		} bitnode,*bitree;
		public:	
			twoSubData_C binSplitDataSet(const Data &data,const int &axis,const double &value);

			double mean_C(const Data &data);

			double MeanErr_C(const Data &data);

			split_C chooseBestSplit(const Data &data,const double &minErr);

			int createBinTree(bitree &t,Data &data);

			int preorder(bitree &t);//�ݹ��������������

			int prune(bitree &t,const Data &testData);

			double predict(bitree t, const RowData &data);//bitree &t���������ã����ܸı�����

			CART(const std::string &file);

			typedef std::shared_ptr<CART> CARTPtr;
	};
}
