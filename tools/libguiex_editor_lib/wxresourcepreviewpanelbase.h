/** 
* @file wxresourcepreviewpanelbase.h
* @brief 
* @author ken
* @date 2011-09-10
*/

#ifndef __KEN_RESOURCE_PREVIEWPANEL_BASE_20110910_H__
#define __KEN_RESOURCE_PREVIEWPANEL_BASE_20110910_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include "libguiex_core/guiex.h"

//============================================================================//
// class
//============================================================================// 

class WxResourcePreviewPanelBase : public wxScrolledWindow
{
public:
	WxResourcePreviewPanelBase( wxWindow* parent );

	virtual void SetResourceName( const wxString& rResourceName ) = 0;

protected:

protected:
	wxString m_strResourceName;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_RESOURCE_PREVIEWPANEL_BASE_20110910_H__
