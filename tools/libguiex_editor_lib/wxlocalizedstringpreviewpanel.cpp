/** 
* @file wxlocalizedstringpreviewpanel.cpp
* @author ken
* @date 2011-09-10
*/


//============================================================================//
// include
//============================================================================// 
#include "wxlocalizedstringpreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 



//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxLocalizedStringPreviewPanel, WxResourcePreviewPanelBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxLocalizedStringPreviewPanel::WxLocalizedStringPreviewPanel( wxWindow* parent )
:WxResourcePreviewPanelBase( parent )
{
	m_pTextBox = new wxTextCtrl( this, wxID_ANY );
	m_pTextBox->SetEditable( false );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( m_pTextBox, 1, wxALL|wxEXPAND );
	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxLocalizedStringPreviewPanel::SetResourceName( const wxString& rResourceName )
{
	m_strResourceName = rResourceName;
	if( !m_strResourceName.empty())
	{
		m_pTextBox->SetLabelText( (wxChar*)CGUILocalizationManager::Instance()->Localize( wx2GuiString(rResourceName)).c_str());
	}
}
//------------------------------------------------------------------------------
