/** 
* @file guistringconv_cocoa.cpp
* @brief used to convert string between different code
* @author ken
* @date 2010-11-17
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/stringconv_cocoa/guistringconv_cocoa.h>
#import <Foundation/Foundation.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIStringConv_cocoa);
	//------------------------------------------------------------------------------
	const char* IGUIStringConv_cocoa::StaticGetModuleName()
	{
		return "IGUIStringConv_cocoa";
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_cocoa::IGUIStringConv_cocoa()
		:IGUIInterfaceStringConv(StaticGetModuleName())
	{
#if !GUI_USE_STANDARD_WCHAR_T
#error "IGUIStringConv_cocoa only support wchar_t mode"	
#endif	
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_cocoa::~IGUIStringConv_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	int	IGUIStringConv_cocoa::DoInitialize(void* pUserData)
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIStringConv_cocoa::DoDestroy()
	{

	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_cocoa::Utf8ToWChar( const CGUIString& rSrc, CGUIStringW& rDst )
	{
		if( rSrc.empty())
		{
			rDst.clear();
			return 0;
		}
		setlocale(LC_CTYPE, "UTF-8");
		
		int iLength = mbstowcs( NULL, rSrc.c_str(), 0 );    
		wchar_t *strTmp = new wchar_t[iLength +1];
		mbstowcs(strTmp, rSrc.c_str(), iLength);
		strTmp[iLength] = 0;
		rDst = strTmp;
		delete[] strTmp;
		return 0;
	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_cocoa::WCharToUtf8( const CGUIStringW& rSrc, CGUIString& rDst )
	{
		if( rSrc.empty())
		{
			rDst.clear();
			return 0;
		}

		setlocale(LC_CTYPE, "UTF-8");
		
		int iLength = wcstombs(NULL, rSrc.c_str(), 0);    
		char *strTmp = new char[iLength +1];
		wcstombs(strTmp, rSrc.c_str(), iLength);
		strTmp[iLength] = 0;
		rDst = strTmp;
		delete[] strTmp;
		
		return 0;
	}
	//------------------------------------------------------------------------------
	void	IGUIStringConv_cocoa::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
}
