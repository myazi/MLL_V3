//#include <io.h>
#include <string.h>
#include <stdio.h>
#include "Bayes.h"

#define CLASS_SUM 2
#define X_RANGE 2  //X_RANGE ��ʾX��ȡֵ��Χ�����ڲ���one-hot���룬����X��ÿһ��������ȡֵ��ΧΪ0��1

namespace MLL
{
	Bayes::DIC Bayes::createVocabList(const std::vector<DataStr> &dataClass)//���ɵ����ֵ�
	{
		int i,j,k,vl;
		static std::vector<std::string> dic;
		for(k=0; k<dataClass.size(); k++)
		{
			for(i=0; i<dataClass[k].size(); i++)
			{
				for(j=0; j<dataClass[k][i].size(); j++)
				{
					for(vl=0; vl<dic.size(); vl++)
					{
						//std::cout<<dataClass[k][i][j]<<std::endl;
						if(!dataClass[k][i][j].compare(dic[vl]))
							break;
					}
					if(vl==dic.size())
					{
						dic.push_back(dataClass[k][i][j]);
					}
				}
			}
		}
		return dic;
	}

	Matrix Bayes::createFectVec(const std::vector<DataStr> &dataClass, const Bayes::DIC &dic)//����ѵ����������
	{
		int i,j,k=0,vl;
		int sampleNum=0;
		for(k=0; k<dataClass.size(); k++)
		{
			sampleNum+=dataClass[k].size();
		}
		Matrix vecX;
		vecX.initMatrix(sampleNum,dic.size()+1,0,"ss");
		int iSample=0;
		for(k=0; k<dataClass.size(); k++)
		{
			for(i=0; i<dataClass[k].size(); i++)//ֻ��һ��
			{
				vecX.data[iSample][dic.size()]=k;//��ǩ
				for(j=0; j<dataClass[k][i].size(); j++)
				{
					for(vl=0; vl<dic.size(); vl++)
					{
						if(!dataClass[k][i][j].compare(dic[vl]))
						{
							vecX.data[iSample][vl]=1;//one-hot��������
							break;
						}
					}
				}
				iSample++;
			}
		}
		return vecX;
	}

	int Bayes::trainNB(const Matrix &X, const Matrix &Y)//ѵ��������ʵ�֣�ע��Բ�������ƽ������
	{
		//�࣬����������ȡֵ
		bayes.pY.initMatrix(CLASS_SUM,1,0,"ss");//�����ʼ��Ϊ2�е�������
		bayes.pX_1Y.initMatrix(CLASS_SUM,X.col,0,"ss");//X_1Y��ʾ��Y��X=1�ĸ��ʣ���֮X=0�ĸ���Ϊ1-
		bayes.pX.initMatrix(X.col,1,0,"ss");//

		int i,j,k;
		for(k=0; k<bayes.pX_1Y.row; k++)
		{
			for(i=0; i<bayes.pX_1Y.col; i++)
			{
				bayes.pX_1Y.data[k][i]=1;//ƽ������,Ĭ�ϳ���һ�Σ����ڹ�һ��ʱ�����������ĳ���Ҳ���ǽ�ȥ,�����ƽ����ָÿһ���ֵ��������ֵ�ģ�δ�漰����
				//�����е�δ�����
			}
		}
		for(i=0; i<X.row; i++)
		{
			if(Y.data[i][0]==0)
			{
				bayes.pY.data[0][0]++;
				for(j=0; j<X.col; j++)
				{
					bayes.pX_1Y.data[0][j]+=X.data[i][j];
				}
			}
			else
			{
				bayes.pY.data[1][0]++;
				for(j=0; j<X.col; j++)
				{
					bayes.pX_1Y.data[1][j]+=X.data[i][j];
				}
			}
		}
		for(i=0; i<X.col; i++)
		{
			//��������x���������������ֵĸ���
			//bayes.pX.data[i][0]=(bayes.pX_1Y.data[0][i]-1) + (bayes.pX_1Y.data[1][i]-1) + 1;
			//bayes.pX.data[i][0]/=bayes.pY.data[0][0] + bayes.pY.data[1][0] + 2;

			//ĳһ����x���������������ֵĸ���
			bayes.pX_1Y.data[0][i]/=bayes.pY.data[0][0] + 2;
			bayes.pX_1Y.data[1][i]/=bayes.pY.data[1][0] + 2;

			bayes.pX.data[i][0] = (bayes.pX_1Y.data[0][i] + bayes.pX_1Y.data[1][i])/2;
		}
		//�����PY����ĸ���
		for(k=0; k<bayes.pY.row; k++)
		{
			bayes.pY.data[k][0]/=X.row;
		}
		std::cout<<"pY="<<bayes.pY.data[0][0]<<std::endl;

		for(k=0; k<bayes.pX_1Y.row; k++)
		{
			for(i=0; i<bayes.pX_1Y.col; i++)
			{
				std::cout<<bayes.pX_1Y.data[k][i]<<" & ";
			}
			std::cout<<"---";
		}
	}

	/**
	������ߺ���
	**/
	int Bayes::classifyNB(const Matrix &testVecX)
	{
		double p0=1.0,p1=1.0;
		int i,j;
		for(i=0; i<testVecX.row; i++)
		{
			p0=1.0,p1=1.0;
			for(j=0; j<testVecX.col ; j++)
			{
				//�������������ֵĸ���Ϊ1-
				if(testVecX.data[i][j]==0)
				{
					p0*=(1-bayes.pX_1Y.data[0][j])/(1-bayes.pX.data[j][0]);
					p1*=(1-bayes.pX_1Y.data[1][j])/(1-bayes.pX.data[j][0]);
				}
				//�����������ֵĸ���
				else
				{
					p0*=(bayes.pX_1Y.data[0][j])/bayes.pX.data[j][0];
					p1*=(bayes.pX_1Y.data[1][j])/bayes.pX.data[j][0];
				}
			}
			p0*=bayes.pY.data[0][0];
			p1*=bayes.pY.data[1][0];
			std::cout<<"p0="<<p0<<"&"<<"p1="<<p1;
			if(p0<p1)
				std::cout<<"class="<<1;
			else
				std::cout<<"class="<<0;
			std::cout<<std::endl;
		}
		return 0;

	}
	/*
	void Bayes::getAllFiles(const std::string &path, const std::vector<std::string> &files)
	{
		//�ļ����
		long   hFile = 0;
		//�ļ���Ϣ
		struct _finddata_t fileinfo;
		std::string p;
		if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
		{
			do
			{
				//�����Ŀ¼,����֮
				//�������,�����б�
				if ((fileinfo.attrib &  _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getAllFiles(p.assign(path).append("/").append(fileinfo.name), files);
				}
				else
				{
					files.push_back(p.assign(path).append("/").append(fileinfo.name));
				}
			}
			while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}
	*/
	Bayes::Bayes(const std::string &file)
	{

		int k,i,j;
		std::string path[CLASS_SUM]= {"data/email/ham/","data/email/spam/"};
		std::vector<std::string> files;
		std::vector< std::vector<std::string> > filess;
		RowDataStr rowData;
		DataStr data;
		std::vector<DataStr> dataClass;
		for(k=0; k<CLASS_SUM; k++)
		{
			//getAllFiles(path[k],files);
			filess.push_back(files);
			files.clear();
		}
		std::cout<<"loadData"<<std::endl;
		std::cout<<"----------------------"<<std::endl;
		for(k=0; k<filess.size(); k++)
		{
			for(i=0; i<filess[k].size(); i++)
			{
				LoadDataStr(rowData,filess[k][i].c_str());
				data.push_back(rowData);
				rowData.clear();
			}
			dataClass.push_back(data);
			data.clear();
		}
		
		Bayes::DIC dic = createVocabList(dataClass);
		
		std::cout<<dic.size();
		/**
		������������
		**/
		Matrix X=createFectVec(dataClass,dic);

		/**
		����������ǩ
		**/
		Matrix Y;
		Y = X.getOneCol(X.col-1);
		X.deleteOneCol(X.col-1);
		std::cout<<X.row<<"*"<<X.col<<std::endl;
		std::cout<<Y.row<<"*"<<Y.col<<std::endl;
		Y.print();
		trainNB(X,Y);///ͨ����������ͱ�ǩ������ò�����⺯�����bayes���ߵĲ���

		classifyNB(X);///�������

		//bayes.pX_1Y.print();
		std::cout<<"----------"<<std::endl;
		Matrix pxt=bayes.pX.transposeMatrix();
		pxt.print();
	}
}
