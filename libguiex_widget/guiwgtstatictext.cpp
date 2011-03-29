/** 
* @file guiwgtstaticimage.h
* @brief used to show a static image
* @author ken
* @date 2006-07-19
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtstatictext.h>
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
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtStaticText);
	//------------------------------------------------------------------------------
	wchar_t CGUIWgtStaticText::ms_wLineBreak = L'\n';
	//------------------------------------------------------------------------------
	CGUIWgtStaticText::CGUIWgtStaticText( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget( StaticGetType(), rName, rSceneName)
	{
		InitStaticText();
	}
	//------------------------------------------------------------------------------
	CGUIWgtStaticText::CGUIWgtStaticText( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitStaticText();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::InitStaticText()
	{
		m_bMultiLine = false;
		m_eTextAlignmentHorz = eTextAlignment_Horz_Center;
		m_eTextAlignmentVert = eTextAlignment_Vert_Center;

		SetFocusable(false);
		SetActivable(false);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::OnCreate()
	{
		CGUIWidget::OnCreate();

		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::RefreshSelf( )
	{
		CGUIWidget::RefreshSelf();
		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::RenderSelf(IGUIInterfaceRender* pRender)
	{	
		if( m_strText.m_strContent.empty())
		{
			return;
		}
		if(m_bMultiLine)
		{
			////////////////////////////////////////////////////////////////////////////
			//render string
			CGUIRect aDestRect = GetClientArea();
			//
			for( TLineList::iterator itor = m_aLineList.begin();
				itor != m_aLineList.end();
				++itor)
			{
				const SLineInfo& aLineInfo = *itor;

				aDestRect.m_fBottom = aDestRect.m_fTop + aLineInfo.m_nLineHeight * GetDerivedScale().m_fHeight;

				//no selection
				DrawString( pRender, m_strText, aDestRect, m_eTextAlignmentHorz,m_eTextAlignmentVert, aLineInfo.m_nStartIdx, aLineInfo.m_nStartIdx+aLineInfo.m_nLength );
				
				aDestRect.m_fTop = aDestRect.m_fBottom;
			}
		}
		else
		{
			DrawString( pRender, m_strText, GetClientArea(), m_eTextAlignmentHorz,m_eTextAlignmentVert);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::SetMultiLine( bool bMultiLine )
	{
		m_bMultiLine = bMultiLine;
		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtStaticText::IsMultiLine( ) const
	{
		return m_bMultiLine;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::SetTextContent(const CGUIStringW& rText)
	{
		m_strText.m_strContent = rText;
		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::SetTextInfo( const CGUIStringInfo& rInfo )
	{
		m_strText.m_aStringInfo = rInfo;
		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	const CGUIStringW& CGUIWgtStaticText::GetTextContent() const
	{
		return m_strText.m_strContent;
	}
	//------------------------------------------------------------------------------
	const CGUIStringInfo& CGUIWgtStaticText::GetTextInfo( ) const
	{
		return m_strText.m_aStringInfo;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::SetTextContentUTF8( const CGUIString& rString)
	{
		CGUIStringW strTemp;
		MultiByteToWideChar( rString, strTemp);
		SetTextContent( strTemp );
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtStaticText::GetTextContentUTF8( ) const
	{
		CGUIString aContentUTF8;
		WideByteToMultiChar( m_strText.m_strContent, aContentUTF8 );
		return aContentUTF8;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::SetTextAlignmentVert( ETextAlignmentVert eAlignment )
	{
		m_eTextAlignmentVert = eAlignment;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::SetTextAlignmentHorz( ETextAlignmentHorz eAlignment )
	{
		m_eTextAlignmentHorz = eAlignment;
	}
	//------------------------------------------------------------------------------
	ETextAlignmentHorz CGUIWgtStaticText::GetTextAlignmentHorz( ) const
	{
		return m_eTextAlignmentHorz;
	}
	//------------------------------------------------------------------------------
	ETextAlignmentVert CGUIWgtStaticText::GetTextAlignmentVert( ) const
	{
		return m_eTextAlignmentVert;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::SetTextColor(const CGUIColor& rColor )
	{
		m_strText.m_aStringInfo.m_aColor = rColor;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::UpdateStringContent()
	{
		m_aLineList.clear();

		if( !m_bMultiLine )
		{
			return;
		}
			
		const CGUIStringInfo& rDefaultInfo = GetTextInfo();

		real fLineMaxWidth = GetPixelSize().GetWidth();
		uint32 nLineWidth = 0;
		SLineInfo aLine;
		aLine.m_nLength = 0;
		aLine.m_nStartIdx = 0; 
		aLine.m_nLineHeight = rDefaultInfo.m_nFontSize; 

		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		real fTotalHeight = 0.f;
		for( uint32 i=0; i<m_strText.m_strContent.size(); ++i)
		{
			if( m_strText.m_strContent[i] == ms_wLineBreak )
			{
				//line break
				++aLine.m_nLength;
				m_aLineList.push_back(aLine);
				fTotalHeight += aLine.m_nLineHeight;
				aLine.m_nLength = 0;
				aLine.m_nLineHeight = rDefaultInfo.m_nFontSize;
				aLine.m_nStartIdx = i+1;
				nLineWidth = 0;
				continue;
			}
			else
			{
				CGUISize aWordSize = pFont->GetCharacterSize(
					m_strText.m_aStringInfo.m_nFontIdx,
					m_strText.m_strContent[i], 
					m_strText.m_aStringInfo.m_nFontSize);
				
				if( nLineWidth + aWordSize.m_fWidth > fLineMaxWidth)
				{
					//new line
					m_aLineList.push_back(aLine);
					fTotalHeight += aLine.m_nLineHeight;

					aLine.m_nLength = 1;
					aLine.m_nLineHeight = GUI_FLOAT2UINT_ROUND( rDefaultInfo.m_nFontSize > aWordSize.m_fHeight ? rDefaultInfo.m_nFontSize : aWordSize.m_fHeight );
					aLine.m_nStartIdx = i;
					nLineWidth = GUI_FLOAT2UINT_ROUND(aWordSize.m_fWidth);
				}
				else
				{
					//add a character to line
					++aLine.m_nLength;
					if( GUI_FLOAT2UINT_ROUND(aWordSize.m_fHeight) > aLine.m_nLineHeight)
					{
						aLine.m_nLineHeight = GUI_FLOAT2UINT_ROUND(aWordSize.m_fHeight);
					}
					nLineWidth += GUI_FLOAT2UINT_ROUND(aWordSize.m_fWidth);
				}
			}
		}
		if( aLine.m_nLength > 0)
		{
			m_aLineList.push_back(aLine);
			fTotalHeight += aLine.m_nLineHeight;
		}

	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtStaticText::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "multiline")
		{
			ValueToProperty( IsMultiLine(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo" )
		{
			ValueToProperty( m_strText.GetStringInfo(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text" )
		{
			CGUIString aStrText;
			WideByteToMultiChar( m_strText.GetContent(), aStrText);
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
	void CGUIWgtStaticText::ProcessProperty( const CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "multiline")
		{
			bool bValue;
			PropertyToValue( rProperty, bValue);
			SetMultiLine( bValue );
		}
		else if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo")
		{
			CGUIStringInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetTextInfo(aInfo);
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text")
		{
			CGUIStringEx aStrText;
			MultiByteToWideChar(rProperty.GetValue(), aStrText.m_strContent);
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
	uint32 CGUIWgtStaticText::OnSizeChanged( CGUIEventSize* pEvent )
	{
		UpdateStringContent();
		return CGUIWidget::OnSizeChanged(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex

