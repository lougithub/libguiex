/** 
* @file wximageepreviewpanelbase.h
* @brief 
* @author ken
* @date 2011-09-10
*/


#ifndef __KEN_IMAGE_PREVIEWPANEL_20110910_H__
#define __KEN_IMAGE_PREVIEWPANEL_20110910_H__

//============================================================================//
// include
//============================================================================// 
#include "wxresourcepreviewpanelbase.h"


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


class WxImagePreviewPanel : public WxResourcePreviewPanelBase
{
public:
	WxImagePreviewPanel( wxWindow* parent );

	virtual void SetResourceName( const wxString& rResourceName ) ;

protected:
	WxImageCanvas* m_pImageCanvas;
	WxImageCanvas* m_pFullImageCanvas;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_IMAGE_PREVIEWPANEL_20110910_H__
