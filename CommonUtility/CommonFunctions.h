#ifndef INCLUDED_COMMON_FUNCTIONS_h
#define INCLUDED_COMMON_FUNCTIONS_h

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "tstdlibs.h"
using namespace tstd;

int strtohex(const char *str, char *hex_buf);
char ascii_to_hex(char data);
std::string ConvertString(std::string str);

//string stringAddOne(string num);
//string stringMinusOne(string num);
int splitString(std::string text,std::vector<std::string> &strList,std::string sepChar);

std::string GetCurrentPathA();
std::wstring GetCurrentPathW();
void SleepMillisecond(int msec);

// Convert wstring to string
//std::string wstr_to_str(const std::wstring& arg);
// Convert string to wstring
//std::wstring str_to_wstr(const std::string& arg);


BOOL   FolderExist(CString strPath);
BOOL CreateFolder(CString strPath);
//创建多层目录
BOOL CreateFolderEx(const CString& szPath);

std::string UTF8ToGB2312(const char *pText);
std::string GB2312ToUTF8(const char *pText);

#endif

