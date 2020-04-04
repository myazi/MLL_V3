#include <io.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "Bayes.h"
#define CLASS_SUM 2
#define X_RANGE 2  //X_RANGE ��ʾX��ȡֵ��Χ�����ڲ���one-hot���룬����X��ÿһ��������ȡֵ��ΧΪ0��1

namespace MLL
{
	Bayes::DIC Bayes::createVocabList(const std::vector<DataStr> &dataClass)//���ɵ����ֵ�
	{
		int i = 0, j = 0, k = 0, vl = 0;
		std::vector<std::string> dic;
		for(k = 0; k < dataClass.size(); k++)
		{
			for(i = 0; i < dataClass[k].size(); i++)
			{
				for(j = 0; j < dataClass[k][i].size(); j++)
				{
					for(vl = 0; vl < dic.size(); vl++)
					{
						//std::cout<<dataClass[k][i][j]<<std::endl;
						if(!dataClass[k][i][j].compare(dic[vl]))
							break;
					}
					if(vl == dic.size())
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
		int i = 0, j = 0, k=0, vl = 0;
		int sampleNum = 0;
		for(k = 0; k < dataClass.size(); k++)
		{
			sampleNum += dataClass[k].size();
		}
		Matrix vecX;
		vecX.initMatrix(sampleNum, dic.size() + 1, 0);
		int iSample = 0;
		for(k = 0; k < dataClass.size(); k++)
		{
			for(i = 0; i < dataClass[k].size(); i++)//ֻ��һ��
			{
				vecX._data[iSample][dic.size()] = k;//��ǩ
				for(j = 0; j < dataClass[k][i].size(); j++)
				{
					for(vl = 0; vl < dic.size(); vl++)
					{
						if(!dataClass[k][i][j].compare(dic[vl]))
						{
							vecX._data[iSample][vl] = 1;//one-hot��������
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
		bayes.pY.initMatrix(CLASS_SUM,1,0);//�����ʼ��Ϊ2�е�������
		bayes.pX_1Y.initMatrix(CLASS_SUM,X._col,0);//X_1Y��ʾ��Y��X=1�ĸ��ʣ���֮X=0�ĸ���Ϊ1-
		bayes.pX.initMatrix(X._col,1,0);//

		int i = 0, j = 0, k = 0;
		for(k = 0; k < bayes.pX_1Y._row; k++)
		{
			for(i = 0; i < bayes.pX_1Y._col; i++)
			{
				bayes.pX_1Y._data[k][i] = 1;//ƽ������,Ĭ�ϳ���һ�Σ����ڹ�һ��ʱ�����������ĳ���Ҳ���ǽ�ȥ,�����ƽ����ָÿһ���ֵ��������ֵ�ģ�δ�漰����
				//�����е�δ�����
			}
		}
		for(i = 0; i < X._row; i++)
		{
			if(Y._data[i][0] == 0)
			{
				bayes.pY._data[0][0]++;
				for(j = 0; j < X._col; j++)
				{
					bayes.pX_1Y._data[0][j] += X._data[i][j];
				}
			}
			else
			{
				bayes.pY._data[1][0]++;
				for(j = 0; j < X._col; j++)
				{
					bayes.pX_1Y._data[1][j] += X._data[i][j];
				}
			}
		}
		for(i = 0; i < X._col; i++)
		{
			//��������x���������������ֵĸ���
			//bayes.pX._data[i][0] = (bayes.pX_1Y._data[0][i] - 1) + (bayes.pX_1Y._data[1][i] - 1) + 1;
			//bayes.pX._data[i][0] /= bayes.pY._data[0][0] + bayes.pY._data[1][0] + 2;

			//ĳһ����x���������������ֵĸ���
			bayes.pX_1Y._data[0][i] /= bayes.pY._data[0][0] + 2;
			bayes.pX_1Y._data[1][i] /= bayes.pY._data[1][0] + 2;

			bayes.pX._data[i][0] = (bayes.pX_1Y._data[0][i] + bayes.pX_1Y._data[1][i]) / 2;
		}
		//�����PY����ĸ���
		/*for(k = 0; k < bayes.pY._row; k++)
		{
			bayes.pY._data[k][0] /= X._row;
		}
		std::cout<< "pY=" << bayes.pY._data[0][0] <<std::endl;
        
		for(k = 0; k < bayes.pX_1Y._row; k++)
		{
			for(i = 0; i < bayes.pX_1Y._col; i++)
			{
				std::cout<< bayes.pX_1Y._data[k][i] <<"\t";
			}
			std::cout<<"--";
		}
        */
	}

	/**
	������ߺ���
	**/
	int Bayes::classifyNB(const Matrix &testVecX)
	{
		double p0 = 1.0, p1 = 1.0;
		int i = 0, j = 0;
		for(i = 0; i < testVecX._row; i++)
		{
			p0 = 1.0, p1 = 1.0;
			for(j = 0; j < testVecX._col; j++)
			{
				//�������������ֵĸ���Ϊ1-
				if(testVecX._data[i][j] == 0)
				{
					p0 *= (1 - bayes.pX_1Y._data[0][j]) /(1 - bayes.pX._data[j][0]);
					p1 *= (1 - bayes.pX_1Y._data[1][j]) /(1 - bayes.pX._data[j][0]);
				}
				//�����������ֵĸ���
				else
				{
					p0 *= bayes.pX_1Y._data[0][j] / bayes.pX._data[j][0];
					p1 *= bayes.pX_1Y._data[1][j] / bayes.pX._data[j][0];
				}
			}
			p0 *= bayes.pY._data[0][0];
			p1 *= bayes.pY._data[1][0];
			std::cout<<"p0="<<p0<<"&"<<"p1="<<p1;
			if(p0 < p1)
				std::cout<<"class="<<1;
			else
				std::cout<<"class="<<0;
			std::cout<<std::endl;
		}
		return 0;

	}
	/*win	
	void Bayes::getAllFiles(const std::string &path, const std::vector<std::string> &files)
	{
		//�ļ����
		long   hFile = 0;
		//�ļ���Ϣ
		_finddata_t fileinfo;
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
    //linux
    void Bayes::getAllFiles(const std::string &path, std::vector<std::string> &files)
    {
        const char *dir = path.c_str();
        DIR *pDir = NULL;
        struct dirent *pFile = NULL;
		std::string p;
        pDir = opendir(dir);
        if (pDir == NULL) return;
        
        while ((pFile = readdir(pDir)) != NULL) 
        {
            if (pFile->d_type & DT_DIR) 
            {
                if (strcmp(pFile->d_name, ".") == 0 
                    || strcmp(pFile->d_name, "..") == 0) continue;
                char Path[256];
                int len = strlen(dir);
                strncpy(Path, dir, len + 1);
                if (dir[len - 1] != '/') strncat(Path, "/", 2);
                strncat(Path, pFile->d_name, strlen(pFile->d_name) + 1);
                //getAllFiles(Path,files);//
            } 
            else
            {
                files.push_back(p.assign(dir).append("/").append(pFile->d_name));
            }
        }
        
        closedir(pDir);
    }
	Bayes::Bayes(const std::string &file)
	{

		int k = 0, i = 0, j = 0;
		std::string path[CLASS_SUM]= {"data/email/ham/","data/email/spam/"};
		std::vector<std::string> files;
		std::vector< std::vector<std::string> > filess;
		RowDataStr rowData;
		DataStr data;
		std::vector<DataStr> dataClass;
		for(k = 0; k < CLASS_SUM; k++)
		{
			getAllFiles(path[k],files);
			filess.push_back(files);
			files.clear();
		}
		for(k = 0; k < filess.size(); k++)
		{
			for(i = 0; i < filess[k].size(); i++)
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
		Matrix X = createFectVec(dataClass,dic);

		/**
		����������ǩ
		**/
		Matrix Y;
		Y = X.getOneCol(X._col-1);
		X.deleteOneCol(X._col-1);
		std::cout<<X._row<<"*"<<X._col<<std::endl;
		std::cout<<Y._row<<"*"<<Y._col<<std::endl;
		Y.print();
		trainNB(X,Y);///ͨ����������ͱ�ǩ������ò�����⺯�����bayes���ߵĲ���

		classifyNB(X);///�������

		Matrix pxt=bayes.pX.transposeMatrix();
		//pxt.print();
	}
}
