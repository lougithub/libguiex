/** 
* @file guiwgtcheckbutton.cpp
* @brief checkbutton used in the system
* @author ken
* @date 2006-08-03
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtcheckbutton.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guievent.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guipropertyconvertor.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtCheckButton);
	//------------------------------------------------------------------------------
	CGUIWgtCheckButton::CGUIWgtCheckButton(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitCheckButton();
	}
	//------------------------------------------------------------------------------
	CGUIWgtCheckButton::CGUIWgtCheckButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitCheckButton();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtCheckButton::InitCheckButton()
	{
		m_bHovering = false;
		m_bPushing = false;
		m_bChecked = false;

		m_pImageNormal = NULL;
		m_pImageHovering = NULL;
		m_pImageDisable = NULL;
		m_pImagePush = NULL;
		m_pImageNormalChecked = NULL;
		m_pImageHoveringChecked = NULL;
		m_pImageDisableChecked = NULL;
		m_pImagePushChecked = NULL;
		m_pImageCheckedOverlay = NULL;
		m_pImageHoverOverlay = NULL;

		SetGenerateClickEvent( true );
		SetFocusable(true);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtCheckButton::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "normal")
		{
			m_pImageNormal = pImage;
			if( GetPixelSize().IsEqualZero() && pImage )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "hover")
		{
			m_pImageHovering = pImage;
		}
		else if( rName == "disable")
		{
			m_pImageDisable = pImage;
		}
		else if( rName == "push")
		{
			m_pImagePush = pImage;
		}
		else if( rName == "normal_checked")
		{
			m_pImageNormalChecked = pImage;
		}
		else if( rName == "hover_checked")
		{
			m_pImageHoveringChecked = pImage;
		}
		else if( rName == "disable_checked")
		{
			m_pImageDisableChecked = pImage;
		}
		else if( rName == "push_checked")
		{
			m_pImagePushChecked = pImage;
		}
		else if( rName == "hover_overlay")
		{
			m_pImageHoverOverlay = pImage;
		}
		else if( rName == "checked_overlay")
		{
			m_pImageCheckedOverlay = pImage;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtCheckButton::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIImage* pImage = NULL;

		if( !m_bChecked )
		{
			if( IsDerivedDisable())
			{
				pImage = m_pImageDisable ? m_pImageDisable : m_pImageNormal;
			}
			else if( m_bHovering && m_bPushing ) 
			{
				pImage = m_pImagePush ? m_pImagePush : m_pImageNormal;
			}
			else if( m_bHovering )
			{ 
				pImage = m_pImageHovering ? m_pImageHovering : m_pImageNormal; 
			}
			else
			{
				pImage = m_pImageNormal;
			}
		}
		else
		{
			if( IsDerivedDisable())
			{
				pImage = m_pImageDisableChecked ? m_pImageDisableChecked : m_pImageNormalChecked ? m_pImageNormalChecked : m_pImageNormal;
			}
			else if( m_bHovering && m_bPushing )
			{
				pImage = m_pImagePushChecked ? m_pImagePushChecked : m_pImageNormalChecked  ? m_pImageNormalChecked : m_pImageNormal;
			}
			else if( m_bHovering )
			{
				pImage = m_pImageHoveringChecked ? m_pImageHoveringChecked : m_pImageNormalChecked  ? m_pImageNormalChecked : m_pImageNormal;
			}
			else
			{
				pImage = m_pImageNormalChecked  ? m_pImageNormalChecked : m_pImageNormal; 
			}
		}

		DrawImage( pRender, pImage, GetBoundArea());

		if( m_bChecked && m_pImageCheckedOverlay )
		{
			DrawImage( pRender, m_pImageCheckedOverlay, GetBoundArea());
		}

		if( m_bHovering && m_pImageHoverOverlay )
		{
			DrawImage( pRender, m_pImageHoverOverlay, GetBoundArea());
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set widget to check state
	* @notice this function won't generate OnCheck event
	*/
	void CGUIWgtCheckButton::SetChecked(bool bChecked)
	{
		m_bChecked = bChecked;
	}
	//------------------------------------------------------------------------------	
	bool CGUIWgtCheckButton::IsChecked() const
	{
		return m_bChecked;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtCheckButton::OnMouseEnter( CGUIEventMouse* pEvent )
	{
		m_bHovering = true;

		return CGUIWidget::OnMouseEnter( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtCheckButton::OnMouseLeave( CGUIEventMouse* pEvent )
	{
		m_bHovering = false;

		return CGUIWidget::OnMouseLeave( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtCheckButton::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		m_bPushing = true;

		return CGUIWidget::OnMouseLeftDown( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtCheckButton::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		m_bPushing = false;

		return CGUIWidget::OnMouseLeftUp( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtCheckButton::OnMouseLeftClick(CGUIEventMouse* pEvent)
	{
		CGUIEventNotification aCheckEvent;
		aCheckEvent.SetEventId((!m_bChecked)?eEVENT_CHECKED:eEVENT_UNCHECKED);
		aCheckEvent.SetReceiver(this);
		GSystem->SendEvent( &aCheckEvent);

		return CGUIWidget::OnMouseLeftClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtCheckButton::OnUnchecked(CGUIEventNotification* pEvent)
	{
		m_bChecked = false;

		return CGUIWidget::OnUnchecked(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtCheckButton::OnChecked(CGUIEventNotification* pEvent)
	{
		m_bChecked = true;

		return CGUIWidget::OnChecked(pEvent);
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtCheckButton::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "checked" )
		{
			ValueToProperty( IsChecked(), rProperty );
		}
		else
		{
			return CGUIWidget::GenerateProperty( rProperty );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtCheckButton::ProcessProperty( const CGUIProperty& rProperty)
	{
		if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "checked")
		{
			bool bChecked;
			PropertyToValue( rProperty, bChecked);
			SetChecked( bChecked );
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex

