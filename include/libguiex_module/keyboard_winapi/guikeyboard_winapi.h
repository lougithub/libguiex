/** 
* @file guikeyboard_default.h
* @brief keyboard interface, use winapi
* @author ken
* @date 2006-08-25
*/

#ifndef __GUI_KEYBOARD_DEFAULT_20060718_H__
#define __GUI_KEYBOARD_DEFAULT_20060718_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiinterfacekeyboard.h>
#include <windows.h>
#include <map>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIKeyboard_winapi : public IGUIInterfaceKeyboard
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIKeyboard_winapi();

		/**
		* @brief destructor
		*/
		virtual ~IGUIKeyboard_winapi();

		/**
		* @brief used to delete this object
		*/
		virtual void	DeleteSelf();

		/**
		* @brief process windows message,
		* convert system keyboard message to guiex keyboard message
		* @return 
		*		- true the income message contain keyboard information
		*		- false the income message doesn't contain keyboard information
		*/
		bool			ProcessWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		/** 
		* @brief initialize keyboard
		* @return 0 for success
		*/
		virtual int DoInitialize(void* );

		/** 
		* @brief destroy keyboard
		* @return 0 for success
		*/
		virtual void DoDestroy();


		EKeyCode VKtoIK(int vk);

	protected:
		std::map<int/*vk*/, EKeyCode/*ik*/> m_mapVkIk;
	};



	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_KEYBOARD_DEFAULT_20060718_H__

