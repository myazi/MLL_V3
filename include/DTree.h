/***
��������ID3,C4.5,CART

����

1��Ϣ����,��Ϣ������,����ָ��
2�������ͣ�string��
3Ԥ��֦
**/

#include "MatrixOpe.h"
#define MAX_SIZE_OF_TRAINING_SET 1000

namespace MLL {

	class DTree
	{
		struct Tree
		{
			int id;
			int split_feat;//��������
			std::string feat_value;//������ֵ
			std::string label;//�������
			int size;
			int kind;
			int parent;
			Tree *next;
		};
		private:
			Tree tree[MAX_SIZE_OF_TRAINING_SET];//�����洢�������������������
			std::stack<Tree> s;
			int node=0;//�����洢�������������������������±�
			std::vector<int> feat_flag;//��ʼ��Ϊ0,���һ������ͳ�ƻ�ʣ��������δʹ��
			DataStr data;

		public:
			std::vector<std::string> getkindattr(const DataStr &data, const int &axis);//���axis���Ե����в�ͬȡֵ

			double calcShannonEntOrGini(const DataStr &data, const std::string &type);

			DataStr splitDataSet(const DataStr &data,const int &axis, const std::string &value);

			int chooseBestFectureTosplit(const DataStr &data, const std::string &type, const double &epsilon, const int &minLen);

			Tree dataToTree(const DataStr &data,const std::string &type, const int &bbestFet);

			int createTree();

			DTree(const std::string &file, const std::string &type);
			
			typedef std::shared_ptr<DTree> DTreePtr;
	};
}
