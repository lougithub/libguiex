/** 
* @file guiwgtedit.h
* @brief internal use, for get string from ime
* @author ken
* @date 2006-08-24
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtedit.h>
#include <libguiex_core\guiinterfacemanager.h>
#include <libguiex_core\guiinterfaceime.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtEdit::ms_strType = "CGUIWgtEdit";
	//------------------------------------------------------------------------------
	CGUIWgtEdit::CGUIWgtEdit(const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitEdit();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtEdit::InitEdit()
	{
		SetFocusable(false);
		m_pIme = NULL;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtEdit::RenderSelf(IGUIInterfaceRender* pRender)
	{

	}
	//------------------------------------------------------------------------------
	void	CGUIWgtEdit::Open()
	{
		m_pIme = CGUIInterfaceManager::Instance()->GetInterfaceIme();
		m_pIme->OpenIme();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtEdit::Close()
	{
		m_pIme->CloseIme();
		m_pIme = NULL;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtEdit::Update( real fDeltaTime )
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

		CGUIWidget::Update( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	const wchar_t*	CGUIWgtEdit::GetResult() const
	{
		return m_strResult.c_str();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtEdit::ClearResult()
	{
		m_strResult.clear();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtEdit::SetCursorPos( const CGUIVector2& rPos )
	{
		if( m_pIme )
		{
			m_pIme->SetCursorPos(rPos);
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
