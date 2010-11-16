/** 
* @file test_common.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2010-10-24
*/

//============================================================================//
// include
//============================================================================//
// Disable the warning that the pack size is changed in this header.
#pragma warning(disable:4103)
#pragma warning(disable:4099)
#pragma pack(push,8)
#include "wx/wxprec.h"
#include "wx/glcanvas.h"
#include "wx/aui/aui.h"
#include "wx/colordlg.h"
#include "wx/cmdline.h"
#include "wx/dirdlg.h"
#include "wx/choicdlg.h"
#pragma pack(pop)

#include <windows.h>

//libguiex
#include <libguiex_core/guiex.h>
#include <libguiex_widget\guiwgt.h>

//libguiex module
#include <libguiex_module\render_opengl\guirender_opengl.h>
#include <libguiex_module\imageloader_devil\guiimageloader_devil.h>
#include <libguiex_module\imageloader_tga\guiimageloader_tga.h>
#include <libguiex_module\keyboard_winapi\guikeyboard_winapi.h>
#include <libguiex_module\mouse_winapi\guimouse_winapi.h>
#include <libguiex_module\font_ft2\guifont_ft2.h>
#include <libguiex_module\filesys_stdio\guifilesys_stdio.h>
#include <libguiex_module\configfile_tinyxml\guiconfigfile_tinyxml.h>
#include <libguiex_module\script_lua\guiscript_lua.h>
#include <libguiex_module\ime_winapi\guiime_winapi.h>

#include <fstream>

#ifdef __cplusplus
extern "C" {
#endif //#ifdef __cplusplus

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

extern guiex::CGUIWidget* SampleInitialize();
extern void SampleDestroy();
extern void SampleUpdate( float fDeltaTime);

//============================================================================//
// class
//============================================================================//
class WxMainApp : public wxApp
{
public:
	virtual		bool OnInit();
};

class WxGLCanvas : public wxGLCanvas
{
public:
	WxGLCanvas( wxWindow *parent, int* args,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("GLCanvas") );

	void OnIdle(wxIdleEvent & event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	//key
	void OnKeyDown(wxKeyEvent& event);

	virtual WXLRESULT MSWWindowProc(WXUINT message,
		WXWPARAM wParam,
		WXLPARAM lParam);
protected:

	DECLARE_EVENT_TABLE()
};

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
	~WxMainFrame();


	/** 
	* @brief show string in output panel
	*/
	void	OutputString( const std::string& rString);
	bool		GetUIInfo( bool bTryCommandLine);

	enum
	{
		//file
		ID_Exit = wxID_HIGHEST+1,
		ID_About,
		ID_Open,

		//view
		ID_VIEW_Fullscreen,
		ID_VIEW_800x600,
		ID_VIEW_1024x786,
		ID_VIEW_1280x800,
		ID_VIEW_Refresh,
		ID_ToggleRenderExtraInfo,
		ID_ToggleScissor,
		ID_ToggleWireframe,
		ID_SetBGColor,
	};

protected:

	//menu
	void OnExit(wxCommandEvent& evt);

	void OnAbout(wxCommandEvent& evt);

	void OnToggleScissor(wxCommandEvent& evt);
	void OnToggleWireframe(wxCommandEvent& evt);
	void OnToggleRenderExtraInfo(wxCommandEvent& evt);
	void OnSetBGColor(wxCommandEvent& evt);

	void OnFullscreen(wxCommandEvent& evt);

	void On800x600(wxCommandEvent& evt);
	void On1024x786(wxCommandEvent& evt);
	void On1280x800(wxCommandEvent& evt);
	void OnRefresh(wxCommandEvent& evt);

public:
	wxAuiManager			m_mgr;
	wxColor					m_aBGColor;
	wxTextCtrl*				m_pOutput;

	guiex::IGUIMouse_winapi*		m_pMouse;
	guiex::IGUIKeyboard_winapi*		m_pKeyboard;
	guiex::IGUIIme_winapi*			m_pIme;
	std::string	m_strUIDataPath;
	std::string	m_strUISceneFilename;
	//std::string	m_strUIPageFilename;

	DECLARE_EVENT_TABLE()
};





//============================================================================//
// function
//============================================================================//


//------------------------------------------------------------------------------
//	WxMainApp
//------------------------------------------------------------------------------
DECLARE_APP(WxMainApp)
IMPLEMENT_APP(WxMainApp)
//-----------------------------------------------------------------------------

bool WxMainApp::OnInit()
{
	//create frame
	wxFrame* frame = new WxMainFrame(NULL,
		wxID_ANY,
		wxT("liguiex test"),
		wxDefaultPosition,
		wxSize( 800, 600));
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
WxGLCanvas::WxGLCanvas( wxWindow *parent, 
					   int* args,
					   wxWindowID id ,
					   const wxPoint& pos ,
					   const wxSize& size ,
					   long style, 
					   const wxString& name )
					   : wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name, args )
{
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnEraseBackground(wxEraseEvent& event)
{
}
//------------------------------------------------------------------------------
WXLRESULT WxGLCanvas::MSWWindowProc(WXUINT uMsg,
									WXWPARAM wParam,
									WXLPARAM lParam)
{
	try
	{
		if( ((WxMainFrame*)wxGetApp().GetTopWindow())->m_pMouse )
		{
			((WxMainFrame*)wxGetApp().GetTopWindow())->m_pMouse->ProcessWindowMessage((HWND)GetHandle(), uMsg, wParam, lParam );
		}
		if( ((WxMainFrame*)wxGetApp().GetTopWindow())->m_pKeyboard )
		{
			((WxMainFrame*)wxGetApp().GetTopWindow())->m_pKeyboard->ProcessWindowMessage((HWND)GetHandle(), uMsg, wParam, lParam );
		}
		if( ((WxMainFrame*)wxGetApp().GetTopWindow())->m_pIme )
		{
			((WxMainFrame*)wxGetApp().GetTopWindow())->m_pIme->ProcessWindowMessage((HWND)GetHandle(), uMsg, wParam, lParam );
		}
	}
	catch (guiex::CGUIBaseException& rError)
	{
		MessageBoxA(NULL, rError.what(), "error", MB_OK);
	}
	return wxGLCanvas::MSWWindowProc(uMsg, wParam, lParam);
}
//------------------------------------------------------------------------------
guiex::CGUITimer g_aOldTimer;
guiex::CGUITimer g_aCurTimer;
bool g_bStarted = false;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	try
	{
		if( g_bStarted )
		{
			g_aCurTimer.UpdateTime();
			float fDeltaTime = (g_aCurTimer - g_aOldTimer) / 1000.f;
			guiex::CGUIWidgetSystem::Instance()->Update( fDeltaTime );
			guiex::CGUIWidgetSystem::Instance()->Render();
			SampleUpdate( fDeltaTime);
			g_aOldTimer = g_aCurTimer;
		}
	}
	catch (guiex::CGUIBaseException& rError)
	{
		MessageBoxA( NULL, rError.what(), "error", MB_OK);
	}

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
	glViewport(0,0,aSize.x,aSize.y);	//定义视口

	guiex::CGUIWidgetSystem::Instance()->SetScreenSize(aSize.x,aSize.y);
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
class CMyLogMsgCallback : public guiex::CGUILogMsgCallback
{
public:
	virtual void	Log( const guiex::CGUILogMsgRecord& rRecord )
	{
		((WxMainFrame*)wxGetApp().GetTopWindow())->OutputString( rRecord.GetMsgData());
	}
};
CMyLogMsgCallback	g_MsgCallback;
//------------------------------------------------------------------------------
static void EditorWarningCB(const char* message, void*)
{
	((WxMainFrame*)wxGetApp().GetTopWindow())->OutputString(message);
}
//------------------------------------------------------------------------------
extern "C" int luaopen_game(lua_State* L);
void FuncInitScript( void* pScriptState)
{
	luaopen_game((lua_State*)pScriptState);
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	WxMainFrame
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(WxMainFrame, wxFrame)
EVT_MENU(ID_Exit, WxMainFrame::OnExit)
EVT_MENU(ID_About, WxMainFrame::OnAbout)
EVT_MENU(ID_VIEW_Fullscreen, WxMainFrame::OnFullscreen)
EVT_MENU(ID_VIEW_800x600, WxMainFrame::On800x600)
EVT_MENU(ID_VIEW_1024x786, WxMainFrame::On1024x786)
EVT_MENU(ID_VIEW_1280x800, WxMainFrame::On1280x800)
EVT_MENU(ID_VIEW_Refresh, WxMainFrame::OnRefresh)
EVT_MENU(ID_ToggleScissor, WxMainFrame::OnToggleScissor)
EVT_MENU(ID_ToggleRenderExtraInfo, WxMainFrame::OnToggleRenderExtraInfo)
EVT_MENU(ID_ToggleWireframe, WxMainFrame::OnToggleWireframe)
EVT_MENU(ID_SetBGColor, WxMainFrame::OnSetBGColor)
END_EVENT_TABLE()
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
WxMainFrame::WxMainFrame(wxWindow* parent,
						 wxWindowID id,
						 const wxString& title,
						 const wxPoint& pos,
						 const wxSize& size,
						 long style)
						 : wxFrame(parent, id, title, pos, size, style)
						 ,m_aBGColor(128,128,128,255)
						 ,m_pMouse(NULL)
						 ,m_pKeyboard(NULL)
						 ,m_pIme(NULL)
{
	// tell wxAuiManager to manage this frame
	m_mgr.SetManagedWindow(this);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create main menu
	wxMenuBar* mb = new wxMenuBar;
	//menu-file
	wxMenu* file_menu = new wxMenu;
	file_menu->Append(ID_Exit, _("Exit"));
	//menu-view
	wxMenu*	view_menu = new wxMenu;
	view_menu->Append(ID_VIEW_Fullscreen, _("Fullscreen"));
	view_menu->Append(ID_VIEW_800x600, wxT("800 x 600"), wxT("Convenience resizer for 800 x 600."));
	view_menu->Append(ID_VIEW_1024x786, wxT("1024 x 768"), wxT("Convenience resizer for 1024 x 768."));
	view_menu->Append(ID_VIEW_1280x800, wxT("1280 x 800"), wxT("Convenience resizer for 1280 x 800."));
	view_menu->AppendSeparator();
	view_menu->Append(ID_ToggleScissor, wxT("Toggle Scissor"), wxT("enable or disable scissor"), wxITEM_CHECK);
	view_menu->Append(ID_ToggleWireframe, wxT("Toggle Wireframe"), wxT("enable or disable wireframe"), wxITEM_CHECK);
	view_menu->Append(ID_ToggleRenderExtraInfo, wxT("Toggle Render Extra Info"), wxT("enable or disable render extra info"), wxITEM_CHECK);
	view_menu->Append(ID_SetBGColor, wxT("Set BG Color"), wxT("set background color"));
	view_menu->Append(ID_VIEW_Refresh, wxT("Refresh"), wxT("refresh."));
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

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create canvas
	int wx_gl_attribs[] = {
		WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 
		WX_GL_DEPTH_SIZE, 24, 
		WX_GL_STENCIL_SIZE, 8,
		0};

	WxGLCanvas* pCanvas = new WxGLCanvas(this, wx_gl_attribs);


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create putput
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

	Show(true);
	pCanvas->SetCurrent();

	// "commit" all changes made to wxAuiManager
	m_mgr.Update();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//initialize guiex system
	try
	{
		GUI_LOG->Open( "gui editor",guiex::CGUILogMsg::FLAG_TIMESTAMP_LITE	|guiex::CGUILogMsg::FLAG_OSTREAM | guiex::CGUILogMsg::FLAG_MSG_CALLBACK);
		GUI_LOG->SetPriorityMask(guiex::GUI_LM_DEBUG	| guiex::GUI_LM_TRACE	|guiex::GUI_LM_WARNING|guiex::GUI_LM_ERROR);
		GUI_LOG->SetOstream( new std::ofstream( "libguiex_test.log", std::ios_base::out | std::ios_base::trunc ), true );
		GUI_LOG->SetCallbackMsg( &g_MsgCallback );
		guiex::CGUIWidgetSystem::Instance()->Initialize();
		guiex::CGUIAssert::SetWarningCB(EditorWarningCB, NULL);
		guiex::CGUIWidgetSystem::Instance()->SetScreenSize(1024, 768);

		//register interface
		GUI_REGISTER_INTERFACE_LIB( "IGUIRender", IGUIRender_opengl);
		//GUI_REGISTER_INTERFACE_LIB( "IGUIImageLoader", IGUIImageLoader_devil);
		GUI_REGISTER_INTERFACE_LIB( "IGUIImageLoader", IGUIImageLoader_tga);
		GUI_REGISTER_INTERFACE_LIB( "IGUIFileSys", IGUIFileSys_stdio);
		GUI_REGISTER_INTERFACE_LIB( "IGUIMouse", IGUIMouse_winapi);
		GUI_REGISTER_INTERFACE_LIB( "IGUIFont", IGUIFont_ft2);
		GUI_REGISTER_INTERFACE_LIB( "IGUIKeyboard", IGUIKeyboard_winapi);
		GUI_REGISTER_INTERFACE_LIB( "IGUIConfigFile", IGUIConfigFile_tinyxml);
		GUI_REGISTER_INTERFACE_LIB_ARG( "IGUIScript", IGUIScript_lua, FuncInitScript);
		GUI_REGISTER_INTERFACE_LIB_ARG( "IGUIIme", IGUIIme_winapi, pCanvas->GetHandle());

		m_pMouse = (guiex::IGUIMouse_winapi*)guiex::CGUIInterfaceManager::Instance()->GetInterfaceMouse();
		m_pKeyboard = (guiex::IGUIKeyboard_winapi*)guiex::CGUIInterfaceManager::Instance()->GetInterfaceKeyboard();
		m_pIme = (guiex::IGUIIme_winapi*)guiex::CGUIInterfaceManager::Instance()->GetInterfaceIme();


		//register widget
		guiex::CGUIWidgetGenerator** pGenerator = guiex::GetAllGenerators();
		while(*pGenerator)
		{
			guiex::CGUIWidgetFactory::Instance()->RegisterGenerator( *pGenerator);
			pGenerator ++;
		}


		//get scene info
		if(  GetUIInfo( true ))
		{	
			guiex::CGUIWidgetSystem::Instance()->SetDataPath(m_strUIDataPath);
			guiex::CGUISceneInfoManager::Instance()->LoadScenes();
			guiex::CGUISceneUtility::LoadResource(m_strUISceneFilename);
			guiex::CGUIWidget* pWidget = SampleInitialize();
			guiex::CGUIWidgetSystem::Instance()->AddPage(pWidget);
			guiex::CGUIWidgetSystem::Instance()->OpenPage(pWidget);
			g_aOldTimer.UpdateTime();
			g_bStarted = true;
		}
	}
	catch (guiex::CGUIBaseException& rError)
	{
		MessageBoxA(NULL, rError.what(), "error", MB_OK);

		guiex::CGUIWidgetSystem::Instance()->Release();
	}

	SetClientSize( 1024, 786 );
	Refresh();
}
//------------------------------------------------------------------------------
WxMainFrame::~WxMainFrame()
{
	m_mgr.UnInit();	

	//release libguiex system
	guiex::CGUIWidgetSystem::Instance()->Release();
	SampleDestroy();
}
//------------------------------------------------------------------------------
bool		WxMainFrame::GetUIInfo( bool bTryCommandLine)
{
	if( bTryCommandLine )
	{
		wxArrayString arrayArgs;
		const wxChar *cmdLine = ::GetCommandLine();
		if ( cmdLine )
		{
			arrayArgs = wxCmdLineParser::ConvertStringToArgs(cmdLine);
		}
		if( arrayArgs.size() >= 2 )
		{
			m_strUIDataPath = arrayArgs[1].char_str(wxConvUTF8).data();
		}
	}


	//chose data path
	wxDirDialog aDlg( this, _T("Choose a libguiex root path"), wxConvUTF8.cMB2WC( m_strUIDataPath.c_str()));
	if( wxID_OK != aDlg.ShowModal())
	{
		return false;
	}
	m_strUIDataPath = (aDlg.GetPath() + wxT("\\")).char_str(wxConvUTF8).data();

	//chose scene
	guiex::CGUIWidgetSystem::Instance()->SetDataPath(m_strUIDataPath);
	guiex::CGUISceneInfoManager::Instance()->LoadScenes();
	const std::vector<guiex::CGUIString>& vecScenes = guiex::CGUISceneInfoManager::Instance()->GetSceneFileNames( );
	wxArrayString arrayScenes;
	for( unsigned i=0; i<vecScenes.size(); ++i )
	{
		arrayScenes.Add( wxConvUTF8.cMB2WC( vecScenes[i].c_str()));
	}
	wxSingleChoiceDialog aChoiceDlg( this, _T("select resource"), _T("select resource scene"), arrayScenes );
	if( aChoiceDlg.ShowModal() != wxID_OK )
	{
		return false;
	}
	m_strUISceneFilename = vecScenes[aChoiceDlg.GetSelection()];

	return true;
}
//------------------------------------------------------------------------------
void	 WxMainFrame::OutputString( const std::string& rString)
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
void WxMainFrame::OnRefresh(wxCommandEvent& evt)
{
	guiex::CGUIWidgetSystem::Instance()->GetCurrentRootWidget()->Refresh();
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

	if( guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender())
	{
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender()->EnableClip(bIsChecked);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnToggleWireframe(wxCommandEvent& evt)
{
	bool bIsChecked = evt.IsChecked();

	if( guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender())
	{
		guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender()->SetWireFrame(bIsChecked);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnToggleRenderExtraInfo(wxCommandEvent& evt)
{
	bool bIsChecked = evt.IsChecked();

	if( guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender())
	{
		guiex::CGUIWidgetSystem::Instance()->SetDrawExtraInfo(bIsChecked);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(_("libguiex test"), _("About"), wxOK, this);
}
//------------------------------------------------------------------------------


