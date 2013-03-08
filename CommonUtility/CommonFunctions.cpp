
#include "stdafx.h"
#include "CommonFunctions.h"
#include <iostream>


#include <algorithm>
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

string GetCurrentPathA()
{
	string retPath= "./";
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

string ConvertString(string str)
{
	string retStr;
	string tmp;
/*	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str[i] != '\\')
			retStr.append(str[i]);
		else
		{
			tmp = str[i];
			if (++i == str.length())
			{
				retStr.append(tmp);
				return retStr;
			}

			tmp.append(str[i]);
			switch (str[i])
			{
			case '\\':
				retStr.append('\\');
				break;
			case 'x':
			case 'X':
			{
				int value = 0;
				if (++i == str.length())
				{
					retStr.append(tmp);
					return retStr;
				}
				tmp.append(str[i]);
				if ((str[i] >= '0' && str[i] <= '9')
						|| (str[i] >= 'A' && str[i] <= 'F')
						|| (str[i] >= 'a' && str[i] <= 'f'))
				{
					value = ascii_to_hex(str[i]) << 4;
				}
				else
				{
					retStr.append(tmp);
					break;
				}
				if (++i == str.length())
				{
					retStr.append(tmp);
					return retStr;
				}
				tmp.append(str[i]);
				if ((str[i] >= '0' && str[i] <= '9')
						|| (str[i] >= 'A' && str[i] <= 'F')
						|| (str[i] >= 'a' && str[i] <= 'f'))
				{
					value += ascii_to_hex(str[i]);
				}
				else
				{
					retStr.append(tmp);
					break;
				}
				retStr.append(value);
				break;
			}
			default:
				retStr.append(tmp);
				break;
			}

		}
	}*/
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

int splitString( string text,vector<string> &strList,string sepChar )
{
	if (text.empty())
		return 0;
	int posStart = 0;
	string tmp;
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
