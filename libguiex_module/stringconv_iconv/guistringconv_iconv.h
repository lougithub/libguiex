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
		* @brief convert utf8 to wchar
		* @return zero for success
		*/
		virtual int Utf8ToWChar( const CGUIString& rSrc, CGUIStringW& rDst );

		/**
		* @brief convert wchar to utf8
		* @return zero for success
		*/
		virtual int WCharToUtf8( const CGUIStringW& rSrc, CGUIString& rDst );
		
		
		virtual void DeleteSelf();
		
	protected:
		virtual int	DoInitialize(void* pUserData);
		virtual void DoDestroy();		
		
	public:
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();
}

#endif //__GUI_STRINVCONV_ICONV_H_20101117__

