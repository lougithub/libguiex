/** 
* @file wximageselectdlg.h
* @brief the dialog to select image file
* @author ken
* @date 2010-12-28
*/

#ifndef __KEN_IMAGE_SELECT_DLG_20101228_H__
#define __KEN_IMAGE_SELECT_DLG_20101228_H__

//============================================================================//
// include
//============================================================================// 
#include "wxresourceselectdlgbase.h"


//============================================================================//
// class
//============================================================================// 

class WxImageCanvas: public wxScrolledWindow
{
public:
	WxImageCanvas( wxWindow *parent, bool bDrawUVRect );
	~WxImageCanvas();

	void OnPaint( wxPaintEvent &event );

	void SetBitmap( wxBitmap* pBitmap );

	void SetUVRect( const wxRect& rTargetRect );


protected:
	wxBitmap* m_pBitmap;
	wxRect m_aUVRect;
	bool m_bDrawUVRect;

	DECLARE_EVENT_TABLE()
};


class WxImageSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxImageSelectDialog( wxWindow* parent );

protected:
	void OnListBoxSelect(wxCommandEvent& WXUNUSED(event));

protected:
	WxImageCanvas* m_pImageCanvas;
	WxImageCanvas* m_pFullImageCanvas;
private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_IMAGE_SELECT_DLG_20101228_H__
