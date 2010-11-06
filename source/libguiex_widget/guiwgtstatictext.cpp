/** 
* @file guiwgtstaticimage.h
* @brief used to show a static image
* @author ken
* @date 2006-07-19
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtstatictext.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiinterfacemanager.h>
#include <libguiex_core\guiinterfacefont.h>
#include <libguiex_core\guistringconvertor.h>
#include <libguiex_core\guiproperty.h>
#include <libguiex_core\guipropertymanager.h>
#include <libguiex_core\guipropertyconvertor.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtStaticText);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtStaticText::ms_strType = "CGUIWgtStaticText";
	wchar_t		CGUIWgtStaticText::ms_wLineBreak = L'\n';
	//------------------------------------------------------------------------------
	CGUIWgtStaticText::CGUIWgtStaticText( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWgtStatic(ms_strType, rName, rProjectName)
	{
		InitStaticText();
	}
	//------------------------------------------------------------------------------
	CGUIWgtStaticText::CGUIWgtStaticText( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWgtStatic(rType, rName, rProjectName)
	{
		InitStaticText();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticText::InitStaticText()
	{
		m_bMultiLine = false;
		m_bAutoExpandHeight = false;

	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtStaticText::Create()
	{
		UpdateStringContent();
		return CGUIWgtStatic::Create();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticText::RenderSelf(IGUIInterfaceRender* pRender)
	{	
		if( m_strText.Empty())
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
	void	CGUIWgtStaticText::SetAutoExpandHeight( bool bAutoExpand )
	{
		m_bAutoExpandHeight = bAutoExpand;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtStaticText::IsAutoExpandHeight( ) const
	{
		return m_bAutoExpandHeight;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtStaticText::SetTextContent(const wchar_t* pText)
	{
		CGUIWgtStatic::SetTextContent( pText );

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

		real		fLineMaxWidth = NEWGetPixelSize().GetWidth();
		uint32		nLineWidth = 0;
		SLineInfo	aLine;
		aLine.m_nLength = 0;
		aLine.m_nStartIdx = 0; 
		aLine.m_nLineHeight = rDefaultInfo.m_nFontSize; 
		const wchar_t*	pContent = m_strText.GetContent();

		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		real	fTotalHeight = 0.f;
		for( uint32 i=0; i<m_strText.Size(); ++i)
		{
			if( pContent[i] == ms_wLineBreak )
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
					m_strText.GetInfo(i).m_nFontIdx,
					m_strText.GetCharacter(i), 
					m_strText.GetInfo(i).m_nFontSize);
				
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

		if( m_bAutoExpandHeight )
		{
			if( fTotalHeight > 0.f )
			{
				SetPixelSize( NEWGetPixelSize().m_fWidth, fTotalHeight );
			}
			else
			{
				SetPixelSize( NEWGetPixelSize().m_fWidth, rDefaultInfo.m_nFontSize );
			}
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtStaticText::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "multiline")
		{
			ValueToProperty( IsMultiLine(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "autoexpand_height" )
		{
			ValueToProperty( IsAutoExpandHeight(), rProperty);
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
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "autoexpand_height" )
		{
			bool bValue;
			PropertyToValue( rProperty, bValue);
			SetAutoExpandHeight( bValue );
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtStaticText::OnSizeChanged( CGUIEventSize* pEvent )
	{
		//UpdateStringContent();
		return CGUIWidget::OnSizeChanged(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex

