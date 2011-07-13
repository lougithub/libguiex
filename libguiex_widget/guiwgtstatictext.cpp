/** 
* @file guiwgtstaticimage.h
* @brief used to show a static image
* @author ken
* @date 2006-07-19
*/


//============================================================================//
// include 
//============================================================================// 
#include "guiwgtstatictext.h"
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
	wchar CGUIWgtStaticText::ms_wLineBreak = L'\n';
	//------------------------------------------------------------------------------
	CGUIWgtStaticText::CGUIWgtStaticText( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtTextBase( StaticGetType(), rName, rSceneName)
	{
		InitStaticText();
	}
	//------------------------------------------------------------------------------
	CGUIWgtStaticText::CGUIWgtStaticText( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtTextBase(rType, rName, rSceneName)
	{
		InitStaticText();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::InitStaticText()
	{
		m_bMultiLine = false;
		m_bClipText = false;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::OnCreate()
	{
		CGUIWgtTextBase::OnCreate();
		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::RefreshSelf( )
	{
		CGUIWgtTextBase::RefreshSelf();
		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::RenderSelf(IGUIInterfaceRender* pRender)
	{	
		if( m_strText.m_strContent.empty())
		{
			return;
		}

		if( IsClipText() && GetClipArea() )
		{
			pRender->PushClipRect( *GetClipArea() );
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

				aDestRect.m_fBottom = aDestRect.m_fTop + aLineInfo.m_fLineHeight * GetDerivedScale().m_fHeight;

				//no selection
				DrawString( pRender, m_strText, aDestRect, m_eTextAlignmentHorz, m_eTextAlignmentVert, aLineInfo.m_nStartIdx, aLineInfo.m_nStartIdx+aLineInfo.m_nLength );
				
				aDestRect.m_fTop = aDestRect.m_fBottom;
			}
		}
		else
		{
			DrawString( pRender, m_strText, GetClientArea(), m_eTextAlignmentHorz,m_eTextAlignmentVert);
		}

		if( IsClipText() && GetClipArea() )
		{
			pRender->PopClipRect( );
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
	void CGUIWgtStaticText::SetClipText( bool bClip )
	{
		m_bClipText = bClip;
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtStaticText::IsClipText( ) const
	{
		return m_bClipText;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::SetTextContent(const CGUIStringW& rText)
	{
		CGUIWgtTextBase::SetTextContent( rText );
		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::SetTextInfo( const CGUIStringRenderInfo& rInfo )
	{
		CGUIWgtTextBase::SetTextInfo( rInfo );
		UpdateStringContent();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStaticText::UpdateStringContent()
	{
		m_aLineList.clear();

		if( !m_bMultiLine )
		{
			return;
		}
			
		const CGUIStringRenderInfo& rDefaultInfo = GetTextInfo();
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		real fLineHeight = pFont->GetFontHeight(rDefaultInfo);

		real fLineMaxWidth = GetPixelSize().GetWidth();
		real fLineWidth = 0.0f;
		SLineInfo aLine;
		aLine.m_nLength = 0;
		aLine.m_nStartIdx = 0; 
		aLine.m_fLineHeight = fLineHeight; 

		real fTotalHeight = 0.f;
		for( uint32 i=0; i<m_strText.m_strContent.size(); ++i)
		{
			if( m_strText.m_strContent[i] == ms_wLineBreak )
			{
				//line break
				++aLine.m_nLength;
				m_aLineList.push_back(aLine);
				fTotalHeight += aLine.m_fLineHeight;
				aLine.m_nLength = 0;
				aLine.m_fLineHeight = fLineHeight;
				aLine.m_nStartIdx = i+1;
				fLineWidth = 0.0f;
				continue;
			}
			else
			{
				CGUISize aWordSize = pFont->GetCharacterSize(
					m_strText.m_strContent[i], 
					m_strText.m_aStringInfo);
				
				if( fLineWidth + aWordSize.m_fWidth > fLineMaxWidth)
				{
					//new line
					m_aLineList.push_back(aLine);
					fTotalHeight += aLine.m_fLineHeight;

					aLine.m_nLength = 1;
					aLine.m_fLineHeight = fLineHeight > aWordSize.m_fHeight ? fLineHeight : aWordSize.m_fHeight;
					aLine.m_nStartIdx = i;
					fLineWidth = aWordSize.m_fWidth;
				}
				else
				{
					//add a character to line
					++aLine.m_nLength;
					if( aWordSize.m_fHeight > aLine.m_fLineHeight)
					{
						aLine.m_fLineHeight = aWordSize.m_fHeight;
					}
					fLineWidth += aWordSize.m_fWidth;
				}
			}
		}
		if( aLine.m_nLength > 0)
		{
			m_aLineList.push_back(aLine);
			fTotalHeight += aLine.m_fLineHeight;
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtStaticText::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "multiline")
		{
			ValueToProperty( IsMultiLine(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "clip_text")
		{
			ValueToProperty( IsClipText(), rProperty);
		}
		else
		{
			return CGUIWgtTextBase::GenerateProperty( rProperty );
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
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "clip_text")
		{
			bool bValue;
			PropertyToValue( rProperty, bValue);
			SetClipText( bValue );
		}
		else
		{
			CGUIWgtTextBase::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtStaticText::OnSizeChanged( CGUIEventSize* pEvent )
	{
		UpdateStringContent();
		return CGUIWgtTextBase::OnSizeChanged(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex

