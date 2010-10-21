/** 
* @file guiinterfacekeyboard.h
* @brief interface for keyboard 
* @author ken
* @date 2006-08-20
*/


#ifndef __GUI_INTERFACE_KEYBOARD_20060820_H_
#define __GUI_INTERFACE_KEYBOARD_20060820_H_

//============================================================================//
// include
//============================================================================// 
#include "guiinterface.h"

#include <list>
#include <vector>


//============================================================================//
// define
//============================================================================// 
namespace guiex
{
	/** 
	* @brief the state of key
	*/
	enum	EKeyState
	{
		KEY_DOWN = 0,
		KEY_UP,

		_KEY_STATE_MAX_
	};

	enum	EKeyEvent
	{
		KEY_EVENT_DOWN,
		KEY_EVENT_UP,
		KEY_EVENT_PRESSED,	//like KEY_DOWN, but it will be generate multi-time if you pressed the key for a several time
		KEY_EVENT_CLICKED,
	};

	/** 
	* @brief the code of key
	*/
	enum EKeyCode
	{
		KC_NONE = 0,

		KC_0,
		KC_1,
		KC_2,
		KC_3,
		KC_4,
		KC_5,
		KC_6,
		KC_7,
		KC_8,
		KC_9,

		KC_A,
		KC_B,
		KC_C,
		KC_D,
		KC_E,
		KC_F,
		KC_G,
		KC_H,
		KC_I,
		KC_J,
		KC_K,
		KC_L,
		KC_M,
		KC_N,
		KC_O,
		KC_P,
		KC_Q,
		KC_R,
		KC_S,
		KC_T,
		KC_U,
		KC_V,
		KC_W,
		KC_X,
		KC_Y,
		KC_Z,

		KC_SHIFT,
		KC_CTRL,
		KC_ALT,
		KC_CAPSLOCK,
		KC_BACK,
		KC_ESCAPE,
		KC_SPACE,
		KC_TAB,
		KC_ENTER,

		KC_PLUS,      // = or +
		KC_COMMA,     // , or <
		KC_MINUS,     // - or _
		KC_PERIOD,    // . or >

		KC_SEMICOLON,     // ; or : 
		KC_SLASH,     // / or ? 
		KC_TILDE,     // ` or ~
		KC_LEFTBRACKET,     // [ or {
		KC_BACKSLASH,     // \ or |
		KC_RIGHTBRACKET,     // ] or }
		KC_QUOTE,     // ' or "

		KC_PAGEUP,
		KC_PAGEDOWN,
		KC_END,
		KC_HOME,
		KC_LEFT,
		KC_RIGHT,
		KC_DOWN,
		KC_UP,
		KC_INSERT,
		KC_DELETE,

		KC_PAD_NUM0, 
		KC_PAD_NUM1, 
		KC_PAD_NUM2, 
		KC_PAD_NUM3, 
		KC_PAD_NUM4, 
		KC_PAD_NUM5, 
		KC_PAD_NUM6, 
		KC_PAD_NUM7, 
		KC_PAD_NUM8,
		KC_PAD_NUM9, 
		KC_PAD_MULTIPLY, 
		KC_PAD_ADD, 
		KC_PAD_SEPARATOR,
		KC_PAD_SUB, 
		KC_PAD_DECIMAL,  
		KC_PAD_DIVIDE,  

		KC_F1,
		KC_F2,
		KC_F3,
		KC_F4,
		KC_F5,
		KC_F6,
		KC_F7,
		KC_F8,
		KC_F9,
		KC_F10,
		KC_F11,
		KC_F12,

		_KEY_CODE_MAX_,
	};

}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class IGUIInterfaceKeyboard
	* @brief interface of keyboard
	*/
	class GUIEXPORT IGUIInterfaceKeyboard: public IGUIInterface
	{
	public:
		struct SKeyEvent 
		{
			EKeyCode            m_eKeyCode;				///< key's code
			EKeyEvent           m_eKeyEvent;			///< key's event

			SKeyEvent(EKeyCode eKeyCode=KC_NONE, EKeyEvent eKeyEvent=KEY_EVENT_DOWN)
				:m_eKeyCode(eKeyCode)
				,m_eKeyEvent(eKeyEvent)
			{
			}
		};

	public:
		/** 
		* @brief constructor
		*/
		IGUIInterfaceKeyboard();

		/** 
		* @brief destructor
		*/
		virtual					~IGUIInterfaceKeyboard();

		/**
		* @brief reset context
		*/
		void			Reset();

		///called when the state of key changed
		bool			ChangeKeyState(EKeyCode eKey, EKeyState eState);

		///has key been pressed until now
		bool			IsKeyPressed(EKeyCode eKey) const;       

		///has key been pressed until now
		bool			HasAnyKeyPressed() const;                      

		///get all key which is pressed
		void			GetAllKeyPressed(std::vector<EKeyCode>& rKeys) const;          

		///update key state before process keyboard
		virtual void	PreUpdate();

		///update key state after process keyboard
		virtual void	PostUpdate();

	protected:
		/**
		* @brief process keyboard event
		* @return whether this event is consumed;
		*/
		bool			ProcessKeyEvent( const SKeyEvent& rEvent );

	protected:
		//key state
		struct SKeyContext
		{
			bool m_bKeyDown[_KEY_CODE_MAX_];
			int  m_nKeyPressedCounter;
		};
		SKeyContext			m_aContext;				///< key's context
	};

}//namespace guiex

#endif //__GUI_INTERFACE_KEYBOARD_20060820_H_

