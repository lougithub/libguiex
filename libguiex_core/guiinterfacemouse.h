/** 
* @file guiinterfacemouse.h
* @brief interface for mouse
* @author ken
* @date 2006-06-26
*/


#ifndef __GUI_INTERFACE_MOUSE_20060426_H_
#define __GUI_INTERFACE_MOUSE_20060426_H_

//============================================================================//
// include
//============================================================================// 
#include "guiinterface.h"
#include "guivector2.h"

#include <list>

//============================================================================//
// define
//============================================================================// 
namespace guiex
{
	enum EMouseState
	{
		MOUSE_UP = 0,
		MOUSE_DOWN,

		_MOUSE_STATE_MAX_,
	};
	enum EMouseButton
	{
		MOUSE_NONE = 0,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,

		_MOUSE_BUTTON_MAX_,
	};
	enum EMouseEvent
	{
		MOUSE_EVENT_DOWN = 0,
		MOUSE_EVENT_UP,
		MOUSE_EVENT_MOVE,
		MOUSE_EVENT_WHEEL,
	};
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class IGUIInterfaceMouse 
	* @brief interface of mouse
	*/
	class GUIEXPORT IGUIInterfaceMouse : public IGUIInterface
	{
	public:
		//mouse event
		struct SMouseEvent 
		{
			EMouseButton m_eButton; ///< mouse's button
			EMouseEvent m_eMouseEvent;		///< mouse's event

			CGUIVector2 m_aMousePos; ///!< mouse's position
			real m_fWheelChange;

			SMouseEvent(EMouseEvent eEvent=MOUSE_EVENT_DOWN,EMouseButton eButton=MOUSE_LEFT, real x = 0.0f, real y = 0.0f, real wheel=0.0f  )
				:m_eButton(eButton)
				,m_eMouseEvent(eEvent)
				,m_aMousePos(x,y)
				,m_fWheelChange(wheel)
			{
			}
		};

	public:
		IGUIInterfaceMouse( const char* szModuleName );
		virtual ~IGUIInterfaceMouse();
		void Reset();

		bool ChangeButtonState( EMouseButton eButton,  EMouseState eState );
		EMouseState GetButtonState(int eButton);
		
		virtual bool ChangeMousePos( const CGUIVector2& rPos );
		const CGUIVector2& GetPosition() const;

		bool ChangeWheel( real fWheelChange );
		real GetWheelChange( ) const;

		virtual void PreUpdate( );
		virtual void PostUpdate( );

	protected:
		bool ProcessMouseEvent( const SMouseEvent& rEvent );
		CGUIVector2 ConvertPosToEngine( const CGUIVector2& rPos );

	private:
		struct SMouseContext
		{
			CGUIVector2 m_aPosCur; ///< current position

			EMouseState	m_eMouseStateCur[_MOUSE_BUTTON_MAX_]; ///< current mouse state
			real m_fWheelChange;
		};
		SMouseContext m_aContext;

	public: 
		static const char* StaticGetModuleType();
	};

}//namespace guiex


#endif //__GUI_INTERFACE_MOUSE_20060426_H_

