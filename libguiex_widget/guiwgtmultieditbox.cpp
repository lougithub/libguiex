/** 
* @file guiwgtmultieditbox.cpp
* @brief multi lineeditbox used in this system
* @author ken
* @date 2007-06-27
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtmultieditbox.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guiinterfacekeyboard.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guimousecursor.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiwidgetmanager.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <numeric>
#include <algorithm>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtMultiEditBox);
	//------------------------------------------------------------------------------
	wchar_t CGUIWgtMultiEditBox::ms_wLineBreak = L'\n';
	//------------------------------------------------------------------------------
	CGUIWgtMultiEditBox::CGUIWgtMultiEditBox(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWgtScrollbarContainer(StaticGetType(), rName, rSceneName)
	{
		InitMultiEditbox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtMultiEditBox::CGUIWgtMultiEditBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtScrollbarContainer(rType, rName, rSceneName)
	{
		InitMultiEditbox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtMultiEditBox::~CGUIWgtMultiEditBox( )
	{
		m_pEdit->SetParent(NULL);
		delete m_pEdit;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::InitMultiEditbox()
	{
		m_pEdit = new CGUIWgtEdit(GetName() + "__EDIT" + GUI_INTERNAL_WIDGET_FLAG, GetSceneName());
		m_pEdit->SetParent(this);

		SetFocusable(true);
		m_nMaxString = 100;		///< max number of string
		m_nCursorIdx = 0;			///< cursor's position in edited string, the first is 0.
		m_nCursorLine = 0;
		m_bReadOnly = false;

		//size ratio
		m_aStringAreaRatio.m_fLeft = m_aStringAreaRatio.m_fTop = 0.0f;
		m_aStringAreaRatio.m_fRight = m_aStringAreaRatio.m_fBottom = 1.0f;

		//for drag
		m_nSelectionStart = 0;
		m_nSelectionEnd = 0;
		m_bDraging = false;
		m_nDragAnchorIdx = 0;

		m_pBG = NULL;
		m_pBGFocus = NULL;
		m_pCursor = NULL;

		//text
		m_eTextAlignmentHorz = eTextAlignment_Horz_Left;
		m_eTextAlignmentVert = eTextAlignment_Vert_Center;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "bg")
		{
			m_pBG = pImage;
			if( GetSize().IsEqualZero() && pImage )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "bg_focus")
		{
			m_pBGFocus = pImage;
		}
		else if( rName == "cursor")
		{
			m_pCursor = pImage;
			if( GetCursorSize().IsEqualZero() && pImage)
			{
				SetCursorSize(pImage->GetSize());
			}
		}
		else 
		{
			CGUIWgtScrollbarContainer::OnSetImage(rName, pImage );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::RefreshSelf()
	{
		CGUIWgtScrollbarContainer::RefreshSelf();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::UpdateClientArea(void)
	{
		CGUIWgtScrollbarContainer::UpdateClientArea( );

		// calculate line height and reset client area size.
		real fHeight = 0.0f;
		for( TLineList::iterator itor= m_aLineList.begin();
			itor != m_aLineList.end();
			++itor)
		{
			fHeight += (*itor).m_fLineHeight;
		}
		m_aClientArea.SetHeight(fHeight);
		m_aClientArea.SetWidth( GetClipArea().GetWidth() );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIWgtScrollbarContainer::RenderSelf(pRender);

		////////////////////////////////////////////////////////////////////////////
		//render bg
		if( m_pBG )
		{
			DrawImage( pRender, m_pBG, GetBoundArea());
		}

		//render focus bg 
		if( m_pBGFocus && IsFocus())
		{
			DrawImage( pRender, m_pBGFocus, GetBoundArea());
		}


		////////////////////////////////////////////////////////////////////////////
		//render cursor
		if( m_pCursor && IsFocus() )
		{
			//render
			if( m_pEdit->IsShowCursor() )
			{
				DrawImage( pRender, m_pCursor, GetCursorRect());
			}
		}

		////////////////////////////////////////////////////////////////////////////
		//render string
		CGUIVector2 aPos = GetClientArea().GetPosition();
		if( !m_strText.m_strContent.empty())
		{
			if( GetSelectionLength())
			{
				/* 
				the text is divided to three section
				xxx yyy zzz
				first <xxx> is not selected 
				second <yyy> is selected
				last <zzz> is not selected
				*/

				int16 nDrawState = 0;	//0 == draw first <xxx>, 1 == draw second <yyy>, 2 == draw last <zzz> 
				CGUIStringRenderInfo aStringInfo = m_strText.GetStringInfo();
				CGUIColor aDefaultColor = aStringInfo.m_aColor;
				uint32 nCurIdx = 0;

				//has selection
				for( TLineList::iterator itor = m_aLineList.begin();
					itor != m_aLineList.end();
					++itor)
				{
					const SLineInfo& aLineInfo = *itor;
					aPos.x = GetClientArea().m_fLeft;
					aPos.y += aLineInfo.m_fLineHeight;

					for( uint32 i=0; i<aLineInfo.m_nLength; ++i)
					{
						nCurIdx = i + aLineInfo.m_nStartIdx;
						if( nDrawState == 0 )
						{
							//draw first <xxx>
							if( nCurIdx >= m_nSelectionStart )
							{
								nDrawState = 1;
								aStringInfo.m_aColor = m_aSelectedTextColor;
							}
						}
						else if( nDrawState ==1 )
						{
							//draw second <yyy>
							if( nCurIdx >= m_nSelectionEnd )
							{
								nDrawState = 2;
								aStringInfo.m_aColor = aDefaultColor;
							}
						}
						else
						{
							//draw last <zzz>
						}
						DrawCharacter(pRender, m_strText.m_strContent[nCurIdx],aStringInfo,aPos);
						aPos.x+=m_vecStringSize[nCurIdx].m_fWidth;
					}
				}
			}
			else
			{
				//no selection
				for( TLineList::iterator itor = m_aLineList.begin();
					itor != m_aLineList.end();
					++itor)
				{
					const SLineInfo& aLineInfo = *itor;
					aPos.y += aLineInfo.m_fLineHeight;

					//no selection
					DrawString( pRender, m_strText, aPos, aLineInfo.m_nStartIdx, aLineInfo.m_nStartIdx+aLineInfo.m_nLength );
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::UpdateSelf( real fDeltaTime )
	{
		if( IsFocus())
		{
			if( wcslen(m_pEdit->GetResult()) )
			{
				//TODO: only test here
				InsertString(m_pEdit->GetResult());
				m_pEdit->ClearResult();
			}
		}

		CGUIWidget::UpdateSelf( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetTextContent(const CGUIStringW& rText)
	{
		ClearSelection( );
		DeleteString(0, -1);
		InsertString( rText );
	}
	//------------------------------------------------------------------------------
	const CGUIStringW& CGUIWgtMultiEditBox::GetTextContent( ) const
	{
		return m_strText.m_strContent;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetTextInfo(const CGUIStringRenderInfo& rInfo)
	{
		m_strText.m_aStringInfo = rInfo;
	}
	//------------------------------------------------------------------------------
	const CGUIStringRenderInfo& CGUIWgtMultiEditBox::GetTextInfo( ) const
	{
		return m_strText.m_aStringInfo;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetTextContentUTF8( const CGUIString& rString)
	{
		CGUIStringW strTemp;
		AppMultiByteToWideChar( rString, strTemp);
		SetTextContent( strTemp );
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtMultiEditBox::GetTextContentUTF8( ) const
	{
		CGUIString aContentUTF8;
		AppWideByteToMultiChar( m_strText.m_strContent, aContentUTF8 );
		return aContentUTF8;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetTextAlignmentVert( ETextAlignmentVert eAlignment )
	{
		m_eTextAlignmentVert = eAlignment;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetTextAlignmentHorz( ETextAlignmentHorz eAlignment )
	{
		m_eTextAlignmentHorz = eAlignment;
	}
	//------------------------------------------------------------------------------
	ETextAlignmentHorz CGUIWgtMultiEditBox::GetTextAlignmentHorz( ) const
	{
		return m_eTextAlignmentHorz;
	}
	//------------------------------------------------------------------------------
	ETextAlignmentVert CGUIWgtMultiEditBox::GetTextAlignmentVert( ) const
	{
		return m_eTextAlignmentVert;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetSelectedTextColor( const CGUIColor& rColor)
	{
		m_aSelectedTextColor = rColor;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetReadOnly(bool bRead)
	{
		m_bReadOnly = bRead;
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtMultiEditBox::IsReadOnly() const
	{
		return m_bReadOnly;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtMultiEditBox::InsertString( const CGUIStringW& rText )
	{
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		uint32 len = rText.size();
		if( m_strText.m_strContent.size() + len > m_nMaxString-1 )
		{
			//reach max number of string
			return;
		}

		if( m_strText.m_strContent.empty())
		{
			//add a line break at last if the string is empty
			m_strText.m_strContent.append( 1, ms_wLineBreak );
			m_vecStringSize.insert(
				m_vecStringSize.begin(), 
				pFont->GetCharacterSize( ms_wLineBreak, m_strText.GetStringInfo()));
		}

		m_strText.m_strContent.insert(m_nCursorIdx, rText );
		for( size_t i=0; i<len; ++i)
		{
			m_vecStringSize.insert(
				m_vecStringSize.begin()+m_nCursorIdx+i, 
				pFont->GetCharacterSize( rText[i], m_strText.GetStringInfo()));
		}

		//format text
		FormatText();

		//set cursor index
		SetCursorIndex(m_nCursorIdx + len);
	}
	//------------------------------------------------------------------------------
	const CGUISize&	CGUIWgtMultiEditBox::GetCursorSize() const
	{
		return m_pEdit->GetCursorSize();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetCursorSize( const CGUISize& rSize )
	{
		m_pEdit->SetCursorSize(rSize);
	}
	//------------------------------------------------------------------------------
	CGUIVector2	CGUIWgtMultiEditBox::GetCursorPos()
	{
		real fWidth = 0.0f;
		real fHeight = 0.0f;

		if( !m_aLineList.empty())
		{
			//calculate height
			for( int32 i=0; i<=m_nCursorLine; ++i )
			{
				fHeight += m_aLineList[i].m_fLineHeight;
			}

			//calculate width
			for( uint32 i=0; i<m_nCursorIdx - m_aLineList[m_nCursorLine].m_nStartIdx; ++i)
			{
				fWidth += m_vecStringSize[m_aLineList[m_nCursorLine].m_nStartIdx+i].m_fWidth;
			}
		}
		else
		{
			IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
			fHeight = pFont->GetFontHeight(m_strText.GetStringInfo());
		}

		CGUIVector2 aPos = GetClientArea( ).GetPosition();
		aPos.x = aPos.x + fWidth;
		aPos.y = aPos.y + fHeight - m_pEdit->GetCursorSize().GetHeight();

		return aPos;
	}
	//------------------------------------------------------------------------------
	CGUIRect CGUIWgtMultiEditBox::GetCursorRect()
	{
		return CGUIRect(GetCursorPos(), m_pEdit->GetCursorSize());
	}
	//------------------------------------------------------------------------------
	real CGUIWgtMultiEditBox::GetStringWidth(int32 nBeginPos, int32 nEndPos) const
	{
		nBeginPos = static_cast<int32>(nBeginPos<0?0:nBeginPos);
		nEndPos = static_cast<int32>(nEndPos<0?m_strText.m_strContent.size():nEndPos);
		nEndPos = static_cast<int32>(nEndPos>static_cast<int32>(m_strText.m_strContent.size())?m_strText.m_strContent.size():nEndPos);
		if( nBeginPos >= nEndPos)
		{
			return 0.0f;
		}
		real fWidth = 0.0f;
		for( std::vector<CGUISize>::const_iterator itor = m_vecStringSize.begin()+nBeginPos;
			itor != m_vecStringSize.begin()+nEndPos;
			++itor)
		{
			fWidth += (*itor).m_fWidth;
		}
		return fWidth;
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtMultiEditBox::DeleteString( int32 nBeginPos, int32 nEndPos)
	{
		if( m_strText.m_strContent.empty())
		{
			return;
		}

		nBeginPos = static_cast<int32>(nBeginPos<0?0:nBeginPos);
		uint32 nTextSize = m_strText.m_strContent.size()-1;
		nEndPos = static_cast<int32>(nEndPos<0?nTextSize:nEndPos);
		nEndPos = static_cast<int32>(nEndPos>static_cast<int32>(nTextSize)?nTextSize:nEndPos);
		if( nBeginPos >= nEndPos)
		{
			return;
		}

		//erase string
		m_strText.m_strContent.erase( nBeginPos, nEndPos);
		m_vecStringSize.erase( m_vecStringSize.begin()+nBeginPos, m_vecStringSize.begin()+nEndPos);


		//format text
		FormatText();

		//update cursor position
		if( m_nCursorIdx>nEndPos )
		{
			SetCursorIndex(m_nCursorIdx - (nEndPos-nBeginPos));
		}
		else if ( m_nCursorIdx>nBeginPos )
		{
			SetCursorIndex(m_nCursorIdx - (m_nCursorIdx-nBeginPos));
		}
		else
		{
			SetCursorIndex(m_nCursorIdx);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetCursorIndex( int32 nIdx, int32 nForceLineIdx )
	{
		if( m_strText.m_strContent.empty())
		{
			nIdx = 0;
		}
		else
		{
			uint32 nTextSize = m_strText.m_strContent.size()-1;
			nIdx = static_cast<int32>(nIdx <0?nTextSize:nIdx);
			nIdx = static_cast<int32>(nIdx > static_cast<int32>(nTextSize)?nTextSize:nIdx);
		}

		m_nCursorIdx = nIdx;

		//update cursor line number
		if( nForceLineIdx != -1)
		{
			m_nCursorLine = nForceLineIdx;
		}
		else
		{
			m_nCursorLine = GetLineNumberFromIndex(m_nCursorIdx);
		}

		//set cursor position for edit
		m_pEdit->SetCursorPos(GetCursorPos());
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetStringAreaRatio(const CGUIRect& rStringAreaRatio)
	{
		m_aStringAreaRatio = rStringAreaRatio;
//		SetRectDirty();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::FormatText()
	{
		real fOldClientRectWidth = GetClientArea().GetWidth();
		FormatText_Imp();
//		SetRectDirty();
		UpdateScrollbars();
		real fNewClientRectWidth = GetClientArea().GetWidth();

		if( !GUI_REAL_EQUAL( fNewClientRectWidth, fOldClientRectWidth))
		{
			//vertical scrollbar changed
			FormatText_Imp();
//			SetRectDirty();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::FormatText_Imp()
	{
		// clear old formatting data
		m_aLineList.clear();

		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		real fLineMaxWidth = GetClientArea().GetWidth();
		real fLineWidth = 0.0f;
		real fLineHeight = pFont->GetFontHeight(m_strText.GetStringInfo());
		SLineInfo aLine;
		aLine.m_nLength = 0;
		aLine.m_nStartIdx = 0; 
		aLine.m_fLineHeight = fLineHeight; 

		for( uint32 i=0; i<m_vecStringSize.size(); ++i)
		{
			if( m_strText.m_strContent[i] == ms_wLineBreak )
			{
				//line break
				++aLine.m_nLength;
				m_aLineList.push_back(aLine);
				aLine.m_nLength = 0;
				aLine.m_fLineHeight = fLineHeight;
				aLine.m_nStartIdx = i+1;
				fLineWidth = 0.0f;
				continue;
			}
			else
			{
				if( fLineWidth + m_vecStringSize[i].m_fWidth > fLineMaxWidth)
				{
					//new line
					m_aLineList.push_back(aLine);
					aLine.m_nLength = 1;
					aLine.m_fLineHeight = fLineHeight>m_vecStringSize[i].m_fHeight?fLineHeight:m_vecStringSize[i].m_fHeight;
					aLine.m_nStartIdx = i;
					fLineWidth = m_vecStringSize[i].m_fWidth;
				}
				else
				{
					//add a character to line
					++aLine.m_nLength;
					if( m_vecStringSize[i].m_fHeight > aLine.m_fLineHeight)
					{
						aLine.m_fLineHeight = m_vecStringSize[i].m_fHeight;
					}
					fLineWidth += m_vecStringSize[i].m_fWidth;
				}
			}
		}
		if( aLine.m_nLength > 0)
		{
			m_aLineList.push_back(aLine);
		}

	
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::ClearSelection()
	{
		// perform action only if required.
		if (GetSelectionLength() != 0)
		{
			SetSelection(0, 0);
		}
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtMultiEditBox::GetSelectionLength(void) const
	{
		return m_nSelectionEnd - m_nSelectionStart;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::SetSelection(size_t start_pos, size_t end_pos)
	{
		if( m_strText.m_strContent.empty())
		{
			start_pos = 0;
			end_pos = 0;
		}
		else
		{
			uint32 nTextSize = m_strText.m_strContent.size()-1;
			if (start_pos > nTextSize)
			{
				start_pos = nTextSize;
			}

			if (end_pos > nTextSize)
			{
				end_pos = nTextSize;
			}
		}

		// ensure start is before end
		if (start_pos > end_pos)
		{
			std::swap(start_pos,end_pos );
		}

		// only change state if values are different.
		if ((start_pos != m_nSelectionStart) || (end_pos != m_nSelectionEnd))
		{
			// setup selection
			m_nSelectionStart = start_pos;
			m_nSelectionEnd	 = end_pos;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::EraseSelectedText( )
	{
		if (GetSelectionLength() != 0)
		{
			// setup new carat position and remove selection highlight.
			DeleteString(m_nSelectionStart, m_nSelectionEnd);
			ClearSelection();
		}
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtMultiEditBox::GetLineNumberFromIndex(uint32 index) const
	{
		if( m_aLineList.empty())
		{
			return 0;
		}
		else if( index >= m_strText.m_strContent.size())
		{
			return m_aLineList.size()-1;
		}
		else
		{
			for (uint32 i=0; i < m_aLineList.size(); ++i)
			{
				if( index < m_aLineList[i].m_nStartIdx + m_aLineList[i].m_nLength )
				{
					return i;
				}
			}
		}
		throw CGUIException("[CGUIWgtMultiEditBox::GetLineNumberFromIndex] - Unable to identify a line from the given index <%d>.", index);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtMultiEditBox::SetCursorIndexByPos( const CGUIVector2& rPos)
	{
		if( m_aLineList.empty())
		{
			//empty line
			m_nCursorIdx = 0;
			m_nCursorLine = 0;
		}
		else
		{
			//get line index
			m_nCursorLine = m_aLineList.size()-1;
			real fHeight = GetClientArea().m_fTop;
			for( uint32 i = 0;i < m_aLineList.size();++i)
			{
				fHeight += m_aLineList[i].m_fLineHeight;
				if( rPos.y <= fHeight)
				{
					m_nCursorLine = i;
					break;
				}
			}

			//get character index in this line
			real fStringWidth = GetClientArea().m_fLeft;
			m_nCursorIdx = -1;
			for( uint32 i=0; i<m_aLineList[m_nCursorLine].m_nLength; ++i)
			{
				fStringWidth+=m_vecStringSize[i+m_aLineList[m_nCursorLine].m_nStartIdx].m_fWidth;
				if( fStringWidth >= rPos.x )
				{
					m_nCursorIdx = i+m_aLineList[m_nCursorLine].m_nStartIdx;
					break;
				}
			}
			if( m_nCursorIdx == -1)
			{
				//at the end of string
				m_nCursorIdx = m_aLineList[m_nCursorLine].m_nStartIdx+m_aLineList[m_nCursorLine].m_nLength;
				if( m_strText.GetContent()[m_nCursorIdx-1] == ms_wLineBreak )
				{
					//for last line_break code
					--m_nCursorIdx;
				}
			}
			else if( (fStringWidth - rPos.x) < (m_vecStringSize[m_nCursorIdx].m_fWidth/2))
			{
				++m_nCursorIdx;
			}
		}

		//set cursor position for edit
		m_pEdit->SetCursorPos(GetCursorPos());

		return m_nCursorIdx;
	}
	//------------------------------------------------------------------------------




	//keyboard event
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::OnKeyPressed_Left(CGUIEventKeyboard* pEvent)
	{
		if( m_strText.m_strContent.empty())
			return;

		if( m_nCursorIdx > 0 )
		{
			SetCursorIndex(m_nCursorIdx-1);
		}

		if (pEvent->GetKeyboardInterface()->IsKeyPressed(KC_SHIFT))
		{
			SetSelection(m_nCursorIdx, m_nDragAnchorIdx);	
		}
		else
		{
			ClearSelection();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::OnKeyPressed_Right(CGUIEventKeyboard* pEvent)
	{
		if( m_strText.m_strContent.empty())
			return;

		if( uint32(m_nCursorIdx) < m_strText.m_strContent.size()-1)
		{
			SetCursorIndex(m_nCursorIdx+1);
		}

		if (pEvent->GetKeyboardInterface()->IsKeyPressed(KC_SHIFT))
		{
			SetSelection(m_nCursorIdx, m_nDragAnchorIdx);	
		}
		else
		{
			ClearSelection();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::OnKeyPressed_Up(CGUIEventKeyboard* pEvent)
	{
		if( m_nCursorLine == 0)
		{
			//first line
			return;
		}
		else
		{
			CGUIVector2 aPos = GetCursorPos();
			aPos.y -= m_aLineList[m_nCursorLine-1].m_fLineHeight;
			SetCursorIndexByPos(aPos);
		}

		if (pEvent->GetKeyboardInterface()->IsKeyPressed(KC_SHIFT))
		{
			SetSelection(m_nCursorIdx, m_nDragAnchorIdx);	
		}
		else
		{
			ClearSelection();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::OnKeyPressed_Down(CGUIEventKeyboard* pEvent)
	{
		if( m_aLineList.empty() ||  m_nCursorLine == int32(m_aLineList.size()-1))
		{
			//last line
			return;
		}
		else
		{
			CGUIVector2 aPos = GetCursorPos();
			aPos.y += m_aLineList[m_nCursorLine].m_fLineHeight;
			SetCursorIndexByPos(aPos);
		}

		if (pEvent->GetKeyboardInterface()->IsKeyPressed(KC_SHIFT))
		{
			SetSelection(m_nCursorIdx, m_nDragAnchorIdx);	
		}
		else
		{
			ClearSelection();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::OnKeyPressed_Delete(CGUIEventKeyboard* pEvent)
	{
		if( !IsReadOnly())
		{
			if (GetSelectionLength() != 0)
			{
				//delete selected text
				EraseSelectedText();
			}
			else
			{
				//delete one character
				DeleteString(m_nCursorIdx, m_nCursorIdx+1);
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::OnKeyPressed_Back(CGUIEventKeyboard* pEvent)
	{
		if( !IsReadOnly())
		{
			if (GetSelectionLength() != 0)
			{
				//delete selected text
				EraseSelectedText();
			}
			else
			{
				//delete one character
				DeleteString(m_nCursorIdx-1, m_nCursorIdx);
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::OnKeyPressed_Home(CGUIEventKeyboard* pEvent)
	{
		if( !m_aLineList.empty())
		{
			SetCursorIndex(m_aLineList[m_nCursorLine].m_nStartIdx);
			if (pEvent->GetKeyboardInterface()->IsKeyPressed(KC_SHIFT))
			{
				SetSelection(m_nCursorIdx, m_nDragAnchorIdx);	
			}
			else
			{
				ClearSelection();
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::OnKeyPressed_End(CGUIEventKeyboard* pEvent)
	{
		if( !m_aLineList.empty())
		{
			const SLineInfo& rLineInfo = m_aLineList[m_nCursorLine];
			if( m_strText.m_strContent[rLineInfo.m_nStartIdx + rLineInfo.m_nLength-1] == ms_wLineBreak)
			{
				SetCursorIndex(rLineInfo.m_nStartIdx + rLineInfo.m_nLength-1, m_nCursorLine);
			}
			else
			{
				SetCursorIndex(rLineInfo.m_nStartIdx + rLineInfo.m_nLength, m_nCursorLine);
			}
			
			if (pEvent->GetKeyboardInterface()->IsKeyPressed(KC_SHIFT))
			{
				SetSelection(m_nCursorIdx, m_nDragAnchorIdx);	
			}
			else
			{
				ClearSelection();
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::OnKeyPressed_Enter(CGUIEventKeyboard* pEvent)
	{
		if (!IsReadOnly())
		{
			// erase selected text
			EraseSelectedText();

			// if there is room
			if (m_strText.m_strContent.size() < m_nMaxString)
			{
				InsertString(L"\n");
			}
		}
	}



	//------------------------------------------------------------------------------
	uint32 CGUIWgtMultiEditBox::OnGetFocus( CGUIEventNotification* pEvent )
	{
		m_pEdit->Open();
		return CGUIWgtScrollbarContainer::OnGetFocus(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtMultiEditBox::OnLostFocus( CGUIEventNotification* pEvent )
	{
		m_pEdit->Close();
		return CGUIWgtScrollbarContainer::OnLostFocus(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtMultiEditBox::OnKeyPressed( CGUIEventKeyboard* pEvent )
	{
		switch( pEvent->GetKeyCode())
		{
		case KC_SHIFT:
			if (GetSelectionLength() == 0)
			{
				m_nDragAnchorIdx = m_nCursorIdx;
			}
			pEvent->Consume(true);
			break;

		case KC_UP:
			OnKeyPressed_Up(pEvent);
			pEvent->Consume(true);
			break;

		case KC_DOWN:
			OnKeyPressed_Down(pEvent);
			pEvent->Consume(true);
			break;

		case KC_LEFT:
			OnKeyPressed_Left(pEvent);
			pEvent->Consume(true);
			break;

		case KC_RIGHT:
			OnKeyPressed_Right(pEvent);
			pEvent->Consume(true);
			break;

		case KC_DELETE:
			OnKeyPressed_Delete(pEvent);
			pEvent->Consume(true);
			break;

		case KC_BACK:
			OnKeyPressed_Back(pEvent);
			pEvent->Consume(true);
			break;

		case KC_HOME:
			OnKeyPressed_Home(pEvent);
			pEvent->Consume(true);
			break;

		case KC_END:
			OnKeyPressed_End(pEvent);
			pEvent->Consume(true);
			break;

		case KC_ENTER:
			OnKeyPressed_Enter(pEvent);
			pEvent->Consume(true);
			break;

		}

		return CGUIWgtScrollbarContainer::OnKeyPressed(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtMultiEditBox::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		CGUIVector2 aMousePos = pEvent->GetLocalPosition();

		if( GetClipArea().IsPointInRect(aMousePos) == true)
		{
			ClearSelection();
			m_bDraging = true;

			//set cursor index
			m_nDragAnchorIdx = SetCursorIndexByPos(aMousePos);
		}

		return CGUIWgtScrollbarContainer::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtMultiEditBox::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		m_bDraging = false;

		return CGUIWgtScrollbarContainer::OnMouseLeftUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtMultiEditBox::OnMouseMove( CGUIEventMouse* pEvent )
	{
		CGUIVector2 aMousePos = pEvent->GetLocalPosition();

		//set cursor
		if( GetClipArea().IsPointInRect(aMousePos) == true)
		{
			CGUIMouseCursor::Instance()->SetCursor("CURSOR_EDIT");
		}
		else
		{
			CGUIMouseCursor::Instance()->SetCursor("CURSOR_DEFAULT");
		}

		//for draging
		if (m_bDraging)
		{
			SetCursorIndexByPos(aMousePos);
			SetSelection(m_nCursorIdx, m_nDragAnchorIdx);
		}

		return CGUIWgtScrollbarContainer::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtMultiEditBox::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "readonly" )
		{
			ValueToProperty( IsReadOnly(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Size && rProperty.GetName() == "cursor_size" )
		{
			ValueToProperty( GetCursorSize(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_StringRenderInfo && rProperty.GetName() == "textinfo" )
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
			return CGUIWgtScrollbarContainer::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtMultiEditBox::ProcessProperty( const CGUIProperty& rProperty )
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for readonly
		if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "readonly")
		{
			bool bValue = false;
			PropertyToValue( rProperty, bValue);
			SetReadOnly( bValue );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for cursor size
		else if( rProperty.GetType() == ePropertyType_Size && rProperty.GetName() == "cursor_size")
		{
			CGUISize aValue;
			PropertyToValue( rProperty, aValue);
			SetCursorSize( aValue );
		}
		else if( rProperty.GetType() == ePropertyType_StringRenderInfo && rProperty.GetName() == "textinfo")
		{
			CGUIStringRenderInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetTextInfo(aInfo);
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text")
		{
			CGUIStringRender aStrText;
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
			CGUIWgtScrollbarContainer::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex


