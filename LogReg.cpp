#include "LogReg.h"

namespace MLL {
                       
    /**
    �ݶ��½��㷨����Ҫ��ȷ�����ݶȷ��򣬲��������õ�����˼�����n��������
    ��Ŀ�꺯����͹�滮���⣬��ô�ֲ���Сֵ����ȫ����Сֵ

    **/
    int LogReg::gradAscent_Log()
    {
        if(_y.col!=1)
        {
            std::cout<<"logReg is two class"<<std::endl;
            return -1;
        }
        Matrix weights(_x.col,_y.col,0.1,"T");
        Matrix xT = _x.transposeMatrix();

        float error=0;///��¼������
        int iter=0;
        int i,j;
        Matrix z(_y.row,_y.col,0,"T");//���ȷ������Ĵ�С
        Matrix grad(_x.col,_y.col,0,"T");
        for(iter=0; iter<_iter; iter++)
        {
            z = _x * weights;
            for(i = 0; i < z.row; i++)
            {
                sigmoid(&z.data[i][0]);
            }
            z = _y - z ;
           for(i = 0; i < z.row; i++)
            {
                z.data[i][0] *= exp( -0.5 * _y.data[i][0]);
            }
            error=0;
            for(i=0; i<_x.row; i++)///ͳ�ƴ�����
                error+=z.data[i][0];
            std::cout<<"error="<<error<<std::endl;
            //if(error<x.row/100 && error>x.row/100)///���ô�����С��һ��ֵʱ�˳�����
               // break;
            grad = xT * z ;///���㸺�ݶȷ���
            //grad = (1.0/x.row) * grad;
            for(i=0; i<grad.row; i++)
                grad.data[i][0]*= _alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
            weights = weights + grad;///�����ݶȷ�����һ������
        }

        /**
        ��֤�㷨����ȷ��
        **/
        int er1=0,er2=0;
        Matrix train=_x * weights;
		std::ofstream weight_file;
		weight_file.open("weight");
		for(int i=0; i < weights.row; i++)
		{
			for(int j=0; j < weights.col; j++)
			{
				weight_file<< weights.data[i][j]<<std::endl;
			}
		}
		weight_file.close();
		std::cout<<"----------train-------"<<std::endl;
        for(i=0; i<_y.row; i++)
        {
            if(train.data[i][0]>0)
            {
				std::cout<<1<<std::endl;
                //std::cout<<1-_y.data[i][0]<<std::endl;
                er1+=(1-_y.data[i][0]);
            }
            else
            {
				std::cout<<0<<std::endl;
                //std::cout<<0-_y.data[i][0]<<std::endl;
                er2-=(0-_y.data[i][0]);
            }
        }
        std::cout<<"er1="<<er1<<std::endl;
        std::cout<<"er2="<<er2<<std::endl;
		
		std::cout<<"-------test-------"<<std::endl;
		Matrix test_x;
		Matrix test_y;
		const char *test_file= "sample_100";
		test_x.LoadData(test_file);

		test_y=test_x.getOneCol(test_x.col-1);
		test_x.deleteOneCol(test_x.col-1);
		std::cout<<"test_x" << test_x.row<<"&&"<<test_x.col<<std::endl;
		std::cout<<"test_y" << test_y.row<<"&&"<<test_y.col<<std::endl;
		Matrix test= test_x * weights;
        er1 = 0;
		er2 = 0;
		for(i=0; i<test_y.row; i++)
        {
			std::cout<<test_y.data[i][0]<<std::endl;
            if(test.data[i][0]>0)
            {
                er1+=(1-test_y.data[i][0]);
            }
            else
            {
                er2-=(0-test_y.data[i][0]);
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
        Matrix xOneRow(1,_x.col,0,"T");
        Matrix xOneRowT(_x.col,1,0,"T");

        Matrix weights(_x.col,_y.col,0.1,"T");
        Matrix z(1,_y.col,0,"T");//���ȷ������Ĵ�С
        Matrix grad(_x.col,_y.col,0,"T");

        double error;
        int i,j,c;
        for(c=0; c<1000; c++)
        {
            for(i=0; i<_x.row; i++)
            {
                xOneRow=_x.getOneRow(i);///���ѡ��һ�������㣬����û�������ѡ�񣬶��ǰ���ѡ��
                z = xOneRow * weights;
               sigmoid(&z.data[i][0]);
                z.data[i][0]=_y.data[i][0]-z.data[i][0];
                xOneRowT = xOneRow.transposeMatrix();
                grad = xOneRowT * z;///����һ��������Ԥ�������ȷ�����ݶȷ���
                for(j=0; j<grad.row; j++)
                    grad.data[j][0]*=_alpha;
                weights = weights + grad; ///����
            }
            }
        //��֤�㷨����ȷ��
        std::cout<<"test"<<std::endl;
        Matrix test = _x * weights;
        for(i=0; i<_y.row; i++)
        {
            if(test.data[i][0]>0)
            {
                std::cout<<1-_y.data[i][0]<<std::endl;
            }
            else
            {
                std::cout<<0-_y.data[i][0]<<std::endl;
            }
        }
    }
    LogReg::LogReg(const std::string &file, const std::string &model, const double alpha,const int iter)
    {
        std::cout<<"loadData:"<< file<<std::endl;
        std::cout<<"----------------------"<<std::endl;
        _x.LoadData(file);
		_y=_x.getOneCol(_x.col-1);
		_x.deleteOneCol(_x.col-1);
        _y.print();
		//const char *file2 = "LDA_sample";
		//_x.LoadData_spare(file2, 17880,2000);
        //_x.print();
		std::cout<<_x.row<<"&"<<_x.col<<std::endl;
        _y.print();
		int sum = 0;
		for(int i=0; i<_y.row; i++)
		{
			for (int j=0;j<_y.col; j++)
			{
				std::cout<<"y="<< "\t"<<i <<"\t" << j <<"\t"<<_y.data[i][j] <<std::endl;
				sum += _y.data[i][j];
			}
		}
		std::cout<<"sum=" << sum<< std::endl;
		_alpha = alpha;
        _iter = iter;
    }
}
