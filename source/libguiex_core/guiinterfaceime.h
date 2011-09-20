/** 
* @file guiime.h
* @brief ime, used by edit
* @author ken
* @date 2006-08-25
*/


#ifndef __GUI_IME_20060825_H__
#define __GUI_IME_20060825_H__
//============================================================================//
// include
//============================================================================// 
#include "guiinterface.h"
#include "guivector2.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class IGUIInterfaceIme 
	* @brief ime
	*/
	class GUIEXPORT IGUIInterfaceIme : public IGUIInterface
	{
	public:
		IGUIInterfaceIme( const char* szModuleName );
		virtual ~IGUIInterfaceIme();

	public:
		///open ime, that's means the ime could receive the input
		virtual void OpenIme() = 0;

		///close ime, that's means the ime won't receive any input
		virtual void CloseIme() = 0;

		///get Result string
		virtual const wchar* GetResultString() const= 0;

		///clear Result string
		virtual void ClearResultString() = 0;

		///set cursor's position, the position is window's position
		virtual void SetCursorPos( const CGUIVector2& rPos ) = 0;

		///get cursor's position
		virtual const CGUIVector2& GetCursorPos() const= 0;

	public: 
		static const char* StaticGetModuleType();
	};

}//namespace guiex

#endif //__GUI_IME_20060825_H__
