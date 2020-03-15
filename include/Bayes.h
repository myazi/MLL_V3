#ifndef Bayes_H
#define Bayes_H
#include "MatrixOpe.h"
#include <memory>
namespace MLL
{
	class Bayes
	{
		struct Par
		{
			Matrix pX_1Y;
			Matrix pY;
			Matrix pX;
		};
		Par bayes;//����bayes���������еĲ���
		
		public:

			typedef std::vector<std::string> DIC;
			
			DIC createVocabList(const std::vector<DataStr> &dataClass);//���ɵ����ֵ�

			Matrix createFectVec(const std::vector<DataStr> &dataClass, const DIC &dic);//����ѵ����������

			int trainNB(const Matrix &X, const Matrix &Y);//ѵ��������ʵ�֣�ע��Բ�������ƽ������

			int classifyNB(const Matrix &testVecX);//������ߺ���

			void getAllFiles(const std::string &path, const std::vector <std::string> &files);

			Bayes(const std::string &file);

			typedef std::shared_ptr<Bayes> BayesPtr;
	};
}
#endif

