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
#include <wx/glcanvas.h>

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



class WxResourceCanvasBase : public wxGLCanvas
{
public:
	WxResourceCanvasBase( wxWindow *parent );
	~WxResourceCanvasBase();

public:
	//event
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& event);

protected:
	virtual void OnUpdate( float fDeltaTime ) = 0;
	virtual void OnRender( guiex::IGUIInterfaceRender* pRender ) = 0;

protected:
	static int wx_gl_attribs[];
	wxTimer m_timer;
	wxGLContext* m_pGLContext;

protected:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_RESOURCE_PREVIEWPANEL_BASE_20110910_H__
