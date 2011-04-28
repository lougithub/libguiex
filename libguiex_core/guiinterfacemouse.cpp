/** 
 * @file guiinterfacemouse.cpp
 * @brief interface for mouse
 * @author ken
 * @date 2006-06-26
 */

//============================================================================//
// include
//============================================================================// 
#include "guiinterfacemouse.h"
#include "guisystem.h"
#include "guiexception.h"

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
	CGUIVector2 IGUIInterfaceMouse::ConvertPosToEngine( const CGUIVector2& rPos )
	{
		const CGUIIntSize& rRawScreenSize = GSystem->GetRawScreenSize();
		
		real newY = rRawScreenSize.m_uHeight - rPos.y;
		real newX = rRawScreenSize.m_uWidth - rPos.x;
		
		CGUIVector2 ret;
		switch ( GSystem->GetScreenOrientation() )
		{
			case eScreenOrientation_Portrait:
				ret.x = rPos.x;
				ret.y = rPos.y;
				break;
			case eScreenOrientation_PortraitUpsideDown:
				ret.x = newX;
				ret.y = newY;
				break;
			case eScreenOrientation_LandscapeLeft:
				ret.x = rPos.y;
				ret.y = newX;
				break;
			case eScreenOrientation_LandscapeRight:
				ret.x = newY;
				ret.y = rPos.x;
				break;
			default:
				CGUIException::ThrowException(" IGUIInterfaceMouse::ConvertPosToEngine: unknown screen orientation");
				break;
		}
		
		return ret;
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
		m_aContext.m_aPosCur = ConvertPosToEngine(rPos);
		
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



