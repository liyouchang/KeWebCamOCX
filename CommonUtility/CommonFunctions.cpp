
#include "stdafx.h"
#include "CommonFunctions.h"
#include <stdio.h>


/**********************************************************************************************************************
 *Function atoh
 * input:str:	[in] two bytes char
 * output: the hex data. -1 for error
 **********************************************************************************************************************/
int atoh(char *str)
{
	int data = 0;
	int len = strlen(str);

	if (str == NULL || len <= 0)
		return 0;

	for (char *a_str = str; *a_str != 0; a_str++)
	{
		char ch = *a_str;
		if (ch >= '0' && ch <= '9')
			data = (data << 4) | (ch - '0');
		else if (ch >= 'a' && ch <= 'f')
			data = (data << 4) | (ch - 'a' + 10);
		else if (ch >= 'A' && ch <= 'F')
			data = (data << 4) | (ch - 'A' + 10);
		else
			return -1;
	}
	return data;
}

/**********************************************************************************************************************
 * Function:	strtohex
 * parameter:
 * 	str:	[in]
 * 	hex_buf:[out]
 * 	hex_num:[out]
 * return:the size of hex data. -1 for error
 **********************************************************************************************************************/
int strtohex(const char *str, char *hex_buf)
{
	if (str == NULL || hex_buf == NULL)
		return -1;

	int len = strlen(str);
	if (len > 256)
	{
		return -1;
	}
	int num = 0;
	int index_buf1 = 0;
	int index_buf2;
	char temp_buf1[256] =
	{ 0 };
	char temp_buf2[256] =
	{ 0 };

	for (int index_str = 0; index_str < len; index_str++)
	{
		char ch = str[index_str];
		if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f')
				|| (ch >= 'A' && ch <= 'F'))
		{
			temp_buf1[index_buf1++] = ch;
			if (index_str < len - 1)
				continue;
		}
		if (index_buf1 <= 0)
			continue;

		int index = 0;
		index_buf2 = 0;
		while (index < index_buf1)
		{
			temp_buf2[index_buf2++] = temp_buf1[index++];
			if (index_buf2 == 2)
			{
				temp_buf2[index_buf2] = 0;
				hex_buf[num++] = (int) atoh(temp_buf2);
				index_buf2 = 0;
			}
		}
		if (index_buf2 != 0)
		{
			temp_buf2[index_buf2] = 0;
			hex_buf[num++] = (int) atoh(temp_buf2);
		}
		index_buf1 = 0;
	}
	return num;
}

char ascii_to_hex(char data)
{
	if (data >= '0' && data <= '9')
		return (data - '0');
	if (data >= 'A' && data <= 'F')
		return (data - 'A' + 10);
	if (data >= 'a' && data <= 'f')
		return (data - 'a' + 10);
	return 0xff;
}

std::string GetCurrentPathA()
{
	std::string retPath= "./";
#ifdef WIN32
	char filePath[MAX_PATH];
	HMODULE hModule= NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,(PCTSTR)GetCurrentPathA,&hModule);
	GetModuleFileNameA(hModule, filePath, MAX_PATH);
//	PathRemoveFileSpec(filePath);
	retPath = filePath;
	int len = retPath.rfind("\\");
	retPath = retPath.substr(0,len+1);
#else
	retPath = "./";
#endif

	return retPath;
}

std::string ConvertString(std::string str)
{
	std::string retStr;
	std::string tmp;
	return retStr;
}

void SleepMillisecond(int msec)
{
#ifdef WIN32
	Sleep(msec);
#else
	struct timeval sTime;
	sTime.tv_sec    = 0;
	sTime.tv_usec   = msec*1000;
	select(0, NULL, NULL, NULL, &sTime);
#endif
}

int splitString( std::string text,std::vector<std::string> &strList,std::string sepChar )
{
	if (text.empty())
		return 0;
	int posStart = 0;
	std::string tmp;
	int posFind = text.find(sepChar);
	while (posFind != -1)
	{
		tmp = text.substr(posStart, posFind - posStart);
		strList.push_back(tmp);
		posStart = posFind + sepChar.length();
		posFind = text.find(sepChar, posStart);
	}
	tmp = text.substr(posStart, text.length());
	strList.push_back(tmp);
	return strList.size();
}

// std::string wstr_to_str( const std::wstring& arg )
// {
// 	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,arg.c_str(),-1,NULL,0,NULL,FALSE);
// 	std::string res( dwNum, '\0' );
// 	WideCharToMultiByte (CP_OEMCP,NULL,arg.c_str(),-1,const_cast<char*>(res.data()),dwNum,NULL,FALSE);
// 	//wcstombs( const_cast<char*>(res.data()) , arg.c_str(), arg.length());
// 	return res;
// }
// 
// std::wstring str_to_wstr( const std::string& arg )
// {
// 	DWORD  dwMinSize = MultiByteToWideChar (CP_ACP, 0, arg.c_str(), -1, NULL, 0);
// 	std::wstring res(dwMinSize, L'\0');
// 	MultiByteToWideChar (CP_ACP, 0, arg.c_str(), -1, const_cast<wchar_t*>(res.data()), dwMinSize);  
// 	return res;
// }

std::wstring GetCurrentPathW()
{
	std::wstring retPath= L"./";
#ifdef WIN32
	wchar_t filePath[MAX_PATH];
	HMODULE hModule= NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,(PCTSTR)GetCurrentPathW,&hModule);
	GetModuleFileNameW(hModule, filePath, MAX_PATH);
	//	PathRemoveFileSpec(filePath);
	retPath = filePath;
	int len = retPath.rfind(L"\\");
	retPath = retPath.substr(0,len+1);
#else
	retPath = "./";
#endif

	return retPath;
}

BOOL FolderExist( CString strPath )
{
	WIN32_FIND_DATA   wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;   
	}
	FindClose(hFind);
	return rValue;
}

BOOL CreateFolder(CString strPath)
{
	SECURITY_ATTRIBUTES attrib;
	attrib.bInheritHandle = FALSE;
	attrib.lpSecurityDescriptor = NULL;
	attrib.nLength =sizeof(SECURITY_ATTRIBUTES);
	//上面定义的属性可以省略。 直接return ::CreateDirectory( path, NULL); 即可
	return ::CreateDirectory( strPath, &attrib);
} 

BOOL CreateFolderEx( const CString& szPath )
{
	CString strDir(szPath);//存放要创建的目录字符串
	//确保以'\'结尾以创建最后一个目录
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//存放每一层目录字符串
	CString strTemp;//一个临时变量,存放目录字符串
	BOOL bSuccess = FALSE;//成功标志
	//遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//如果当前字符不是'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(_T('\\'));
		}
	}
	//遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ;
	}
	return bSuccess;
}

std::string Convert(const char* strIn, int sourceCodepage, int targetCodepage)
{
	int len=strlen(strIn);
	int unicodeLen=MultiByteToWideChar(sourceCodepage,0,strIn,-1,NULL,0);
	wchar_t* pUnicode;
	pUnicode=new wchar_t[unicodeLen+1];
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));
	MultiByteToWideChar(sourceCodepage,0,strIn,-1,(LPWSTR)pUnicode,unicodeLen);
	char * pTargetData = NULL;
	int targetLen=WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(char *)pTargetData,0,NULL,NULL);
	pTargetData=new char[targetLen+1];
	memset(pTargetData,0,targetLen+1);
	WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,pTargetData,targetLen,NULL,NULL);
	
	std::string result = pTargetData;
	delete [] pUnicode;
	delete [] pTargetData;

	return result;
}


//UTF-8 转为 GB2312
std::string UTF8ToGB2312(const char *pText)
{
	return 	Convert(pText,CP_UTF8,CP_ACP);
}

//GB2312 转为 UTF-8
std::string GB2312ToUTF8(const char *pText)
{
	return Convert(pText,CP_ACP,CP_UTF8);
}

/*****************************************************************
** 函数名:GetPath
** 输 入: 无
** 输 出: CString strPath
**        strPath非空, 表示用户选择的文件夹路径
**        strPath为空, 表示用户点击了“取消”键，取消选择
** 功能描述:显示“选择文件夹”对话框，让用户选择文件夹
****************************************************************/
CString GetDirectory()
{
	CString strPath = _T("");
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bInfo.lpszTitle = _T("请选择路径: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;    

	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bInfo) ; //显示选择对话框
	if(lpDlist != NULL)  //用户按了确定按钮
	{
		TCHAR chPath[255]; //用来存储路径的字符串
		SHGetPathFromIDList(lpDlist, chPath);//把项目标识列表转化成字符串
		strPath = chPath; //将TCHAR类型的字符串转换为CString类型的字符串
	}

	if(strPath.IsEmpty())
		return _T("");

	if(strPath.Right(1)!=_T("\\"))
		strPath+=_T("\\");

	return strPath;
}


void trace( const char* format, ... )
{
	char buffer[1000];

	va_list argptr;
	va_start(argptr, format);
	vsprintf(buffer, format, argptr);
	va_end(argptr);

	OutputDebugStringA(buffer);
}
