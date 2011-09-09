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
#include "resourcelist.h"
#include "wxeditorid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 



//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxSoundSelectDialog, wxDialog )
EVT_BUTTON( ID_SoundSelect_BTN_OK, WxSoundSelectDialog::OnOK )
EVT_BUTTON( ID_SoundSelect_BTN_CANCEL, WxSoundSelectDialog::OnCANCEL )
EVT_BUTTON( ID_SoundSelect_BTN_PLAY, WxSoundSelectDialog::OnPlay )
EVT_LISTBOX( ID_SoundListID, WxSoundSelectDialog::OnListBoxSelect )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxSoundSelectDialog::WxSoundSelectDialog( wxWindow* parent )
:wxDialog( parent, wxID_ANY, _T("select sound"), wxDefaultPosition,wxSize(800,600), wxDEFAULT_DIALOG_STYLE/*wxNO_3D*/)
{
	m_pListBox = new wxListBox(
		this, 
		ID_SoundListID,
		wxDefaultPosition, 
		wxDefaultSize,
		CResourceList::Instance()->GetSoundList(),
		wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT );

	wxButton *pBtnOk = new wxButton( this, ID_SoundSelect_BTN_OK, wxT("OK") );
	wxButton *pBtnCancel = new wxButton( this, ID_SoundSelect_BTN_CANCEL, wxT("CANCEL") );
	wxButton *pBtnPlay = new wxButton( this, ID_SoundSelect_BTN_PLAY, wxT("Play") );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );

	sizerTop->Add( m_pListBox, 1, wxALL|wxEXPAND );

	wxSizer *sizerRow1 = new wxBoxSizer( wxHORIZONTAL );
	sizerRow1->Add( pBtnPlay, 0, wxALIGN_CENTER );
	sizerRow1->AddSpacer( 10 );
	sizerRow1->Add( pBtnOk, 0, wxALIGN_CENTER );
	sizerRow1->AddSpacer( 10 );
	sizerRow1->Add( pBtnCancel, 0, wxALIGN_CENTER );
	sizerTop->Add( sizerRow1, 0, wxALIGN_CENTER_HORIZONTAL);

	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxSoundSelectDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_OK);
	EndModal(wxID_OK);
} 
//------------------------------------------------------------------------------
void WxSoundSelectDialog::OnCANCEL(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_CANCEL);
	EndModal(wxID_CANCEL);
}
//------------------------------------------------------------------------------
void WxSoundSelectDialog::OnListBoxSelect(wxCommandEvent& event)
{
	long sel = event.GetSelection();
	m_strSoundName = m_pListBox->GetString( sel );
}
//------------------------------------------------------------------------------
void WxSoundSelectDialog::OnPlay(wxCommandEvent& event)
{
	if( !m_strSoundName.empty())
	{
		CGUIInterfaceManager::Instance()->GetInterfaceSound()->PlayEffect( wx2GuiString(m_strSoundName ));
	}
}
//------------------------------------------------------------------------------
