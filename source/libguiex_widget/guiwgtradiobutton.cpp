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
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtRadioButton);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtRadioButton::ms_strType = "CGUIWgtRadioButton";
	//------------------------------------------------------------------------------
	CGUIWgtRadioButton::CGUIWgtRadioButton(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWgtCheckButton(ms_strType, rName, rSceneName)
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
	CGUIWgtRadioButton*	CGUIWgtRadioButton::FromWidget( CGUIWidget* pWidget )
	{
		if( !pWidget )
		{
			throw CGUIException("[CGUIWgtRadioButton::FromWidget]: the given widget is nil" );
		}
		if( pWidget->GetType() != ms_strType )
		{
			throw CGUIException("[CGUIWgtRadioButton::FromWidget]: the real type of given widget is <%s>!", pWidget->GetType().c_str());
		}
		return dynamic_cast<CGUIWgtRadioButton *>(pWidget);
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
						CGUIWidgetSystem::Instance()->SendEvent( &aRadioEvent);
					}
					pWidget = pWidget->GetNextSibling();
				}

				//send onchecked event
				CGUIEventNotification aRadioEvent;
				aRadioEvent.SetEventId(eEVENT_CHECKED);
				aRadioEvent.SetReceiver(this);
				CGUIWidgetSystem::Instance()->SendEvent( &aRadioEvent);
			}
		}

		return CGUIWidget::OnMouseLeftClick(pEvent);
	}
	//------------------------------------------------------------------------------

}//namespace guiex

