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
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxFontSelectDialog, WxResourceSelectDialogBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxFontSelectDialog::WxFontSelectDialog( wxWindow* parent )
:WxResourceSelectDialogBase( parent, _T("select font"), CGUIResourcePool::Instance()->GetFontList())
{
	m_pTextBox = new wxTextCtrl( m_pShowPanel, wxID_ANY );
	m_pTextBox->SetEditable( false );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( m_pTextBox, 0, wxALIGN_CENTER );
	m_pShowPanel->SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxFontSelectDialog::OnListBoxSelect(wxCommandEvent& event)
{
	WxResourceSelectDialogBase::OnListBoxSelect( event );

	wxString strFontDesc = CGUIResourcePool::Instance()->GetFontDesc(  m_strResourceName );
	m_pTextBox->SetLabelText( strFontDesc );
}
//------------------------------------------------------------------------------
