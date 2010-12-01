/** 
* @file guimouse_default.cpp
* @brief mouse interface, use winapi
* @author ken
* @date 2006-08-25
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\mouse_winapi\guimouse_winapi.h>



//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIMouse_winapi);
	//------------------------------------------------------------------------------
	IGUIMouse_winapi::IGUIMouse_winapi()
	{
	}
	//------------------------------------------------------------------------------
	IGUIMouse_winapi::~IGUIMouse_winapi()
	{
	}
	//------------------------------------------------------------------------------
	int IGUIMouse_winapi::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIMouse_winapi::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void	IGUIMouse_winapi::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	bool			IGUIMouse_winapi::ProcessWindowMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
			ChangeButtonState( MOUSE_LEFT, MOUSE_DOWN);
			return true;
		case WM_LBUTTONUP:
			ChangeButtonState( MOUSE_LEFT, MOUSE_UP);
			return true;
		case WM_MBUTTONDOWN:
			ChangeButtonState( MOUSE_MIDDLE, MOUSE_DOWN);
			return true;
		case WM_MBUTTONUP:
			ChangeButtonState( MOUSE_MIDDLE, MOUSE_UP);
			return true;
		case WM_RBUTTONDOWN:
			ChangeButtonState( MOUSE_RIGHT, MOUSE_DOWN);
			return true;
		case WM_RBUTTONUP:
			ChangeButtonState( MOUSE_RIGHT, MOUSE_UP);
			return true;
		case WM_MOUSEMOVE:
			ChangeMousePos( CGUIVector2(LOWORD(lParam),HIWORD(lParam)));
			return true;
		}

		return false;
	}
	//------------------------------------------------------------------------------


}//namespace guiex
