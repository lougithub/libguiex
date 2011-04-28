/** 
* @file guiinterfacekeyboard.cpp
* @brief interface for keyboard
* @author ken
* @date 2006-08-20
*/

//============================================================================//
// include
//============================================================================// 
#include "guiinterfacekeyboard.h"
#include "guisystem.h"

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceKeyboard::StaticGetModuleType()
	{ 
		return "IGUIKeyboard";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceKeyboard::IGUIInterfaceKeyboard( const char* szModuleName )
		:IGUIInterface( StaticGetModuleType(), szModuleName )
	{
		Reset();
	}
	//------------------------------------------------------------------------------ 
	IGUIInterfaceKeyboard::~IGUIInterfaceKeyboard()
	{

	}
	//------------------------------------------------------------------------------ 
	void IGUIInterfaceKeyboard::Reset()
	{
		memset(&m_aContext, 0, sizeof(SKeyContext));
	}
	//------------------------------------------------------------------------------ 
	bool IGUIInterfaceKeyboard::ProcessKeyEvent( const SKeyEvent& rEvent )
	{
		return GSystem->ProcessKeyboardInput( rEvent );
	}
	//------------------------------------------------------------------------------ 
	bool IGUIInterfaceKeyboard::ChangeKeyState(EKeyCode eKey, EKeyState eState)
	{
		GUI_ASSERT( eKey >= KC_NONE && eKey < _KEY_CODE_MAX_,"wrong parameter" );
		GUI_ASSERT( eState >= KEY_DOWN && eState < _KEY_STATE_MAX_,"wrong parameter" );

		bool bConsumed = false;
		if( eKey == KC_NONE )
		{
			return bConsumed;
		}


		switch( eState )
		{
			//the key is pressed
		case KEY_DOWN:
			{
				if( m_aContext.m_bKeyDown[eKey])
				{
					bConsumed = ProcessKeyEvent(SKeyEvent(eKey, KEY_EVENT_PRESSED));
				}
				else
				{
					m_aContext.m_bKeyDown[eKey] = true;

					//first time
					bConsumed = ProcessKeyEvent(SKeyEvent(eKey, KEY_EVENT_DOWN));
					bConsumed = bConsumed || ProcessKeyEvent(SKeyEvent(eKey, KEY_EVENT_PRESSED));

					//set pressed flag
					m_aContext.m_nKeyPressedCounter++;
				}
			}
			break;

			//key is released
		case KEY_UP:
			{
				if( m_aContext.m_bKeyDown[eKey])
				{
					m_aContext.m_bKeyDown[eKey] = false;

					bConsumed = ProcessKeyEvent(SKeyEvent(eKey, KEY_EVENT_CLICKED));
					bConsumed = bConsumed || ProcessKeyEvent(SKeyEvent(eKey, KEY_EVENT_UP));

					m_aContext.m_nKeyPressedCounter--;
				}
				else
				{

				}
			}
			break;
				
		default:
			break;
		}

		return bConsumed;
	}
	//------------------------------------------------------------------------------ 
	bool IGUIInterfaceKeyboard::IsKeyPressed(EKeyCode eKey) const       
	{
		GUI_ASSERT( eKey >= KC_NONE && eKey < _KEY_CODE_MAX_,"wrong parameter");
		return m_aContext.m_bKeyDown[eKey];
	}
	//------------------------------------------------------------------------------ 
	bool IGUIInterfaceKeyboard::HasAnyKeyPressed() const                      
	{
		return m_aContext.m_nKeyPressedCounter > 0 ? true : false;
	}
	//------------------------------------------------------------------------------ 
	void IGUIInterfaceKeyboard::GetAllKeyPressed(std::vector<EKeyCode>& rKeys) const          
	{
		for( int i = KC_NONE; i < _KEY_CODE_MAX_; ++i )
		{
			if( m_aContext.m_bKeyDown[i] )
			{
				rKeys.push_back((EKeyCode)i);
			}
		}
	}
	//------------------------------------------------------------------------------ 
	void IGUIInterfaceKeyboard::PreUpdate()
	{

	}
	//------------------------------------------------------------------------------ 
	void IGUIInterfaceKeyboard::PostUpdate()
	{
	}
	//------------------------------------------------------------------------------ 
}//namespace guiex


