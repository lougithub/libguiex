/** 
* @file editor_viewer.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-16
*/

//============================================================================//
// include
//============================================================================//
#include "editor_viewer.h"

#include <libguiex_module/keyboard_winapi/guikeyboard_winapi.h>
#include <libguiex_module/mouse_winapi/guimouse_winapi.h>
#include <libguiex_module/ime_winapi/guiime_winapi.h>

#ifdef __cplusplus
extern "C"
{
#endif //#ifdef __cplusplus
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#ifdef __cplusplus
}
#endif //#ifdef __cplusplus


//============================================================================//
// function
//============================================================================//
//------------------------------------------------------------------------------
static void EditorWarningCB(const char* message, void*)
{
	((WxMainFrame*)wxGetApp().GetTopWindow())->OutputString(message);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	CGUIFrameworkViewer
//------------------------------------------------------------------------------
CGUIFrameworkViewer* CGUIFrameworkViewer::ms_pFramework = NULL;
//------------------------------------------------------------------------------
CGUIFrameworkViewer::CGUIFrameworkViewer( )
:CGUIFramework( )
{
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	WxMainApp
//------------------------------------------------------------------------------
IMPLEMENT_APP(WxMainApp)
//-----------------------------------------------------------------------------
bool WxMainApp::OnInit()
{
	//create frame
	wxFrame* frame = new WxMainFrame( NULL, wxID_ANY, wxT("liguiex viewer"), wxDefaultPosition, wxSize( 1024, 768));

	SetTopWindow(frame);
	frame->Show();

	return true;
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	WxGLCanvas
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(WxGLCanvas, wxGLCanvas)
EVT_SIZE(WxGLCanvas::OnSize)
EVT_IDLE(WxGLCanvas::OnIdle)
EVT_KEY_DOWN( WxGLCanvas::OnKeyDown )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxGLCanvas::WxGLCanvas( wxWindow *parent, int* args, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name )
: wxGLCanvas( parent, (wxGLCanvas*) NULL, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name, args )
{
	m_aOldTimer.UpdateTime();
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnEraseBackground(wxEraseEvent& event)
{
}
//------------------------------------------------------------------------------
WXLRESULT WxGLCanvas::MSWWindowProc(WXUINT uMsg, WXWPARAM wParam, WXLPARAM lParam)
{
	try
	{
		if( CGUIInterfaceManager::Instance()->GetInterfaceMouseTyped<IGUIMouse_winapi>() )
		{
			CGUIInterfaceManager::Instance()->GetInterfaceMouseTyped<IGUIMouse_winapi>()->ProcessWindowMessage((HWND)GetHandle(), uMsg, wParam, lParam );
		}
		if( CGUIInterfaceManager::Instance()->GetInterfaceKeyboardTyped<IGUIKeyboard_winapi>() )
		{
			CGUIInterfaceManager::Instance()->GetInterfaceKeyboardTyped<IGUIKeyboard_winapi>()->ProcessWindowMessage((HWND)GetHandle(), uMsg, wParam, lParam );
		}
		if( CGUIInterfaceManager::Instance()->GetInterfaceImeTyped<IGUIIme_winapi>() )
		{
			CGUIInterfaceManager::Instance()->GetInterfaceImeTyped<IGUIIme_winapi>()->ProcessWindowMessage((HWND)GetHandle(), uMsg, wParam, lParam );
		}
	}
	catch (CGUIBaseException& rError)
	{
		MessageBoxA(NULL, rError.what(), "error", MB_OK);
	}
	return wxGLCanvas::MSWWindowProc(uMsg, wParam, lParam);
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnIdle(wxIdleEvent & event)
{
	if( !GetParent()->IsShown())
	{
		return;
	}

	SetCurrent();

	/* clear color and depth buffers */
	const wxColour& rBGColor = ((WxMainFrame*)wxGetApp().GetTopWindow())->m_aBGColor;
	glClearColor( rBGColor.Red() / 255.f, rBGColor.Green() / 255.f, rBGColor.Blue() / 255.f, rBGColor.Alpha() / 255.f );
	glClearStencil( 0 );
	glClearDepth(1.0f); // Depth buffer setup 
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear screen and depth buffer 

	m_aCurTimer.UpdateTime();
	CGUIFrameworkViewer::ms_pFramework->Update( (m_aCurTimer - m_aOldTimer) / 1000.f );
	m_aOldTimer = m_aCurTimer;

	CGUIFrameworkViewer::ms_pFramework->Render();

	glFlush();
	SwapBuffers();

	event.RequestMore();
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnSize(wxSizeEvent& event)
{
	// this is also necessary to update the context on some platforms
	wxGLCanvas::OnSize(event);
	wxSize aSize = event.GetSize();
	SetCurrent();
	glViewport(0,0,aSize.x,aSize.y);
	GSystem->SetScreenSize(aSize.x,aSize.y);
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnKeyDown( wxKeyEvent& event )
{
	if (event.GetKeyCode() == WXK_ESCAPE )
	{
		if( ((WxMainFrame*)GetParent())->IsFullScreen())
		{
			((WxMainFrame*)GetParent())->ShowFullScreen(false);
			((WxMainFrame*)GetParent())->Refresh();
			event.Skip();
		}
	}
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	CMyLogMsgCallback
//------------------------------------------------------------------------------
CMyLogMsgCallback CMyLogMsgCallback::g_MsgCallback;
//------------------------------------------------------------------------------
void CMyLogMsgCallback::Log( const CGUILogMsgRecord& rRecord )
{
	((WxMainFrame*)wxGetApp().GetTopWindow())->OutputString( rRecord.GetMsgData());
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	WxMainFrame
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(WxMainFrame, wxFrame)
EVT_MENU(ID_Open, WxMainFrame::OnOpen)
EVT_MENU(ID_Exit, WxMainFrame::OnExit)
EVT_MENU(ID_About, WxMainFrame::OnAbout)
EVT_MENU(ID_VIEW_Fullscreen, WxMainFrame::OnFullscreen)
EVT_MENU(ID_VIEW_800x600, WxMainFrame::On800x600)
EVT_MENU(ID_VIEW_1024x786, WxMainFrame::On1024x786)
EVT_MENU(ID_VIEW_1280x800, WxMainFrame::On1280x800)
EVT_MENU(ID_ToggleScissor, WxMainFrame::OnToggleScissor)
EVT_MENU(ID_ToggleWireframe, WxMainFrame::OnToggleWireframe)
EVT_MENU(ID_SetBGColor, WxMainFrame::OnSetBGColor)
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxMainFrame::WxMainFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxFrame(parent, id, title, pos, size, style)
,m_aBGColor(128,128,128,255)
{
	// tell wxAuiManager to manage this frame
	m_mgr.SetManagedWindow(this);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create main menu
	wxMenuBar* mb = new wxMenuBar;
	//menu-file
	wxMenu* file_menu = new wxMenu;
	file_menu->Append(ID_Open, _("Open"));
	file_menu->Append(ID_Exit, _("Exit"));
	//menu-view
	wxMenu*	view_menu = new wxMenu;
	view_menu->Append(ID_VIEW_Fullscreen, _("Fullscreen"));
	view_menu->Append(ID_VIEW_800x600, wxT("800 x 600"), wxT("Convenience resizer for 800 x 600."));
	view_menu->Append(ID_VIEW_1024x786, wxT("1024 x 768"), wxT("Convenience resizer for 1024 x 768."));
	view_menu->Append(ID_VIEW_1024x786, wxT("1280 x 800"), wxT("Convenience resizer for 1280 x 800."));
	view_menu->AppendSeparator();
	view_menu->Append(ID_ToggleScissor, wxT("Toggle Scissor"), wxT("enable or disable scissor"), wxITEM_CHECK);
	view_menu->Append(ID_ToggleWireframe, wxT("Toggle Wireframe"), wxT("enable or disable wireframe"), wxITEM_CHECK);
	view_menu->Append(ID_SetBGColor, wxT("Set BG Color"), wxT("set background color"));
	//menu-about
	wxMenu* help_menu = new wxMenu;
	help_menu->Append(ID_About, _("About..."));
	mb->Append(file_menu, _("File"));
	mb->Append(view_menu, _("View"));
	mb->Append(help_menu, _("Help"));
	SetMenuBar(mb);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// min size for the frame itself isn't completely done.
	// see the end up wxAuiManager::Update() for the test
	// code. For now, just hard code a frame minimum size
	SetMinSize(wxSize(640,480));
	SetClientSize( 1024, 786 );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create canvas
	int wx_gl_attribs[] = 
	{
		WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 
		WX_GL_DEPTH_SIZE, 24, 
		WX_GL_STENCIL_SIZE, 8,
		0
	};
	WxGLCanvas* pCanvas = new WxGLCanvas(this, wx_gl_attribs);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create output
	m_pOutput = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//add to aui manager
	m_mgr.AddPane(pCanvas, wxAuiPaneInfo().
		Name(wxT("Canvas")).Caption(wxT("Canvas")).
		CaptionVisible(false).
		PaneBorder(false).
		CloseButton(false).
		Center().
		Dockable(false));

	m_mgr.AddPane(m_pOutput, wxAuiPaneInfo().
		Name(wxT("Output")).Caption(wxT("Output")).
		CloseButton(false).
		Float().
		BestSize(400,100).
		Dockable(false));

	//get data path
	CGUIString strDataPath;
	if( false == GetUIDataPath( strDataPath ) )
	{
		Close( true );
		return;
	}

	//init framework
	Show(true);
	pCanvas->SetCurrent();

	CGUIFrameworkViewer::ms_pFramework = new CGUIFrameworkViewer( );
	CGUIFrameworkViewer::ms_pFramework->Initialize( CGUISize(1024, 768), strDataPath.c_str() );
	CGUIAssert::SetWarningCB( EditorWarningCB, NULL );
	CGUISceneManager::Instance()->RegisterScenesFromDir( "/",".uip" );
	m_mgr.Update();

	TryOpenUIPage( true );

	// "commit" all changes made to wxAuiManager
	Refresh();
}
//------------------------------------------------------------------------------
WxMainFrame::~WxMainFrame()
{
	m_mgr.UnInit();	

	//release libguiex system
	GSystem->Release();
	delete GSystem;
}
//------------------------------------------------------------------------------
void WxMainFrame::TryOpenUIPage( bool bCheckCommandLine )
{
	CGUIString strUISceneName;
	CGUIString strUIPageName;

	bool bCommandlineFound = false;
	if( bCheckCommandLine )
	{
		wxArrayString arrayArgs;
		const wxChar *cmdLine = ::GetCommandLine();
		if ( cmdLine )
		{
			arrayArgs = wxCmdLineParser::ConvertStringToArgs(cmdLine);
		}

		if( arrayArgs.size() >= 4 )
		{
			strUISceneName = arrayArgs[2].char_str(wxConvUTF8).data();
			strUIPageName = arrayArgs[3].char_str(wxConvUTF8).data();
			bCommandlineFound = true;
		}
	}

	if( !bCommandlineFound )
	{
		//chose scene
		const std::vector<CGUIString>& vecScenes = CGUISceneManager::Instance()->GetSceneNames( );
		wxArrayString arrayScenes;
		for( unsigned i=0; i<vecScenes.size(); ++i )
		{
			arrayScenes.Add( wxConvUTF8.cMB2WC( vecScenes[i].c_str()));
		}
		wxSingleChoiceDialog aChoiceDlg( this, _T("select scene"), _T("select scene files"), arrayScenes );
		if( aChoiceDlg.ShowModal() != wxID_OK )
		{
			return;
		}
		strUISceneName = vecScenes[aChoiceDlg.GetSelection()];

		//chose page file
		const std::vector<CGUIString>& vecPages = CGUISceneManager::Instance()->GetScene(strUISceneName)->GetWidgetFiles();
		wxArrayString arrayPages;
		for( unsigned i=0; i<vecPages.size(); ++i )
		{
			arrayPages.Add( wxConvUTF8.cMB2WC( vecPages[i].c_str()));
		}
		wxSingleChoiceDialog aPageChoiceDlg( this, _T("select scene"), _T("select scene files"), arrayPages );
		if( aPageChoiceDlg.ShowModal() != wxID_OK )
		{
			return;
		}
		strUIPageName = vecPages[aPageChoiceDlg.GetSelection()];
	}

	//open it
	try
	{
		CGUISceneManager::Instance()->UnregisterAllScenes();
		CGUISceneManager::Instance()->RegisterScenesFromDir();

		CGUISceneManager::Instance()->LoadResource( strUISceneName );
		CGUIWidget* pWidget = CGUIWidgetManager::Instance()->LoadPage( strUIPageName, strUISceneName );
		GSystem->OpenPage(pWidget);
	}
	catch (CGUIBaseException& rError)
	{
		::MessageBoxA(NULL, rError.what(), "error", MB_OK);

		Close( true );
	}
}
//------------------------------------------------------------------------------
bool WxMainFrame::GetUIDataPath( CGUIString& rDataPath )
{
	wxArrayString arrayArgs;
	const wxChar *cmdLine = ::GetCommandLine();
	if ( cmdLine )
	{
		arrayArgs = wxCmdLineParser::ConvertStringToArgs(cmdLine);
	}
	if( arrayArgs.size() >= 2 )
	{
		rDataPath = arrayArgs[1].char_str(wxConvUTF8).data();
		return true;
	}

	//chose data path
	wxDirDialog aDlg( this, _T("Choose data path") );
	if( wxID_OK != aDlg.ShowModal())
	{
		return false;
	}
	rDataPath = (aDlg.GetPath() + wxT("\\")).char_str(wxConvUTF8).data();
	return true;
}
//------------------------------------------------------------------------------
void WxMainFrame::OutputString( const std::string& rString)
{
	m_pOutput->AppendText(wxConvUTF8.cMB2WC(rString.c_str()).data());
	m_pOutput->AppendText(wxString(_T("\n")));
}
//------------------------------------------------------------------------------
void WxMainFrame::On1280x800(wxCommandEvent& evt)
{
	this->SetClientSize( 1280, 800 );
	Refresh();
}
//------------------------------------------------------------------------------
void WxMainFrame::On1024x786(wxCommandEvent& evt)
{
	this->SetClientSize( 1024, 768 );
	Refresh();
}
//------------------------------------------------------------------------------
void WxMainFrame::On800x600(wxCommandEvent& evt)
{
	this->SetClientSize( 800, 600 );
	Refresh();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnFullscreen(wxCommandEvent& evt)
{
	ShowFullScreen(true);
	Refresh();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnSetBGColor(wxCommandEvent& evt)
{
	wxColourDialog aColorDlg;
	if(wxID_OK == aColorDlg.ShowModal())
	{
		m_aBGColor = aColorDlg.GetColourData().GetColour(); 
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnToggleScissor(wxCommandEvent& evt)
{
	bool bIsChecked = evt.IsChecked();

	if( CGUIInterfaceManager::Instance()->GetInterfaceRender())
	{
		CGUIInterfaceManager::Instance()->GetInterfaceRender()->EnableClip(bIsChecked);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnToggleWireframe(wxCommandEvent& evt)
{
	bool bIsChecked = evt.IsChecked();

	if( CGUIInterfaceManager::Instance()->GetInterfaceRender())
	{
		CGUIInterfaceManager::Instance()->GetInterfaceRender()->SetWireFrame(bIsChecked);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
	GSystem->DestroyAllWidgets();
	GSystem->ReleaseAllResources();

	TryOpenUIPage( false );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(_("libguiex viewer"), _("About"), wxOK, this);
}
//------------------------------------------------------------------------------
