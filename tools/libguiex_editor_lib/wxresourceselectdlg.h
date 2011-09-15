/** 
* @file wxresourceselectdlg.h
* @brief 
* @author ken
* @date 2011-09-09
*/

#ifndef __KEN_RESOURCE_SELECT_DLG_20110909_H__
#define __KEN_RESOURCE_SELECT_DLG_20110909_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include "libguiex_core/guiex.h"

//============================================================================//
// class
//============================================================================// 
class WxResourcePreviewPanelBase;


//============================================================================//
// WxResourceSelectDialogBase
//============================================================================// 
class WxResourceSelectDialogBase : public wxDialog
{
public:
	WxResourceSelectDialogBase( wxWindow* parent, const wxString& title, const wxArrayString& rResourceList );
	const wxString& GetResourceName() const;

	virtual int ShowModal();

protected:
	virtual void OnOK(wxCommandEvent& WXUNUSED(event));
	virtual void OnCANCEL(wxCommandEvent& WXUNUSED(event));
	virtual void OnListBoxSelect(wxCommandEvent& WXUNUSED(event));

	void InitSelectDlg();
	virtual WxResourcePreviewPanelBase* GeneratePreviewPanel( wxWindow* pParent ) = 0;

protected:
	WxResourcePreviewPanelBase* m_pPreviewPanel;
	wxListBox* m_pListBox;
	wxString m_strResourceName;

private:
	DECLARE_EVENT_TABLE()
};


//============================================================================//
// WxAnimationSelectDialog
//============================================================================// 
class WxAnimationSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxAnimationSelectDialog( wxWindow* parent );

protected:
	virtual WxResourcePreviewPanelBase* GeneratePreviewPanel( wxWindow* pParent );
};



//============================================================================//
// WxFontSelectDialog
//============================================================================// 
class WxFontSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxFontSelectDialog( wxWindow* parent );

protected:
	virtual WxResourcePreviewPanelBase* GeneratePreviewPanel( wxWindow* pParent );
};



//============================================================================//
// WxImageSelectDialog
//============================================================================//
class WxImageSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxImageSelectDialog( wxWindow* parent );

protected:
	virtual WxResourcePreviewPanelBase* GeneratePreviewPanel( wxWindow* pParent );
};

//============================================================================//
// WxLocalizedstringSelectDialog
//============================================================================// 
class WxLocalizedstringSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxLocalizedstringSelectDialog( wxWindow* parent );

protected:
	virtual WxResourcePreviewPanelBase* GeneratePreviewPanel( wxWindow* pParent );
};


//============================================================================//
// WxSoundSelectDialog
//============================================================================// 
class WxSoundSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxSoundSelectDialog( wxWindow* parent );

protected:
	virtual WxResourcePreviewPanelBase* GeneratePreviewPanel( wxWindow* pParent );
};



#endif //__KEN_RESOURCE_SELECT_DLG_20110909_H__
