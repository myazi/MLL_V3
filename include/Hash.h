#ifndef HASH_H
#define HASH_H
#include "MatrixOpe.h"
namespace MLL
{
	class Hash 
	{
		private:
			Matrix _X;
			Matrix _B;
			int _bits;
			int _iter;
			double _lambda;
		public:
			Hash(const std::string &file, const std::string &model, const int &bits, const double &lambda, const int &iter);
			int Normalize(Matrix &x);
			void init_B();
			void BCSH2();
		typedef std::shared_ptr<Hash> HashPtr;

	};
}




#endif
