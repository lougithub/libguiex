/** 
* @file guiwgtradiobutton.cpp
* @brief radio button used in the system
* @author ken
* @date 2006-08-03
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtradiobutton.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guievent.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtRadioButton);
	//------------------------------------------------------------------------------
	CGUIWgtRadioButton::CGUIWgtRadioButton(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWgtCheckButton(StaticGetType(), rName, rSceneName)
	{
		InitRadioButton();
	}
	//------------------------------------------------------------------------------
	CGUIWgtRadioButton::CGUIWgtRadioButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtCheckButton(rType, rName, rSceneName)
	{
		InitRadioButton();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtRadioButton::InitRadioButton()
	{
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtRadioButton::OnMouseLeftClick(CGUIEventMouse* pEvent)
	{
		if( m_bChecked == false )
		{
			//send onunchecked event
			if( GetParent())
			{
				//uncheck other radiobutton
				CGUIWidget* pWidget = GetParent()->GetChild();
				while( pWidget)
				{
					if( pWidget != this &&
						pWidget->GetType() == GetType())
					{
						CGUIEventNotification aRadioEvent;
						aRadioEvent.SetEventId(eEVENT_UNCHECKED);
						aRadioEvent.SetReceiver(pWidget);
						GSystem->SendEvent( &aRadioEvent);
					}
					pWidget = pWidget->GetNextSibling();
				}

				//send onchecked event
				CGUIEventNotification aRadioEvent;
				aRadioEvent.SetEventId(eEVENT_CHECKED);
				aRadioEvent.SetReceiver(this);
				GSystem->SendEvent( &aRadioEvent);
			}
		}

		return CGUIWidget::OnMouseLeftClick(pEvent);
	}
	//------------------------------------------------------------------------------

}//namespace guiex

