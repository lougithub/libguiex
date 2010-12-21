/** 
* @file guikeyboard_default.cpp
* @brief keyboard interface, use winapi
* @author ken
* @date 2006-08-25
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\keyboard_winapi\guikeyboard_winapi.h>



//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIKeyboard_winapi);
	//------------------------------------------------------------------------------
	const char* IGUIKeyboard_winapi::StaticGetModuleName()
	{
		return "IGUIKeyboard_winapi";
	}
	//------------------------------------------------------------------------------
	IGUIKeyboard_winapi::IGUIKeyboard_winapi()
		:IGUIInterfaceKeyboard( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	IGUIKeyboard_winapi::~IGUIKeyboard_winapi()
	{
	}
	//------------------------------------------------------------------------------
	int IGUIKeyboard_winapi::DoInitialize(void* )
	{
		struct VK_IK{
			int vk;
			EKeyCode ik;
		}vk_ik[] = 
		{
			0x30,           KC_0,
			0x31,           KC_1,
			0x32,           KC_2,
			0x33,           KC_3,
			0x34,           KC_4,
			0x35,           KC_5,
			0x36,           KC_6,
			0x37,           KC_7,
			0x38,           KC_8,
			0x39,           KC_9,

			0x41,           KC_A,
			0x42,           KC_B,
			0x43,           KC_C,
			0x44,           KC_D,
			0x45,           KC_E,
			0x46,           KC_F,
			0x47,           KC_G,
			0x48,           KC_H,
			0x49,           KC_I,
			0x4A,           KC_J,
			0x4B,           KC_K,
			0x4C,           KC_L,
			0x4D,           KC_M,
			0x4E,           KC_N,
			0x4F,           KC_O,
			0x50,           KC_P,
			0x51,           KC_Q,
			0x52,           KC_R,
			0x53,           KC_S,
			0x54,           KC_T,
			0x55,           KC_U,
			0x56,           KC_V,
			0x57,           KC_W,
			0x58,           KC_X,
			0x59,           KC_Y,
			0x5A,           KC_Z,

			VK_SHIFT,       KC_SHIFT,
			VK_CONTROL,     KC_CTRL,
			VK_MENU,        KC_ALT,
			VK_CAPITAL,     KC_CAPSLOCK,
			VK_BACK,        KC_BACK,
			VK_SPACE,       KC_SPACE,
			VK_ESCAPE,      KC_ESCAPE,
			VK_TAB,         KC_TAB,
			VK_RETURN,      KC_ENTER,

			VK_OEM_PLUS,    KC_PLUS,
			VK_OEM_COMMA,   KC_COMMA,
			VK_OEM_MINUS,   KC_MINUS,
			VK_OEM_PERIOD,  KC_PERIOD,

			VK_OEM_1,       KC_SEMICOLON,     // ; or : ,
			VK_OEM_2,       KC_SLASH,     // / or ? ,
			VK_OEM_3,       KC_TILDE,     // ` or ~,
			VK_OEM_4,       KC_LEFTBRACKET,     // [ or {,
			VK_OEM_5,       KC_BACKSLASH,     // \ or |,
			VK_OEM_6,       KC_RIGHTBRACKET,     // ] or },
			VK_OEM_7,       KC_QUOTE,     // ' or ",

			VK_PRIOR,       KC_PAGEUP,
			VK_NEXT,        KC_PAGEDOWN,
			VK_END,         KC_END,
			VK_HOME,        KC_HOME,
			VK_LEFT,        KC_LEFT,
			VK_UP,          KC_UP,
			VK_RIGHT,       KC_RIGHT,
			VK_DOWN,        KC_DOWN,
			VK_INSERT,      KC_INSERT,
			VK_DELETE,      KC_DELETE,

			VK_NUMPAD0,     KC_PAD_NUM0, 
			VK_NUMPAD1,     KC_PAD_NUM1, 
			VK_NUMPAD2,     KC_PAD_NUM2, 
			VK_NUMPAD3,     KC_PAD_NUM3, 
			VK_NUMPAD4,     KC_PAD_NUM4, 
			VK_NUMPAD5,     KC_PAD_NUM5, 
			VK_NUMPAD6,     KC_PAD_NUM6, 
			VK_NUMPAD7,     KC_PAD_NUM7, 
			VK_NUMPAD8,     KC_PAD_NUM8,
			VK_NUMPAD9,     KC_PAD_NUM9, 
			VK_MULTIPLY,    KC_PAD_MULTIPLY, 
			VK_ADD,         KC_PAD_ADD, 
			VK_SEPARATOR,   KC_PAD_SEPARATOR, 
			VK_SUBTRACT,    KC_PAD_SUB, 
			VK_DECIMAL,     KC_PAD_DECIMAL,  
			VK_DIVIDE,      KC_PAD_DIVIDE,  

			VK_F1,          KC_F1,
			VK_F2,          KC_F2,
			VK_F3,          KC_F3,
			VK_F4,          KC_F4,
			VK_F5,          KC_F5,
			VK_F6,          KC_F6,
			VK_F7,          KC_F7,
			VK_F8,          KC_F8,
			VK_F9,          KC_F9,
			VK_F10,         KC_F10,
			VK_F11,         KC_F11,
			VK_F12,         KC_F12,
		};

		//add to map
		for(int i=0; i<_KEY_CODE_MAX_ - 1; ++i)
		{
			m_mapVkIk.insert( std::make_pair( vk_ik[i].vk, vk_ik[i].ik ));
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIKeyboard_winapi::DoDestroy()
	{
		m_mapVkIk.clear();
	}
	//------------------------------------------------------------------------------
	void	IGUIKeyboard_winapi::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	bool			IGUIKeyboard_winapi::ProcessWindowMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			ChangeKeyState(VKtoIK((int)wParam), KEY_DOWN );
			return true;

		case WM_SYSKEYUP:
		case WM_KEYUP:
			ChangeKeyState(VKtoIK((int)wParam), KEY_UP );
			return true;
		}

		//don't match any message
		return false;
	}
	//------------------------------------------------------------------------------
	EKeyCode IGUIKeyboard_winapi::VKtoIK(int vk)
	{
		std::map<int/*vk*/, EKeyCode/*ik*/>::iterator itor = m_mapVkIk.find(vk);
		if( itor == m_mapVkIk.end())
		{
			//not found
			return KC_NONE;
		}
		else
		{
			return itor->second;
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
