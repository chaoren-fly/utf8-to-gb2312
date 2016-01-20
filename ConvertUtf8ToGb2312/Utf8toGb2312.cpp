#include <io.h>
#include <iostream>
#include "Utf8toGb2312.h"
#include "Unicode_Gb2312_Table.h"
#include "File_manage.h"
#include "Configfile.h"
using std::string;

Utf8toGb2312* Utf8toGb2312::instance = 0;
Utf8toGb2312* Utf8toGb2312::GetInstance()
{
	if (instance == NULL)
		instance = new Utf8toGb2312();
	return instance;
}

void Utf8toGb2312::Conv_Utf8_files(const char* directory_old, Configfile& config, const char* directory_new /*= NULL*/, bool cascade /*= false*/)
{
	if (!directory_old)
		return;

	//文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string filespec = string(directory_old).append("/*");
	if ((hFile = _findfirst(filespec.c_str(), &fileinfo)) == -1)
		return;
	do
	{
		bool convert_result = false;
		if (fileinfo.attrib& _A_SUBDIR)
		{//目录
			if (strcmp(fileinfo.name, ".") == 0 ||			//当前目录
				strcmp(fileinfo.name, "..") == 0 ||			//上级目录
				!cascade)
				continue;
			string cas_old_directory = string(directory_old).append("/").append(fileinfo.name);
			const char* cas_new_directory = NULL;
			if (directory_new != NULL){
				string cas_new_directory_s = string(directory_new).append("/").append(fileinfo.name);
				cas_new_directory = cas_new_directory_s.c_str();
			}
			Conv_Utf8_files(cas_old_directory.c_str(), config, cas_new_directory, cascade);
		}
		else
		{
			if (config.check_file_support(fileinfo.name)){
				convert_result = Conv_Utf8_file(directory_old, directory_new, fileinfo.name);
				config.increace_convertfile(convert_result);

				if (convert_result)
					std::cout << "successfully";
				else
					std::cout<<"unsuccessfully";
				std::cout << "converted file:\t" << directory_old << "/" << fileinfo.name << std::endl;

			}
		}
	} while (_findnext(hFile, &fileinfo) == 0);

	_findclose(hFile);
}

bool Utf8toGb2312::Conv_Utf8_file(const char* directory_old, const char* directory_new, const char* filename)
{
	bool result = true;
	string path_old = string(directory_old).append("/").append(filename);

	string tmpfile_name = string(filename).append(".tmp");
	string path_tmp = directory_new == NULL ? string(path_old).append(".tmp") : string(directory_new).append("/").append(tmpfile_name);
	FILE *tmpfile = File_manage::file_open(path_tmp.c_str(), "w");
	FILE *file = File_manage::file_open(path_old.c_str(), "r");
	if ((tmpfile == NULL) || (file == NULL))
		return false;

	char buf[BUFFERSIZE], buf2[BUFFERSIZE * 6];
	while (fgets(buf, BUFFERSIZE, file))
	{
		memset(buf2, 0, sizeof(buf2));
		if (!UTF_8ToGB2312(buf2, buf, strlen(buf)))
		{//转换过程中出现问题
			sprintf_s(buf2, sizeof(buf2), "%s", buf);
			result = false;
		}
		fputs(buf2, tmpfile);
	}
	File_manage::file_close(tmpfile);
	File_manage::file_close(file);

	if ((directory_new == NULL) || (strcmp(directory_old, directory_new) == 0))
	{//此处需要替换原文件
		string filename_pre = string(filename).append(".pre");
		string path_pre = string(directory_old).append("/").append(filename_pre);
		File_manage::file_rename(directory_old, filename, filename_pre.c_str());
		File_manage::file_rename(directory_old, tmpfile_name.c_str(), filename);
		File_manage::file_delete(path_pre.c_str());
	}
	else{
		File_manage::file_rename(directory_new, tmpfile_name.c_str(), filename);
	}
	return result;
}

bool Utf8toGb2312::UTF_8ToGB2312(char*pOut, char *pInput, int pLen)
{
	int res, i = 0, j = 0;
	char tempbuf[10];
	memset(tempbuf, 0, sizeof(tempbuf));
	while (i < pLen)
	{
		res = UTF_8ToUnicode(tempbuf, pInput + i);
		if (res <= 0)
			return false;
		else if (res == 1)
		{
			*(pOut + j) = *(pInput + i);
			j += 1;
		}
		else{
			UnicodeToGB2312(pOut + j, tempbuf);
			j += 2;
		}
		i += res;
	}

	return true;
}

int Utf8toGb2312::UTF_8ToUnicode(char* pOutput, char *pInput)
{
	char b1, b2, b3, b4, b5, b6;
	int len = enc_get_utf8_size(*pInput);
	switch (len)
	{
	case 0:
		*pOutput = *pInput;
		len += 1;
		break;
	case 2:
		b1 = *pInput;
		b2 = *(pInput + 1);
		if ((b2 & 0xC0) != 0x80)
			return 0;
		*pOutput = (b1 << 6) + (b2 & 0x3F);
		*(pOutput + 1) = (b1 >> 2) & 0x07;
		break;
	case 3:
		b1 = *pInput;
		b2 = *(pInput + 1);
		b3 = *(pInput + 2);
		if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80))
			return 0;
		*pOutput = ((b2 & 0x03) << 6) + (b3 & 0x3F);
		*(pOutput + 1) = ((b1 & 0x0F) << 4) | ((b2 >> 2) & 0x0F);
		break;
	case 4:
		b1 = *pInput;
		b2 = *(pInput + 1);
		b3 = *(pInput + 2);
		b4 = *(pInput + 3);
		if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
			|| ((b4 & 0xC0) != 0x80))
			return 0;
		*pOutput = (b3 << 6) + (b4 & 0x3F);
		*(pOutput + 1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
		*(pOutput + 2) = ((b1 << 2) & 0x1C) + ((b2 >> 4) & 0x03);
		break;
	case 5:
		b1 = *pInput;
		b2 = *(pInput + 1);
		b3 = *(pInput + 2);
		b4 = *(pInput + 3);
		b5 = *(pInput + 4);
		if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
			|| ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80))
			return 0;
		*pOutput = (b4 << 6) + (b5 & 0x3F);
		*(pOutput + 1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
		*(pOutput + 2) = (b2 << 2) + ((b3 >> 4) & 0x03);
		*(pOutput + 3) = (b1 << 6);
		break;
	case 6:
		b1 = *pInput;
		b2 = *(pInput + 1);
		b3 = *(pInput + 2);
		b4 = *(pInput + 3);
		b5 = *(pInput + 4);
		b6 = *(pInput + 5);
		if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
			|| ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
			|| ((b6 & 0xC0) != 0x80))
			return 0;
		*pOutput = (b5 << 6) + (b6 & 0x3F);
		*(pOutput + 1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
		*(pOutput + 2) = (b3 << 2) + ((b4 >> 4) & 0x03);
		*(pOutput + 3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
		break;
	default:
		return 0;
	}
	return len;
}

void Utf8toGb2312::UnicodeToGB2312(char*pOut, char *pInput)
{
	unsigned short tmp, gb2312_tmp;
	memcpy(&tmp, pInput, 2);
	gb2312_tmp = SearchCodeTable(tmp);

	gb2312_tmp = (gb2312_tmp >> 8 | gb2312_tmp << 8);//调整为小端，大地址存高位，小地址存低位

	memcpy(pOut, &gb2312_tmp, 2);
}

unsigned short Utf8toGb2312::SearchCodeTable(unsigned short unicode)
{
	int mid, start = 0, end = TABLE_LEN - 1;
	while (start <= end)
	{
		mid = (start + end) / 2;
		if (unicode_gb_table[mid].unicode == unicode)
			return unicode_gb_table[mid].gb2312;
		else if (unicode_gb_table[mid].unicode < unicode)
			start = mid + 1;
		else
			end = mid - 1;
	}
	return 0;
}

int Utf8toGb2312::enc_get_utf8_size(const unsigned char pInput)
{
	int num = 0, temp = 0x80;
	while (temp&pInput)
	{
		num++;
		temp = (temp >> 1);
	}
	return num;
}





