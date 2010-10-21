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
			const CGUIRect& rStringClipRect = GetClientClipRect();
			CGUIRect aDestRect = GetClientRect();
			//
			for( TLineList::iterator itor = m_aLineList.begin();
				itor != m_aLineList.end();
				++itor)
			{
				const SLineInfo& aLineInfo = *itor;

				aDestRect.m_fBottom = aDestRect.m_fTop + aLineInfo.m_nLineHeight * GetScale().m_fHeight;

				//no selection
				DrawString( pRender, m_strText, aDestRect, GetTextAlignment(), &rStringClipRect, aLineInfo.m_nStartIdx, aLineInfo.m_nStartIdx+aLineInfo.m_nLength );
				
				aDestRect.m_fTop = aDestRect.m_fBottom;
			}
		}
		else
		{
			DrawString( pRender, m_strText, GetClientRect(), GetTextAlignment(), &GetClipRect());
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
	void	CGUIWgtStaticText::SetTextInfo( const CGUIStringExInfo& rInfo )
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
			
		const CGUIStringExInfo& rDefaultInfo = GetTextInfo();

		real		fLineMaxWidth = GetClientRect().GetWidth();
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
				SetSize( GetSize().m_fWidth, fTotalHeight );
			}
			else
			{
				SetSize( GetSize().m_fWidth, rDefaultInfo.m_nFontSize );
			}
		}
	}
	//------------------------------------------------------------------------------
	CGUIProperty*	CGUIWgtStaticText::GenerateProperty(const CGUIString& rName, const CGUIString& rType )
	{
		CGUIProperty* pProperty = NULL;

		if( rName == "MULTILINE" && rType=="BOOL")
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsMultiLine( )));
		}
		else if( rName == "AUTOEXPAND_HEIGHT" && rType=="BOOL")
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsAutoExpandHeight( )));
		}

		return pProperty ? pProperty : CGUIWidget::GenerateProperty(rName, rType);
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtStaticText::ProcessProperty( const CGUIProperty* pProperty)
	{
		CGUIWidget::ProcessProperty(pProperty);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for parent
		/*
		*<property name="SHOW_VERT_SCROLLBAR" type="BOOL" value="true" />
		*/
		if( pProperty->GetName() == "MULTILINE" && pProperty->GetType()=="BOOL")
		{
			SetMultiLine(CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
		else if( pProperty->GetName() == "AUTOEXPAND_HEIGHT" && pProperty->GetType()=="BOOL")
		{
			SetAutoExpandHeight(CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}

	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtStaticText::OnSizeChange( CGUIEventSize* pEvent )
	{
		//UpdateStringContent();
		return CGUIWidget::OnSizeChange(pEvent);
	}
}//namespace guiex

