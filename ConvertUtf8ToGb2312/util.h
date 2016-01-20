#pragma once
#include <tchar.h>


//选择目录
const char*  Get_directory(const char* title);

const char* Get_filters();

void TcharToChar(const TCHAR * tchar, char * _char);
void pcharToWStr(const char *pchar, wchar_t *wchar);
