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
#include <wx/wxprec.h>

//============================================================================//
// class
//============================================================================// 

class WxImageCanvas: public wxScrolledWindow
{
public:
	WxImageCanvas( wxWindow *parent, bool bAutoDelete, bool bDrawUVRect );
	~WxImageCanvas();

	void OnPaint( wxPaintEvent &event );

	void SetBitmap( wxBitmap* pBitmap )
	{
		if( m_pBitmap && m_bAutoDelete )
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
		m_pBitmap = pBitmap;
	}

	void SetUVRect( const wxRect& rTargetRect )
	{
		m_aUVRect = rTargetRect;
	}


protected:
	wxBitmap* m_pBitmap;
	bool m_bAutoDelete;
	wxRect m_aUVRect;
	bool m_bDrawUVRect;

	DECLARE_EVENT_TABLE()
};


class WxImageSelectDialog : public wxDialog
{
public:
	WxImageSelectDialog( wxWindow* parent );

	const wxString& GetImageName() const
	{
		return m_strImageName;
	}

protected:
	void OnOK(wxCommandEvent& WXUNUSED(event));
	void OnCANCEL(wxCommandEvent& WXUNUSED(event));
	void OnListBoxSelect(wxCommandEvent& WXUNUSED(event));

protected:
	WxImageCanvas* m_pImageCanvas;
	WxImageCanvas* m_pFullImageCanvas;
	wxListBox* m_pListBox;
	wxString m_strImageName;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_IMAGE_SELECT_DLG_20101228_H__
