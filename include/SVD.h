#ifndef SVD_H
#define SVD_H
#include "MatrixOpe.h"

namespace MLL {

	class SVD {
		private:
			Matrix _A;
			Matrix _u;
			Matrix _v;
			int _d;
			double _alpha;
			double _iter;

		public:
			void init_uv();
			void init_uv_sparse(const int &row, const int &col);
			int MF();
			int Sparse_MF();
			int NMF();
			SVD(const std::string &file, const std::string &model, const int &d, const double &alpha, const int &iter);
			~SVD(){}
			typedef std::shared_ptr<SVD> SVDPtr;
			
	};
}

#endif

