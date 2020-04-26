#ifndef PCA_H
#define PCA_H
#include "MatrixOpe.h"

namespace MLL 
{
	class PCA
	{
		private:
			Matrix _A;

		public:
			PCA(const std::string &file);
	};
}


#endif
