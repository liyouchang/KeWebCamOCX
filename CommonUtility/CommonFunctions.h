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
using namespace std;

int strtohex(const char *str, char *hex_buf);
char ascii_to_hex(char data);
string ConvertString(string str);

//string stringAddOne(string num);
//string stringMinusOne(string num);
int splitString(string text,vector<string> &strList,string sepChar);

string GetCurrentPathA();

void SleepMillisecond(int msec);



#endif

