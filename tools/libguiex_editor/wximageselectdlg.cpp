/** 
* @file savefiledlg.cpp
* @brief the dialog to help save this to file
* @author ken
* @date 2007-09-25
*/

//============================================================================//
// include
//============================================================================// 
#include "wximageselectdlg.h"
#include "resourcelist.h"
#include "wxeditorid.h"
#include "editorutility.h"
#include <wx/filename.h> 

//============================================================================//
// function
//============================================================================// 
BEGIN_EVENT_TABLE(WxImageCanvas, wxScrolledWindow)
EVT_PAINT(WxImageCanvas::OnPaint)
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxImageCanvas::WxImageCanvas( wxWindow *parent, bool bAutoDelete, bool bDrawUVRect )
				   : wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER )
				   , m_pBitmap( NULL )
				   , m_bAutoDelete( bAutoDelete )
				   , m_bDrawUVRect( bDrawUVRect )
{
}
//------------------------------------------------------------------------------
WxImageCanvas::~WxImageCanvas()
{
	if( m_bAutoDelete )
	{
		if( m_pBitmap )
		{
			delete m_pBitmap;
		}
	}
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
BEGIN_EVENT_TABLE( WxImageSelectDialog, wxDialog )
EVT_BUTTON( ID_ImageSelect_BTN_OK, WxImageSelectDialog::OnOK )
EVT_BUTTON( ID_ImageSelect_BTN_CANCEL, WxImageSelectDialog::OnCANCEL )
EVT_LISTBOX( ID_ImageListID, WxImageSelectDialog::OnListBoxSelect )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxImageSelectDialog::WxImageSelectDialog( wxWindow* parent )
:wxDialog( parent, wxID_ANY, _T("select image"), wxDefaultPosition,wxSize(800,600), wxDEFAULT_DIALOG_STYLE/*wxNO_3D*/)
{
	m_pListBox = new wxListBox(
		this, 
		ID_ImageListID,
		wxDefaultPosition, 
		wxDefaultSize,
		CResourceList::Instance()->GetImageList(),
		wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SORT );
	m_pImageCanvas = new WxImageCanvas( this, false, false ); 
	m_pFullImageCanvas = new WxImageCanvas( this, true, true ); 

	wxButton *pBtnOk = new wxButton( this, ID_ImageSelect_BTN_OK, wxT("OK") );
	wxButton *pBtnCancel = new wxButton( this, ID_ImageSelect_BTN_CANCEL, wxT("CANCEL") );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );

	wxSizer *sizerRow0 = new wxBoxSizer( wxHORIZONTAL );
	wxSizer *sizerImages = new wxBoxSizer( wxVERTICAL );

	sizerImages->Add( m_pImageCanvas, 1, wxALL|wxEXPAND );
	sizerImages->Add( m_pFullImageCanvas, 1, wxALL|wxEXPAND );

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
void WxImageSelectDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_OK);
	EndModal(wxID_OK);
} 
//------------------------------------------------------------------------------
void WxImageSelectDialog::OnCANCEL(wxCommandEvent& WXUNUSED(event))
{
	SetReturnCode(wxID_CANCEL);
	EndModal(wxID_CANCEL);
}
//------------------------------------------------------------------------------
void WxImageSelectDialog::OnListBoxSelect(wxCommandEvent& event)
{
	long sel = event.GetSelection();
	m_strImageName = m_pListBox->GetString( sel );

	//thumbnail
	wxBitmap* pBitmap = CResourceList::Instance()->GetOriginalImageThumbnail(  m_strImageName );
	m_pImageCanvas->SetBitmap( pBitmap );
	m_pImageCanvas->Refresh();

	//full image
	m_pFullImageCanvas->SetBitmap( NULL );
	if( !m_strImageName.empty() )
	{
		CGUIImage* pImage = CGUIImageManager::Instance()->AllocateResource( wx2GuiString( m_strImageName ) );
		if( pImage )
		{
			if( pImage->GetImageType() == eImageType_FromFile )
			{
				wxString rImagePath = Gui2wxString( GSystem->GetDataPath() + pImage->GetFullFilePath() );
				wxFileName filename( rImagePath );
				if ( filename.FileExists() )
				{
					wxImage* pWxImage = new wxImage( filename.GetFullPath(), wxBITMAP_TYPE_TGA );
					if ( pWxImage && pWxImage->Ok() )
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
			}
			pImage->RefRelease();
		}
	}

	m_pFullImageCanvas->Refresh();
}
//------------------------------------------------------------------------------
