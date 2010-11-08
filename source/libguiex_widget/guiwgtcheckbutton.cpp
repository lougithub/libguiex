/** 
* @file guiwgtcheckbutton.cpp
* @brief checkbutton used in the system
* @author ken
* @date 2006-08-03
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtcheckbutton.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guievent.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiimage.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtCheckButton);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtCheckButton::ms_strType = "CGUIWgtCheckButton";
	//------------------------------------------------------------------------------
	CGUIWgtCheckButton::CGUIWgtCheckButton(const CGUIString& rName, const CGUIString& rProjectName)
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitCheckButton();
	}
	//------------------------------------------------------------------------------
	CGUIWgtCheckButton::CGUIWgtCheckButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
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
		SetSelfActivable(false);

	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtCheckButton::Create()
	{
		return CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtCheckButton::OnSetImage( const CGUIString& rName,CGUIImage* pImage )
	{
		if( rName == "BTN_NORMAL")
		{
			m_pImageNormal = pImage;
			if( NEWGetPixelSize().IsEqualZero() && pImage )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "BTN_HOVER")
		{
			m_pImageHovering = pImage;
		}
		else if( rName == "BTN_DISABLE")
		{
			m_pImageDisable = pImage;
		}
		else if( rName == "BTN_PUSH")
		{
			m_pImagePush = pImage;
		}
		else if( rName == "BTN_NORMAL_CHECKED")
		{
			m_pImageNormalChecked = pImage;
		}
		else if( rName == "BTN_HOVER_CHECKED")
		{
			m_pImageHoveringChecked = pImage;
		}
		else if( rName == "BTN_DISABLE_CHECKED")
		{
			m_pImageDisableChecked = pImage;
		}
		else if( rName == "BTN_PUSH_CHECKED")
		{
			m_pImagePushChecked = pImage;
		}
		else if( rName == "BTN_HOVER_OVERLAY")
		{
			m_pImageHoverOverlay = pImage;
		}
		else if( rName == "BTN_CHECKED_OVERLAY")
		{
			m_pImageCheckedOverlay = pImage;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtCheckButton::RenderSelf(IGUIInterfaceRender* pRender)
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
	void	CGUIWgtCheckButton::SetCheck(bool bChecked)
	{
		m_bChecked = bChecked;
	}
	//------------------------------------------------------------------------------	
	bool	CGUIWgtCheckButton::IsCheck() const
	{
		return m_bChecked;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtCheckButton::SetValue(const CGUIString& rName, const CGUIString& rValue)
	{
		if( rName == "Checked")
		{
			SetCheck(rValue=="true"?true:false);
		}
		else
		{
			CGUIWidget::SetValue(rName, rValue);
		}
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWgtCheckButton::GetValue(const CGUIString& rName) const
	{
		if( rName == "Checked")
		{
			return IsCheck()==true?"true":"false";
		}
		else
		{
			return CGUIWidget::GetValue(rName);
		}
	}
	//------------------------------------------------------------------------------
	CGUIWgtCheckButton*	CGUIWgtCheckButton::FromWidget( CGUIWidget* pWidget )
	{
		if( !pWidget )
		{
			throw CGUIException("[CGUIWgtCheckButton::FromWidget]: the given widget is nil" );
		}
		if( pWidget->GetType() != ms_strType )
		{
			throw CGUIException("[CGUIWgtCheckButton::FromWidget]: the real type of given widget is <%s>!", pWidget->GetType().c_str());
		}
		return dynamic_cast<CGUIWgtCheckButton *>(pWidget);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtCheckButton::OnMouseEnter( CGUIEventMouse* pEvent )
	{
		m_bHovering = true;

		return CGUIWidget::OnMouseEnter( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtCheckButton::OnMouseLeave( CGUIEventMouse* pEvent )
	{
		m_bHovering = false;

		return CGUIWidget::OnMouseLeave( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtCheckButton::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		m_bPushing = true;

		return CGUIWidget::OnMouseLeftDown( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtCheckButton::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		m_bPushing = false;

		return CGUIWidget::OnMouseLeftUp( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtCheckButton::OnMouseLeftClick(CGUIEventMouse* pEvent)
	{
		CGUIEventNotification aCheckEvent;
		aCheckEvent.SetEventId((!m_bChecked)?eEVENT_CHECKED:eEVENT_UNCHECKED);
		aCheckEvent.SetReceiver(this);
		CGUIWidgetSystem::Instance()->SendEvent( &aCheckEvent);

		return CGUIWidget::OnMouseLeftClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtCheckButton::OnUnchecked(CGUIEventNotification* pEvent)
	{
		m_bChecked = false;

		return CGUIWidget::OnUnchecked(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtCheckButton::OnChecked(CGUIEventNotification* pEvent)
	{
		m_bChecked = true;

		return CGUIWidget::OnChecked(pEvent);
	}
	//------------------------------------------------------------------------------
	CGUIProperty*	CGUIWgtCheckButton::GenerateProperty(const CGUIString& rName, const CGUIString& rType )
	{
		//CGUIProperty* pProperty = NULL;
		//
		//if( rName == "CHECKED" && rType == "BOOL" )
		//{
		//	pProperty = CGUIPropertyManager::Instance()->CreateProperty(
		//		rName, 
		//		rType, 
		//		CGUIStringConvertor::BoolToString(IsCheck( )));
		//}

		//return pProperty ? pProperty : CGUIWidget::GenerateProperty(rName, rType);
		return NULL;
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtCheckButton::ProcessProperty( const CGUIProperty* pProperty)
	{
		//CGUIWidget::ProcessProperty(pProperty);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		////property for CHECKED
		///*
		//*<property name="CHECKED" type="BOOL" value="true" />
		//*/
		//if( pProperty->GetName() == "CHECKED" && pProperty->GetType()=="BOOL")
		//{
		//	SetCheck(StringToValue(pProperty->GetValue()));
		//}
	}
	//------------------------------------------------------------------------------
}//namespace guiex

