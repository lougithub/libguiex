/** 
* @file guistringconv_winapi.cpp
* @brief used to convert string between different code
* @author ken
* @date 2010-11-17
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\stringconv_winapi\guistringconv_winapi.h>
#include <windows.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIStringConv_winapi);
	//------------------------------------------------------------------------------
	const char* IGUIStringConv_winapi::StaticGetModuleName()
	{
		return "IGUIStringConv_winapi";
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_winapi::IGUIStringConv_winapi()
		:IGUIInterfaceStringConv(StaticGetModuleName())
	{
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_winapi::~IGUIStringConv_winapi()
	{
	}
	//------------------------------------------------------------------------------
	int	IGUIStringConv_winapi::DoInitialize(void* pUserData)
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIStringConv_winapi::DoDestroy()
	{

	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_winapi::Utf8ToUtf16( const CGUIString& rSrc, CGUIStringW& rDst )
	{
		if( rSrc.empty())
		{
			return 0;
		}

		size_t	buf_size = rSrc.size()+1;
		wchar_t* dst = new wchar_t[buf_size];
		::MultiByteToWideChar(CP_UTF8,0,rSrc.c_str(),-1,dst,buf_size);
		rDst = dst;
		delete[] dst;

		return 0;
	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_winapi::Utf16ToUtf8( const CGUIStringW& rSrc, CGUIString& rDst )
	{
		if( rSrc.empty())
		{
			return 0;
		}

		size_t buf_size = (rSrc.size()*4)+1;
		char* dst = (char*)(new char[buf_size]);
		::WideCharToMultiByte(CP_UTF8,0,rSrc.c_str(),-1,dst,buf_size, NULL, NULL );
		rDst.append(dst);
		delete[] dst;

		return 0;
	}
	//------------------------------------------------------------------------------
	void	IGUIStringConv_winapi::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
}
