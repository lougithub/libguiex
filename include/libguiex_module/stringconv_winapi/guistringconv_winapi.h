/** 
* @file guistringconv_winapi.h
* @brief used to convert string between different code
* @author ken
* @date 2010-11-17
*/


#ifndef __GUI_STRINVCONV_WINAPI_H_20101117__
#define __GUI_STRINVCONV_WINAPI_H_20101117__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacestringconv.h>



//============================================================================//
// class
//============================================================================// 
namespace guiex
{


	class GUIEXPORT IGUIStringConv_Winapi : public IGUIInterfaceStringConv
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIStringConv_Winapi();

		/**
		* @brief destructor
		*/
		virtual ~IGUIStringConv_Winapi();

		/**
		* @brief convert utf8 to utf16
		* @return zero for success
		*/
		virtual int Utf8ToUtf16( const CGUIString& rSrc, CGUIStringEx& rDst );

		/**
		* @brief convert utf16 to utf8
		* @return zero for success
		*/
		virtual int Utf16ToUtf8( const CGUIStringEx& rSrc, CGUIString& rDst );

		virtual void DeleteSelf();

	protected:
		/** 
		* @brief implement initialization
		* @return 0 for successful
		*/
		virtual int	DoInitialize(void* pUserData);

		/** 
		* @brief implement destruct
		*/
		virtual void DoDestroy();
	};

	GUI_INTERFACE_DECLARE();
}

#endif //__GUI_STRINVCONV_WINAPI_H_20101117__

