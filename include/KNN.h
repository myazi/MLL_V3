#ifndef KNN_H
#define KNN_H

#include "MatrixOpe.h"
namespace MLL 
{
	class KNN
	{
		public:
			int autoNorm(Matrix &x);

			Matrix cdistances(const Matrix &test, const Matrix &x);

			Matrix getK(const Matrix &oneTest, const Matrix &x, const int &K);

			int classfiy(Matrix &testData,const Matrix &testDatay, Matrix &x, const Matrix &y,const int &K);

			KNN(const std::string &file);

			typedef std::shared_ptr<KNN> KNNPtr;
	};
}
#endif
