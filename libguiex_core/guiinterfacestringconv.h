/** 
* @file guiinterfacestringconv.h
* @brief used to convert string between different code
* @author ken
* @date 2010-11-17
*/


#ifndef __GUI_INTERFACE_STRINGCONV_H_20101117__
#define __GUI_INTERFACE_STRINGCONV_H_20101117__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiinterface.h"
#include "guistring.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class IGUIInterfaceStringConv
	* @brief used to convert string between different code
	*/
	class GUIEXPORT IGUIInterfaceStringConv : public IGUIInterface
	{
	public:
		/** 
		* @brief constructor
		*/
		IGUIInterfaceStringConv( const char* szModuleName );

		/** 
		* @brief destructor
		*/
		virtual ~IGUIInterfaceStringConv();


	public:
		/**
		* @brief convert utf8 to utf16
		* @return zero for success
		*/
		virtual int Utf8ToUtf16( const CGUIString& rSrc, CGUIStringW& rDst ) = 0;

		/**
		* @brief convert utf16 to utf8
		* @return zero for success
		*/
		virtual int Utf16ToUtf8( const CGUIStringW& rSrc, CGUIString& rDst ) = 0;

	public: 
		static const char* StaticGetModuleType();
	};
}//namespace guiex

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	/**
	* @brief get command interface
	*/
	GUIEXPORT IGUIInterfaceStringConv*	GetInterface_StringConv( );

}//namespace guiex

#endif //__GUI_INTERFACE_STRINGCONV_H_20101117__

