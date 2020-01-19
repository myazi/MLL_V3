#ifndef LoadData_H
#define LoadData_H
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <math.h>
#include <stdlib.h>

/**
������ֵ����������
*/

typedef std::vector<std::vector<float> > Data;  //����Ϊģ�����
typedef std::vector<float> RowData;
typedef std::vector<float> ColData;

typedef std::vector<std::vector<std::string> > DataStr;  //����Ϊģ��
typedef std::vector<std::string> RowDataStr;
typedef std::vector<std::string> ColDataStr;

void LoadDataNum_spare(Data &data, const std::string &infile);
void LoadDataNum(Data &data, const std::string &infile);
void LoadDataStr(DataStr &data, const std::string &infile);
void LoadDataStr(RowDataStr &data, const std::string &infile);

#endif // LoadData_H
