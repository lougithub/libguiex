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
	WxImageCanvas( wxWindow *parent, wxWindowID, const wxPoint &pos, const wxSize &size );
	~WxImageCanvas();

	void OnPaint( wxPaintEvent &event );

	void SetBitmap( const wxBitmap* pBitmap )
	{
		m_pBitmap = pBitmap;
	}

protected:
	const wxBitmap* m_pBitmap;

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
	wxListBox* m_pListBox;
	wxString m_strImageName;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_IMAGE_SELECT_DLG_20101228_H__
