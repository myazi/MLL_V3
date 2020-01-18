#include <LoadData.h>
/**
加载数值型样本数据
*/
typedef std::vector<std::vector<float> > Data;  //定义为模板更好
typedef std::vector<float> RowData;
typedef std::vector<float> ColData;

typedef std::vector<std::vector<std::string> > DataStr;  //定义为模板
typedef std::vector<std::string> RowDataStr;
typedef std::vector<std::string> ColDataStr;

void LoadDataNum_spare(Data &data, const std::string &infile)
{
    std::cout<< "data filename is:"<< infile<<std::endl;
    std::ifstream dataFile;
    dataFile.open(infile);
    std::string tmpstrline;
	std::string onestr;
	int pos = 0;
	int index = 0;
    double val = 0;
    RowData rowData;
	int i = 0;
    if(dataFile)
    {
        while(!dataFile.eof())
        {
            getline(dataFile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
			std::stringstream input(tmpstrline);
            if(tmpstrline!="\0")
            {
				while(input>>onestr)
                {
					pos = 0;
					pos = onestr.find(":");	
					std::stringstream input_index(onestr.substr(0,pos));
					std::stringstream input_val(onestr.substr(pos+1,onestr.size() - pos - 1));
                    input_index >> index;
					input_val >> val;
					std::cout<<"load_spare"<<onestr<<'\t'<<index<<'\t'<<val<<std::endl;
					data[i][index] = val;
                }
            }
			i++;
        }
    }
    dataFile.close();
}
void LoadDataTest(Data &data, const std::string &infile)
{
    std::cout<< "data filename is:"<< infile<<std::endl;
    std::ifstream dataFile(infile);
    std::string tmpstrline;
    float val;
    RowData rowData;
    if(dataFile)
    {
        //while(dataFile>>)
        while(!dataFile.eof())
        {
            getline(dataFile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
            std::stringstream input(tmpstrline);
            if(tmpstrline!="\0")
            {
                while(input>>val)
                {
                    rowData.push_back(val);
                }
                data.push_back(rowData);
            }
            rowData.clear();
        }
    }
    else
    {
        std::cerr<< "data file open fail"<<std::endl;
    }

    dataFile.close();
}
void LoadDataNum(Data &data, const std::string &infile)
{
    std::cout<< "data filename is:"<< infile<<std::endl;
    std::ifstream dataFile;
    dataFile.open(infile);
    std::string tmpstrline;
    float val;
    RowData rowData;
    if(dataFile)
    {
        while(!dataFile.eof())
        {
            getline(dataFile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
            //char buffer[256];
            //dataFile.getline(buffer,100);//读取文件中一行的数据，保存为string类型
            std::stringstream input(tmpstrline);
            if(tmpstrline!="\0")
            {
                while(input>>val)
                {
                    rowData.push_back(val);
                }
                data.push_back(rowData);
            }
            rowData.clear();
        }
    }
    dataFile.close();
}
void LoadDataStr(DataStr &data, const std::string &infile)
{
    std::cout<< "data filename is:"<< infile<<std::endl;
    std::ifstream dataFile;
    dataFile.open(infile);
    std::string tmpstrline;
    std::string val;
    RowDataStr rowData;
    if(dataFile)
    {
        while(!dataFile.eof())
        {
            getline(dataFile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
            std::stringstream input(tmpstrline);
            if(tmpstrline!="\0")
            {
                while(input>>val)
                {
                    rowData.push_back(val);
                }
                data.push_back(rowData);
            }
            rowData.clear();
        }
    }
    dataFile.close();
}
void LoadDataStr(RowDataStr &data, const std::string &infile)
{
    std::cout<< "data filename is:"<< infile<<std::endl;
    std::ifstream dataFile;
    dataFile.open(infile);
    std::string tmpstrline;
    std::string val;
    RowDataStr rowData;
    if(dataFile)
    {
        while(!dataFile.eof())
        {
            getline(dataFile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
            std::stringstream input(tmpstrline);
            if(tmpstrline!="\0")
            {
                while(input>>val)
                {
                    data.push_back(val);
                }
            }
        }
    }
    dataFile.close();
}
