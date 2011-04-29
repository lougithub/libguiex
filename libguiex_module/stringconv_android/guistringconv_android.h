/** 
* @file guistringconv_android.h
* @brief used to convert string between different code
* @author ken
* @date 2011-04-29
*/

#ifndef __GUI_STRINVCONV_ANDROID_H_20110429__
#define __GUI_STRINVCONV_ANDROID_H_20110429__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacestringconv.h>



//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIStringConv_android : public IGUIInterfaceStringConv
	{
	public:
		IGUIStringConv_android();
		virtual ~IGUIStringConv_android();

		/**
		* @brief convert utf8 to utf16
		* @return zero for success
		*/
		virtual int Utf8ToUtf16( const CGUIString& rSrc, CGUIStringW& rDst );

		/**
		* @brief convert utf16 to utf8
		* @return zero for success
		*/
		virtual int Utf16ToUtf8( const CGUIStringW& rSrc, CGUIString& rDst );

		virtual void DeleteSelf();

	protected:
		virtual int	DoInitialize(void* pUserData);
		virtual void DoDestroy();

	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();
}

#endif //__GUI_STRINVCONV_ANDROID_H_20110429__

