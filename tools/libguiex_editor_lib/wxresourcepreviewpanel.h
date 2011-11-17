/** 
* @file wxresourcepreviewpanel.h
* @brief 
* @author ken
* @date 2011-09-10
*/

#ifndef __KEN_RESOURCE_PREVIEWPANEL_20110910_H__
#define __KEN_RESOURCE_PREVIEWPANEL_20110910_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include <wx/glcanvas.h>
#include "libguiex_core/guiex.h"

//============================================================================//
// class
//============================================================================// 



//============================================================================//
// WxResourcePreviewPanelBase
//============================================================================// 
class WxResourcePreviewPanelBase : public wxScrolledWindow
{
public:
	WxResourcePreviewPanelBase( wxWindow* parent, const guiex::CGUIString& rResourceType, bool bSupportCanvas );
	virtual void SetResourceName( const wxString& rResourceName ) = 0;
	const guiex::CGUIString& GetResourceType( ) const;

	bool IsSupportCanvas() const
	{
		return m_bIsSupportCanvas;
	}

protected:
	wxString m_strResourceName;
	bool m_bIsSupportCanvas;

private:
	guiex::CGUIString m_strResourceType;
};



//============================================================================//
// WxAnimationPreviewPanel
//============================================================================// 
class WxAnimationPreviewPanel: public WxResourcePreviewPanelBase
{
public:
	WxAnimationPreviewPanel( wxWindow *parent );

	virtual void SetResourceName( const wxString& rResourceName );

protected:
	class WxAnimationCanvas* m_pAnimationCanvas;
};


//============================================================================//
// WxFontPreviewPanel
//============================================================================// 
class WxFontPreviewPanel : public WxResourcePreviewPanelBase
{
public:
	WxFontPreviewPanel( wxWindow* parent );

	virtual void SetResourceName( const wxString& rResourceName );

protected:
	wxTextCtrl* m_pTextBox;
};


//============================================================================//
// WxImagePreviewPanel
//============================================================================// 
class WxImagePreviewPanel : public WxResourcePreviewPanelBase
{
public:
	WxImagePreviewPanel( wxWindow* parent );

	virtual void SetResourceName( const wxString& rResourceName ) ;

protected:
	class WxImageCanvas* m_pImageCanvas;
	class WxImageCanvas* m_pFullImageCanvas;
};



//============================================================================//
// WxLocalizedStringPreviewPanel
//============================================================================// 
class WxLocalizedStringPreviewPanel : public WxResourcePreviewPanelBase
{
public:
	WxLocalizedStringPreviewPanel( wxWindow* parent );

	virtual void SetResourceName( const wxString& rResourceName );

protected:
	wxTextCtrl* m_pTextBox;
};



//============================================================================//
// WxSoundPreviewPanel
//============================================================================// 
class WxSoundPreviewPanel : public WxResourcePreviewPanelBase
{
public:
	WxSoundPreviewPanel( wxWindow* parent );

	virtual void SetResourceName( const wxString& rResourceName );

protected:
	void OnPlay(wxCommandEvent& WXUNUSED(event));

private:
	DECLARE_EVENT_TABLE()
};


//============================================================================//
// WxDefaultPreviewPanel
//============================================================================// 
class WxDefaultPreviewPanel : public WxResourcePreviewPanelBase
{
public:
	WxDefaultPreviewPanel( wxWindow* parent );

	virtual void SetResourceName( const wxString& rResourceName );

protected:
	wxTextCtrl* m_pTextBox;
};

//============================================================================//
// context
//============================================================================// 
class WxGLContextRef
{
public:
	static void CreateGLContext( wxGLCanvas* pCanvas );
	static void DestroyGLContext( );
	static wxGLContext* GetGLContext();

protected:
	static wxGLContext* m_pGLContext;
	static guiex::uint32 m_uContextCount;
};



#endif //__KEN_RESOURCE_PREVIEWPANEL_20110910_H__
