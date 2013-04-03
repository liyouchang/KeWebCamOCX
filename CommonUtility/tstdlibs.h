#ifndef _TSTDLIBS_H
#define _TSTDLIBS_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#ifdef _WIN32
	#include <tchar.h> // For _T() macro!
#else // Non Windows Platform!
	#ifdef _UNICODE
		typedef wchar_t TCHAR;
		#ifndef _T
			#define _T(s) L##s
		#endif
		#ifndef _TSTR
			#define _TSTR(s) L##s
	    #endif
	#else
	    typedef wchar_t TCHAR;
		#ifndef _T
			#define _T(s) s
		#endif
		#ifndef _TSTR
	        #define _TSTR(s) s
		#endif
	#endif
#endif

namespace tstd
{
#ifdef _UNICODE
	typedef std::wstring			tstring;
	typedef std::wostream			tostream;
	typedef std::wistream			tistream;
	typedef std::wiostream			tiostream;
	typedef std::wistringstream		tistringstream;
	typedef std::wostringstream		tostringstream;
	typedef std::wstringstream		tstringstream;
	typedef std::wifstream			tifstream;
	typedef std::wofstream			tofstream;
	typedef std::wfstream			tfstream;
	typedef std::wfilebuf			tfilebuf;
	typedef std::wios			    tios;
	typedef std::wstreambuf			tstreambuf;
	typedef std::wstreampos			tstreampos;
	typedef std::wstringbuf			tstringbuf;

// declare an unnamed namespace as a superior alternative to the use of global static variable declarations.
namespace
{
    tostream& tcout = std::wcout;
	tostream& tcerr = std::wcerr;
	tostream& tclog = std::wclog;
	tistream& tcin	= std::wcin;

    std::wostream& tendl( std::wostream& output )
    {
        output << std::endl;
        return output;
    }

	tstring wstr_to_tstr(const std::wstring& arg)
	{
		return arg;
	}

	tstring str_to_tstr(const std::string& arg)
	{
		tstring res(arg.length(), L'\0');
		mbstowcs(const_cast<wchar_t*>(res.data()), arg.c_str(), arg.length());
		return res;
	}

	std::wstring tstr_to_wstr(const tstring& arg)
	{
		return arg;
	}

	std::string tstr_to_str(const tstring& arg)
	{
		std::string res(arg.length(), '\0');
		wcstombs(const_cast<char*>(res.data()), arg.c_str(), arg.length());
		return res;
	}
}

#else

	typedef std::string			    tstring;
	typedef std::ostream			tostream;
	typedef std::istream			tistream;
	typedef std::iostream			tiostream;
	typedef std::istringstream		tistringstream;
	typedef std::ostringstream		tostringstream;
	typedef std::stringstream		tstringstream;
	typedef std::ifstream			tifstream;
	typedef std::ofstream			tofstream;
	typedef std::fstream			tfstream;
	typedef std::filebuf			tfilebuf;
	typedef std::ios				tios;
	typedef std::streambuf			tstreambuf;
	typedef std::streampos			tstreampos;
	typedef std::stringbuf			tstringbuf;

// declare an unnamed namespace as a superior alternative to the use of global static variable declarations.
namespace
{
    tostream& tcout = std::cout;
	tostream& tcerr = std::cerr;
	tostream& tclog = std::clog;
	tistream& tcin	= std::cin;

    std::ostream& tendl( std::ostream& output )
    {
        output << std::endl;
        return output;
    }

	tstring wstr_to_tstr(const std::wstring& arg)
	{
		tstring res( arg.length(), '\0' );
		wcstombs( const_cast<char*>(res.data()) , arg.c_str(), arg.length());
           return res;
	}

	tstring str_to_tstr(const std::string& arg)
	{
		return arg;
	}

	std::wstring tstr_to_wstr(const tstring& arg)
	{
		std::wstring res(arg.length(), L'\0');
        mbstowcs(const_cast<wchar_t*>(res.data()), arg.c_str(), arg.length());
        return res;
	}

    std::string tstr_to_str(const tstring& arg)
    {
        return arg;
    }
}

#endif
}

#endif

