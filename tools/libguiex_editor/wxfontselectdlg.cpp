/** 
* @file wxfontselectdlg.cpp
* @brief the dialog to select font.
* @author ken
* @date 2011-04-22
*/

//============================================================================//
// include
//============================================================================// 
#include "wxfontselectdlg.h"
#include "resourcelist.h"
#include "wxeditorid.h"
#include "editorutility.h"

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxFontSelectDialog, wxDialog )
EVT_BUTTON( ID_FontSelect_BTN_OK, WxFontSelectDialog::OnOK )
EVT_BUTTON( ID_FontSelect_BTN_CANCEL, WxFontSelectDialog::OnCANCEL )
EVT_LISTBOX( ID_FontListID, WxFontSelectDialog::OnListBoxSelect )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxFontSelectDialog::WxFontSelectDialog( wxWindow* parent )
:wxDialog( parent, wxID_ANY, _T("select font"), wxDefaultPosition,wxSize(800,600), wxDEFAULT_DIALOG_STYLE/*wxNO_3D*/)
{
	m_pListBox = new wxListBox(
		this, 
		ID_FontListID,
		wxDefaultPosition, 
		wxDefaultSize,
		CResourceList::Instance()->GetFontList(),
		wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT );

	m_pTextBox = new wxTextCtrl( this, wxID_ANY );
	m_pTextBox->SetEditable( false );

	wxButton *pBtnOk = new wxButton( this, ID_FontSelect_BTN_OK, wxT("OK") );
	wxButton *pBtnCancel = new wxButton( this, ID_FontSelect_BTN_CANCEL, wxT("CANCEL") );

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
void WxFontSelectDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_OK);
	EndModal(wxID_OK);
} 
//------------------------------------------------------------------------------
void WxFontSelectDialog::OnCANCEL(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_CANCEL);
	EndModal(wxID_CANCEL);
}
//------------------------------------------------------------------------------
void WxFontSelectDialog::OnListBoxSelect(wxCommandEvent& event)
{
	long sel = event.GetSelection();
	m_strFontName = m_pListBox->GetString( sel );

	const wxString* pFontDesc = CResourceList::Instance()->GetFontDesc(  m_strFontName );
	if( pFontDesc )
	{
		m_pTextBox->SetLabelText( *pFontDesc );
	}
	else
	{
		m_pTextBox->SetLabelText( wxString() );
	}
}
//------------------------------------------------------------------------------
