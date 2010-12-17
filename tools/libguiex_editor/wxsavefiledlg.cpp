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

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxSaveFileDialog, wxDialog )
EVT_BUTTON( ID_DC_BTN_OK, WxSaveFileDialog::OnOK )
EVT_BUTTON( ID_DC_BTN_NO, WxSaveFileDialog::OnNO )
EVT_BUTTON( ID_DC_BTN_CANCEL, WxSaveFileDialog::OnCANCEL )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxSaveFileDialog::WxSaveFileDialog(wxWindow* parent, const wxArrayString& rFileArray)
:wxDialog(parent, wxID_ANY, _T("libguiex editor"), wxDefaultPosition,wxSize(463,265),
		  wxDEFAULT_DIALOG_STYLE/*wxNO_3D*/)
{
	wxStaticText* pStatic = new wxStaticText(this, wxID_ANY, wxT("Save changes to the following items?"),
		wxPoint(15,10), wxDefaultSize, 0);
	wxListBox* pList = new wxListBox(this, wxID_ANY, wxPoint(20, 35), 
		wxSize(418,148), rFileArray,wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT);

	wxButton *pBtnOk = new wxButton( this, ID_DC_BTN_OK, wxT("OK"), wxPoint(143, 197), wxSize(95,18));
	wxButton *pBtnNo = new wxButton( this, ID_DC_BTN_NO, wxT("NO"), wxPoint(243, 197), wxSize(95,18));
	wxButton *pBtnCancel = new wxButton( this, ID_DC_BTN_CANCEL, wxT("CANCEL"), wxPoint(343, 197),wxSize(95,18));
}
//------------------------------------------------------------------------------
void	WxSaveFileDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_OK);
	EndModal(wxID_OK);
} 
//------------------------------------------------------------------------------
void	WxSaveFileDialog::OnCANCEL(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_CANCEL);
	EndModal(wxID_CANCEL);
}
//------------------------------------------------------------------------------
void	WxSaveFileDialog::OnNO(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_NO);
	EndModal(wxID_NO);
}
//------------------------------------------------------------------------------
