/** 
* @file wxfontpreviewpanel.cpp
* @author ken
* @date 2011-09-10
*/

//============================================================================//
// include
//============================================================================// 
#include "wxfontpreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxFontPreviewPanel, WxResourcePreviewPanelBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxFontPreviewPanel::WxFontPreviewPanel( wxWindow* parent )
:WxResourcePreviewPanelBase( parent )
{
	m_pTextBox = new wxTextCtrl( this, wxID_ANY );
	m_pTextBox->SetEditable( false );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( this, 0, wxALIGN_CENTER );
	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxFontPreviewPanel::SetResourceName( const wxString& rResourceName )
{
	if( m_strResourceName != rResourceName )
	{
		m_strResourceName = rResourceName;
		wxString strFontDesc = CGUIResourcePool::Instance()->GetFontDesc(  m_strResourceName );
		m_pTextBox->SetLabelText( strFontDesc );
	}
}
//------------------------------------------------------------------------------
