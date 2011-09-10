/** 
* @file wxsoundpreviewpanel.cpp
* @author ken
* @date 2011-09-10
*/


//============================================================================//
// include
//============================================================================// 
#include "wxsoundpreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 



//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxSoundPreviewPanel, WxResourcePreviewPanelBase )
EVT_BUTTON( ID_SoundSelect_BTN_PLAY, WxSoundPreviewPanel::OnPlay )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxSoundPreviewPanel::WxSoundPreviewPanel( wxWindow* parent )
:WxResourcePreviewPanelBase( parent )
{
	wxButton *pBtnPlay = new wxButton( this, ID_SoundSelect_BTN_PLAY, wxT("Play") );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( pBtnPlay, 0, wxALIGN_CENTER );
	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxSoundPreviewPanel::SetResourceName( const wxString& rResourceName )
{
	m_strResourceName = rResourceName;
}
//------------------------------------------------------------------------------
void WxSoundPreviewPanel::OnPlay(wxCommandEvent& event)
{
	if( !m_strResourceName.empty())
	{
		CGUIInterfaceManager::Instance()->GetInterfaceSound()->PlayEffect( wx2GuiString(m_strResourceName ) );
	}
}
//------------------------------------------------------------------------------
