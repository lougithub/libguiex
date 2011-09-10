/** 
* @file wxanimationpreviewpanel.h
* @brief 
* @author ken
* @date 2011-09-10
*/

#ifndef __KEN_ANIMATION_PREVIEWPANEL_20110910_H__
#define __KEN_ANIMATION_PREVIEWPANEL_20110910_H__

//============================================================================//
// include
//============================================================================//
#include "wxresourcepreviewpanelbase.h"


//============================================================================//
// class
//============================================================================// 

class WxAnimationPreviewPanel: public WxResourcePreviewPanelBase
{
public:
	WxAnimationPreviewPanel( wxWindow *parent );
	~WxAnimationPreviewPanel();

	void OnPaint( wxPaintEvent &event );
	void OnTimer(wxTimerEvent& event);

	virtual void SetResourceName( const wxString& rResourceName );

protected:
	void ClearBitmapData();

protected:
	wxTimer m_timer;

	const guiex::CGUIAnimationData* m_pAnimData;

	std::vector<wxBitmap*> m_vecBitmaps;
	guiex::uint32 m_nCurrentFrame;
	float m_fElapsedTime;

	DECLARE_EVENT_TABLE()
};

#endif //__KEN_ANIMATION_PREVIEWPANEL_20110910_H__
