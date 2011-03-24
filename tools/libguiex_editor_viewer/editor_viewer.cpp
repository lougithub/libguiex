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

#include <algorithm>
#include <fstream>


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
wxString Gui2wxString( const CGUIString& rString )
{
	return wxConvUTF8.cMB2WC(rString.c_str());
}
//------------------------------------------------------------------------------
CGUIString wx2GuiString( const wxString& rString )
{
	return wxConvUTF8.cWC2MB(rString.c_str()).data();
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
void setVSync(int interval=1)
{
	const char *extensions = (const char*)glGetString( GL_EXTENSIONS );

	if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
	{
		return; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
	}
	else
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

		if( wglSwapIntervalEXT )
		{
			wglSwapIntervalEXT(interval);
		}
	}
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
void CGUIFrameworkViewer::SetupLogSystem( )
{
	GUI_LOG->Open( "gui_framework_log", CGUILogMsg::FLAG_TIMESTAMP_LITE | CGUILogMsg::FLAG_OSTREAM | CGUILogMsg::FLAG_MSG_CALLBACK );
	GUI_LOG->SetPriorityMask( GUI_LM_DEBUG | GUI_LM_TRACE | GUI_LM_WARNING|GUI_LM_ERROR );
	GUI_LOG->SetOstream( new std::ofstream( "libguiex_viewer.log", std::ios_base::out | std::ios_base::trunc ), true );
	GUI_LOG->SetCallbackMsg( &CViewerLogMsgCallback::g_MsgCallback );
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
		if( CGUISystem::Instance() )
		{
			if( CGUIInterfaceManager::Instance()->GetInterfaceMouse() )
			{
				CGUIInterfaceManager::Instance()->GetInterfaceMouseTyped<IGUIMouse_winapi>()->ProcessWindowMessage((HWND)GetHandle(), uMsg, wParam, lParam );
			}
			if( CGUIInterfaceManager::Instance()->GetInterfaceKeyboard() )
			{
				CGUIInterfaceManager::Instance()->GetInterfaceKeyboardTyped<IGUIKeyboard_winapi>()->ProcessWindowMessage((HWND)GetHandle(), uMsg, wParam, lParam );
			}
			if( CGUIInterfaceManager::Instance()->GetInterfaceIme() )
			{
				CGUIInterfaceManager::Instance()->GetInterfaceImeTyped<IGUIIme_winapi>()->ProcessWindowMessage((HWND)GetHandle(), uMsg, wParam, lParam );
			}
		}	
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString( rError.what() ), _T("error") );
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
	GSystem->SetScreenSize(aSize.x,aSize.y);
	glViewport(0,0,aSize.x,aSize.y);
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
//	CViewerLogMsgCallback
//------------------------------------------------------------------------------
CViewerLogMsgCallback CViewerLogMsgCallback::g_MsgCallback;
//------------------------------------------------------------------------------
void CViewerLogMsgCallback::Log( const CGUILogMsgRecord& rRecord )
{
	((WxMainFrame*)wxGetApp().GetTopWindow())->OutputString( rRecord.GetMsgData());
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	WxMainFrame
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(WxMainFrame, wxFrame)
EVT_MENU(ID_OpenPage, WxMainFrame::OnOpenPage)
EVT_MENU(ID_AddPage, WxMainFrame::OnAddPage)
EVT_MENU(ID_ClosePage, WxMainFrame::OnClosePage)
EVT_MENU(ID_Exit, WxMainFrame::OnExit)
EVT_MENU(ID_About, WxMainFrame::OnAbout)
EVT_MENU(ID_VIEW_Fullscreen, WxMainFrame::OnFullscreen)
EVT_MENU(ID_ToggleScissor, WxMainFrame::OnToggleScissor)
EVT_MENU(ID_ToggleExtraInfo, WxMainFrame::OnToggleExtraInfo)
EVT_MENU(ID_ToggleWireframe, WxMainFrame::OnToggleWireframe)
EVT_MENU(ID_Refresh, WxMainFrame::OnRefresh)
EVT_MENU(ID_SetBGColor, WxMainFrame::OnSetBGColor)
EVT_MENU(ID_VIEW_800x600, WxMainFrame::On800x600)
EVT_MENU(ID_VIEW_1024x786, WxMainFrame::On1024x786)
EVT_MENU(ID_VIEW_1280x800, WxMainFrame::On1280x800)
EVT_MENU(ID_VIEW_Iphone480x320, WxMainFrame::OnIphone480x320)
EVT_MENU(ID_VIEW_Iphone320x480, WxMainFrame::OnIphone320x480)
EVT_MENU(ID_VIEW_Iphone960x640, WxMainFrame::OnIphone960x640)
EVT_MENU(ID_VIEW_Iphone640x960, WxMainFrame::OnIphone640x960)
EVT_MENU(ID_VIEW_Ipad1024x768, WxMainFrame::OnIpad1024x768)
EVT_MENU(ID_VIEW_Ipad768x1024, WxMainFrame::OnIpad768x1024)
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
	file_menu->Append(ID_OpenPage, _("Open Page..."));
	file_menu->Append(ID_AddPage, _("Add Page..."));
	file_menu->Append(ID_ClosePage, _("Close Page..."));
	file_menu->AppendSeparator();
	file_menu->Append(ID_Exit, _("Exit"));
	//menu-view
	wxMenu*	view_menu = new wxMenu;
	view_menu->Append(ID_VIEW_Fullscreen, _("Fullscreen"));
	view_menu->Append(ID_VIEW_800x600, wxT("800 x 600"), wxT("Convenience resizer for 800 x 600."));
	view_menu->Append(ID_VIEW_1024x786, wxT("1024 x 768"), wxT("Convenience resizer for 1024 x 768."));
	view_menu->Append(ID_VIEW_1280x800, wxT("1280 x 800"), wxT("Convenience resizer for 1280 x 800."));
	view_menu->Append(ID_VIEW_Iphone480x320, wxT("Iphone480x320"), wxT("Convenience resizer for Iphone480x320."));
	view_menu->Append(ID_VIEW_Iphone320x480, wxT("Iphone320x480"), wxT("Convenience resizer for Iphone320x480."));
	view_menu->Append(ID_VIEW_Iphone960x640, wxT("Iphone960x640"), wxT("Convenience resizer for Iphone960x640."));
	view_menu->Append(ID_VIEW_Iphone640x960, wxT("Iphone640x960"), wxT("Convenience resizer for Iphone640x960."));
	view_menu->Append(ID_VIEW_Ipad1024x768, wxT("Ipad1024x768"), wxT("Convenience resizer for Ipad1024x768."));
	view_menu->Append(ID_VIEW_Ipad768x1024, wxT("Ipad768x1024"), wxT("Convenience resizer for Ipad768x1024."));
	view_menu->AppendSeparator();
	view_menu->Append(ID_ToggleScissor, wxT("Toggle Scissor"), wxT("enable or disable scissor"), wxITEM_CHECK);
	view_menu->Append(ID_ToggleWireframe, wxT("Toggle Wireframe"), wxT("enable or disable wireframe"), wxITEM_CHECK);
	view_menu->Append(ID_ToggleExtraInfo, wxT("render extra info"), wxT("enable or disable render extra info"), wxITEM_CHECK);
	view_menu->Append(ID_Refresh, wxT("Refresh"), wxT("refresh widgets"));
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
	SetMinSize(wxSize(100,100));
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
	setVSync(1);

	CGUIFrameworkViewer::ms_pFramework = new CGUIFrameworkViewer( );
	CGUIFrameworkViewer::ms_pFramework->Initialize( CGUIIntSize(1024, 768), strDataPath.c_str() );
	CGUIAssert::SetWarningCB( EditorWarningCB, NULL );
	CGUISceneManager::Instance()->RegisterScenesFromDir( "/",".uip" );

	m_mgr.Update();

	OpenUIPage( true );

	// "commit" all changes made to wxAuiManager
	Refresh();
}
//------------------------------------------------------------------------------
WxMainFrame::~WxMainFrame()
{
	m_mgr.UnInit();	

	//release libguiex system
	if( CGUIFrameworkViewer::ms_pFramework )
	{
		CGUIFrameworkViewer::ms_pFramework->Release();
		delete CGUIFrameworkViewer::ms_pFramework;
		CGUIFrameworkViewer::ms_pFramework = NULL;
	}
}
//------------------------------------------------------------------------------
bool WxMainFrame::GetClosePageInfo( CGUIString& rScene, std::vector<CGUIString>& rPages )
{
	//chose scene
	std::vector<CGUIString> vecScenes;
	for( TMapScene::iterator itor = m_mapScenes.begin();
		itor != m_mapScenes.end();
		++itor )
	{
		vecScenes.push_back( itor->first );
	}
	wxArrayString arrayScenes;
	for( unsigned i=0; i<vecScenes.size(); ++i )
	{
		arrayScenes.Add( Gui2wxString( vecScenes[i]));
	}
	wxSingleChoiceDialog aSceneChoiceDlg( this, _T("select scene"), _T("select scene files"), arrayScenes );
	if( aSceneChoiceDlg.ShowModal() != wxID_OK )
	{
		return false;
	}
	rScene = vecScenes[aSceneChoiceDlg.GetSelection()];

	//chose page file
	const std::vector<CGUIString>& vecPages = m_mapScenes[rScene];
	wxArrayString arrayPages;
	for( unsigned i=0; i<vecPages.size(); ++i )
	{
		arrayPages.Add( Gui2wxString( vecPages[i]));
	}
	wxMultiChoiceDialog aPageChoiceDlg( this, _T("select page to close"), _T("select page files"), arrayPages );
	if( aPageChoiceDlg.ShowModal() != wxID_OK )
	{
		return false;
	}
	wxArrayInt arrayPageSelections = aPageChoiceDlg.GetSelections();
	for( unsigned i=0; i<arrayPageSelections.size(); ++i )
	{
		rPages.push_back( vecPages[arrayPageSelections[i]] );
	}

	return true;
}
//------------------------------------------------------------------------------
bool WxMainFrame::GetOpenPageInfo( CGUIString& rScene, std::vector<CGUIString>& rPages )
{
	//chose scene
	const std::vector<CGUIString>& vecScenes = CGUISceneManager::Instance()->GetSceneNames( );
	wxArrayString arrayScenes;
	for( unsigned i=0; i<vecScenes.size(); ++i )
	{
		arrayScenes.Add( Gui2wxString( vecScenes[i]));
	}
	wxSingleChoiceDialog aSceneChoiceDlg( this, _T("select scene"), _T("select scene files"), arrayScenes );
	if( aSceneChoiceDlg.ShowModal() != wxID_OK )
	{
		return false;
	}
	rScene = vecScenes[aSceneChoiceDlg.GetSelection()];

	//chose page file
	const std::vector<CGUIString>& vecPages = CGUISceneManager::Instance()->GetScene(rScene)->GetWidgetFiles();
	wxArrayString arrayPages;
	for( unsigned i=0; i<vecPages.size(); ++i )
	{
		arrayPages.Add( Gui2wxString( vecPages[i]));
	}
	wxMultiChoiceDialog aPageChoiceDlg( this, _T("select pages to open"), _T("select page files"), arrayPages );
	if( aPageChoiceDlg.ShowModal() != wxID_OK )
	{
		return false;
	}
	wxArrayInt arrayPageSelections = aPageChoiceDlg.GetSelections();
	for( unsigned i=0; i<arrayPageSelections.size(); ++i )
	{
		rPages.push_back( vecPages[arrayPageSelections[i]] );
	}

	return true;
}
//------------------------------------------------------------------------------
void WxMainFrame::OpenUIPage( bool bCheckCommandLine )
{
	std::vector<CGUIString> arrayUIPageNames;
	CGUIString strUISceneName;

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
			strUISceneName = wx2GuiString( arrayArgs[2] );
			arrayUIPageNames.push_back( wx2GuiString( arrayArgs[3] ) );
			bCommandlineFound = true;
		}
	}

	if( !bCommandlineFound )
	{
		if( !GetOpenPageInfo(strUISceneName, arrayUIPageNames))
		{
			return;
		}
	}

	try
	{
		//GSystem->CloseAll();

		//free resource
		for( TMapScene::iterator itor = m_mapScenes.begin();
			itor != m_mapScenes.end();
			++itor )
		{
			CGUISceneManager::Instance()->ReleaseWidgets( itor->first );
			CGUISceneManager::Instance()->ReleaseResources( itor->first );
		}

		//open new pages
		m_mapScenes.clear();
		m_mapScenes[strUISceneName] = arrayUIPageNames;
		for( TMapScene::iterator itor = m_mapScenes.begin();
			itor != m_mapScenes.end();
			++itor )
		{
			CGUISceneManager::Instance()->LoadResources( itor->first );
			CGUISceneManager::Instance()->LoadWidgets( itor->first );
		}
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString( rError.what() ), _T("error") );
		Close( true );
	}

	ReOpenPages();
}
//------------------------------------------------------------------------------
void WxMainFrame::ReOpenPages()
{
	for( TMapScene::iterator itor = m_mapScenes.begin();
		itor != m_mapScenes.end();
		++itor )
	{
		const CGUIString& rSceneName = itor->first;
		std::vector<CGUIString>& rPagesInScene = itor->second;
		for( unsigned i=0; i<rPagesInScene.size(); ++i )
		{
			CGUIWidget* pPage = CGUIWidgetManager::Instance()->GetPage( rPagesInScene[i], rSceneName );
			if( pPage->IsOpen() == false )
			{
				GSystem->GetUICanvas()->OpenUIPage( pPage );
			}
		}
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
		rDataPath = wx2GuiString( arrayArgs[1]);
		return true;
	}

	//chose data path
	wxDirDialog aDlg( this, _T("Choose data path") );
	if( wxID_OK != aDlg.ShowModal())
	{
		return false;
	}
	rDataPath = wx2GuiString(aDlg.GetPath() + wxT("\\"));
	return true;
}
//------------------------------------------------------------------------------
void WxMainFrame::OutputString( const std::string& rString)
{
	m_pOutput->AppendText(Gui2wxString(rString).data());
	m_pOutput->AppendText(wxString(_T("\n")));
}
//------------------------------------------------------------------------------
void WxMainFrame::On800x600(wxCommandEvent& evt)
{
	SetResolution( 800, 600 );
}
//------------------------------------------------------------------------------
void WxMainFrame::On1024x786(wxCommandEvent& evt)
{
	SetResolution( 1024, 768 );
}
//------------------------------------------------------------------------------
void WxMainFrame::On1280x800(wxCommandEvent& evt)
{
	SetResolution( 1280, 800 );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnIphone480x320(wxCommandEvent& evt)
{
	SetResolution( 480, 320 );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnIphone320x480(wxCommandEvent& evt)
{
	SetResolution( 320, 480 );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnIphone960x640(wxCommandEvent& evt)
{
	SetResolution( 960, 640 );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnIphone640x960(wxCommandEvent& evt)
{
	SetResolution( 640, 960 );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnIpad1024x768(wxCommandEvent& evt)
{
	SetResolution( 1024, 768 );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnIpad768x1024(wxCommandEvent& evt)
{
	SetResolution( 768, 1024 );
}
//------------------------------------------------------------------------------
void WxMainFrame::SetResolution( int width, int height )
{
	SetClientSize( width, height );
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
void WxMainFrame::OnToggleExtraInfo(wxCommandEvent& evt)
{
	bool bIsChecked = evt.IsChecked();

	if( guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender())
	{
		guiex::GSystem->SetDrawExtraInfo(bIsChecked);
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
void WxMainFrame::OnRefresh(wxCommandEvent& evt)
{
	guiex::GSystem->GetUICanvas()->GetCurrentRootWidget()->Refresh();
	Refresh();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnOpenPage(wxCommandEvent& WXUNUSED(event))
{
	OpenUIPage( false );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnAddPage(wxCommandEvent& WXUNUSED(event))
{
	std::vector<CGUIString> arrayUIPageNames;
	CGUIString strUISceneName;

	if( !GetOpenPageInfo(strUISceneName, arrayUIPageNames))
	{
		return;
	}

	try
	{
		//open new pages
		TMapScene::iterator itorScene = m_mapScenes.find( strUISceneName );
		if( itorScene == m_mapScenes.end())
		{
			//new scene
			m_mapScenes[strUISceneName] = arrayUIPageNames;
			
			CGUISceneManager::Instance()->LoadResources( strUISceneName );
			CGUISceneManager::Instance()->LoadWidgets( strUISceneName );
		}
		else
		{
			std::vector<CGUIString>& rArrayPages = itorScene->second;

			for( uint32 i=0; i<arrayUIPageNames.size(); ++i )
			{
				if( std::find( rArrayPages.begin(), rArrayPages.end(), arrayUIPageNames[i] ) == rArrayPages.end() )
				{
					rArrayPages.push_back( arrayUIPageNames[i] );
				}
			}
		}
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString( rError.what() ), _T("error") );
		Close( true );
	}

	ReOpenPages( );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnClosePage(wxCommandEvent& WXUNUSED(event))
{
	std::vector<CGUIString> arrayUIPageNames;
	CGUIString strUISceneName;

	if( !GetClosePageInfo(strUISceneName, arrayUIPageNames))
	{
		return;
	}
	if( m_mapScenes.find( strUISceneName ) == m_mapScenes.end() )
	{
		return;
	}
	std::vector<CGUIString>& rOpenPages = m_mapScenes[strUISceneName];
	for( uint32 i=0; i<arrayUIPageNames.size(); ++i )
	{
		std::vector<CGUIString>::iterator itorFind = std::find( rOpenPages.begin(), rOpenPages.end(), arrayUIPageNames[i] );
		if( itorFind != rOpenPages.end() )
		{
			rOpenPages.erase( itorFind );
		}
	}

	if( rOpenPages.empty() )
	{
		//GSystem->CloseAll();

		//release resource
		m_mapScenes.erase( m_mapScenes.find( strUISceneName ));
		try
		{
			CGUISceneManager::Instance()->ReleaseWidgets( strUISceneName );
			CGUISceneManager::Instance()->ReleaseResources( strUISceneName );
		}
		catch (CGUIBaseException& rError)
		{
			wxMessageBox( Gui2wxString( rError.what() ), _T("error") );
			Close( true );
		}
	}
	ReOpenPages();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox( _T("libguiex viewer"), _T("About") );
}
//------------------------------------------------------------------------------
