/** 
 * @file editor_viewer.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-10-16
 */
#ifndef __EDITOR_VIEWER_20091016_H__
#define	__EDITOR_VIEWER_20091016_H__

// Disable the warning that the pack size is changed in this header.
#pragma warning(disable:4103)
#pragma warning(disable:4099)


//============================================================================//
// include
//============================================================================//
#include <wx/wxprec.h>
#include <wx/glcanvas.h>
#include <wx/aui/aui.h>
#include <wx/cmdline.h>
#include <wx/colordlg.h>
#include <wx/dirdlg.h>
#include <wx/choicdlg.h>
#include <wx/filename.h>

#include <libguiex_framework/guiframework.h>
using namespace guiex;

//============================================================================//
// class
//============================================================================//

//-----------------------------------------------------------------------------
//	WxMainApp
//-----------------------------------------------------------------------------
class WxMainApp : public wxApp
{
public:
	virtual bool OnInit();
};
DECLARE_APP(WxMainApp)


//-----------------------------------------------------------------------------
//	WxMainFrame
//-----------------------------------------------------------------------------
class WxMainFrame : public wxFrame
{
public:
	WxMainFrame(
		wxWindow* parent, 
		wxWindowID id, 
		const wxString& title, 
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);
	virtual ~WxMainFrame();

	void OutputString( const std::string& rString);

	enum
	{
		//file
		ID_Exit = wxID_HIGHEST+1,
		ID_About,
		ID_OpenPage,
		ID_AddPage,
		ID_ClosePage,

		//view
		ID_VIEW_800x600,
		ID_VIEW_1024x786,
		ID_VIEW_1280x800,
		ID_VIEW_Iphone480x320,
		ID_VIEW_Iphone320x480,
		ID_VIEW_Iphone960x640,
		ID_VIEW_Iphone640x960,
		ID_VIEW_Ipad1024x768,
		ID_VIEW_Ipad768x1024,
		ID_VIEW_Fullscreen,

		ID_ToggleScissor,
		ID_ToggleWireframe,
		ID_SetBGColor,
		ID_Refresh,
		ID_ToggleExtraInfo,
	};

protected:
	bool GetUIDataPath( CGUIString& rDataPath );
	bool GetOpenPageInfo( CGUIString& rScene, std::vector<CGUIString>& rPages );
	bool GetClosePageInfo( CGUIString& rScene, std::vector<CGUIString>& rPages );
	void OpenUIPage( bool bCheckCommandLine );
	void ReOpenPages( );

protected:
	//menu
	void OnExit(wxCommandEvent& evt);
	void OnAbout(wxCommandEvent& evt);
	void OnOpenPage(wxCommandEvent& evt);
	void OnClosePage(wxCommandEvent& evt);
	void OnAddPage(wxCommandEvent& evt);
	void OnFullscreen(wxCommandEvent& evt);
	void OnToggleScissor(wxCommandEvent& evt);
	void OnToggleExtraInfo(wxCommandEvent& evt);
	void OnToggleWireframe(wxCommandEvent& evt);
	void OnSetBGColor(wxCommandEvent& evt);
	void OnRefresh(wxCommandEvent& evt);

	void SetResolution( int width, int height );

	void On800x600(wxCommandEvent& evt);
	void On1024x786(wxCommandEvent& evt);
	void On1280x800(wxCommandEvent& evt);
	void OnIphone480x320(wxCommandEvent& evt);
	void OnIphone320x480(wxCommandEvent& evt);
	void OnIphone960x640(wxCommandEvent& evt);
	void OnIphone640x960(wxCommandEvent& evt);
	void OnIpad1024x768(wxCommandEvent& evt);
	void OnIpad768x1024(wxCommandEvent& evt);

public:
	wxAuiManager m_mgr;
	wxColor m_aBGColor;
	wxTextCtrl* m_pOutput;

	typedef std::map<CGUIString, std::vector<CGUIString> > TMapScene;
	TMapScene m_mapScenes;

	DECLARE_EVENT_TABLE()
};


//-----------------------------------------------------------------------------
//	WxGLCanvas
//-----------------------------------------------------------------------------
class WxGLCanvas : public wxGLCanvas
{
public:
	WxGLCanvas( 
		wxWindow *parent, int* args,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, 
		const wxString& name = _T("GLCanvas") );

	void OnIdle(wxIdleEvent & event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnKeyDown(wxKeyEvent& event);

	virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

protected:
	CGUITimer m_aOldTimer;
	CGUITimer m_aCurTimer;

	DECLARE_EVENT_TABLE()
};


//-----------------------------------------------------------------------------
//	CViewerLogMsgCallback
//-----------------------------------------------------------------------------
class CViewerLogMsgCallback : public CGUILogMsgCallback
{
public:
	virtual void Log( const CGUILogMsgRecord& rRecord );

	static CViewerLogMsgCallback g_MsgCallback;
};

#endif
