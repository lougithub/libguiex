/** 
* @file wxsoundselectdlg.h
* @brief the dialog to select sound file
* @author ken
* @date 2010-12-28
*/

#ifndef __KEN_SOUND_SELECT_DLG_20110108_H__
#define __KEN_SOUND_SELECT_DLG_20110108_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>

//============================================================================//
// class
//============================================================================// 

class WxSoundSelectDialog : public wxDialog
{
public:
	WxSoundSelectDialog( wxWindow* parent );

	const wxString& GetSoundName() const
	{
		return m_strSoundName;
	}

protected:
	void OnOK(wxCommandEvent& WXUNUSED(event));
	void OnCANCEL(wxCommandEvent& WXUNUSED(event));
	void OnPlay(wxCommandEvent& WXUNUSED(event));
	void OnListBoxSelect(wxCommandEvent& WXUNUSED(event));

protected:
	wxListBox* m_pListBox;
	wxString m_strSoundName;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_SOUND_SELECT_DLG_20110108_H__
