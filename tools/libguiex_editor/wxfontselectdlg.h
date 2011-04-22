/** 
* @file wxfontselectdlg.h
* @brief the dialog to select font.
* @author ken
* @date 2011-04-22
*/

#ifndef __KEN_FONT_SELECT_DLG_20110422_H__
#define __KEN_FONT_SELECT_DLG_20110422_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>

//============================================================================//
// class
//============================================================================// 

class WxFontSelectDialog : public wxDialog
{
public:
	WxFontSelectDialog( wxWindow* parent );

	const wxString& GetFontName() const
	{
		return m_strFontName;
	}

protected:
	void OnOK(wxCommandEvent& WXUNUSED(event));
	void OnCANCEL(wxCommandEvent& WXUNUSED(event));
	void OnListBoxSelect(wxCommandEvent& WXUNUSED(event));

protected:
	wxListBox* m_pListBox;
	wxTextCtrl* m_pTextBox;

	wxString m_strFontName;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_FONT_SELECT_DLG_20110422_H__
