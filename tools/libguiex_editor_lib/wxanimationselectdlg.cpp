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
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"
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
	for( uint32 i=0; i<uint32(m_vecBitmaps.size()); ++i)
	{
		delete m_vecBitmaps[i];
	}
	m_vecBitmaps.clear();
	m_nCurrentFrame = 0;
	m_fElapsedTime = 0.0f;

	m_timer.Stop();
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::SetAnimationData( guiex::CGUIAnimationData* pData )
{
	if( m_pAnimData != pData )
	{
		m_pAnimData = pData;
		UpdaterBitmapData();
	}
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
		m_vecBitmaps.push_back( CGUIResourcePool::Instance()->GenerateOriginalImageThumbnail(images[i]));
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
BEGIN_EVENT_TABLE( WxAnimationSelectDialog, WxResourceSelectDialogBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxAnimationSelectDialog::WxAnimationSelectDialog( wxWindow* parent )
:WxResourceSelectDialogBase( parent, _T("select animation"), CGUIResourcePool::Instance()->GetAnimationList() )
{
	m_pAnimationCanvas = new WxAnimationCanvas( m_pShowPanel ); 

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( m_pAnimationCanvas, 1, wxALL|wxEXPAND );
	m_pShowPanel->SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxAnimationSelectDialog::OnListBoxSelect(wxCommandEvent& event)
{
	WxResourceSelectDialogBase::OnListBoxSelect( event );

	//thumbnail
	CGUIAnimationData* pAnimationData = CGUIResourcePool::Instance()->GetAnimationData(  m_strResourceName );
	m_pAnimationCanvas->SetAnimationData( pAnimationData );
}
//------------------------------------------------------------------------------
