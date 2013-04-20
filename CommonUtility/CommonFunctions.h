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
using namespace std;

int strtohex(const char *str, char *hex_buf);
char ascii_to_hex(char data);
string ConvertString(string str);

//string stringAddOne(string num);
//string stringMinusOne(string num);
int splitString(string text,vector<string> &strList,string sepChar);

string GetCurrentPathA();
wstring GetCurrentPathW();
void SleepMillisecond(int msec);

// Convert wstring to string
std::string wstr_to_str(const std::wstring& arg);
// Convert string to wstring
std::wstring str_to_wstr(const std::string& arg);


BOOL   FolderExist(CString strPath);
BOOL CreateFolder(CString strPath);
//创建多层目录
BOOL CreateFolderEx(const CString& szPath);

#endif

