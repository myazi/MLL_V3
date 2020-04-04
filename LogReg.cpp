#include "LogReg.h"

namespace MLL {
                       
    /**
    �ݶ��½��㷨����Ҫ��ȷ�����ݶȷ��򣬲��������õ�����˼�����n��������
    ��Ŀ�꺯����͹�滮���⣬��ô�ֲ���Сֵ����ȫ����Сֵ

    **/
    int LogReg::gradAscent_Log()
    {
        if(_y._col!=1)
        {
            std::cout<<"logReg is two class"<<std::endl;
            return -1;
        }
        Matrix weights(_x._col,_y._col,0.1);
        Matrix xT = _x.transposeMatrix();

        float error=0;///��¼������
        int iter=0;
        int i,j;
        Matrix z(_y._row,_y._col,0);//���ȷ������Ĵ�С
        Matrix grad(_x._col,_y._col,0);
        for(iter=0; iter<_iter; iter++)
        {
            z = _x * weights;
            for(i = 0; i < z._row; i++)
            {
                sigmoid(z._data[i][0]);
            }
            z = _y - z ;
			for(i = 0; i < z._row; i++)
            {
                z._data[i][0] *= exp( -0.5 * _y._data[i][0]);
            }
            error=0;
            for(i=0; i<_x._row; i++)///ͳ�ƴ�����
                error+=z._data[i][0];
            //std::cout<<"error="<<error<<std::endl;
            //if(error<x._row/100 && error>x._row/100)///���ô�����С��һ��ֵʱ�˳�����
               // break;
            grad = xT * z ;///���㸺�ݶȷ���
            //grad = (1.0/x._row) * grad;
            for(i=0; i<grad._row; i++)
                grad._data[i][0]*= _alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
            weights = weights + grad;///�����ݶȷ�����һ������
        }

        /**
        ��֤�㷨����ȷ��
        **/
        int er1=0,er2=0;
        Matrix train=_x * weights;
		std::ofstream weight_file;
		weight_file.open("weight");
		for(int i=0; i < weights._row; i++)
		{
			for(int j=0; j < weights._col; j++)
			{
				weight_file<< weights._data[i][j]<<std::endl;
			}
		}
		weight_file.close();
		std::cout<<"----------train-------"<<std::endl;
        for(i=0; i<_y._row; i++)
        {
            if(train._data[i][0]>0)
            {
				std::cout<<1<<std::endl;
                //std::cout<<1-_y._data[i][0]<<std::endl;
                er1+=(1-_y._data[i][0]);
            }
            else
            {
				std::cout<<0<<std::endl;
                //std::cout<<0-_y._data[i][0]<<std::endl;
                er2-=(0-_y._data[i][0]);
            }
        }
        std::cout<<"er1="<<er1<<std::endl;
        std::cout<<"er2="<<er2<<std::endl;
		
		std::cout<<"-------test-------"<<std::endl;
		Matrix test_x;
		Matrix test_y;
		const char *test_file= "sample_100";
		test_x.init_by_data(test_file);

		test_y=test_x.getOneCol(test_x._col-1);
		test_x.deleteOneCol(test_x._col-1);
		std::cout<<"test_x" << test_x._row<<"&&"<<test_x._col<<std::endl;
		std::cout<<"test_y" << test_y._row<<"&&"<<test_y._col<<std::endl;
		Matrix test= test_x * weights;
        er1 = 0;
		er2 = 0;
		for(i=0; i<test_y._row; i++)
        {
			std::cout<<test_y._data[i][0]<<std::endl;
            if(test._data[i][0]>0)
            {
                er1+=(1-test_y._data[i][0]);
            }
            else
            {
                er2-=(0-test_y._data[i][0]);
            }
        }
        std::cout<<"er1="<<er1<<std::endl;
        std::cout<<"er2="<<er2<<std::endl;
    }
    /**
    ����ݶ��½����ݶ��½�����ͬ�����ڸ��ݶȷ����ȷ�����ݶ��½��Ǹ������е�������ȷ�����ݶȷ���
    ������ݶ��½�ÿ��ֻ��һ����������ȷ�����ݶȷ�����Ȼ����ȫ���ţ������ŵ����������ӣ�ͬ������

    **/
    int LogReg::stoGradAscent_Log()//����ݶ��½�ÿһ��ѡ��m�������������ݶ��½����򣬸ô�����ֻѡ��һ��������������ݶ��½���������ֵ
    {
        Matrix xOnerow(1,_x._col,0);
        Matrix xOnerowT(_x._col,1,0);

        Matrix weights(_x._col,_y._col,0.1);
        Matrix z(1,_y._col,0);//���ȷ������Ĵ�С
        Matrix grad(_x._col,_y._col,0);

        double error;
        int i,j,c;
        for(c=0; c<1000; c++)
        {
            for(i=0; i<_x._row; i++)
            {
                xOnerow=_x.getOneRow(i);///���ѡ��һ�������㣬����û�������ѡ�񣬶��ǰ���ѡ��
                z = xOnerow * weights;
               sigmoid(z._data[i][0]);
                z._data[i][0]=_y._data[i][0]-z._data[i][0];
                xOnerowT = xOnerow.transposeMatrix();
                grad = xOnerowT * z;///����һ��������Ԥ�������ȷ�����ݶȷ���
                for(j=0; j<grad._row; j++)
                    grad._data[j][0]*=_alpha;
                weights = weights + grad; ///����
            }
            }
        //��֤�㷨����ȷ��
        std::cout<<"test"<<std::endl;
        Matrix test = _x * weights;
        for(i=0; i<_y._row; i++)
        {
            if(test._data[i][0]>0)
            {
                std::cout<<1-_y._data[i][0]<<std::endl;
            }
            else
            {
                std::cout<<0-_y._data[i][0]<<std::endl;
            }
        }
    }
    LogReg::LogReg(const std::string &file, const std::string &model, const double alpha,const int iter)
    {
        std::cout<<"load_data:"<< file<<std::endl;
        std::cout<<"----------------------"<<std::endl;
        _x.init_by_data(file);
		_y=_x.getOneCol(_x._col-1);
		_x.deleteOneCol(_x._col-1);
        _y.print();
		//const char *file2 = "LDA_sample";
		//_x.LoadData_spare(file2, 17880,2000);
        //_x.print();
		std::cout<<_x._row<<"&"<<_x._col<<std::endl;
        _y.print();
		int sum = 0;
		for(int i=0; i<_y._row; i++)
		{
			for (int j=0;j<_y._col; j++)
			{
				std::cout<<"y="<< "\t"<<i <<"\t" << j <<"\t"<<_y._data[i][j] <<std::endl;
				sum += _y._data[i][j];
			}
		}
		std::cout<<"sum=" << sum<< std::endl;
		_alpha = alpha;
        _iter = iter;
    }
}
