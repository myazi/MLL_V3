#ifndef LogReg_H
#define LogReg_H
#include "MatrixOpe.h"
#include <iostream>
#include <memory>
namespace MLL {
    class LogReg {
        private:
            Matrix _x;
            Matrix _y;
            Matrix _w;
            double _alpha;
            int _iter; 
            enum moedl {gradAscent, stoGradAscent};
        public:
            LogReg(const char *file, const string &model, const double alpha,const int iter);
            int gradAscent_Log();
            int stoGradAscent_Log();//随机梯度下降每一次选择m个样本进行求梯度下降方向，该代码中只选择一个样本进行求解梯度下降方向与数值
            inline Matrix get_x() const
            {
                return _x;
            };
            inline Matrix get_y() const
            {
                return _y;
            };
            inline Matrix get_w() const
            {
                return _w;
            };
		typedef std::shared_ptr<LogReg> LogRegPtr;
};
}
#endif
