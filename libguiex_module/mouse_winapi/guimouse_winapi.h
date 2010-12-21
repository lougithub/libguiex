/** 
* @file guimouse_default.h
* @brief mouse interface, use winapi
* @author ken
* @date 2006-08-25
*/

#ifndef __GUI_MOUSE_WINAPI_20060825_H__
#define __GUI_MOUSE_WINAPI_20060825_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacemouse.h>
#include <windows.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIMouse_winapi : public IGUIInterfaceMouse
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIMouse_winapi();

		/**
		* @brief destructor
		*/
		virtual ~IGUIMouse_winapi();

		/**
		* @brief used to delete this object
		*/
		virtual void DeleteSelf();

		/**
		* @brief process windows message,
		* convert system mouse message to guiex mouse message
		* @return 
		*		- true the income message contain mouse information
		*		- false the income message doesn't contain mouse information
		*/
		bool ProcessWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	protected:
		/** 
		* @brief initialize render
		* @return 0 for success
		*/
		virtual int DoInitialize(void* );

		/** 
		* @brief destroy render
		* @return 0 for success
		*/
		virtual void DoDestroy();


	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_MOUSE_WINAPI_20060825_H__

