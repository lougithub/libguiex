/** 
* @file wxsoundselectdlg.cpp
* @brief 
* @author ken
* @date 2010-01-08
*/

//============================================================================//
// include
//============================================================================// 
#include "wxsoundselectdlg.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 



//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxSoundSelectDialog, wxDialog )
EVT_BUTTON( ID_SoundSelect_BTN_PLAY, WxSoundSelectDialog::OnPlay )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxSoundSelectDialog::WxSoundSelectDialog( wxWindow* parent )
:WxResourceSelectDialogBase( parent, _T("select sound"), CGUIResourcePool::Instance()->GetSoundList())
{
	wxButton *pBtnPlay = new wxButton( m_pShowPanel, ID_SoundSelect_BTN_PLAY, wxT("Play") );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( m_pListBox, 0, wxALIGN_CENTER );
	m_pShowPanel->SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxSoundSelectDialog::OnPlay(wxCommandEvent& event)
{
	if( !m_strResourceName.empty())
	{
		CGUIInterfaceManager::Instance()->GetInterfaceSound()->PlayEffect( wx2GuiString(m_strResourceName ) );
	}
}
//------------------------------------------------------------------------------
