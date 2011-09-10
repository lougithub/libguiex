/** 
* @file wxlocalizedstringpreviewpanel.h
* @author ken
* @date 2011-09-10
*/

#ifndef __KEN_LOCALIZATION_PREVIEWPANEL_20110910_H__
#define __KEN_LOCALIZATION_PREVIEWPANEL_20110910_H__

//============================================================================//
// include
//============================================================================// 
#include "wxresourcepreviewpanelbase.h"

//============================================================================//
// class
//============================================================================// 

class WxLocalizedStringPreviewPanel : public WxResourcePreviewPanelBase
{
public:
	WxLocalizedStringPreviewPanel( wxWindow* parent );

	virtual void SetResourceName( const wxString& rResourceName );

protected:
	wxTextCtrl* m_pTextBox;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_LOCALIZATION_PREVIEWPANEL_20110910_H__
