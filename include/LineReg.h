#ifndef LineReg_H
#define LineReg_H
#include "MatrixOpe.h"
#include <iostream>
#include <memory>
namespace MLL{
    class LineReg{
        private:
            Matrix _x;
            Matrix _y;
            Matrix _w;
            double _lamd = 0.1;//
            double _k = 1;//
            enum moedl {gradAscent, stoGradAscent};
        public:
            LineReg(const char *file, const string &model,const double &lamd, const double &k);
            int regression();
            int ridgeRegres();
            int lwlrtest();
            Matrix locWeightLineReg(Matrix &test);
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
    typedef std::shared_ptr<LineReg> LineRegPtr;
    };
}
#endif
