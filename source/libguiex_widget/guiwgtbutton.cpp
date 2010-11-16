/** 
* @file guiwgtbutton.cpp
* @brief widget: button
* @author ken
* @date 2006-06-08
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtbutton.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiimage.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtButton);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtButton::ms_strType = "CGUIWgtButton";
	//------------------------------------------------------------------------------
	CGUIWgtButton::CGUIWgtButton(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(ms_strType, rName, rSceneName)
		,m_aTextOffset(0.f,0.f)
	{
		InitButton();
	}
	//------------------------------------------------------------------------------
	CGUIWgtButton::CGUIWgtButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitButton();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::InitButton()
	{
		m_bHovering = false;
		m_bPushing = false;

		m_pImageNormal = NULL;
		m_pImageHovering = NULL;
		m_pImageDisable = NULL;
		m_pImagePush = NULL;
		m_pImageHoverOverlay = NULL;
		m_pImagePushOverlay = NULL;
		

		SetFocusable(true);
		SetSelfActivable(false);
		SetGenerateClickEvent( true );
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtButton::Create()
	{
		return CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtButton::OnSetImage( const CGUIString& rName, const CGUIImage* pImage )
	{
		if( rName == "BTN_NORMAL")
		{
			m_pImageNormal = pImage;
			if( pImage && GetSize().IsEqualZero())
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "BTN_HOVER")
		{
			m_pImageHovering = pImage;
		}
		else if( rName == "BTN_HOVER_OVERLAY")
		{
			m_pImageHoverOverlay = pImage;
		}
		else if( rName == "BTN_PUSH_OVERLAY")
		{
			m_pImagePushOverlay = pImage;
		}
		else if( rName == "BTN_DISABLE")
		{
			m_pImageDisable = pImage;
		}
		else if( rName == "BTN_PUSH")
		{
			m_pImagePush = pImage;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::RefreshSelf()
	{
		CGUIWidget::RefreshSelf();

		//update client area
		m_aStringArea = GetBoundArea();
		if( m_aTextOffset.x < m_aStringArea.GetWidth() )
		{
			m_aStringArea.m_fLeft += m_aTextOffset.x;
		}
		else
		{
			m_aStringArea.m_fLeft = m_aStringArea.m_fRight;
		}
		if( m_aTextOffset.y < m_aStringArea.GetHeight() )
		{
			m_aStringArea.m_fTop += m_aTextOffset.y;
		}
		else
		{
			m_aStringArea.m_fTop = m_aStringArea.m_fBottom;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtButton::RenderSelf(IGUIInterfaceRender* pRender)
	{
		const CGUIImage* pImage = NULL;
		CGUIStringEx* pString = NULL;

		if( IsDerivedDisable())
		{
			if( !m_strTextDisable.Empty() )
			{
				pString = &m_strTextDisable;
			}
			else
			{
				pString = &m_strText;
			}
			pImage = m_pImageDisable ? m_pImageDisable : m_pImageNormal;
		}
		else if( m_bHovering && m_bPushing )
		{
			if( !m_strTextPush.Empty() )
			{
				pString = &m_strTextPush;
			}
			else
			{
				pString = &m_strText;
			}
			pImage = m_pImagePush ? m_pImagePush : m_pImageNormal;
		}
		else if( m_bHovering )
		{
			if( !m_strTextHoving.Empty() )
			{
				pString = &m_strTextHoving;
			}
			else
			{
				pString = &m_strText;
			}
			pImage = m_pImageHovering ? m_pImageHovering : m_pImageNormal; 
		}
		else
		{
			pString = &m_strText;
			pImage = m_pImageNormal;
		}

		DrawImage( pRender, pImage, GetBoundArea());
		if( m_bHovering && m_pImageHoverOverlay )
		{
			DrawImage( pRender, m_pImageHoverOverlay, GetBoundArea());
		}
		

		DrawString(pRender, *pString, m_aStringArea, GetTextAlignment());
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetBtnTextContent_Hover(const wchar_t* pText )
	{
		m_strTextHoving.SetContent(pText);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetBtnTextInfo_Hover(const CGUIStringInfo& rInfo )
	{
		m_strTextHoving.SetDefaultInfo(&rInfo);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetBtnTextContent_Disable(const wchar_t* pText )
	{
		m_strTextDisable.SetContent(pText);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetBtnTextInfo_Disable(const CGUIStringInfo& rInfo )
	{
		m_strTextDisable.SetDefaultInfo(&rInfo);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetBtnTextContent_Push(const wchar_t* pText )
	{
		m_strTextPush.SetContent(pText);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetBtnTextInfo_Push(const CGUIStringInfo& rInfo )
	{
		m_strTextPush.SetDefaultInfo(&rInfo);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtButton::SetTextContent(const wchar_t* pText)
	{
		CGUIWidget::SetTextContent( pText );

		if( !m_strTextHoving.Empty())
		{
			m_strTextHoving.SetContent(pText);
		}
		if( !m_strTextDisable.Empty())
		{
			m_strTextDisable.SetContent(pText);
		}
		if( !m_strTextPush.Empty())
		{
			m_strTextPush.SetContent(pText);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtButton::SetStringOffset( const CGUIVector2& rPos)
	{
		m_aTextOffset = rPos;
	}
	//------------------------------------------------------------------------------
	const CGUIVector2&	CGUIWgtButton::GetStringOffset(  ) const
	{
		return m_aTextOffset;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtButton::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo_hover" )
		{
			ValueToProperty( m_strTextHoving.GetDefaultInfo(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo_disable" )
		{
			ValueToProperty( m_strTextDisable.GetDefaultInfo(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo_push" )
		{
			ValueToProperty( m_strTextPush.GetDefaultInfo(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text_hover" )
		{
			CGUIString aStrText;
			WideByteToMultiChar( m_strTextHoving.GetContent(), aStrText);
			rProperty.SetValue(aStrText);
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text_push" )
		{
			CGUIString aStrText;
			WideByteToMultiChar( m_strTextPush.GetContent(), aStrText);
			rProperty.SetValue(aStrText);
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text_disable" )
		{
			CGUIString aStrText;
			WideByteToMultiChar( m_strTextDisable.GetContent(), aStrText);
			rProperty.SetValue(aStrText);
		}
		else if( rProperty.GetType() == ePropertyType_Vector2 && rProperty.GetName() == "text_offset" )
		{
			ValueToProperty( GetStringOffset(), rProperty );
		}
		else
		{
			return CGUIWidget::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::ProcessProperty( const CGUIProperty& rProperty)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for text
		if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo_hover")
		{
			CGUIStringInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetBtnTextInfo_Hover(aInfo);
		}
		else if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo_push")
		{
			CGUIStringInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetBtnTextInfo_Push(aInfo);
		}
		else if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo_disable")
		{
			CGUIStringInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetBtnTextInfo_Disable(aInfo);
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text_hover")
		{
			CGUIStringEx aStrText;
			MultiByteToWideChar(rProperty.GetValue(), aStrText);
			SetBtnTextContent_Hover(aStrText.GetContent());
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text_push")
		{
			CGUIStringEx aStrText;
			MultiByteToWideChar(rProperty.GetValue(), aStrText);
			SetBtnTextContent_Push(aStrText.GetContent());
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text_disable")
		{
			CGUIStringEx aStrText;
			MultiByteToWideChar(rProperty.GetValue(), aStrText);
			SetBtnTextContent_Disable(aStrText.GetContent());
		}
		else if( rProperty.GetType() == ePropertyType_Vector2 && rProperty.GetName() == "text_offset")
		{
			CGUIVector2 aValue;
			PropertyToValue( rProperty, aValue );
			SetStringOffset( aValue );
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	uint32		CGUIWgtButton::OnMouseEnter( CGUIEventMouse* pEvent )
	{
		m_bHovering = true;

		return CGUIWidget::OnMouseEnter( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtButton::OnMouseLeave( CGUIEventMouse* pEvent )
	{
		m_bHovering = false;

		return CGUIWidget::OnMouseLeave( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtButton::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		m_bPushing = true;

		return CGUIWidget::OnMouseLeftDown( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtButton::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		m_bPushing = false;

		return CGUIWidget::OnMouseLeftUp( pEvent );
	}
	//------------------------------------------------------------------------------
}//namespace guiex
