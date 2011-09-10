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
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxLocalizedstringSelectDialog, WxResourceSelectDialogBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxLocalizedstringSelectDialog::WxLocalizedstringSelectDialog( wxWindow* parent )
:WxResourceSelectDialogBase( parent, _T("select localization"), CGUIResourcePool::Instance()->GetLocalizationList())
{
	m_pTextBox = new wxTextCtrl( m_pShowPanel, wxID_ANY );
	m_pTextBox->SetEditable( false );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( m_pTextBox, 1, wxALL|wxEXPAND );
	m_pShowPanel->SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxLocalizedstringSelectDialog::OnListBoxSelect(wxCommandEvent& event)
{
	WxResourceSelectDialogBase::OnListBoxSelect( event );

	m_pTextBox->SetLabelText( (wchar_t*)CGUILocalizationManager::Instance()->Localize( wx2GuiString(m_strResourceName )).c_str());
}
//------------------------------------------------------------------------------
