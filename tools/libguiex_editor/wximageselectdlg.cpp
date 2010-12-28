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

//============================================================================//
// function
//============================================================================// 
BEGIN_EVENT_TABLE(WxImageCanvas, wxScrolledWindow)
EVT_PAINT(WxImageCanvas::OnPaint)
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxImageCanvas::WxImageCanvas( wxWindow *parent, wxWindowID id,
				   const wxPoint &pos, const wxSize &size )
				   : wxScrolledWindow( parent, id, pos, size, wxSUNKEN_BORDER )
				   , m_pBitmap( NULL )
{
}
//------------------------------------------------------------------------------
WxImageCanvas::~WxImageCanvas()
{

}
//------------------------------------------------------------------------------
void WxImageCanvas::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
	wxPaintDC dc( this );
	PrepareDC( dc );

	if( m_pBitmap )
	{
		dc.DrawBitmap( *m_pBitmap, 0, 0 );
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
	m_pImageCanvas = new WxImageCanvas( this, wxID_ANY, wxDefaultPosition, wxDefaultSize ); 

	wxButton *pBtnOk = new wxButton( this, ID_ImageSelect_BTN_OK, wxT("OK") );
	wxButton *pBtnCancel = new wxButton( this, ID_ImageSelect_BTN_CANCEL, wxT("CANCEL") );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );

	wxSizer *sizerRow0 = new wxBoxSizer( wxHORIZONTAL );
	sizerRow0->Add( m_pImageCanvas, 1, wxALL|wxEXPAND );
	sizerRow0->Add( m_pListBox, 1, wxALL|wxEXPAND );
	sizerTop->Add( sizerRow0, 1, wxALL|wxEXPAND );

	wxSizer *sizerRow1 = new wxBoxSizer( wxHORIZONTAL );
	sizerRow1->Add( pBtnOk, 0, wxALIGN_CENTER );
	sizerRow1->AddSpacer( 10 );
	sizerRow1->Add( pBtnCancel, 0, wxALIGN_CENTER );
	sizerTop->Add( sizerRow1, 0, wxALIGN_CENTER_HORIZONTAL);

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
	const wxBitmap* pBitmap = CResourceList::Instance()->GetOriginalImageThumbnail( m_strImageName );
	m_pImageCanvas->SetBitmap( pBitmap );
	m_pImageCanvas->Refresh();
}
//------------------------------------------------------------------------------
