/** 
* @file guiinterfacemouse.cpp
* @brief interface for mouse
* @author ken
* @date 2006-06-26
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacemouse.h>
#include <libguiex_core/guisystem.h>

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceMouse::StaticGetModuleType( )
	{ 
		return "IGUIMouse";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceMouse::IGUIInterfaceMouse( const char* szModuleName )
		:IGUIInterface( StaticGetModuleType(), szModuleName )
	{
		Reset();
	}
	//------------------------------------------------------------------------------ 
	IGUIInterfaceMouse::~IGUIInterfaceMouse()
	{

	}
	//--------------------------------------------------------------------------------------
	void IGUIInterfaceMouse::Reset()
	{

		m_aContext.m_fWheelChange = 0.0f;
		for(int i=0; i<_MOUSE_BUTTON_MAX_; ++i )
		{
			m_aContext.m_eMouseStateCur[i]	= MOUSE_UP;
		}
	}
	//------------------------------------------------------------------------------ 
	/**
	* @brief process mouse event
	* @return whether this event is consumed;
	*/
	bool IGUIInterfaceMouse::ProcessMouseEvent( const SMouseEvent& rEvent )
	{
		return GSystem->ProcessMouseInput( rEvent );
	}
	//--------------------------------------------------------------------------------------
	/**
	* @brief set state of button
	*/
	bool IGUIInterfaceMouse::ChangeButtonState( EMouseButton eButton,  EMouseState eState )
	{
		GUI_ASSERT( eButton > MOUSE_NONE && eButton < _MOUSE_BUTTON_MAX_,"wrong parameter" );
		GUI_ASSERT( eState >= 0 && eState < _MOUSE_STATE_MAX_,"wrong parameter" );

		m_aContext.m_eMouseStateCur[eButton] = eState;

		bool bConsumed = false;

		switch(eState)
		{
		case MOUSE_UP:
			bConsumed = ProcessMouseEvent(SMouseEvent( MOUSE_EVENT_UP,eButton, m_aContext.m_aPosCur.x, m_aContext.m_aPosCur.y));
			break;
		case MOUSE_DOWN:
			bConsumed = ProcessMouseEvent(SMouseEvent( MOUSE_EVENT_DOWN,eButton, m_aContext.m_aPosCur.x, m_aContext.m_aPosCur.y));
			break;
		default:
			GUI_FORCE_ASSERT("unknown mouse button state");
			break;
		}

		return bConsumed;
	}
	//--------------------------------------------------------------------------------------
	/**
	* @brief set mouse position
	*/
	bool IGUIInterfaceMouse::ChangeMousePos( const CGUIVector2& rPos )
	{
		m_aContext.m_aPosCur = rPos;

		return ProcessMouseEvent(SMouseEvent( MOUSE_EVENT_MOVE,MOUSE_NONE, m_aContext.m_aPosCur.x, m_aContext.m_aPosCur.y));
	}
	//--------------------------------------------------------------------------------------
	/**
	* @brief set mouse position
	*/
	bool IGUIInterfaceMouse::ChangeWheel( real fWheelChange )
	{
		m_aContext.m_fWheelChange = fWheelChange;

		return ProcessMouseEvent(SMouseEvent( MOUSE_EVENT_WHEEL,MOUSE_MIDDLE, m_aContext.m_aPosCur.x, m_aContext.m_aPosCur.y, fWheelChange));
	}
	//--------------------------------------------------------------------------------------
	/**
	* @brief get current state of button
	*/
	EMouseState	IGUIInterfaceMouse::GetButtonState(int eButton)
	{
		GUI_ASSERT( eButton > MOUSE_NONE && eButton < _MOUSE_BUTTON_MAX_,"wrong parameter" );
		return m_aContext.m_eMouseStateCur[eButton];
	}
	//--------------------------------------------------------------------------------------

	/**
	* @brief get current position of mouse, global position
	*/
	const CGUIVector2& IGUIInterfaceMouse::GetPosition() const
	{
		return m_aContext.m_aPosCur;
	}
	//--------------------------------------------------------------------------------------
	/**
	* @brief get mouse position
	*/
	real IGUIInterfaceMouse::GetWheelChange( ) const
	{
		return m_aContext.m_fWheelChange;
	}
	//--------------------------------------------------------------------------------------
	void IGUIInterfaceMouse::PreUpdate( )
	{

	}
	//--------------------------------------------------------------------------------------
	void IGUIInterfaceMouse::PostUpdate( )
	{
		m_aContext.m_fWheelChange = 0.0f;
	}
	//--------------------------------------------------------------------------------------

}//namespace guiex



