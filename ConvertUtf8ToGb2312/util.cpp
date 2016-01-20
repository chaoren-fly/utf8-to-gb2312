#include "util.h"
#include <string>
#include <Shlobj.h>

const char*  Get_directory(const char* title)
{

	wchar_t wtitle[MAX_PATH];
	const char* dir_title = (title == NULL)||!strcmp(title,"") ? "请选择目录" : title;
	pcharToWStr(dir_title, wtitle);

	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = wtitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
		return NULL;
	SHGetPathFromIDList(idl, szBuffer);

	static char buf[MAX_PATH];
	TcharToChar(szBuffer, buf);
	return buf;
}

const char* Get_filters()
{
	return  "h,cpp,cxx";
}

void TcharToChar(const TCHAR * tchar, char * _char)
{
	int iLength;
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
void pcharToWStr(const char *pchar, wchar_t *wchar)
{
	MultiByteToWideChar(CP_ACP, 0, pchar, strlen(pchar) + 1, wchar, 100);
}
