#ifndef RF_H
#define RF_H

#include "MatrixOpe.h"

namespace MLL
{
	class RF
	{
		struct twoSubData
		{
			Data left;
			Data right;
		};
		struct split
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
		} bitnode,*bitree;
		public:
			twoSubData binSplitDataSet(const Data &data, const int &axis, const double &value);

			double mean(const Data &data);

			double MeanErr(const Data &data);

			split chooseBestSplit(const Data &data, const int &deep, const double &minErr, const int &minLen);

			int createBinTree(bitree &t,const Data &data, const int &deep, const int &epsilon);

			int preorder(bitree &t);//�ݹ��������������

			double predict(bitree t, const RowData &data);//bitree &t���������ã����ܸı�����

			Data Bootstrap(const Data &data);

			RF(const std::string &file, const int &numIt, const int &deep, const int &epsilon);

			typedef std::shared_ptr<RF> RFPtr;
	};
}
#endif
