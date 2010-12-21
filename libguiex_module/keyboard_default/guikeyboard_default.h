/** 
* @file guikeyboard_default.h
* @brief default keyboard interface, the user has to 
* set keyboard value himself
* @author ken
* @date 2006-07-18
*/

#ifndef __GUI_KEYBOARD_DEFAULT_20060718_H__
#define __GUI_KEYBOARD_DEFAULT_20060718_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacekeyboard.h>



namespace guiex
{

	//============================================================================//
	// class
	//============================================================================// 
	class GUIEXPORT IGUIKeyboard_default : public IGUIInterfaceKeyboard
	{
	public:
		IGUIKeyboard_default();
		virtual ~IGUIKeyboard_default();

		virtual void DeleteSelf();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();


	public: 
		static const char* StaticGetModuleName();
	};


	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_KEYBOARD_DEFAULT_20060718_H__

