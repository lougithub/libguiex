/** 
* @file wxresourceselectdlgbase.cpp
* @brief 
* @author ken
* @date 2011-09-09
*/


//============================================================================//
// include
//============================================================================// 
#include "wxresourceselectdlgbase.h"
#include "wxresourcepreviewpanelbase.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 


//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxResourceSelectDialogBase, wxDialog )
EVT_BUTTON( ID_ResourceSelect_BTN_OK, WxResourceSelectDialogBase::OnOK )
EVT_BUTTON( ID_ResourceSelect_BTN_CANCEL, WxResourceSelectDialogBase::OnCANCEL )
EVT_LISTBOX( ID_ResourceListID, WxResourceSelectDialogBase::OnListBoxSelect )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxResourceSelectDialogBase::WxResourceSelectDialogBase( wxWindow* parent, const wxString& title, const wxArrayString& rResourceList )
:wxDialog( parent, wxID_ANY, title, wxDefaultPosition, wxSize(800,600), wxDEFAULT_DIALOG_STYLE/*wxNO_3D*/)
,m_pPreviewPanel(NULL)
{
	m_pListBox = new wxListBox(
		this, 
		ID_ResourceListID,
		wxDefaultPosition, 
		wxDefaultSize,
		rResourceList,
		wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT );
}
//------------------------------------------------------------------------------
void WxResourceSelectDialogBase::InitSelectDlg()
{

	wxButton *pBtnOk = new wxButton( this, ID_ResourceSelect_BTN_OK, wxT("OK") );
	wxButton *pBtnCancel = new wxButton( this, ID_ResourceSelect_BTN_CANCEL, wxT("CANCEL") );
	m_pPreviewPanel = GeneratePreviewPanel( this ); 

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	wxSizer *sizerRow0 = new wxBoxSizer( wxHORIZONTAL );
	wxSizer *sizerImages = new wxBoxSizer( wxVERTICAL );

	sizerImages->Add( m_pPreviewPanel, 1, wxALL|wxEXPAND );
	sizerRow0->Add( m_pListBox, 1, wxALL|wxEXPAND );
	sizerRow0->Add( sizerImages, 1, wxALL|wxEXPAND );
	sizerTop->Add( sizerRow0, 1, wxALL|wxEXPAND );

	wxSizer *sizerButtons = new wxBoxSizer( wxHORIZONTAL );
	sizerButtons->Add( pBtnOk, 0, wxALIGN_CENTER );
	sizerButtons->AddSpacer( 10 );
	sizerButtons->Add( pBtnCancel, 0, wxALIGN_CENTER );
	sizerTop->Add( sizerButtons, 0, wxALIGN_CENTER_HORIZONTAL);

	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
int WxResourceSelectDialogBase::ShowModal()
{
	if( !m_pPreviewPanel )
	{
		InitSelectDlg();
	}
	return wxDialog::ShowModal();
}
//------------------------------------------------------------------------------
void WxResourceSelectDialogBase::OnOK(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_OK);
	EndModal(wxID_OK);
} 
//------------------------------------------------------------------------------
void WxResourceSelectDialogBase::OnCANCEL(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_CANCEL);
	EndModal(wxID_CANCEL);
}
//------------------------------------------------------------------------------
void WxResourceSelectDialogBase::OnListBoxSelect(wxCommandEvent& event)
{
	long sel = event.GetSelection();
	m_strResourceName = m_pListBox->GetString( sel );
	m_pPreviewPanel->SetResourceName( m_strResourceName );
}
//------------------------------------------------------------------------------
const wxString& WxResourceSelectDialogBase::GetResourceName() const
{
	return m_strResourceName;
}
//------------------------------------------------------------------------------
