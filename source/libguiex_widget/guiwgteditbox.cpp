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
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guimousecursor.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guipropertymanager.h>

#include <numeric>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtEditBox);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtEditBox::ms_strType = "CGUIWgtEditBox";
	//------------------------------------------------------------------------------
	CGUIWgtEditBox::CGUIWgtEditBox(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(ms_strType, rName, rSceneName)
	{
		InitEditbox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtEditBox::CGUIWgtEditBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
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
		m_pEdit = new CGUIWgtEdit(GetName()+"__EDIT__auto__", GetSceneName());
		m_pEdit->SetParent(this);

		SetFocusable(true);
		SetSelfActivable(false);
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
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "EDIT_BG")
		{
			m_pBG = pImage;
			if( GetSize().IsEqualZero() && pImage )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		if( rName == "EDIT_BGFOCUS")
		{
			m_pBGFocus = pImage;
		}
		if( rName == "EDIT_CURSOR")
		{
			m_pCursor = pImage;
			if( GetCursorSize().IsEqualZero() && pImage)
			{
				SetCursorSize(pImage->GetSize());
			}
		}
	}
	//------------------------------------------------------------------------------
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
	void CGUIWgtEditBox::SetReadOnly(bool bRead)
	{
		m_bReadOnly = bRead;
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtEditBox::IsReadOnly() const
	{
		return m_bReadOnly;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::SetTextMasked(bool bMask)
	{
		if( bMask != m_bMaskText)
		{
			m_bMaskText = bMask;
			if( bMask )
			{
				m_strMaskText = m_strText;
				m_strMaskText.Replace(0,m_wMaskCodePoint, m_strMaskText.Size());
			}
			else
			{
				m_strMaskText.Clear();
			}

			CGUIStringEx* pString = m_bMaskText?&m_strMaskText:&m_strText;
			uint32 nSize = pString->Size();
			IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
			m_vecStringWidth.clear();
			for( uint32 i=0; i<nSize; ++i)
			{
				m_vecStringWidth.insert(
					m_vecStringWidth.begin()+i, 
					uint32(pFont->GetCharacterSize(pString->GetInfo(i).m_nFontIdx,pString->GetCharacter(i), pString->GetInfo(i).m_nFontSize).m_fWidth));
			}
		}
	
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtEditBox::IsTextMasked() const
	{
		return m_bMaskText;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::SetMaskCode(wchar_t wMaskCode)
	{
		if( m_wMaskCodePoint != wMaskCode )
		{
			m_wMaskCodePoint = wMaskCode;
			if( m_bMaskText )
			{
				m_strMaskText.Replace(0,m_wMaskCodePoint, m_strMaskText.Size());

				uint32 nSize = m_strMaskText.Size();
				IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
				m_vecStringWidth.clear();
				for( uint32 i=0; i<nSize; ++i)
				{
					m_vecStringWidth.insert(
						m_vecStringWidth.begin()+i,
						uint32(pFont->GetCharacterSize(m_strMaskText.GetInfo(i).m_nFontIdx,m_strMaskText.GetCharacter(i), m_strMaskText.GetInfo(i).m_nFontSize).m_fWidth));
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	wchar_t CGUIWgtEditBox::GetTextMasked() const
	{
		return m_wMaskCodePoint;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::RefreshSelf( )
	{
		CGUIWidget::RefreshSelf();

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
		CGUIVector2	aPos = GetBoundArea().GetPosition();
		aPos.x += m_fTextWidthRel;
		if( !m_strText.Empty())
		{
			CGUIStringEx* pRenderString = m_bMaskText?&m_strMaskText:&m_strText;

			//push clip rect
			pRender->PushClipRect( getFullTransform(), m_aStringAreaRect );

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
				CGUIStringInfo	aStringInfo = pRenderString->GetDefaultInfo();
				CGUIColor aDefaultColor = aStringInfo.m_aColor;
				aStringInfo.m_aColor = m_aSelectedTextColor;
				pRenderString->SetDefaultInfo( &aStringInfo);
				DrawString( pRender, *pRenderString, aPos, m_nSelectionStart, m_nSelectionEnd);
				aPos.x += std::accumulate(m_vecStringWidth.begin()+m_nSelectionStart, m_vecStringWidth.begin()+m_nSelectionEnd, 0.0f);

				//draw last not selected section
				aStringInfo.m_aColor = aDefaultColor;
				pRenderString->SetDefaultInfo( &aStringInfo);
				DrawString( pRender, *pRenderString,aPos, m_nSelectionEnd, pRenderString->Size());
			}
			else
			{
				//no selection
				DrawString( pRender, *pRenderString,aPos);
			}

			//pop clip rect
			pRender->PopClipRect( );
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

		CGUIWidget::UpdateSelf( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::SetStringAreaRatio(const CGUIRect& rStringAreaRatio)
	{
		m_aStringAreaRatio = rStringAreaRatio;
	}
	//------------------------------------------------------------------------------
	const CGUIRect& CGUIWgtEditBox::GetStringAreaRatio( ) const
	{
		return m_aStringAreaRatio;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::SetTextContent(const wchar_t* pText)
	{
		ClearSelection();
		DeleteString(0, -1);
		InsertString(pText);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::InsertString(const wchar_t* pChar)
	{
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		uint32 len = wcslen(pChar);
		if( len == 0 )
		{
			//nothing added
			return;
		}
		if( m_strText.Size() + wcslen(pChar) > m_nMaxString )
		{
			//reach max number of string
			return;
		}

		//insert string
		m_strText.Insert(m_nCursorIdx, pChar );
		if(m_bMaskText)
		{
			m_strMaskText.Insert( m_nCursorIdx, m_wMaskCodePoint,len );
		}

		//string width
		CGUIStringEx* pString = m_bMaskText?&m_strMaskText:&m_strText;
		for( uint32 i=0; i<len; ++i)
		{
			m_vecStringWidth.insert(
				m_vecStringWidth.begin()+m_nCursorIdx+i, 
				uint32(pFont->GetCharacterSize(pString->GetInfo(m_nCursorIdx+i).m_nFontIdx,pString->GetCharacter(m_nCursorIdx+i), pString->GetInfo(m_nCursorIdx+i).m_nFontSize).m_fWidth));
		}

		//cursor id
		SetCursorIndex(m_nCursorIdx + len);
	}
	//------------------------------------------------------------------------------
	CGUIVector2	CGUIWgtEditBox::GetCursorPos()
	{
		real fWidth = GetStringWidth(0, m_nCursorIdx);
		real fHeight = 0.0f;

		CGUIVector2 aPos = m_aStringAreaRect.GetPosition();
		aPos.x = aPos.x+fWidth+m_fTextWidthRel-m_pEdit->GetCursorSize().m_fWidth/2;
		aPos.y = aPos.y + fHeight + (m_aStringAreaRect.GetHeight() - ( m_pEdit->GetCursorSize().m_fHeight + GetTextInfo().m_nFontSize )/2 );

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
		nEndPos = static_cast<int32>(nEndPos<0?m_strText.Size():nEndPos);
		nEndPos = static_cast<int32>(nEndPos>static_cast<int32>(m_strText.Size())?m_strText.Size():nEndPos);
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
		nEndPos = static_cast<int32>(nEndPos<0?m_strText.Size():nEndPos);
		nEndPos = static_cast<int32>(nEndPos>static_cast<int32>(m_strText.Size())?m_strText.Size():nEndPos);
		if( nBeginPos >= nEndPos)
		{
			return;
		}

		//erase string
		m_strText.Erase( nBeginPos, nEndPos);
		if(m_bMaskText)
		{
			m_strMaskText.Erase( nBeginPos, nEndPos);
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
	void CGUIWgtEditBox::SetCursorIndex( int32 nPos )
	{
		if( nPos<0 || nPos>int32(m_strText.Size()))
		{
			nPos = m_strText.Size();
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
			nIdx = m_strText.Size();
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
	void CGUIWgtEditBox::SetSelection(uint32 start_pos, uint32 end_pos)
	{
		if (start_pos > m_strText.Size())
		{
			start_pos = m_strText.Size();
		}

		if (end_pos > m_strText.Size())
		{
			end_pos = m_strText.Size();
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
		if( m_nCursorIdx < m_strText.Size())
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
		SetCursorIndex(m_strText.Size());
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
		return CGUIWidget::OnGetFocus(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtEditBox::OnLostFocus( CGUIEventNotification* pEvent )
	{
		m_pEdit->Close();
		return CGUIWidget::OnLostFocus(pEvent);
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

		return CGUIWidget::OnKeyPressed(pEvent);
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

		return CGUIWidget::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtEditBox::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		m_bDraging = false;

		return CGUIWidget::OnMouseLeftUp(pEvent);
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

		return CGUIWidget::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
	CGUIProperty* CGUIWgtEditBox::GenerateProperty(const CGUIString& rName, const CGUIString& rType )
	{
		//CGUIProperty* pProperty = NULL;
		//
		//if( rName == "MASK" && rType == "BOOL" )
		//{
		//	pProperty = CGUIPropertyManager::Instance()->CreateProperty(
		//		rName, 
		//		rType, 
		//		CGUIStringConvertor::BoolToString(IsTextMasked( )));
		//}
		//else if( rName == "MASK_CODE" && rType == "CGUIString" )
		//{
		//	//pProperty = CGUIPropertyManager::Instance()->CreateProperty(
		//	//	rName, 
		//	//	rType, 
		//	//	GetTextMasked( ));
		//}
		//else if( rName == "MAX_TEXT_NUM" && rType == "uint32" )
		//{
		//	pProperty = CGUIPropertyManager::Instance()->CreateProperty(
		//		rName, 
		//		rType, 
		//		CGUIStringConvertor::UInt32ToString(GetMaxTextNum( )));
		//}
		//else if( rName == "STRING_AREA_RATIO" && rType == "CGUIRect" )
		//{
		//	pProperty = CGUIPropertyManager::Instance()->CreateProperty(
		//		rName, 
		//		rType, 
		//		CGUIStringConvertor::RectToString(GetStringAreaRatio( )));
		//}
		//else if( rName == "READONLY" && rType == "BOOL")
		//{
		//	pProperty = CGUIPropertyManager::Instance()->CreateProperty(
		//		rName, 
		//		rType, 
		//		CGUIStringConvertor::BoolToString(IsReadOnly( )));
		//}
		//else if( rName == "CURSOR_SIZE" && rType == "SIZE")
		//{
		//	pProperty = CGUIPropertyManager::Instance()->CreateProperty(
		//		rName, 
		//		rType, 
		//		CGUIStringConvertor::SizeToString(GetCursorSize( )));
		//}
		//return pProperty ? pProperty : CGUIWidget::GenerateProperty(rName, rType);
		return NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEditBox::ProcessProperty( const CGUIProperty* pProperty)
	{
		//CGUIWidget::ProcessProperty(pProperty);

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		////property for text masked
		///*
		//*<property name="MASK" type="BOOL" value="true" />
		//*/
		//if( pProperty->GetName() == "MASK" && pProperty->GetType() == "BOOL")
		//{
		//	SetTextMasked(StringToValue(pProperty->GetValue()));
		//}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		////property for maske code
		///*
		//*<property name="MASK_CODE" type="CGUIString" value="*" />
		//*/
		//else if( pProperty->GetName() == "MASK_CODE" &&  pProperty->GetType() == "CGUIString")
		//{
		//	//SetMaskCode(pProperty->GetValue().c_str()[0]);
		//}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		////property for max text num
		///*
		//*<property name="MAX_TEXT_NUM" type="uint32" value="*" />
		//*/
		//else if( pProperty->GetName() == "MAX_TEXT_NUM" &&  pProperty->GetType() == "uint32")
		//{
		//	SetMaxTextNum(CGUIStringConvertor::StringToUInt32(pProperty->GetValue()));
		//}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		////property for string area ratio
		///*
		//*<property name="STRING_AREA_RATIO" type="CGUIRect" value="0.0,0.0,1.0,1.0" />
		//*/
		//else if( pProperty->GetName() == "STRING_AREA_RATIO" && pProperty->GetType() == "CGUIRect")
		//{
		//	SetStringAreaRatio(CGUIStringConvertor::StringToRect(pProperty->GetValue()));
		//}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		////property for readonly
		///*
		//*<property name="READONLY" type="BOOL" value="false" />
		//*/
		//else if( pProperty->GetName() == "READONLY" && pProperty->GetType() == "BOOL")
		//{
		//	SetReadOnly(StringToValue(pProperty->GetValue()));
		//}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		////property for cursor size
		///*
		//*<property name="CURSOR_SIZE" type="SIZE" value="2,10" />
		//*/
		//else if( pProperty->GetName() == "CURSOR_SIZE" && pProperty->GetType() == "SIZE")
		//{
		//	SetCursorSize(CGUIStringConvertor::StringToSize(pProperty->GetValue()));
		//}
	}
	//------------------------------------------------------------------------------


}//namespace guiex


