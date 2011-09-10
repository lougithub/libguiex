/** 
* @file wximageepreviewpanelbase.cpp
* @brief 
* @author ken
* @date 2011-09-10
*/

//============================================================================//
// include
//============================================================================// 
#include "wximagepreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"
#include <wx/filename.h> 

//============================================================================//
// function
//============================================================================// 
BEGIN_EVENT_TABLE(WxImageCanvas, wxScrolledWindow)
EVT_PAINT(WxImageCanvas::OnPaint)
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxImageCanvas::WxImageCanvas( wxWindow *parent, bool bDrawUVRect )
: wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER )
, m_pBitmap( NULL )
, m_bDrawUVRect( bDrawUVRect )
{
}
//------------------------------------------------------------------------------
WxImageCanvas::~WxImageCanvas()
{
	if( m_pBitmap )
	{
		delete m_pBitmap;
	}
}
//------------------------------------------------------------------------------
void WxImageCanvas::SetBitmap( wxBitmap* pBitmap )
{
	if( m_pBitmap )
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	m_pBitmap = pBitmap;

	Refresh();
}
//------------------------------------------------------------------------------
void WxImageCanvas::SetUVRect( const wxRect& rTargetRect )
{
	m_aUVRect = rTargetRect;
}
//------------------------------------------------------------------------------
void WxImageCanvas::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
	wxPaintDC dc( this );
	PrepareDC( dc );

	if( m_pBitmap )
	{
		dc.SetBackground(wxBrush(*wxBLACK, wxSOLID));

		wxSize aCanvasSize = GetSize();
		wxSize aBitmapSize = m_pBitmap->GetSize();
		wxPoint aDrawPoint( 0, 0 );
		if( aCanvasSize.GetWidth() > aBitmapSize.GetWidth() )
		{
			aDrawPoint.x = (aCanvasSize.GetWidth() - aBitmapSize.GetWidth()) / 2;
		}
		if( aCanvasSize.GetHeight() > aBitmapSize.GetHeight() )
		{
			aDrawPoint.y = (aCanvasSize.GetHeight() - aBitmapSize.GetHeight()) / 2;
		}
		dc.DrawBitmap( *m_pBitmap, aDrawPoint );

		dc.SetPen(wxPen(*wxGREEN, 2));
		dc.SetBrush(wxBrush(*wxBLACK, wxTRANSPARENT));
		dc.DrawRectangle( aDrawPoint, aBitmapSize );

		if( m_bDrawUVRect )
		{
			dc.SetPen(wxPen(*wxRED, 1));
			wxRect aUVRect( m_aUVRect.GetPosition() + aDrawPoint, m_aUVRect.GetSize() );
			dc.DrawRectangle( aUVRect );
		}
	}
}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxImagePreviewPanel, WxResourcePreviewPanelBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxImagePreviewPanel::WxImagePreviewPanel( wxWindow* parent )
:WxResourcePreviewPanelBase( parent )
{
	m_pImageCanvas = new WxImageCanvas( this, false ); 
	m_pFullImageCanvas = new WxImageCanvas( this, true ); 

	wxSizer *sizerImages = new wxBoxSizer( wxVERTICAL );
	sizerImages->Add( m_pImageCanvas, 1, wxALL|wxEXPAND );
	sizerImages->Add( m_pFullImageCanvas, 1, wxALL|wxEXPAND );
	SetSizer( sizerImages );

	Show(true);
}
//------------------------------------------------------------------------------
void WxImagePreviewPanel::SetResourceName( const wxString& rResourceName ) 
{
	if( m_strResourceName == rResourceName )
	{
		return;
	}
	m_strResourceName = rResourceName;

	//thumbnail
	wxBitmap* pBitmap = CGUIResourcePool::Instance()->GenerateOriginalImageThumbnail(  m_strResourceName );
	m_pImageCanvas->SetBitmap( pBitmap );

	//full image
	m_pFullImageCanvas->SetBitmap( NULL );
	if( !m_strResourceName.empty() )
	{
		CGUIImage* pImage = CGUIImageManager::Instance()->AllocateResource( wx2GuiString( m_strResourceName ) );
		if( pImage && pImage->GetImageType() == eImageType_FromFile )
		{
			wxImage* pWxImage = LoadwxImageByGuiImage( pImage );
			if( pWxImage )
			{
				wxBitmap* pOriginalBitmap = new wxBitmap( *pWxImage );
				delete pWxImage;
				pWxImage = NULL;

				m_pFullImageCanvas->SetBitmap( pOriginalBitmap );
				const CGUIRect& rRect = pImage->GetUVRect();
				wxSize newSize( 
					pOriginalBitmap->GetSize().GetWidth() * rRect.GetWidth(), 
					pOriginalBitmap->GetSize().GetHeight()* rRect.GetHeight() );
				wxPoint newPoint( 
					rRect.GetPosition().x * pOriginalBitmap->GetSize().GetWidth(), 
					rRect.GetPosition().y * pOriginalBitmap->GetSize().GetHeight() );
				wxRect aUVRect( newPoint, newSize );
				m_pFullImageCanvas->SetUVRect( aUVRect );
			}
		}
		if( pImage )
		{
			pImage->RefRelease();
		}
	}

	m_pFullImageCanvas->Refresh();
}
//------------------------------------------------------------------------------
