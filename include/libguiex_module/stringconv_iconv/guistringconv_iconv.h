/** 
* @file guistringconv_iconv.h
* @brief used to convert string between different code
* @author ken
* @date 2010-11-17
*/


#ifndef __GUI_STRINVCONV_ICONV_H_20101117__
#define __GUI_STRINVCONV_ICONV_H_20101117__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacestringconv.h>
#include <iconv.h>
#include <errno.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{


	class GUIEXPORT IGUIStringConv_iconv : public IGUIInterfaceStringConv
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIStringConv_iconv();

		/**
		* @brief destructor
		*/
		virtual ~IGUIStringConv_iconv();

		/**
		* @brief convert utf8 to utf16
		* @return zero for success
		*/
		virtual int Utf8ToUtf16( const CGUIString& rSrc, CGUIStringEx& rDst ) = 0;

		/**
		* @brief convert utf16 to utf8
		* @return zero for success
		*/
		virtual int Utf16ToUtf8( const CGUIStringEx& rSrc, CGUIString& rDst ) = 0;
	};

	GUI_INTERFACE_DECLARE();
}

#endif //__GUI_STRINVCONV_ICONV_H_20101117__

