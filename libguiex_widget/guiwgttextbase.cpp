/** 
* @file guiwgttextbase.h
* @brief base class for widget who want to use text.
* @author ken
* @date 2011-04-01
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgttextbase.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guipropertyconvertor.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIWgtTextBase::CGUIWgtTextBase( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitTextBase();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTextBase::InitTextBase()
	{
		m_eTextAlignmentHorz = eTextAlignment_Horz_Center;
		m_eTextAlignmentVert = eTextAlignment_Vert_Center;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTextBase::SetTextContent(const CGUIStringW& rText)
	{
		m_strText.m_strContent = rText;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTextBase::SetTextInfo( const CGUIStringInfo& rInfo )
	{
		m_strText.m_aStringInfo = rInfo;
	}
	//------------------------------------------------------------------------------
	const CGUIStringW& CGUIWgtTextBase::GetTextContent() const
	{
		return m_strText.m_strContent;
	}
	//------------------------------------------------------------------------------
	const CGUIStringInfo& CGUIWgtTextBase::GetTextInfo( ) const
	{
		return m_strText.m_aStringInfo;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTextBase::SetTextContentUTF8( const CGUIString& rString)
	{
		CGUIStringW strTemp;
		AppMultiByteToWideChar( rString, strTemp);
		SetTextContent( strTemp );
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtTextBase::GetTextContentUTF8( ) const
	{
		CGUIString aContentUTF8;
		AppWideByteToMultiChar( m_strText.m_strContent, aContentUTF8 );
		return aContentUTF8;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTextBase::SetTextAlignmentVert( ETextAlignmentVert eAlignment )
	{
		m_eTextAlignmentVert = eAlignment;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTextBase::SetTextAlignmentHorz( ETextAlignmentHorz eAlignment )
	{
		m_eTextAlignmentHorz = eAlignment;
	}
	//------------------------------------------------------------------------------
	ETextAlignmentHorz CGUIWgtTextBase::GetTextAlignmentHorz( ) const
	{
		return m_eTextAlignmentHorz;
	}
	//------------------------------------------------------------------------------
	ETextAlignmentVert CGUIWgtTextBase::GetTextAlignmentVert( ) const
	{
		return m_eTextAlignmentVert;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTextBase::SetTextColor(const CGUIColor& rColor )
	{
		m_strText.m_aStringInfo.m_aColor = rColor;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtTextBase::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo" )
		{
			ValueToProperty( m_strText.GetStringInfo(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text" )
		{
			CGUIString aStrText;
			AppWideByteToMultiChar( m_strText.GetContent(), aStrText);
			rProperty.SetValue(aStrText);
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
	void CGUIWgtTextBase::ProcessProperty( const CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo")
		{
			CGUIStringInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetTextInfo(aInfo);
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text")
		{
			CGUIStringEx aStrText;
			AppMultiByteToWideChar(rProperty.GetValue(), aStrText.m_strContent);
			SetTextContent(aStrText.GetContent());
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
}//namespace guiex

