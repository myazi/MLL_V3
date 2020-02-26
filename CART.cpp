/***

�ع��� CART

����

1��С������ ,��Ϻ������þ�ֵ������ ��Ϻ�����������ģ��
2������ֵ��ѡ���Ǵ����������ѡ�񣬲��ǰ�����ѡ��
2�������ͣ�string������ֵ
3Ԥ��֦�����֦

**/

#include "CART.h"
#define MAX 1000000000

namespace MLL
{
	CART::twoSubData_C CART::binSplitDataSet(const Data &data,const int &axis,const double &value)
	{
		twoSubData_C twosubdata;
		for(size_t i=0; i<data.size(); i++)
		{
			if(data[i][axis]<=value)
			{
				twosubdata.left.push_back(data[i]);
			}
			else
			{
				twosubdata.right.push_back(data[i]);
			}
		}
		return twosubdata;
	}
	double CART::mean_C(const Data &data)
	{
		if(data.size()==0)
			return 0;
		double meanvalue=0;
		int label_index=data[0].size()-1;
		for(size_t i=0; i<data.size(); i++)
		{
			meanvalue+=data[i][label_index];
		}
		meanvalue/=data.size();
		return meanvalue;
	}
	double CART::MeanErr_C(const Data &data)
	{
		if(data.size()==0)
			return 0;
		double meanvalue=mean_C(data);
		double meanErr=0;
		int label_index=data[0].size()-1;
		for(size_t i=0; i<data.size(); i++)
		{
			meanErr+=(data[i][label_index]-meanvalue)*(data[i][label_index]-meanvalue);
		}
		meanErr=sqrt(meanErr/data.size());
		return meanErr;
	}
	CART::split_C CART::chooseBestSplit(const Data &data,const double &minErr)
	{
		size_t i,j;
		double oldMeanErr=MeanErr_C(data);
		double bestMeanErr=MAX;
		double newMeanErr;
		int signvalue=1;
		twoSubData_C twosubdata;
		split_C sp;
		int label_index=data[0].size()-1;
		double value=data[0][label_index];
		for(i=0; i<data.size(); i++)
		{
			if(data[i][label_index]!=value)
			{
				signvalue=0;
				break;
			}
		}
		if(signvalue)
		{
			std::cout<<"signvalue"<<std::endl;
			sp.bestIndex=-1;
			sp.value=mean_C(data);
			return sp;
		}
		for(i=0; i<data[0].size()-1; i++)
		{
			for(j=0; j<data.size(); j++)
			{
				twosubdata=binSplitDataSet(data,i,data[j][i]);
				if(twosubdata.left.size()==0 || twosubdata.right.size()==0)
					continue;
				newMeanErr=MeanErr_C(twosubdata.left)+MeanErr_C(twosubdata.right);
				if(newMeanErr<bestMeanErr)
				{
					sp.bestIndex=i;
					sp.value=data[j][i];
					bestMeanErr=newMeanErr;
				}
			}
		}
		if(oldMeanErr-bestMeanErr<minErr)
		{
			sp.bestIndex=-1;
			sp.value=mean_C(data);
		}
		return sp;
	}
	int CART::createBinTree(bitree &t,Data &data)
	{
		if(data.size()==0)
			return 0;
		if(!(t=(bitree)malloc(sizeof(bitnode)))) exit(-1);
		split_C sp=chooseBestSplit(data,0.001);
		t->feature=sp.bestIndex;
		t->meanValue=sp.value;
		//t->data=data;//Ϊʲô�ṹ��data���ܸ�ֵ
		if(t->feature==-1)
		{
			t->left=NULL;
			t->right=NULL;
			std::cout<<"feat-1"<<std::endl;
			return 0;
		}
		//std::cout<<"feature="<<t->feature<<"  value="<<t->meanValue<<std::endl;
		twoSubData_C twosubdata=binSplitDataSet(data,sp.bestIndex,sp.value);
		createBinTree((t->left),twosubdata.left);
		createBinTree((t->right),twosubdata.right);
		return 0;
	}
	int CART::preorder(bitree &t)//�ݹ��������������
	{
		if(t!=NULL)
		{
			if(t->feature==-1)
				std::cout<<t->feature<<"  "<<t->meanValue<<"  "<<std::endl;//<<t->data.size()<<std::endl;//ֻ���Ҷ�ӽڵ�
			if(t->left!=NULL)
				preorder(t->left);
			if(t->right!=NULL)
				preorder(t->right);
		}
		return 0;
	}
	int CART::prune(bitree &t,const Data &testData)
	{
		if(t==NULL||testData.size()==0)//��������Ƿ�ΪNULL,��Ȼ����Ĳ���ִ�в���
			return 0;
		if(t->left->feature!=-1||t->right->feature!=-1)
		{
			twoSubData_C twosubdata=binSplitDataSet(testData,t->feature,t->meanValue);
			if(t->left->feature!=-1)
			{
				prune(t->left,twosubdata.left);
			}
			if(t->right->feature!=-1)
			{
				prune(t->right,twosubdata.right);
			}
			if(t->right->feature==-1 && t->left->feature==-1)
			{
				prune(t,testData);
			}
		}
		else
		{
			twoSubData_C twosubdata=binSplitDataSet(testData,t->feature,t->meanValue);
			double errortwo=MeanErr_C(twosubdata.left) + MeanErr_C(twosubdata.right);
			double errorone=MeanErr_C(testData);
			if(errorone<errortwo)
			{
				std::cout<<"merge"<<std::endl;
				t->left=NULL;
				t->right=NULL;
				t->feature=-1;
				t->meanValue=mean_C(testData);
			}
			return 0;
		}
		return 0;
	}
	double CART::predict(bitree t, const RowData &data)//bitree &t���������ã����ܸı�����
	{
		while(t)
		{
			if(t->feature==-1)
				return t->meanValue;
			else
			{
				if(data[t->feature] <= t->meanValue)
					t=t->left;
				else
					t=t->right;
			}
		}
		return 0;
	}
	CART::CART(const std::string &file)
	{
		Data data;
		LoadData_t(data,"data/cart.txt");
		bitree t;
		createBinTree(t,data);
		preorder(t);
		Data testdata;
		LoadData_t(testdata,"data/carttest.txt");
		prune(t,testdata);
		preorder(t);
		std::vector<double> labels(testdata.size(),0);
		for(size_t i =0; i<testdata.size(); i++)
		{
			labels[i]=predict(t,testdata[i]);
			std::cout<<labels[i]<<"&&"<<testdata[i][testdata[0].size()-1]<<std::endl;
		}
	}
}
