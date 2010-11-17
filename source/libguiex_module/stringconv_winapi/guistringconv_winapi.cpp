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
	GUI_INTERFACE_IMPLEMENT(IGUIStringConv_Winapi);
	//------------------------------------------------------------------------------
	IGUIStringConv_Winapi::IGUIStringConv_Winapi()
	{
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_Winapi::~IGUIStringConv_Winapi()
	{
	}
	//------------------------------------------------------------------------------
	int	IGUIStringConv_Winapi::DoInitialize(void* pUserData)
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIStringConv_Winapi::DoDestroy()
	{

	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_Winapi::Utf8ToUtf16( const CGUIString& rSrc, CGUIStringEx& rDst )
	{
		if( rSrc.empty())
		{
			return 0;
		}

		size_t	buf_size = rSrc.size()+1;
		wchar_t* dst = new wchar_t[buf_size];
		::MultiByteToWideChar(CP_UTF8,0,rSrc.c_str(),-1,dst,buf_size);
		rDst.Append(dst);
		delete[] dst;

		return 0;
	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_Winapi::Utf16ToUtf8( const CGUIStringEx& rSrc, CGUIString& rDst )
	{
		if( rSrc.Empty())
		{
			return 0;
		}

		size_t buf_size = (rSrc.Size()+1)*4;
		char* dst = (char*)(new char[buf_size]);
		::WideCharToMultiByte(CP_UTF8,0,rSrc.GetContent(),-1,dst,buf_size, NULL, NULL );
		rDst.append(dst);
		delete[] dst;

		return 0;
	}
	//------------------------------------------------------------------------------
	void	IGUIStringConv_Winapi::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
}
