/** 
* @file wxresourceselectdlgbase.h
* @brief 
* @author ken
* @date 2011-09-09
*/

#ifndef __KEN_RESOURCE_SELECT_DLG_BASE_20110909_H__
#define __KEN_RESOURCE_SELECT_DLG_BASE_20110909_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include "libguiex_core/guiex.h"

//============================================================================//
// class
//============================================================================// 

class WxResourceSelectDialogBase : public wxDialog
{
public:
	WxResourceSelectDialogBase( wxWindow* parent, const wxString& title, const wxArrayString& rResourceList );
	const wxString& GetResourceName() const;

protected:
	virtual void OnOK(wxCommandEvent& WXUNUSED(event));
	virtual void OnCANCEL(wxCommandEvent& WXUNUSED(event));
	virtual void OnListBoxSelect(wxCommandEvent& WXUNUSED(event));

protected:
	wxPanel* m_pShowPanel;
	wxListBox* m_pListBox;
	wxString m_strResourceName;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_RESOURCE_SELECT_DLG_BASE_20110909_H__
