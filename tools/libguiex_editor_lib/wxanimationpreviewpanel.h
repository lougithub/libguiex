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
class WxAnimationCanvas : public WxResourceCanvasBase
{
public:
	WxAnimationCanvas( wxWindow *parent );
	~WxAnimationCanvas();

	void SetAnimationName( const guiex::CGUIString& rImageName );

protected:
	virtual void OnUpdate( float fDeltaTime );
	virtual void OnRender( guiex::IGUIInterfaceRender* pRender );

	void ClearAnimation();

protected:
	guiex::CGUIString m_strAnimationName;
	guiex::CGUIAnimation* m_pGUIAnimation;
};

class WxAnimationPreviewPanel: public WxResourcePreviewPanelBase
{
public:
	WxAnimationPreviewPanel( wxWindow *parent );

	virtual void SetResourceName( const wxString& rResourceName );

protected:
	WxAnimationCanvas* m_pAnimationCanvas;

	DECLARE_EVENT_TABLE()
};

#endif //__KEN_ANIMATION_PREVIEWPANEL_20110910_H__
