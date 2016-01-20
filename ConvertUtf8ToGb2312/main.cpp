#include <iostream>
#include "Utf8toGb2312.h"
#include "Configfile.h"
#include "util.h"

int main()
{
	const char* filter = "h,cpp,cxx,c";
	//TODO: 添加一个文本输入框。

	Configfile config(filter);

	std::string title = "默认转换 ";
	title.append(filter, strlen(filter));
	title.append(" 文件类型,可在main函数修改,请选择转换文件目录");
	const char *directory = Get_directory(title.c_str());

	if (directory == NULL)
		return 0;

	Utf8toGb2312::GetInstance()->Conv_Utf8_files(directory, config, NULL, true);


	std::cout << "\n\n------------------results-----------------------------\n\n";
	std::cout <<"successfully converted  files' count:\t\t" <<config.get_ok_convertfile() <<"\n";
	std::cout <<"unsuccessfully converted  files' count:\t\t" << config.get_error_convertfile() << "\n";
	std::cout << "all done\n";
	system("pause");

	return 0;
}


