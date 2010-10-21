/** 
* @file guiwgttabbutton.cpp
* @brief tabbutton used in the system
* @author ken
* @date 2007-07-06
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgttabbutton.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guievent.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtTabButton);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtTabButton::ms_strType = "CGUIWgtTabButton";
	//------------------------------------------------------------------------------
	CGUIWgtTabButton::CGUIWgtTabButton(const CGUIString& rName, const CGUIString& rProjectName)
		:CGUIWgtCheckButton(ms_strType, rName, rProjectName)
	{
		InitTabButton();
	}
	//------------------------------------------------------------------------------
	CGUIWgtTabButton::CGUIWgtTabButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWgtCheckButton(rType, rName, rProjectName)
	{
		InitTabButton();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTabButton::InitTabButton()
	{
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtTabButton::UpdateDirtyRect()
	{
		//client rect
		m_aClientRect = GetParent()->GetClientRect();
		m_aClientRect.m_fTop += m_aWidgetRect.GetHeight();

		//clip rect for client
		m_aClientClipRect = m_aClientRect;
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtTabButton::IsSelected(void) const
	{
		return IsCheck();
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtTabButton::OnMouseLeftClick(CGUIEventMouse* pEvent)
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
						pWidget->GetType() == GetType() )
					{
						CGUIEventNotification aRadioEvent;
						aRadioEvent.SetEventId(eEVENT_UNCHECKED);
						aRadioEvent.SetReceiver(pWidget);
						CGUIWidgetSystem::Instance()->SendEvent( &aRadioEvent);
						pWidget->GetChild()->Hide( );
					}
					pWidget = pWidget->GetNextSibling();
				}

				//send onchecked event
				CGUIEventNotification aRadioEvent;
				aRadioEvent.SetEventId(eEVENT_CHECKED);
				aRadioEvent.SetReceiver(this);
				CGUIWidgetSystem::Instance()->SendEvent( &aRadioEvent);
				this->GetChild()->Show( );
			}
		}

		return CGUIWidget::OnMouseLeftClick(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex

