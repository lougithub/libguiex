/** 
* @file savefiledlg.cpp
* @brief the dialog to help save this to file
* @author ken
* @date 2007-09-25
*/

//============================================================================//
// include
//============================================================================// 
#include "wxsavefiledlg.h"
#include "wxeditorid.h"

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxSaveFileDialog, wxDialog )
EVT_BUTTON( ID_SaveFile_BTN_OK, WxSaveFileDialog::OnOK )
EVT_BUTTON( ID_SaveFile_BTN_NO, WxSaveFileDialog::OnNO )
EVT_BUTTON( ID_SaveFile_BTN_CANCEL, WxSaveFileDialog::OnCANCEL )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxSaveFileDialog::WxSaveFileDialog(wxWindow* parent, const wxArrayString& rFileArray)
:wxDialog(parent, wxID_ANY, _T("libguiex editor"), wxDefaultPosition,wxSize(800,600), wxDEFAULT_DIALOG_STYLE )
{
	wxStaticText* pStatic = new wxStaticText(this, wxID_ANY, wxT("Save changes to the following items?"),
		wxDefaultPosition, wxDefaultSize, 0);
	wxListBox* pList = new wxListBox(this, wxID_ANY, wxDefaultPosition, 
		wxDefaultSize, rFileArray,wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT);

	wxButton *pBtnOk = new wxButton( this, ID_SaveFile_BTN_OK, wxT("OK"));
	wxButton *pBtnNo = new wxButton( this, ID_SaveFile_BTN_NO, wxT("NO"));
	wxButton *pBtnCancel = new wxButton( this, ID_SaveFile_BTN_CANCEL, wxT("CANCEL"));

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( pStatic, 0, wxALL|wxEXPAND );
	sizerTop->Add( pList, 1, wxALL|wxEXPAND );
	wxSizer *sizerRow = new wxBoxSizer( wxHORIZONTAL );
	sizerRow->Add( pBtnOk, 0 );
	sizerRow->AddSpacer( 10 );
	sizerRow->Add( pBtnNo, 0 );
	sizerRow->AddSpacer( 10 );
	sizerRow->Add( pBtnCancel );
	sizerTop->Add( sizerRow, 0, wxALIGN_CENTER_HORIZONTAL );

	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxSaveFileDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_OK);
	EndModal(wxID_OK);
} 
//------------------------------------------------------------------------------
void WxSaveFileDialog::OnCANCEL(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_CANCEL);
	EndModal(wxID_CANCEL);
}
//------------------------------------------------------------------------------
void WxSaveFileDialog::OnNO(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_NO);
	EndModal(wxID_NO);
}
//------------------------------------------------------------------------------
