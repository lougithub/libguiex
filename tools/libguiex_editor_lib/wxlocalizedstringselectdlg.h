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
#include "wxresourceselectdlgbase.h"

//============================================================================//
// class
//============================================================================// 

class WxLocalizedstringSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxLocalizedstringSelectDialog( wxWindow* parent );

protected:
	void OnListBoxSelect(wxCommandEvent& WXUNUSED(event));

protected:
	wxTextCtrl* m_pTextBox;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_LOCALIZATION_SELECT_DLG_20110406_H__
