/** 
* @file guiwgtedit.h
* @brief internal use, for get string from ime
* @author ken
* @date 2006-08-24
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtedit.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfaceime.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtEdit::ms_strType = "CGUIWgtEdit";
	//------------------------------------------------------------------------------
	CGUIWgtEdit::CGUIWgtEdit(const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(ms_strType, rName, rSceneName)
	{
		InitEdit();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEdit::InitEdit()
	{
		SetFocusable(false);
		SetSizeType(eScreenValue_Percentage);
		SetSize( 1.0f, 1.0f );
		SetActivable(false);
		SetOpenWithParent(false);
		SetMouseConsumed(false);
		SetHitable( false );	

		m_pIme = NULL;

		m_fBlinkSpeed = 0.5f;		///< blink speed, in second
		m_bShowCursor = true;
		m_aCursorShowTime = 0.0f;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEdit::RenderSelf(IGUIInterfaceRender* pRender)
	{

	}
	//------------------------------------------------------------------------------
	void CGUIWgtEdit::Open()
	{
		m_pIme = CGUIInterfaceManager::Instance()->GetInterfaceIme();
		m_pIme->OpenIme();

		CGUIWidget::Open();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEdit::Close()
	{
		m_pIme->CloseIme();
		m_pIme = NULL;

		CGUIWidget::Close();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEdit::UpdateSelf( real fDeltaTime )
	{
		//get result string
		if( m_pIme )
		{
			if( wcslen(m_pIme->GetResultString()) )
			{
				m_strResult += m_pIme->GetResultString();
				m_pIme->ClearResultString();
			}
		}

		//update cursor show time
		m_aCursorShowTime += fDeltaTime;
		if( m_aCursorShowTime >= m_fBlinkSpeed )
		{
			m_aCursorShowTime = 0.0f;
			m_bShowCursor = !m_bShowCursor;
		}

		CGUIWidget::UpdateSelf( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	const wchar_t*	CGUIWgtEdit::GetResult() const
	{
		return m_strResult.c_str();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEdit::ClearResult()
	{
		m_strResult.clear();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEdit::SetCursorPos( const CGUIVector2& rPos )
	{
		if( m_pIme )
		{
			m_pIme->SetCursorPos(rPos);
		}
	}
	//------------------------------------------------------------------------------
	const CGUISize& CGUIWgtEdit::GetCursorSize() const
	{
		return m_aCursorSize;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEdit::SetCursorSize( const CGUISize& rSize )
	{
		m_aCursorSize = rSize;
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtEdit::IsShowCursor() const
	{
		return m_bShowCursor;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtEdit::ResetShowCursor() 
	{
		m_aCursorShowTime = 0.0f;
		m_bShowCursor = true;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
