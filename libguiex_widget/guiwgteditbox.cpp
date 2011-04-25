/** 
* @file guiwgteditbox.cpp
* @brief editbox, used to edit texture
* @author ken
* @date 2006-08-07
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgteditbox.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guiinterfacekeyboard.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guimousecursor.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiwidgetmanager.h>
#include <libguiex_core/guipropertyconvertor.h>

#include <numeric>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtEditBox);
	//------------------------------------------------------------------------------
	CGUIWgtEditBox::CGUIWgtEditBox(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWgtTextBase(StaticGetType(), rName, rSceneName)
	{
		InitEditbox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtEditBox::CGUIWgtEditBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtTextBase(rType, rName, rSceneName)
	{
		InitEditbox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtEditBox::~CGUIWgtEditBox( )
	{
		m_pEdit->SetParent(NULL);
		delete m_pEdit;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::InitEditbox()
	{
		m_pEdit = new CGUIWgtEdit(GetName()+"__EDIT"+ GUI_INTERNAL_WIDGET_FLAG, GetSceneName());
		m_pEdit->SetParent(this);
		m_pEdit->SetCursorSize( CGUISize( 2.0f, 16.0f) );

		SetFocusable(true);
		m_nMaxString = 100;		///< max number of string
		m_nCursorIdx = 0;			///< cursor's position in edited string, the first is 0.
		m_fTextWidthRel = 0.0f;
		m_bMaskText = false;
		m_wMaskCodePoint = GUI_DEFAULT_EDITBOX_MASK;
		m_bReadOnly = false;

		//for drag
		m_nSelectionStart = 0;
		m_nSelectionEnd = 0;
		m_bDraging = false;
		m_nDragAnchorIdx = 0;

		//size ratio
		m_aStringAreaRatio.m_fLeft = m_aStringAreaRatio.m_fTop = 0.0f;
		m_aStringAreaRatio.m_fRight = m_aStringAreaRatio.m_fBottom = 1.0f;

		//image
		m_pBG = NULL;
		m_pBGFocus = NULL;
		m_pCursor = NULL;

		//text
		m_eTextAlignmentHorz = eTextAlignment_Horz_Left;
		m_eTextAlignmentVert = eTextAlignment_Vert_Center;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "bg")
		{
			m_pBG = pImage;
			if( GetSize().IsEqualZero() && pImage )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		if( rName == "bg_focus")
		{
			m_pBGFocus = pImage;
		}
		if( rName == "cursor")
		{
			m_pCursor = pImage;
		}
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief set selected text color
	*/
	void CGUIWgtEditBox::SetSelectedTextColor( const CGUIColor& rColor)
	{
		m_aSelectedTextColor = rColor;
	}
	//------------------------------------------------------------------------------
	const CGUISize&	 CGUIWgtEditBox::GetCursorSize() const
	{
		return m_pEdit->GetCursorSize();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::SetCursorSize( const CGUISize& rSize )
	{
		m_pEdit->SetCursorSize(rSize);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::SetMaxTextNum( uint32 num)
	{
		m_nMaxString = num;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtEditBox::GetMaxTextNum( ) const
	{
		return m_nMaxString;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief set text readonly
	*/
	void CGUIWgtEditBox::SetReadOnly(bool bRead)
	{
		m_bReadOnly = bRead;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief is text readonly
	*/
	bool CGUIWgtEditBox::IsReadOnly() const
	{
		return m_bReadOnly;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief set text masked
	*/
	void CGUIWgtEditBox::SetTextMasked(bool bMask)
	{
		if( bMask != m_bMaskText)
		{
			m_bMaskText = bMask;
			if( bMask )
			{
				m_strMaskText = m_strText;
				m_strMaskText.m_strContent.replace( 0, 1, m_strMaskText.m_strContent.size(), m_wMaskCodePoint);
			}
			else
			{
				m_strMaskText.m_strContent.clear();
			}

			CGUIStringRender* pString = m_bMaskText ? &m_strMaskText : &m_strText;
			uint32 nSize = pString->m_strContent.size();
			IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
			m_vecStringWidth.clear();
			for( uint32 i=0; i<nSize; ++i)
			{
				m_vecStringWidth.insert(
					m_vecStringWidth.begin()+i, 
					pFont->GetCharacterSize( pString->m_strContent[i],pString->m_aStringInfo).m_fWidth);
			}
		}

	}
	//------------------------------------------------------------------------------
	/** 
	* @brief is text masked
	*/
	bool CGUIWgtEditBox::IsTextMasked() const
	{
		return m_bMaskText;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief is text masked
	*/
	void CGUIWgtEditBox::SetMaskCode(wchar_t wMaskCode)
	{
		if( m_wMaskCodePoint != wMaskCode )
		{
			m_wMaskCodePoint = wMaskCode;
			if( m_bMaskText )
			{
				m_strMaskText.m_strContent.replace(0,1, m_strMaskText.m_strContent.size(), m_wMaskCodePoint );

				uint32 nSize = m_strMaskText.m_strContent.size();
				IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
				m_vecStringWidth.clear();
				for( uint32 i=0; i<nSize; ++i)
				{
					m_vecStringWidth.insert(
						m_vecStringWidth.begin()+i,
						pFont->GetCharacterSize(m_strMaskText.m_strContent[i], m_strMaskText.m_aStringInfo).m_fWidth);
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief is text masked
	*/
	wchar_t CGUIWgtEditBox::GetTextMasked() const
	{
		return m_wMaskCodePoint;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::RefreshSelf( )
	{
		CGUIWgtTextBase::RefreshSelf();

		m_aStringAreaRect.m_fLeft = GetBoundArea().m_fLeft+(GetPixelSize().GetWidth()*m_aStringAreaRatio.m_fLeft);
		m_aStringAreaRect.m_fRight = GetBoundArea().m_fLeft+(GetPixelSize().GetWidth()*m_aStringAreaRatio.m_fRight);
		m_aStringAreaRect.m_fTop = GetBoundArea().m_fTop+(GetPixelSize().GetHeight()*m_aStringAreaRatio.m_fTop);
		m_aStringAreaRect.m_fBottom = GetBoundArea().m_fTop+(GetPixelSize().GetHeight()*m_aStringAreaRatio.m_fBottom);

	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::RenderSelf(IGUIInterfaceRender* pRender)
	{
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

		//render cursor
		if( m_pCursor && IsFocus() )
		{
			//render
			if( m_pEdit->IsShowCursor() )
			{
				DrawImage( pRender, m_pCursor, GetCursorRect());
			}
		}

		//render string
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		CGUIVector2	aPos = GetBoundArea().GetPosition();
		aPos.x += m_fTextWidthRel;
		aPos.y += pFont->GetFontHeight(GetTextInfo());
		if( !m_strText.m_strContent.empty())
		{
			CGUIStringRender* pRenderString = m_bMaskText?&m_strMaskText:&m_strText;

			if( GetSelectionLength())
			{
				//has selection

				/* 
				the text is divided to three section
				xxx yyy zzz
				first <xxx> is not selected 
				second <yyy> is selected
				last <zzz> is not selected
				*/

				//draw first not selected section
				DrawString( pRender, *pRenderString, aPos, 0, m_nSelectionStart);
				aPos.x += std::accumulate(m_vecStringWidth.begin(), m_vecStringWidth.begin()+m_nSelectionStart, 0.0f);

				//draw second selected section
				CGUIColor aDefaultColor = pRenderString->m_aStringInfo.m_aColor;

				pRenderString->m_aStringInfo.m_aColor = m_aSelectedTextColor;
				DrawString( pRender, *pRenderString, aPos, m_nSelectionStart, m_nSelectionEnd);
				aPos.x += std::accumulate(m_vecStringWidth.begin()+m_nSelectionStart, m_vecStringWidth.begin()+m_nSelectionEnd, 0.0f);

				//draw last not selected section
				pRenderString->m_aStringInfo.m_aColor = aDefaultColor;
				DrawString( pRender, *pRenderString, aPos, m_nSelectionEnd, pRenderString->m_strContent.size());
			}
			else
			{
				//no selection
				DrawString( pRender, *pRenderString,aPos);
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::UpdateSelf( real fDeltaTime )
	{
		if( IsFocus() )
		{
			if( IsReadOnly() )
			{
				m_pEdit->ClearResult();
			}
			else
			{
				if( wcslen(m_pEdit->GetResult()) )
				{
					if( GetSelectionLength() != 0)
					{
						EraseSelectedText();
					}

					InsertString(m_pEdit->GetResult());
					m_pEdit->ClearResult();
				}
			}
		}

		CGUIWgtTextBase::UpdateSelf( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief set string area ratio
	*/
	void CGUIWgtEditBox::SetStringAreaRatio(const CGUIRect& rStringAreaRatio)
	{
		m_aStringAreaRatio = rStringAreaRatio;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get string area ratio
	*/
	const CGUIRect& CGUIWgtEditBox::GetStringAreaRatio( ) const
	{
		return m_aStringAreaRatio;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief set widget text
	*/
	void CGUIWgtEditBox::SetTextContent(const CGUIStringW& rText)
	{
		ClearSelection( );
		DeleteString( 0, -1 );
		InsertString( rText );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::SetTextInfo(const CGUIStringRenderInfo& rInfo)
	{
		CGUIWgtTextBase::SetTextInfo( rInfo );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::InsertString( const CGUIStringW& rText )
	{
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		uint32 len = rText.size();
		if( len == 0 )
		{
			//nothing added
			return;
		}
		if( m_strText.m_strContent.size() + len > m_nMaxString )
		{
			//reach max number of string
			return;
		}

		//insert string
		m_strText.m_strContent.insert( m_nCursorIdx, rText );
		if(m_bMaskText)
		{
			m_strMaskText.m_strContent.insert( m_nCursorIdx, m_wMaskCodePoint,len );
		}

		//string width
		CGUIStringRender* pString = m_bMaskText ? &m_strMaskText : &m_strText;
		for( uint32 i=0; i<len; ++i)
		{
			m_vecStringWidth.insert(
				m_vecStringWidth.begin()+m_nCursorIdx+i, 
				pFont->GetCharacterSize(pString->m_strContent[m_nCursorIdx+i], pString->m_aStringInfo).m_fWidth);
		}

		//cursor id
		SetCursorIndex(m_nCursorIdx + len);
	}
	//------------------------------------------------------------------------------
	CGUIVector2	CGUIWgtEditBox::GetCursorPos()
	{
		real fWidth = GetStringWidth(0, m_nCursorIdx);
		real fHeight = 0.0f;

		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		CGUIVector2 aPos = m_aStringAreaRect.GetPosition();
		aPos.x = aPos.x+fWidth+m_fTextWidthRel-m_pEdit->GetCursorSize().m_fWidth/2;
		aPos.y = aPos.y + fHeight + (m_aStringAreaRect.GetHeight() - ( m_pEdit->GetCursorSize().m_fHeight + pFont->GetFontHeight(GetTextInfo()) )/2 );

		return aPos;
	}
	//------------------------------------------------------------------------------
	CGUIRect CGUIWgtEditBox::GetCursorRect()
	{
		return CGUIRect(GetCursorPos(), m_pEdit->GetCursorSize());
	}
	//------------------------------------------------------------------------------
	real CGUIWgtEditBox::GetStringWidth(int32 nBeginPos, int32 nEndPos) const
	{
		nBeginPos = static_cast<int32>(nBeginPos<0?0:nBeginPos);
		nEndPos = static_cast<int32>(nEndPos<0?m_strText.m_strContent.size():nEndPos);
		nEndPos = static_cast<int32>(nEndPos>static_cast<int32>(m_strText.m_strContent.size())?m_strText.m_strContent.size():nEndPos);
		if( nBeginPos >= nEndPos)
		{
			return 0.0f;
		}
		return std::accumulate(m_vecStringWidth.begin()+nBeginPos, m_vecStringWidth.begin()+nEndPos, 0.0f);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::DeleteString( int32 nBeginPos, int32 nEndPos)
	{
		nBeginPos = static_cast<int32>(nBeginPos<0?0:nBeginPos);
		nEndPos = static_cast<int32>(nEndPos<0?m_strText.m_strContent.size():nEndPos);
		nEndPos = static_cast<int32>(nEndPos>static_cast<int32>(m_strText.m_strContent.size())?m_strText.m_strContent.size():nEndPos);
		if( nBeginPos >= nEndPos)
		{
			return;
		}

		//erase string
		m_strText.m_strContent.erase( nBeginPos, nEndPos);
		if(m_bMaskText)
		{
			m_strMaskText.m_strContent.erase( nBeginPos, nEndPos);
		}
		m_vecStringWidth.erase( m_vecStringWidth.begin()+nBeginPos, m_vecStringWidth.begin()+nEndPos);

		//update cursor position
		if( int32(m_nCursorIdx)>nEndPos )
		{
			SetCursorIndex(m_nCursorIdx - (nEndPos-nBeginPos));
		}
		else if ( int32(m_nCursorIdx)>nBeginPos )
		{
			SetCursorIndex(m_nCursorIdx - (m_nCursorIdx-nBeginPos));
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::UpdateStringPos()
	{
		real fStringWidth = GetStringWidth(0, m_nCursorIdx)+m_pEdit->GetCursorSize().GetWidth();
		real fClientWidth = m_aStringAreaRect.GetWidth();
		m_fTextWidthRel = (fClientWidth - fStringWidth)<0.0f?fClientWidth - fStringWidth:0.0f;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Set the current position of the carat.
	* @param nPos New index for the insert cursor relative to the start of the text.  
	* If the value specified is greater than the number of characters in the Editbox or is negative, 
	* the cursor is positioned at the end of the text.
	*/
	void CGUIWgtEditBox::SetCursorIndex( int32 nPos )
	{
		if( nPos<0 || nPos>int32(m_strText.m_strContent.size()))
		{
			nPos = m_strText.m_strContent.size();
		}

		m_nCursorIdx = nPos;
		m_pEdit->ResetShowCursor();

		//set cursor position for edit
		m_pEdit->SetCursorPos(GetCursorPos());

		//update string position
		UpdateStringPos();
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtEditBox::GetTextIndexFromPos( const CGUIVector2& rPos)
	{
		real fStringWidth = m_fTextWidthRel+m_aStringAreaRect.m_fLeft;
		int32 nIdx = -1;
		for( uint32 i=0; i<m_vecStringWidth.size(); ++i )
		{
			fStringWidth+=m_vecStringWidth[i];
			if( fStringWidth >= rPos.x )
			{
				nIdx = i;
				break;
			}
		}

		if( nIdx == -1)
		{
			//at the end of string
			nIdx = m_strText.m_strContent.size();
		}
		else if( (fStringWidth - rPos.x) <= (m_vecStringWidth[nIdx]/2))
		{
			//mouse point is at right part of text
			++nIdx;
		}

		return nIdx;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::ClearSelection()
	{
		// perform action only if required.
		if (GetSelectionLength() != 0)
		{
			SetSelection(0, 0);
		}
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtEditBox::GetSelectionLength(void) const
	{
		return m_nSelectionEnd - m_nSelectionStart;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::EraseSelectedText( )
	{
		if (GetSelectionLength() != 0)
		{
			// setup new carat position and remove selection highlight.
			//SetCursorIndex(m_nSelectionStart);
			DeleteString(m_nSelectionStart, m_nSelectionEnd);
			ClearSelection();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::SetSelection(size_t start_pos, size_t end_pos)
	{
		if (start_pos > m_strText.m_strContent.size())
		{
			start_pos = m_strText.m_strContent.size();
		}

		if (end_pos > m_strText.m_strContent.size())
		{
			end_pos = m_strText.m_strContent.size();
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





	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::OnKeyPressed_Left(CGUIEventKeyboard* pEvent)
	{
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
	void CGUIWgtEditBox::OnKeyPressed_Right(CGUIEventKeyboard* pEvent)
	{
		if( m_nCursorIdx < m_strText.m_strContent.size())
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
	void CGUIWgtEditBox::OnKeyPressed_Delete(CGUIEventKeyboard* pEvent)
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
	void CGUIWgtEditBox::OnKeyPressed_Back(CGUIEventKeyboard* pEvent)
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
	void CGUIWgtEditBox::OnKeyPressed_Home(CGUIEventKeyboard* pEvent)
	{
		SetCursorIndex(0);
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
	void CGUIWgtEditBox::OnKeyPressed_End(CGUIEventKeyboard* pEvent)
	{
		SetCursorIndex(m_strText.m_strContent.size());
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
	uint32 CGUIWgtEditBox::OnGetFocus( CGUIEventNotification* pEvent )
	{
		m_pEdit->Open();
		return CGUIWgtTextBase::OnGetFocus(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtEditBox::OnLostFocus( CGUIEventNotification* pEvent )
	{
		m_pEdit->Close();
		return CGUIWgtTextBase::OnLostFocus(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtEditBox::OnKeyPressed( CGUIEventKeyboard* pEvent )
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

		case KC_LEFT:
			OnKeyPressed_Left(pEvent);
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
		}

		//crack here, consume all key event for editbox here
		pEvent->Consume(true);

		return CGUIWgtTextBase::OnKeyPressed(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtEditBox::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		const CGUIVector2& rMouseLocalPos = pEvent->GetLocalPosition();

		if( m_aStringAreaRect.IsPointInRect(rMouseLocalPos) == true)
		{
			ClearSelection();
			m_bDraging = true;

			//set cursor index
			m_nDragAnchorIdx = GetTextIndexFromPos(rMouseLocalPos);
			SetCursorIndex(m_nDragAnchorIdx);
		}

		return CGUIWgtTextBase::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtEditBox::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		m_bDraging = false;

		return CGUIWgtTextBase::OnMouseLeftUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtEditBox::OnMouseMove( CGUIEventMouse* pEvent )
	{
		const CGUIVector2& rMouseLocalPos = pEvent->GetLocalPosition();

		//set cursor
		if( m_aStringAreaRect.IsPointInRect(rMouseLocalPos) == true)
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
			SetCursorIndex(GetTextIndexFromPos(rMouseLocalPos));
			SetSelection(m_nCursorIdx, m_nDragAnchorIdx);
		}

		return CGUIWgtTextBase::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief load widget config from property
	*/
	int32 CGUIWgtEditBox::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "mask" )
		{
			ValueToProperty( IsTextMasked(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_UInt32 && rProperty.GetName() == "max_text_num" )
		{
			ValueToProperty( GetMaxTextNum(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Rect && rProperty.GetName() == "client_area" )
		{
			ValueToProperty( GetStringAreaRatio(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "readonly" )
		{
			ValueToProperty( IsReadOnly(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Size && rProperty.GetName() == "cursor_size" )
		{
			ValueToProperty( GetCursorSize(), rProperty);
		}
		else
		{
			return CGUIWgtTextBase::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process property
	*/
	void CGUIWgtEditBox::ProcessProperty( const CGUIProperty& rProperty )
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for text masked
		if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "mask")
		{
			bool bValue = false;
			PropertyToValue( rProperty, bValue);
			SetTextMasked( bValue );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for max text num
		else if( rProperty.GetType() == ePropertyType_UInt32 && rProperty.GetName() == "max_text_num" )
		{
			uint32 uValue = 0;
			PropertyToValue( rProperty, uValue);
			SetMaxTextNum( uValue );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for string area ratio
		else if( rProperty.GetType() == ePropertyType_Rect && rProperty.GetName() == "client_area" )
		{
			CGUIRect aValue;
			PropertyToValue( rProperty, aValue);
			SetStringAreaRatio( aValue );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for readonly
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "readonly")
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
		else
		{
			CGUIWgtTextBase::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------


}//namespace guiex


