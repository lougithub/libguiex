/** 
* @file wximageepreviewpanelbase.h
* @brief 
* @author ken
* @date 2011-09-10
*/


#ifndef __KEN_IMAGE_PREVIEWPANEL_20110910_H__
#define __KEN_IMAGE_PREVIEWPANEL_20110910_H__

//============================================================================//
// include
//============================================================================// 
#include "wxresourcepreviewpanelbase.h"


//============================================================================//
// class
//============================================================================// 
class WxImageCanvas : public WxResourceCanvasBase
{
public:
	WxImageCanvas( wxWindow *parent );
	~WxImageCanvas();

	void SetImageName( const guiex::CGUIString& rImageName );
	void SetUVRect( const wxRect& rTargetRect );

protected:
	virtual void OnUpdate( float fDeltaTime );
	virtual void OnRender( guiex::IGUIInterfaceRender* pRender );

	void ClearImage();

protected:
	guiex::CGUIString m_strImageName;
	guiex::CGUIImage* m_pGUIImage;
	wxRect m_aUVRect;
};

class WxImagePreviewPanel : public WxResourcePreviewPanelBase
{
public:
	WxImagePreviewPanel( wxWindow* parent );

	virtual void SetResourceName( const wxString& rResourceName ) ;

protected:
	WxImageCanvas* m_pImageCanvas;
	WxImageCanvas* m_pFullImageCanvas;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_IMAGE_PREVIEWPANEL_20110910_H__
