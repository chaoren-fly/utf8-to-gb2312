#include "File_manage.h"
#include <windows.h>
#include <string>
#include <io.h>
using std::string;

File_manage::File_manage()
{
}


File_manage::~File_manage()
{
}

FILE* File_manage::file_open(const char* path, const char* mode)
{
	FILE *file = NULL; 
	fopen_s(&file, path, mode);
	return file;
}

void File_manage::file_delete(const char* path)
{
	int res = _access(path, 0);

	if (_access(path, 0) == 0)//����ļ�������  
	{
		SetFileAttributesA(path, 0);
		DeleteFileA(path);
	}
}

//directoryΪ�ձ�ʾ��ǰ·��
void File_manage::file_delete(const char* directory, const char* filename)
{
	if (filename == NULL)
		return;
	string path = directory == NULL ? string(filename) : string(directory).append("/").append(filename);
	file_delete(path.c_str());
}

void File_manage::file_rename(const char* directory, const char* oldname, const char* newname)
{
	if (newname == NULL || (strcmp(oldname, newname) == 0))
		return;

	string path_oldname = string(directory).append("/").append(oldname);
	string path_newname = string(directory).append("/").append(newname);

	if (!_access(path_oldname.c_str(), 0))
	{
		rename(path_oldname.c_str(), path_newname.c_str());
	}
}

void File_manage::file_close(FILE* file)
{
	fclose(file);
}
