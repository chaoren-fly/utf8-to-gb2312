#pragma once

#include <cstdio>
class File_manage
{
public:
	File_manage();
	~File_manage();

	static FILE* file_open(const char* path, const char* mode);
	static void file_close(FILE* file);
	static void file_rename(const char* directory, const char* oldname, const char* newname);
	static void file_delete(const char* path);
	static void file_delete(const char* directory, const char* filename);

};

