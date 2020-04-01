#include "MatrixOpe.h"

namespace MLL 
{
	class KMeans
	{
		struct CenAndDis
		{
			Matrix cen;//聚类中心
			Matrix dis;//每一个样本到其聚类中心的距离，即为一个m行的列向量
		};
		private:
			Matrix _x;
		public:
			double distances(Matrix xOne,Matrix kCen);

			Matrix randCent(Matrix x,int kNum);

			CenAndDis kMeans(Matrix &x,const int &kNum, const int &Iter);

			Matrix subMatrix(const Matrix &x, const Matrix &clusterAssment,const int &label);

			int biKmeans(Matrix &x,const int &kNum,const int &Iter);

			KMeans(const std::string &file);

			typedef std::shared_ptr<KMeans> KMeansPtr;
	};
}
