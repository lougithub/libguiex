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
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guilocalizationmanager.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtButton);
	//------------------------------------------------------------------------------
	CGUIWgtButton::CGUIWgtButton(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
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

		memset( m_pImage, 0, sizeof(m_pImage) );
		m_pImageHoverOverlay = NULL;
		
		m_eTextAlignmentHorz = eTextAlignment_Horz_Center;
		m_eTextAlignmentVert = eTextAlignment_Vert_Center;

		SetFocusable(true);
		SetGenerateClickEvent( true );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "normal")
		{
			m_pImage[eButtonState_Normal] = pImage;
			if( pImage && GetSize().IsEqualZero())
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "hover")
		{
			m_pImage[eButtonState_Hover] = pImage;
		}
		else if( rName == "push")
		{
			m_pImage[eButtonState_Push] = pImage;
		}
		else if( rName == "disable")
		{
			m_pImage[eButtonState_Disable] = pImage;
		}
		else if( rName == "hover_overlay")
		{
			m_pImageHoverOverlay = pImage;
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
	void CGUIWgtButton::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIWidget::RenderSelf( pRender );

		EButtonState eType = eButtonState_Normal;
		if( IsDerivedDisable())
		{
			eType = eButtonState_Disable;
		}
		else if( m_bHovering && m_bPushing )
		{
			eType = eButtonState_Push;
		}
		else if( m_bHovering )
		{
			eType = eButtonState_Hover;
		}

		CGUIStringRender* pString = m_strText[eType].m_strContent.empty() ? &m_strText[eButtonState_Normal] : &m_strText[eType];
		CGUIImage* pImage = m_pImage[eType] ? m_pImage[eType] : m_pImage[eButtonState_Normal];

		DrawImage( pRender, pImage, GetBoundArea());
		if( m_bHovering && m_pImageHoverOverlay )
		{
			DrawImage( pRender, m_pImageHoverOverlay, GetBoundArea());
		}
	
		DrawString( pRender, *pString, m_aStringArea, m_eTextAlignmentHorz, m_eTextAlignmentVert );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetStringOffset( const CGUIVector2& rPos)
	{
		m_aTextOffset = rPos;
	}
	//------------------------------------------------------------------------------
	const CGUIVector2& CGUIWgtButton::GetStringOffset(  ) const
	{
		return m_aTextOffset;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtButton::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_StringRenderInfo && rProperty.GetName() == "textinfo_normal" )
		{
			ValueToProperty( m_strText[eButtonState_Normal].GetStringInfo(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_StringRenderInfo && rProperty.GetName() == "textinfo_hover" )
		{
			ValueToProperty( m_strText[eButtonState_Hover].GetStringInfo(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_StringRenderInfo && rProperty.GetName() == "textinfo_disable" )
		{
			ValueToProperty( m_strText[eButtonState_Disable].GetStringInfo(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_StringRenderInfo && rProperty.GetName() == "textinfo_push" )
		{
			ValueToProperty( m_strText[eButtonState_Push].GetStringInfo(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_LocalizedString && rProperty.GetName() == "text_normal" )
		{
			CGUIString aStrText;
			AppWideByteToMultiChar( m_strText[eButtonState_Normal].GetContent(), aStrText);
			rProperty.SetValue(aStrText);
		}
		else if( rProperty.GetType() == ePropertyType_LocalizedString && rProperty.GetName() == "text_hover" )
		{
			CGUIString aStrText;
			AppWideByteToMultiChar( m_strText[eButtonState_Hover].GetContent(), aStrText);
			rProperty.SetValue(aStrText);
		}
		else if( rProperty.GetType() == ePropertyType_LocalizedString && rProperty.GetName() == "text_push" )
		{
			CGUIString aStrText;
			AppWideByteToMultiChar( m_strText[eButtonState_Push].GetContent(), aStrText);
			rProperty.SetValue(aStrText);
		}
		else if( rProperty.GetType() == ePropertyType_LocalizedString && rProperty.GetName() == "text_disable" )
		{
			CGUIString aStrText;
			AppWideByteToMultiChar( m_strText[eButtonState_Disable].GetContent(), aStrText);
			rProperty.SetValue(aStrText);
		}
		else if( rProperty.GetType() == ePropertyType_Vector2 && rProperty.GetName() == "text_offset" )
		{
			ValueToProperty( GetStringOffset(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_TextAlignmentHorz && rProperty.GetName() == "text_alignment_horz" )
		{
			ValueToProperty( GetTextAlignmentHorz(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_TextAlignmentVert && rProperty.GetName() == "text_alignment_vert" )
		{
			ValueToProperty( GetTextAlignmentVert(), rProperty);
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
		if( rProperty.GetType() == ePropertyType_StringRenderInfo && rProperty.GetName() == "textinfo_normal")
		{
			CGUIStringRenderInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetTextInfo(aInfo, eButtonState_Normal);
		}
		else if( rProperty.GetType() == ePropertyType_StringRenderInfo && rProperty.GetName() == "textinfo_hover")
		{
			CGUIStringRenderInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetTextInfo(aInfo, eButtonState_Hover);
		}
		else if( rProperty.GetType() == ePropertyType_StringRenderInfo && rProperty.GetName() == "textinfo_push")
		{
			CGUIStringRenderInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetTextInfo(aInfo, eButtonState_Push);
		}
		else if( rProperty.GetType() == ePropertyType_StringRenderInfo && rProperty.GetName() == "textinfo_disable")
		{
			CGUIStringRenderInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetTextInfo(aInfo, eButtonState_Disable);
		}
		else if( rProperty.GetType() == ePropertyType_LocalizedString && rProperty.GetName() == "text_normal")
		{
			CGUIStringRender aStrText;
			AppMultiByteToWideChar(rProperty.GetValue(), aStrText.m_strContent);
			SetTextContent(aStrText.GetContent(), eButtonState_Normal);
		}
		else if( rProperty.GetType() == ePropertyType_LocalizedString && rProperty.GetName() == "text_hover")
		{
			CGUIStringRender aStrText;
			AppMultiByteToWideChar(rProperty.GetValue(), aStrText.m_strContent);
			SetTextContent(aStrText.GetContent(), eButtonState_Hover);
		}
		else if( rProperty.GetType() == ePropertyType_LocalizedString && rProperty.GetName() == "text_push")
		{
			CGUIStringRender aStrText;
			AppMultiByteToWideChar(rProperty.GetValue(), aStrText.m_strContent);
			SetTextContent(aStrText.GetContent(), eButtonState_Push);
		}
		else if( rProperty.GetType() == ePropertyType_LocalizedString && rProperty.GetName() == "text_disable")
		{
			CGUIStringRender aStrText;
			AppMultiByteToWideChar(rProperty.GetValue(), aStrText.m_strContent);
			SetTextContent(aStrText.GetContent(), eButtonState_Disable);
		}
		else if( rProperty.GetType() == ePropertyType_Vector2 && rProperty.GetName() == "text_offset")
		{
			CGUIVector2 aValue;
			PropertyToValue( rProperty, aValue );
			SetStringOffset( aValue );
		}
		else if( rProperty.GetType() == ePropertyType_TextAlignmentHorz && rProperty.GetName() == "text_alignment_horz" )
		{
			ETextAlignmentHorz eTextAlignmentH = eTextAlignment_Horz_Center;
			PropertyToValue( rProperty, eTextAlignmentH );
			SetTextAlignmentHorz( eTextAlignmentH );
		}
		else if( rProperty.GetType() == ePropertyType_TextAlignmentVert && rProperty.GetName() == "text_alignment_vert" )
		{
			ETextAlignmentVert eTextAlignmentV = eTextAlignment_Vert_Center;
			PropertyToValue( rProperty, eTextAlignmentV );
			SetTextAlignmentVert( eTextAlignmentV );
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetTextAlignmentVert( ETextAlignmentVert eAlignment )
	{
		m_eTextAlignmentVert = eAlignment;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetTextAlignmentHorz( ETextAlignmentHorz eAlignment )
	{
		m_eTextAlignmentHorz = eAlignment;
	}
	//------------------------------------------------------------------------------
	ETextAlignmentHorz CGUIWgtButton::GetTextAlignmentHorz( ) const
	{
		return m_eTextAlignmentHorz;
	}
	//------------------------------------------------------------------------------
	ETextAlignmentVert CGUIWgtButton::GetTextAlignmentVert( ) const
	{
		return m_eTextAlignmentVert;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetTextContent( const CGUIStringW& rText, EButtonState eButtonState )
	{
		GUI_ASSERT( eButtonState < __eButtonState_NUM__, "invalid button state");
		m_strText[eButtonState].m_strContent = CGUILocalizationManager::Instance()->Localize( rText );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetTextInfo(const CGUIStringRenderInfo& rInfo, EButtonState eButtonState )
	{
		GUI_ASSERT( eButtonState < __eButtonState_NUM__, "invalid button state");
		m_strText[eButtonState].m_aStringInfo = rInfo;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetTextColor(const CGUIColor& rColor, EButtonState eButtonState)
	{
		GUI_ASSERT( eButtonState < __eButtonState_NUM__, "invalid button state");
		m_strText[eButtonState].m_aStringInfo.m_aColor = rColor;
	}
	//------------------------------------------------------------------------------
	const CGUIStringW& CGUIWgtButton::GetTextContent( EButtonState eButtonState ) const
	{
		GUI_ASSERT( eButtonState < __eButtonState_NUM__, "invalid button state");
		return m_strText[eButtonState].m_strContent;
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtButton::IsTextContentEmpty( EButtonState eButtonState ) const
	{
		GUI_ASSERT( eButtonState < __eButtonState_NUM__, "invalid button state");
		return m_strText[eButtonState].m_strContent.empty();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtButton::SetTextContentUTF8( const CGUIString& rString, EButtonState eButtonState)
	{
		GUI_ASSERT( eButtonState < __eButtonState_NUM__, "invalid button state");
		CGUIStringW strTemp;
		AppMultiByteToWideChar( rString, strTemp);
		SetTextContent( strTemp, eButtonState );
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtButton::GetTextContentUTF8( EButtonState eButtonState ) const
	{
		GUI_ASSERT( eButtonState < __eButtonState_NUM__, "invalid button state");
		CGUIString aContentUTF8;
		AppWideByteToMultiChar( m_strText[eButtonState].m_strContent, aContentUTF8 );
		return aContentUTF8;
	}
	//------------------------------------------------------------------------------
	const CGUIStringRender&	CGUIWgtButton::GetText( EButtonState eButtonState ) const
	{
		GUI_ASSERT( eButtonState < __eButtonState_NUM__, "invalid button state");
		return m_strText[eButtonState];
	}
	//------------------------------------------------------------------------------
	const CGUIStringRenderInfo& CGUIWgtButton::GetTextInfo( EButtonState eButtonState ) const
	{
		GUI_ASSERT( eButtonState < __eButtonState_NUM__, "invalid button state");
		return m_strText[eButtonState].m_aStringInfo;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	uint32 CGUIWgtButton::OnMouseEnter( CGUIEventMouse* pEvent )
	{
		m_bHovering = true;

		return CGUIWidget::OnMouseEnter( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtButton::OnMouseLeave( CGUIEventMouse* pEvent )
	{
		m_bHovering = false;

		return CGUIWidget::OnMouseLeave( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtButton::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		m_bPushing = true;

		return CGUIWidget::OnMouseLeftDown( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtButton::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		m_bPushing = false;

		return CGUIWidget::OnMouseLeftUp( pEvent );
	}
	//------------------------------------------------------------------------------

}//namespace guiex
