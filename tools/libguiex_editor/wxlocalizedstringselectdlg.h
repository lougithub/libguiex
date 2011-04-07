/** 
* @file wxlocalizationselectdlg.h
* @brief the dialog to select localization.
* @author ken
* @date 2011-04-06
*/

#ifndef __KEN_LOCALIZATION_SELECT_DLG_20110406_H__
#define __KEN_LOCALIZATION_SELECT_DLG_20110406_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>

//============================================================================//
// class
//============================================================================// 

class WxLocalizedstringSelectDialog : public wxDialog
{
public:
	WxLocalizedstringSelectDialog( wxWindow* parent );

	const wxString& GetLocalzedString() const
	{
		return m_strLocalizedString;
	}

protected:
	void OnOK(wxCommandEvent& WXUNUSED(event));
	void OnCANCEL(wxCommandEvent& WXUNUSED(event));
	void OnListBoxSelect(wxCommandEvent& WXUNUSED(event));

protected:
	wxListBox* m_pListBox;
	wxTextCtrl* m_pTextBox;

	wxString m_strLocalizedString;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_LOCALIZATION_SELECT_DLG_20110406_H__
