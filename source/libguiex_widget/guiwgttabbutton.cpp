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
	void		CGUIWgtTabButton::RefreshImpl()
	{
		CGUIWgtCheckButton::RefreshImpl();

		//client rect
		CGUIVector2 aParentPos = GetParent()->GetClientArea().GetPosition();
		ParentToLocal( aParentPos );
		m_aClientArea.SetPosition( aParentPos );
		m_aClientArea.SetSize( GetParent()->GetClipArea().GetSize() );
		m_aClientArea.m_fTop += NEWGetPixelSize().GetHeight();

		//clip rect for client
		m_aClipArea = m_aClientArea;
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
				CGUIWidget* pTempTabBtn = GetParent()->GetChild();
				while( pTempTabBtn)
				{
					if( pTempTabBtn->GetType() == GetType() )
					{
						if( pTempTabBtn != this )
						{
							//send event
							CGUIEventNotification aCheckEvent;
							aCheckEvent.SetEventId(eEVENT_UNCHECKED);
							aCheckEvent.SetReceiver(pTempTabBtn);
							CGUIWidgetSystem::Instance()->SendEvent( &aCheckEvent);

							//hide child
							if( pTempTabBtn->GetChild() )
							{
								pTempTabBtn->GetChild()->SetVisible( false );
							}
						}
						else
						{
							//send onchecked event
							CGUIEventNotification aCheckEvent;
							aCheckEvent.SetEventId(eEVENT_CHECKED);
							aCheckEvent.SetReceiver(this);
							CGUIWidgetSystem::Instance()->SendEvent( &aCheckEvent);

							//show child
							if( this->GetChild() )
							{
								this->GetChild()->SetVisible( true );
							}
						}
					}
					pTempTabBtn = pTempTabBtn->GetNextSibling();
				}
			}
		}

		return CGUIWidget::OnMouseLeftClick(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex

