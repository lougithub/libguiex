/** 
* @file wxanimationselectdlg.cpp
* @brief the dialog to select animation file
* @author ken
* @date 2011-09-07
*/

//============================================================================//
// include
//============================================================================// 
#include "wxanimationselectdlg.h"
#include "resourcelist.h"
#include "wxeditorid.h"
#include "editorutility.h"
#include <wx/filename.h> 

//============================================================================//
// function
//============================================================================// 
BEGIN_EVENT_TABLE(WxAnimationCanvas, wxScrolledWindow)
EVT_PAINT(WxAnimationCanvas::OnPaint)
EVT_TIMER(110, WxAnimationCanvas::OnTimer)
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxAnimationCanvas::WxAnimationCanvas( wxWindow *parent )
				   : wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER )
				   , m_pAnimData( NULL )
					, m_nCurrentFrame(0)
					, m_fElapsedTime(0.0f)
					,m_timer(this, 110)
{
}
//------------------------------------------------------------------------------
WxAnimationCanvas::~WxAnimationCanvas()
{
	ClearBitmapData();
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::ClearBitmapData()
{
	m_vecBitmaps.clear();
	m_nCurrentFrame = 0;
	m_fElapsedTime = 0.0f;

	m_timer.Stop();
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::UpdaterBitmapData()
{
	ClearBitmapData();
	if( !m_pAnimData )
	{
		return;
	}
	const std::vector<CGUIString>& images = m_pAnimData->GetImages();

	for( uint32 i=0; i<uint32(images.size()); ++i)
	{
		m_vecBitmaps.push_back( CResourceList::Instance()->GetOriginalImageThumbnail(images[i]));
	}

	m_timer.Start( m_pAnimData->GetInterval() * 1000 );
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::OnTimer(wxTimerEvent& event)
{
	m_nCurrentFrame = (m_nCurrentFrame+1) % m_vecBitmaps.size();
	Refresh();
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
	wxPaintDC dc( this );
	PrepareDC( dc );

	if( m_pAnimData )
	{
		wxBitmap* pCurrentBitmap = m_vecBitmaps[m_nCurrentFrame];
		dc.SetBackground(wxBrush(*wxBLACK, wxSOLID));

		wxSize aCanvasSize = GetSize();
		wxSize aBitmapSize = pCurrentBitmap->GetSize();
		wxPoint aDrawPoint( 0, 0 );
		if( aCanvasSize.GetWidth() > aBitmapSize.GetWidth() )
		{
			aDrawPoint.x = (aCanvasSize.GetWidth() - aBitmapSize.GetWidth()) / 2;
		}
		if( aCanvasSize.GetHeight() > aBitmapSize.GetHeight() )
		{
			aDrawPoint.y = (aCanvasSize.GetHeight() - aBitmapSize.GetHeight()) / 2;
		}
		dc.DrawBitmap( *pCurrentBitmap, aDrawPoint );
	}

}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxAnimationSelectDialog, wxDialog )
EVT_BUTTON( ID_ImageSelect_BTN_OK, WxAnimationSelectDialog::OnOK )
EVT_BUTTON( ID_ImageSelect_BTN_CANCEL, WxAnimationSelectDialog::OnCANCEL )
EVT_LISTBOX( ID_ImageListID, WxAnimationSelectDialog::OnListBoxSelect )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxAnimationSelectDialog::WxAnimationSelectDialog( wxWindow* parent )
:wxDialog( parent, wxID_ANY, _T("select animation"), wxDefaultPosition,wxSize(800,600), wxDEFAULT_DIALOG_STYLE/*wxNO_3D*/)
{
	m_pListBox = new wxListBox(
		this, 
		ID_ImageListID,
		wxDefaultPosition, 
		wxDefaultSize,
		CResourceList::Instance()->GetAnimationList(),
		wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT );
	m_pAnimationCanvas = new WxAnimationCanvas( this ); 

	wxButton *pBtnOk = new wxButton( this, ID_ImageSelect_BTN_OK, wxT("OK") );
	wxButton *pBtnCancel = new wxButton( this, ID_ImageSelect_BTN_CANCEL, wxT("CANCEL") );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );

	wxSizer *sizerRow0 = new wxBoxSizer( wxHORIZONTAL );
	wxSizer *sizerImages = new wxBoxSizer( wxVERTICAL );

	sizerImages->Add( m_pAnimationCanvas, 1, wxALL|wxEXPAND );

	sizerRow0->Add( sizerImages, 1, wxALL|wxEXPAND );
	sizerRow0->Add( m_pListBox, 1, wxALL|wxEXPAND );
	sizerTop->Add( sizerRow0, 1, wxALL|wxEXPAND );

	wxSizer *sizerButtons = new wxBoxSizer( wxHORIZONTAL );
	sizerButtons->Add( pBtnOk, 0, wxALIGN_CENTER );
	sizerButtons->AddSpacer( 10 );
	sizerButtons->Add( pBtnCancel, 0, wxALIGN_CENTER );
	sizerTop->Add( sizerButtons, 0, wxALIGN_CENTER_HORIZONTAL);

	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxAnimationSelectDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_OK);
	EndModal(wxID_OK);
} 
//------------------------------------------------------------------------------
void WxAnimationSelectDialog::OnCANCEL(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_CANCEL);
	EndModal(wxID_CANCEL);
}
//------------------------------------------------------------------------------
void WxAnimationSelectDialog::OnListBoxSelect(wxCommandEvent& event)
{
	long sel = event.GetSelection();
	m_strAnimationName = m_pListBox->GetString( sel );

	//thumbnail
	CGUIAnimationData* pAnimationData = CResourceList::Instance()->GetFullAnimationThumbnail(  m_strAnimationName );
	m_pAnimationCanvas->SetAnimationData( pAnimationData );
	m_pAnimationCanvas->Refresh();
}
//------------------------------------------------------------------------------
