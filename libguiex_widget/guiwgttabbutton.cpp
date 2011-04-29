/** 
* @file guiwgttabbutton.cpp
* @brief tabbutton used in the system
* @author ken
* @date 2007-07-06
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgttabbutton.h"
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
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtTabButton);
	//------------------------------------------------------------------------------
	CGUIWgtTabButton::CGUIWgtTabButton(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWgtCheckButton(StaticGetType(), rName, rSceneName)
	{
		InitTabButton();
	}
	//------------------------------------------------------------------------------
	CGUIWgtTabButton::CGUIWgtTabButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtCheckButton(rType, rName, rSceneName)
	{
		InitTabButton();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTabButton::InitTabButton()
	{
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtTabButton::RefreshSelf()
	{
		CGUIWgtCheckButton::RefreshSelf();

		//client rect
		CGUIVector2 aParentPos = GetParent()->GetClientArea().GetPosition();
		ParentToLocal( aParentPos );
		m_aClientArea.SetPosition( aParentPos );
		m_aClientArea.SetSize( GetParent()->GetClipArea().GetSize() );
		m_aClientArea.m_fTop += GetPixelSize().GetHeight();

		//clip rect for client
		m_aClipArea = m_aClientArea;
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtTabButton::IsSelected(void) const
	{
		return IsChecked();
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtTabButton::OnMouseLeftClick(CGUIEventMouse* pEvent)
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
							GSystem->SendEvent( &aCheckEvent);

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
							GSystem->SendEvent( &aCheckEvent);

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

