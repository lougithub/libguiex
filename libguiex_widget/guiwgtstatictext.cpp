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
		:CGUIWgtStatic( StaticGetType(), rName, rSceneName)
	{
		InitStaticText();
	}
	//------------------------------------------------------------------------------
	CGUIWgtStaticText::CGUIWgtStaticText( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtStatic(rType, rName, rSceneName)
	{
		InitStaticText();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticText::InitStaticText()
	{
		m_bMultiLine = false;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::OnCreate()
	{
		CGUIWgtStatic::OnCreate();

		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::RefreshSelf( )
	{
		CGUIWgtStatic::RefreshSelf();
		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticText::RenderSelf(IGUIInterfaceRender* pRender)
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
				DrawString( pRender, m_strText, aDestRect, GetTextAlignment(), aLineInfo.m_nStartIdx, aLineInfo.m_nStartIdx+aLineInfo.m_nLength );
				
				aDestRect.m_fTop = aDestRect.m_fBottom;
			}
		}
		else
		{
			DrawString( pRender, m_strText, GetClientArea(), GetTextAlignment());
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticText::SetMultiLine( bool bMultiLine )
	{
		m_bMultiLine = bMultiLine;
		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtStaticText::IsMultiLine( ) const
	{
		return m_bMultiLine;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticText::SetTextContent(const CGUIStringW& rText)
	{
		CGUIWgtStatic::SetTextContent( rText );

		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticText::SetTextInfo( const CGUIStringInfo& rInfo )
	{
		CGUIWgtStatic::SetTextInfo( rInfo );

		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtStaticText::UpdateStringContent()
	{
		m_aLineList.clear();

		if( !m_bMultiLine )
		{
			return;
		}
			
		const CGUIStringInfo& rDefaultInfo = GetTextInfo();

		real		fLineMaxWidth = GetPixelSize().GetWidth();
		uint32		nLineWidth = 0;
		SLineInfo	aLine;
		aLine.m_nLength = 0;
		aLine.m_nStartIdx = 0; 
		aLine.m_nLineHeight = rDefaultInfo.m_nFontSize; 

		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		real	fTotalHeight = 0.f;
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
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtStaticText::OnSizeChanged( CGUIEventSize* pEvent )
	{
		UpdateStringContent();
		return CGUIWidget::OnSizeChanged(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex

