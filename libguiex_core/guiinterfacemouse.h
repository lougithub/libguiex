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

namespace guiex
{
//============================================================================//
// define
//============================================================================// 
	enum	EMouseState
	{
		MOUSE_UP = 0,
		MOUSE_DOWN,

		_MOUSE_STATE_MAX_,
	};
	enum	EMouseButton
	{
		MOUSE_NONE		= 0,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,

		_MOUSE_BUTTON_MAX_,
	};
	enum	EMouseEvent
	{
		MOUSE_EVENT_DOWN		= 0,
		MOUSE_EVENT_UP,
		MOUSE_EVENT_MOVE,
		MOUSE_EVENT_WHEEL,
	};
}

namespace guiex
{
//============================================================================//
// class
//============================================================================// 
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
		EMouseButton        m_eButton;			///< mouse's button
		EMouseEvent         m_eMouseEvent;		///< mouse's event

		CGUIVector2			m_aMousePos;		///!< mouse's position
		real				m_fWheelChange;

		SMouseEvent(EMouseEvent eEvent=MOUSE_EVENT_DOWN,EMouseButton eButton=MOUSE_LEFT, real x = 0.0f, real y = 0.0f, real wheel=0.0f  )
			:m_eButton(eButton)
			,m_eMouseEvent(eEvent)
			,m_aMousePos(x,y)
			,m_fWheelChange(wheel)
		{

		}
	};

public:
	/** 
	 * @brief constructor
	 */
	IGUIInterfaceMouse();

	/** 
	 * @brief destructor
	 */
	virtual					~IGUIInterfaceMouse();

	/**
	 * @brief reset contex
	 */
	void					Reset();

	/**
	 * @brief set state of button
	 */
	bool					ChangeButtonState( EMouseButton eButton,  EMouseState eState );

	/**
	 * @brief set mouse position
	 */
	virtual bool			ChangeMousePos( const CGUIVector2& rPos );

	/**
	 * @brief set mouse position
	 */
	bool					ChangeWheel( real fWheelChange );

	/**
	 * @brief get current state of button
	 */
	EMouseState				GetButtonState(int eButton);

	/**
	 * @brief get current position of mouse, global position
	 */
	const CGUIVector2&		GetPosition() const;

	/**
	 * @brief get mouse position
	 */
	real					GetWheelChange( ) const;

	/**
	 * @brief update mouse state
	 */
	virtual void			PreUpdate( );

	/**
	* @brief update mouse state
	*/
	virtual void			PostUpdate( );

protected:
	/**
	* @brief process mouse event
	* @return whether this event is consumed;
	*/
	bool					ProcessMouseEvent( const SMouseEvent& rEvent );

private:
	struct SMouseContext
	{
		CGUIVector2		m_aPosCur;									///< current position

		EMouseState		m_eMouseStateCur[_MOUSE_BUTTON_MAX_];			///< current mouse state
		real			m_fWheelChange;
	};
	SMouseContext		m_aContext;
};



}//namespace guiex


#endif __GUI_INTERFACE_MOUSE_20060426_H_

