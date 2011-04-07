/** 
* @file wxlocalizationselectdlg.cpp
* @brief the dialog to select localization.
* @author ken
* @date 2011-04-06
*/

//============================================================================//
// include
//============================================================================// 
#include "wxlocalizedstringselectdlg.h"
#include "resourcelist.h"
#include "wxeditorid.h"
#include "editorutility.h"

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxLocalizedstringSelectDialog, wxDialog )
EVT_BUTTON( ID_LocalizationSelect_BTN_OK, WxLocalizedstringSelectDialog::OnOK )
EVT_BUTTON( ID_LocalizationSelect_BTN_CANCEL, WxLocalizedstringSelectDialog::OnCANCEL )
EVT_LISTBOX( ID_LocalizationListID, WxLocalizedstringSelectDialog::OnListBoxSelect )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxLocalizedstringSelectDialog::WxLocalizedstringSelectDialog( wxWindow* parent )
:wxDialog( parent, wxID_ANY, _T("select localization"), wxDefaultPosition,wxSize(800,600), wxDEFAULT_DIALOG_STYLE/*wxNO_3D*/)
{
	m_pListBox = new wxListBox(
		this, 
		ID_LocalizationListID,
		wxDefaultPosition, 
		wxDefaultSize,
		CResourceList::Instance()->GetLocalizationList(),
		wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT );

	m_pTextBox = new wxTextCtrl( this, wxID_ANY );
	m_pTextBox->SetEditable( false );

	wxButton *pBtnOk = new wxButton( this, ID_LocalizationSelect_BTN_OK, wxT("OK") );
	wxButton *pBtnCancel = new wxButton( this, ID_LocalizationSelect_BTN_CANCEL, wxT("CANCEL") );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );

	wxSizer *sizerRow0 = new wxBoxSizer( wxHORIZONTAL );
	sizerRow0->Add( m_pTextBox, 1, wxALL|wxEXPAND );
	sizerRow0->Add( m_pListBox, 1, wxALL|wxEXPAND );
	sizerTop->Add( sizerRow0, 1, wxALL|wxEXPAND );

	wxSizer *sizerRow1 = new wxBoxSizer( wxHORIZONTAL );
	sizerRow1->Add( pBtnOk, 0, wxALIGN_CENTER );
	sizerRow1->AddSpacer( 10 );
	sizerRow1->Add( pBtnCancel, 0, wxALIGN_CENTER );
	sizerTop->Add( sizerRow1, 0, wxALIGN_CENTER_HORIZONTAL);

	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxLocalizedstringSelectDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_OK);
	EndModal(wxID_OK);
} 
//------------------------------------------------------------------------------
void WxLocalizedstringSelectDialog::OnCANCEL(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_CANCEL);
	EndModal(wxID_CANCEL);
}
//------------------------------------------------------------------------------
void WxLocalizedstringSelectDialog::OnListBoxSelect(wxCommandEvent& event)
{
	long sel = event.GetSelection();
	m_strLocalizedString = m_pListBox->GetString( sel );

	m_pTextBox->SetLabelText( CGUILocalizationManager::Instance()->Localize( wx2GuiString(m_strLocalizedString )));
}
//------------------------------------------------------------------------------
