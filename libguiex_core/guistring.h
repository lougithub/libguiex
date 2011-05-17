/** 
 * @file guistring.h
 * @brief string used in this system.
 * @author ken
 * @date 2006-04-07
 */

#ifndef __GUI_STRING_20060407_H__
#define __GUI_STRING_20060407_H__

//============================================================================//
// 	include
//============================================================================// 
#include "guibase.h"

#include <string>


//============================================================================//
// 	declare
//============================================================================// 
namespace guiex
{
	typedef std::string CGUIString;
#if GUI_USE_STANDARD_WCHAR_T
	typedef std::wstring CGUIStringW;
#else
	typedef std::basic_string<wchar, std::char_traits<wchar>, std::allocator<wchar> > CGUIStringW;
#endif
}
 
#endif //__GUI_STRING_20060407_H__
 


  
