/** 
* @file wxsoundpreviewpanel.h
* @author ken
* @date 2011-09-10
*/

#ifndef __KEN_SOUND_PREVIEWPANEL_20110910_H__
#define __KEN_SOUND_PREVIEWPANEL_20110910_H__

//============================================================================//
// include
//============================================================================// 
#include "wxresourcepreviewpanelbase.h"

//============================================================================//
// class
//============================================================================// 

class WxSoundPreviewPanel : public WxResourcePreviewPanelBase
{
public:
	WxSoundPreviewPanel( wxWindow* parent );

	virtual void SetResourceName( const wxString& rResourceName );

protected:
	void OnPlay(wxCommandEvent& WXUNUSED(event));

protected:

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_SOUND_PREVIEWPANEL_20110910_H__
