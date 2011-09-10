/** 
* @file wxanimationpreviewpanel.cpp
* @brief 
* @author ken
* @date 2011-09-10
*/


//============================================================================//
// include
//============================================================================// 
#include "wxanimationpreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"
#include <wx/filename.h> 

//============================================================================//
// function
//============================================================================// 
BEGIN_EVENT_TABLE(WxAnimationPreviewPanel, WxResourcePreviewPanelBase)
EVT_PAINT(WxAnimationPreviewPanel::OnPaint)
EVT_TIMER(110, WxAnimationPreviewPanel::OnTimer)
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxAnimationPreviewPanel::WxAnimationPreviewPanel( wxWindow *parent )
: WxResourcePreviewPanelBase( parent )
, m_pAnimData( NULL )
, m_nCurrentFrame(0)
, m_fElapsedTime(0.0f)
, m_timer(this, 110)
{
}
//------------------------------------------------------------------------------
WxAnimationPreviewPanel::~WxAnimationPreviewPanel()
{
	ClearBitmapData();
}
//------------------------------------------------------------------------------
void WxAnimationPreviewPanel::SetResourceName( const wxString& rResourceName )
{
	if( m_strResourceName != rResourceName )
	{
		m_strResourceName = rResourceName;

		ClearBitmapData();

		m_pAnimData = CGUIResourcePool::Instance()->GetAnimationData(  m_strResourceName );
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
}
//------------------------------------------------------------------------------
void WxAnimationPreviewPanel::ClearBitmapData()
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
void WxAnimationPreviewPanel::OnTimer(wxTimerEvent& event)
{
	m_nCurrentFrame = (m_nCurrentFrame+1) % m_vecBitmaps.size();
	Refresh();
}
//------------------------------------------------------------------------------
void WxAnimationPreviewPanel::OnPaint( wxPaintEvent &WXUNUSED(event) )
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
