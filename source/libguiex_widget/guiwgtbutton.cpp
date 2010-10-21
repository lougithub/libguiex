/** 
* @file guiwgtbutton.cpp
* @brief widget: button
* @author ken
* @date 2006-06-08
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtbutton.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guiproperty.h>
#include <libguiex_core\guipropertymanager.h>
#include <libguiex_core\guistringconvertor.h>

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
	CGUIWgtButton::CGUIWgtButton(const CGUIString& rName, const CGUIString& rProjectName)
		:CGUIWidget(ms_strType, rName, rProjectName)
		,m_aRelativePos(0.f,0.f)
	{
		InitButton();
	}
	//------------------------------------------------------------------------------
	CGUIWgtButton::CGUIWgtButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
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
		SetFlag(eFLAG_EVENT_CLICK, true);
		SetFlag(eFLAG_OPEN_WITH_PARENT, true);
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtButton::Create()
	{
		return CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtButton::OnSetImage( const CGUIString& rName,CGUIImage* pImage )
	{
		if( rName == "BTN_NORMAL")
		{
			m_pImageNormal = pImage;
			if( GetSize().IsEqualZero() && pImage )
			{
				SetSize(pImage->GetSize());
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
	void	CGUIWgtButton::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIImage* pImage = NULL;
		CGUIStringEx* pString = NULL;

		if( IsDisable())
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

		DrawImage( pRender, pImage, GetRect(), pRender->GetAndIncZ(),&GetClipRect());
		if( m_bHovering && m_pImageHoverOverlay )
		{
			DrawImage( pRender, m_pImageHoverOverlay, GetRect(), pRender->GetAndIncZ(),&GetClipRect());
		}
		
		CGUIRect rDrawRect = GetClientRect();
		rDrawRect.m_fTop += (m_aRelativePos.y*GetScale().m_fHeight);
		rDrawRect.m_fLeft += (m_aRelativePos.x*GetScale().m_fWidth);
		DrawString(pRender, *pString, rDrawRect, GetTextAlignment(), &GetClipRect());
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
	void	CGUIWgtButton::SetBtnTextContent(const wchar_t* pText, const char* pType)
	{
		GUI_ASSERT(pType, "invalid parameter");

		if( CGUIString("NORMAL") == pType)
		{
			m_strText.SetContent(pText);
		}
		else if( CGUIString("HOVER") == pType)
		{
			m_strTextHoving.SetContent(pText);
		}
		else if( CGUIString("DISABLE") == pType)
		{
			m_strTextDisable.SetContent(pText);
		}
		else if( CGUIString("PUSH") == pType)
		{
			m_strTextPush.SetContent(pText);
		}
		else
		{
			throw CGUIException("unknown type of text! <%s>", pType);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtButton::SetBtnTextInfo(const CGUIStringExInfo& rInfo, const char* pType)
	{
		GUI_ASSERT(pType, "invalid parameter");
		if( CGUIString("NORMAL") == pType)
		{
			m_strText.SetDefaultInfo(&rInfo);
		}
		else if( CGUIString("HOVER") == pType)
		{
			m_strTextHoving.SetDefaultInfo(&rInfo);
		}
		else if( CGUIString("DISABLE") == pType)
		{
			m_strTextDisable.SetDefaultInfo(&rInfo);
		}
		else if( CGUIString("PUSH") == pType)
		{
			m_strTextPush.SetDefaultInfo(&rInfo);
		}
		else
		{
			throw CGUIException("unknown type of text! <%s>", pType);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtButton::SetRelativePos( const CGUIVector2& rPos)
	{
		m_aRelativePos = rPos;
	}
	//------------------------------------------------------------------------------
	const CGUIVector2&	CGUIWgtButton::GetRelativePos(  ) const
	{
		return m_aRelativePos;
	}
	//------------------------------------------------------------------------------
	CGUIProperty*	CGUIWgtButton::GenerateProperty(const CGUIString& rName, const CGUIString& rType )
	{
		CGUIProperty* pProperty = NULL;
		
		if( rName == "TEXT_INFO_HOVER" && rType == "STRING_INFO" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);

			const CGUIStringExInfo& rInfo = m_strTextHoving.GetDefaultInfo();
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("SIZE","UINT",CGUIStringConvertor::UIntToString(rInfo.m_nFontSize)));
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("FONT_INDEX","UINT",CGUIStringConvertor::UIntToString(rInfo.m_nFontIdx)));
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("COLOR","COLOR",CGUIStringConvertor::ColorToString(rInfo.m_aColor)));
		}
		if( rName == "TEXT_INFO_DISABLE" && rType == "STRING_INFO" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);

			const CGUIStringExInfo& rInfo = m_strTextDisable.GetDefaultInfo();
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("SIZE","UINT",CGUIStringConvertor::UIntToString(rInfo.m_nFontSize)));
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("FONT_INDEX","UINT",CGUIStringConvertor::UIntToString(rInfo.m_nFontIdx)));
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("COLOR","COLOR",CGUIStringConvertor::ColorToString(rInfo.m_aColor)));
		}
		if( rName == "TEXT_INFO_PUSH" && rType == "STRING_INFO" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);

			const CGUIStringExInfo& rInfo = m_strTextPush.GetDefaultInfo();
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("SIZE","UINT",CGUIStringConvertor::UIntToString(rInfo.m_nFontSize)));
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("FONT_INDEX","UINT",CGUIStringConvertor::UIntToString(rInfo.m_nFontIdx)));
			pProperty->AddProperty(CGUIPropertyManager::Instance()->CreateProperty("COLOR","COLOR",CGUIStringConvertor::ColorToString(rInfo.m_aColor)));
		}
		else if( rName == "TEXT_NORMAL" && rType == "STRING" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);
			CGUIString aStrText;
			CGUIStringConvertor::WideByteToMultiChar( m_strText.GetContent(), aStrText);
			pProperty->SetValue(aStrText);
		}
		else if( rName == "TEXT_HOVER" && rType == "STRING" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);
			CGUIString aStrText;
			CGUIStringConvertor::WideByteToMultiChar( m_strTextHoving.GetContent(), aStrText);
			pProperty->SetValue(aStrText);
		}
		else if( rName == "TEXT_Push" && rType == "STRING" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);
			CGUIString aStrText;
			CGUIStringConvertor::WideByteToMultiChar( m_strTextPush.GetContent(), aStrText);
			pProperty->SetValue(aStrText);
		}
		else if( rName == "TEXT_DISABLE" && rType == "STRING" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			pProperty->SetName(rName);
			pProperty->SetType(rType);
			CGUIString aStrText;
			CGUIStringConvertor::WideByteToMultiChar( m_strTextDisable.GetContent(), aStrText);
			pProperty->SetValue(aStrText);
		}
		else if( rName == "RELATIVE_POSITION" && rType == "VECTOR2" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::Vector2ToString(GetRelativePos()));
		}
		return pProperty ? pProperty : CGUIWidget::GenerateProperty(rName, rType);
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtButton::ProcessProperty( const CGUIProperty* pProperty)
	{
		CGUIWidget::ProcessProperty(pProperty);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for text
		/*
		*<property name="TEXT_INFO_NORMAL" type="STRING_INFO">
		*		<property name="SIZE"	type="NUMBER" value="22"/>
		*		<property name="FONT_INDEX" type="NUMBER" value="0"/>
		*		<property name="COLOR" type="COLOR" value="0.5,0.0,0.6,1.0"/>
		*</property>
		*<property name="TEXT_NORMAL" type="STRING">

		*<property name="TEXT_INFO_HOVER" type="STRING_INFO">
		*		<property name="SIZE"	type="NUMBER" value="22"/>
		*		<property name="FONT_INDEX" type="NUMBER" value="0"/>
		*		<property name="COLOR" type="COLOR" value="0.5,0.0,0.6,1.0"/>
		*</property>
		*<property name="TEXT_HOVER" type="STRING">

		*<property name="TEXT_INFO_DISABLE" type="STRING_INFO">
		*		<property name="SIZE"	type="NUMBER" value="22"/>
		*		<property name="FONT_INDEX" type="NUMBER" value="0"/>
		*		<property name="COLOR" type="COLOR" value="0.5,0.0,0.6,1.0"/>
		*</property>
		*<property name="TEXT_DISABLE" type="STRING">

		*<property name="TEXT_INFO_PUSH" type="STRING_INFO">
		*		<property name="SIZE"	type="NUMBER" value="22"/>
		*		<property name="FONT_INDEX" type="NUMBER" value="0"/>
		*		<property name="COLOR" type="COLOR" value="0.5,0.0,0.6,1.0"/>
		*</property>
		*<property name="TEXT_PUSH" type="STRING">
		*/
		if( pProperty->GetName() == "TEXT_INFO_HOVER" && pProperty->GetType()=="STRING_INFO")
		{
			const CGUIProperty* pPropertySize = pProperty->GetProperty("SIZE");
			const CGUIProperty* pPropertyIdx = pProperty->GetProperty("FONT_INDEX");
			const CGUIProperty* pPropertyColor = pProperty->GetProperty("COLOR");
			CGUIStringExInfo aInfo;
			aInfo.m_aColor = CGUIStringConvertor::StringToColor(pPropertyColor->GetValue());
			aInfo.m_nFontIdx = CGUIStringConvertor::StringToInt(pPropertyIdx->GetValue());
			aInfo.m_nFontSize = CGUIStringConvertor::StringToUInt(pPropertySize->GetValue());
			SetBtnTextInfo(aInfo, "HOVER");
		}
		else if( pProperty->GetName() == "TEXT_HOVER" && pProperty->GetType()=="STRING")
		{
			CGUIStringEx aStrText;
			CGUIStringConvertor::MultiByteToWideChar(pProperty->GetValue(), aStrText);
			SetBtnTextContent(aStrText.GetContent(),"HOVER");
		}
		else if( pProperty->GetName() == "TEXT_INFO_DISABLE" && pProperty->GetType()=="STRING_INFO")
		{
			const CGUIProperty* pPropertySize = pProperty->GetProperty("SIZE");
			const CGUIProperty* pPropertyIdx = pProperty->GetProperty("FONT_INDEX");
			const CGUIProperty* pPropertyColor = pProperty->GetProperty("COLOR");
			CGUIStringExInfo aInfo;
			aInfo.m_aColor = CGUIStringConvertor::StringToColor(pPropertyColor->GetValue());
			aInfo.m_nFontIdx = CGUIStringConvertor::StringToInt(pPropertyIdx->GetValue());
			aInfo.m_nFontSize = CGUIStringConvertor::StringToUInt(pPropertySize->GetValue());
			SetBtnTextInfo(aInfo, "DISABLE");
		}
		else if( pProperty->GetName() == "TEXT_DISABLE" && pProperty->GetType()=="STRING")
		{
			CGUIStringEx aStrText;
			CGUIStringConvertor::MultiByteToWideChar(pProperty->GetValue(), aStrText);
			SetBtnTextContent(aStrText.GetContent(),"DISABLE");
		}
		else if( pProperty->GetName() == "TEXT_INFO_PUSH" && pProperty->GetType()=="STRING_INFO")
		{
			const CGUIProperty* pPropertySize = pProperty->GetProperty("SIZE");
			const CGUIProperty* pPropertyIdx = pProperty->GetProperty("FONT_INDEX");
			const CGUIProperty* pPropertyColor = pProperty->GetProperty("COLOR");
			CGUIStringExInfo aInfo;
			aInfo.m_aColor = CGUIStringConvertor::StringToColor(pPropertyColor->GetValue());
			aInfo.m_nFontIdx = CGUIStringConvertor::StringToInt(pPropertyIdx->GetValue());
			aInfo.m_nFontSize = CGUIStringConvertor::StringToUInt(pPropertySize->GetValue());
			SetBtnTextInfo(aInfo, "PUSH");
		}
		else if( pProperty->GetName() == "TEXT_PUSH" && pProperty->GetType()=="STRING")
		{
			CGUIStringEx aStrText;
			CGUIStringConvertor::MultiByteToWideChar(pProperty->GetValue(), aStrText);
			SetBtnTextContent(aStrText.GetContent(),"PUSH");
		}
		else if( pProperty->GetName() == "RELATIVE_POSITION" && pProperty->GetType() == "VECTOR2" )
		{
			SetRelativePos(CGUIStringConvertor::StringToVector2(pProperty->GetValue()));
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
