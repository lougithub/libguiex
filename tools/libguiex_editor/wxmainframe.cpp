/** 
* @file mainframe.cpp
* @brief main frame used by application
* @author ken
* @date 2007-09-10
*/

//============================================================================//
// include
//============================================================================// 
#include "wxmainframe.h"
#include "wxmainapp.h"
#include "wxsavefiledlg.h"
#include "wxeditorcanvascontainer.h"
#include "wxoutputpanel.h"
#include "wxwizardcreatewidget.h"
#include "propertysheetfunc.h"
#include "propertyconfigmgr.h"
#include "editorutility.h"
#include "toolcache.h"

//wxwidgets
#include <wx/treectrl.h>
#include <wx/artprov.h>
#include <wx/colordlg.h>

//libguiex module
#include <libguiex_widget/guiwgt.h>

#include <libguiex_module\render_opengl\guirender_opengl.h>
#include <libguiex_module\imageloader_tga\guiimageloader_tga.h>
#include <libguiex_module\keyboard_winapi\guikeyboard_winapi.h>
#include <libguiex_module\mouse_winapi\guimouse_winapi.h>
#include <libguiex_module\font_ft2\guifont_ft2.h>
#include <libguiex_module\font_dummy\guifont_dummy.h>
#include <libguiex_module\filesys_stdio\guifilesys_stdio.h>
#include <libguiex_module\configfile_tinyxml\guiconfigfile_tinyxml.h>
#include <libguiex_module\script_lua\guiscript_lua.h>
#include <libguiex_module\ime_winapi\guiime_winapi.h>
#include <libguiex_module\stringconv_winapi\guistringconv_winapi.h>

//images
#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/copy.xpm"
#include "bitmaps/paste.xpm"
#include "bitmaps/delete.xpm"
#include "bitmaps/help.xpm"
#include "bitmaps/icon.xpm"
#include "bitmaps/gem_blue.xpm"
#include "bitmaps/gem_red.xpm"


//============================================================================//
// define
//============================================================================// 
#define	TITLE_WIDGET_CONFIG "Widget Config"
#define	TITLE_SCRIPT "Script"
#define	TITLE_RESOURCE_CONFIG "Resource"


// verify that the item is ok and insult the user if it is not
#define CHECK_ITEM( item ) if ( !item.IsOk() ) \
{ \
	wxMessageBox(wxT("Please select some item first!"), wxT("Tree sample error") ); \
	return; \
}


//============================================================================//
// function
//============================================================================// 
class CTreeNode : public wxTreeItemData
{
public:
	std::string	m_strFileType;
};

//------------------------------------------------------------------------------
static void EditorWarningCB(const char* message, void*)
{
	GetMainFrame()->OutputString(message);
}


//============================================================================//
// function
//============================================================================// 

BEGIN_EVENT_TABLE(WxMainFrame, wxFrame)

//menu
EVT_MENU(ID_Open, WxMainFrame::OnOpen)
EVT_MENU(ID_NewWidgetFile, WxMainFrame::OnNewWidgetFile)
EVT_UPDATE_UI(ID_NewWidgetFile, WxMainFrame::OnUpdateNewWidgetFile)
EVT_MENU(ID_Exit, WxMainFrame::OnExit)
EVT_MENU(ID_About, WxMainFrame::OnAbout)
EVT_UPDATE_UI(ID_Close, WxMainFrame::OnUpdateClose)
EVT_MENU(ID_Close, WxMainFrame::OnClose)
EVT_UPDATE_UI(ID_Save, WxMainFrame::OnUpdateSave)
EVT_MENU(ID_Save, WxMainFrame::OnSave)
EVT_UPDATE_UI(ID_SaveAs, WxMainFrame::OnUpdateSaveAs)
EVT_MENU(ID_SaveAs, WxMainFrame::OnSaveAs)
EVT_UPDATE_UI(ID_SaveAll, WxMainFrame::OnUpdateSaveAll)
EVT_MENU(ID_SaveAll, WxMainFrame::OnSaveAll)

EVT_UPDATE_UI(ID_CreateWidget, WxMainFrame::OnUpdateCreateWidget)
EVT_MENU(ID_CreateWidget, WxMainFrame::OnCreateWidget)

EVT_UPDATE_UI(ID_DeleteWidget, WxMainFrame::OnUpdateDeleteWidget)
EVT_MENU(ID_DeleteWidget, WxMainFrame::OnDeleteWidget)

EVT_UPDATE_UI(ID_WidgetUp, WxMainFrame::OnUpdateWidgetUp)
EVT_MENU(ID_WidgetUp, WxMainFrame::OnWidgetUp)
EVT_UPDATE_UI(ID_WidgetDown, WxMainFrame::OnUpdateWidgetDown)
EVT_MENU(ID_WidgetDown, WxMainFrame::OnWidgetDown)

//EVT_MENU(ID_RunScript, WxMainFrame::OnRunScript)

EVT_MENU(ID_VIEW_Fullscreen, WxMainFrame::OnFullscreen)

EVT_UPDATE_UI(ID_VIEW_800x600, WxMainFrame::OnUpdateResolution)
EVT_MENU(ID_VIEW_800x600, WxMainFrame::On800x600)
EVT_UPDATE_UI(ID_VIEW_1024x786, WxMainFrame::OnUpdateResolution)
EVT_MENU(ID_VIEW_1024x786, WxMainFrame::On1024x786)
EVT_UPDATE_UI(ID_VIEW_1280x800, WxMainFrame::OnUpdateResolution)
EVT_MENU(ID_VIEW_1280x800, WxMainFrame::On1280x800)
EVT_UPDATE_UI(ID_VIEW_Iphone480x320, WxMainFrame::OnUpdateResolution)
EVT_MENU(ID_VIEW_Iphone480x320, WxMainFrame::OnIphone480x320)
EVT_UPDATE_UI(ID_VIEW_Iphone320x480, WxMainFrame::OnUpdateResolution)
EVT_MENU(ID_VIEW_Iphone320x480, WxMainFrame::OnIphone320x480)
EVT_UPDATE_UI(ID_VIEW_Iphone960x640, WxMainFrame::OnUpdateResolution)
EVT_MENU(ID_VIEW_Iphone960x640, WxMainFrame::OnIphone960x640)
EVT_UPDATE_UI(ID_VIEW_Iphone640x960, WxMainFrame::OnUpdateResolution)
EVT_MENU(ID_VIEW_Iphone640x960, WxMainFrame::OnIphone640x960)
EVT_UPDATE_UI(ID_VIEW_Ipad1024x768, WxMainFrame::OnUpdateResolution)
EVT_MENU(ID_VIEW_Ipad1024x768, WxMainFrame::OnIpad1024x768)
EVT_UPDATE_UI(ID_VIEW_Ipad768x1024, WxMainFrame::OnUpdateResolution)
EVT_MENU(ID_VIEW_Ipad768x1024, WxMainFrame::OnIpad768x1024)

EVT_MENU(ID_ToggleScissor, WxMainFrame::OnToggleScissor)
EVT_MENU(ID_ToggleExtraInfo, WxMainFrame::OnToggleExtraInfo)
EVT_MENU(ID_ToggleWireframe, WxMainFrame::OnToggleWireframe)
EVT_MENU(ID_SetBGColor, WxMainFrame::OnSetBGColor)
EVT_MENU(ID_Refresh, WxMainFrame::OnRefresh)

EVT_KEY_DOWN(WxMainFrame::OnKeyDown)

EVT_CLOSE(WxMainFrame::OnCloseWindow)

EVT_MENU_RANGE( ID_RecentPathsBaseId, ID_RecentPathsEndId, WxMainFrame::OnRecentPaths )
EVT_MENU_RANGE( ID_RecentScenesBaseId, ID_RecentScenesEndId, WxMainFrame::OnRecentScenes )


//file tree
EVT_TREE_ITEM_MENU(WIDGET_ID_TreeCtrl_File, WxMainFrame::OnTreeItemMenu)
EVT_MENU(ID_ITEM_Widget_Render, WxMainFrame::OnTreeItemWidgetRender)
EVT_MENU(ID_ITEM_Widget_View, WxMainFrame::OnTreeItemWidgetView)
EVT_MENU(ID_ITEM_Widget_Edit, WxMainFrame::OnTreeItemWidgetEdit)
EVT_MENU(ID_ITEM_Edit_EXTERNAL, WxMainFrame::OnTreeItemEditExternal)
EVT_MENU(ID_ITEM_Script_Edit, WxMainFrame::OnTreeItemScriptEdit)
EVT_MENU(ID_ITEM_Image_Edit, WxMainFrame::OnTreeItemImageEdit)

//widget tree
EVT_TREE_SEL_CHANGED(WIDGET_ID_TreeCtrl_Widget, WxMainFrame::OnWidgetTreeSelected) 

//notebook
EVT_AUINOTEBOOK_PAGE_CHANGED(WIDGET_ID_NoteBook_Canvas, OnBookPageChanged)
EVT_AUINOTEBOOK_PAGE_CLOSE(WIDGET_ID_NoteBook_Canvas, OnBookPageClose)

//property sheet
EVT_PG_CHANGED( WIDGET_ID_PG, WxMainFrame::OnPropertyGridChange )

END_EVENT_TABLE()

//------------------------------------------------------------------------------
WxMainFrame::WxMainFrame(wxWindow* parent,
				 wxWindowID id,
				 const wxString& title,
				 const wxPoint& pos,
				 const wxSize& size,
				 long style)
				 : wxFrame(parent, id, title, pos, size, style)
				 ,m_bIsSceneOpened(false)
				 ,m_pCanvas(NULL)
				 ,m_pPropGridMan(NULL)
				 ,m_pOutput(NULL)
				 ,m_pToolbar(NULL)
				 ,m_pTreeCtrl_Widget(NULL)
				 ,m_pTreeCtrl_File(NULL)
				 //,m_pNoteBook_Config(NULL)
				 ,m_pNoteBook_Canvas(NULL)
				 ,m_aScreenSize(800, 600)
				 ,m_aBGColor(128,128,128,255)
				 ,m_pCurrentEditingWidget( NULL )
{
	// tell wxAuiManager to manage this frame
	m_mgr.SetManagedWindow(this);

	//set icon
	SetIcon(wxIcon(icon_xpm));


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create main menu
	CreateMenu();


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//STATUS
	CreateStatusBar();
	GetStatusBar()->SetStatusText(_("Ready"));



	///////////////////////////////////////////////////////////////////////////////////////////////////
	// min size for the frame itself isn't completely done.
	// see the end up wxAuiManager::Update() for the test
	// code. For now, just hard code a frame minimum size
	SetMinSize(wxSize(400,300));


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create some toolbars
	m_pToolbar = CreateToolbar();


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create notebook
	m_pNoteBook_Canvas = CreateCanvasNotebook();
	//m_pNoteBook_Config = CreateConfigNotebook();


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//// create property sheet
	wxPanel* pGridPanel = CreatePropGridPanel();



	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create file tree
	m_pTreeCtrl_File = CreateFileTreeCtrl();
	ResetFileTreeCtrl();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create widget tree
	m_pTreeCtrl_Widget = CreateWidgetTreeCtrl();
	ResetWidgetTreeCtrl();
	

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create putput
	m_pOutput = CreateOutput();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//add to aui manager
	m_mgr.AddPane(m_pToolbar, wxAuiPaneInfo().
		Name(wxT("tb1")).Caption(wxT("Toolbar")).
		ToolbarPane().Top().Row(1).
		LeftDockable(false).RightDockable(false));

	m_mgr.AddPane(m_pNoteBook_Canvas, wxAuiPaneInfo().Name(wxT("notebook_content")).
		CenterPane().PaneBorder(false));

	m_mgr.AddPane(m_pOutput, wxAuiPaneInfo().
		Name(wxT("Output")).Caption(wxT("Output")).
		Bottom().
		Dockable(true));

	//m_mgr.AddPane(m_pNoteBook_Config, wxAuiPaneInfo().BestSize(150,600).
	//	Name(wxT("Config")).Caption(wxT("Config")).
	//	Left().Layer(1).Position(1));

	m_mgr.AddPane(m_pTreeCtrl_File, wxAuiPaneInfo().BestSize(150,600).
		Name(wxT("Scene")).Caption(wxT("Scene")).
		Left().Layer(1).Position(1));

	m_mgr.AddPane(m_pTreeCtrl_Widget, wxAuiPaneInfo().BestSize(150,600).
		Name(wxT("Widget")).Caption(wxT("Widget")).
		Left().Layer(1).Position(2));

	//m_pNoteBook_Config->AddPage(m_pTreeCtrl_File, _T("Scene"));
	//m_pNoteBook_Config->AddPage(m_pTreeCtrl_Widget, _T("Widget"));	

	m_mgr.AddPane(pGridPanel, wxAuiPaneInfo().BestSize(250,600).
		Name(wxT("Property")).Caption(wxT("Property")).
		Right().Layer(1).Position(1));

	Show(true);

	// "commit" all changes made to wxAuiManager
	m_mgr.Update();


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//initialize guiex system
	try
	{
		CGUISystem* pGUISystem = new CGUISystem;
		GSystem->Initialize();
#ifdef NDEBUG
#else
		GUI_LOG->Open( "gui editor",CGUILogMsg::FLAG_TIMESTAMP_LITE	/*|CGUILogMsg::FLAG_STDERR*/);
		GUI_LOG->SetPriorityMask(GUI_LM_DEBUG	| GUI_LM_TRACE	|GUI_LM_WARNING|GUI_LM_ERROR);
#endif
		//GSystem->SetRunScript(GetMenuBar()->GetMenu(GetMenuBar()->FindMenu(_T("Edit")))->IsChecked(ID_RunScript));
		GSystem->SetRunScript(false);
		CGUIAssert::SetWarningCB(EditorWarningCB, NULL);
		GSystem->SetScreenSize(m_aScreenSize.x, m_aScreenSize.y);

		//register interface
		//GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengl);
		GUI_REGISTER_INTERFACE_LIB( IGUIImageLoader_tga);
		GUI_REGISTER_INTERFACE_LIB( IGUIFileSys_stdio);
		GUI_REGISTER_INTERFACE_LIB( IGUIMouse_winapi);
		GUI_REGISTER_INTERFACE_LIB( IGUIFont_ft2);
		//GUI_REGISTER_INTERFACE_LIB( IGUIFont_dummy);
		GUI_REGISTER_INTERFACE_LIB( IGUIKeyboard_winapi);
		GUI_REGISTER_INTERFACE_LIB( IGUIConfigFile_tinyxml);
		GUI_REGISTER_INTERFACE_LIB( IGUIStringConv_winapi);
		//GUI_REGISTER_INTERFACE_LIB( LIBGUIEX_COMMAND_TCP_DLL);
		//GUI_REGISTER_INTERFACE_LIB( LIBGUIEX_SOUND_OPENAL_DLL);
		GUI_REGISTER_INTERFACE_LIB( IGUIScript_lua);
		GUI_REGISTER_INTERFACE_LIB_ARG( IGUIIme_winapi, (HWND*)GetHandle());

		//register widget
		CGUIWidgetGenerator** pGenerator = GetAllGenerators();
		while(*pGenerator)
		{
			CGUIWidgetFactory::Instance()->RegisterGenerator( *pGenerator);
			pGenerator ++;
		}
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );

		GSystem->Release();
		delete GSystem;
	}
}
//------------------------------------------------------------------------------
WxMainFrame::~WxMainFrame()
{
	CloseCanvas();
	CloseScene();
	m_mgr.UnInit();	
	
	//release libguiex system
	GSystem->Release();
	delete GSystem;
}
//------------------------------------------------------------------------------
WxOutputPanel* WxMainFrame::CreateOutput()
{
	WxOutputPanel * pOutput = new WxOutputPanel( this, wxID_ANY);
	return pOutput;
}
//------------------------------------------------------------------------------
wxPanel* WxMainFrame::CreatePropGridPanel()
{
	wxPanel* panel = new wxPanel(this,-1,wxDefaultPosition,wxDefaultSize);

	int style = 
		wxPG_BOLD_MODIFIED |
		wxPG_SPLITTER_AUTO_CENTER |
		//wxPG_AUTO_SORT |
		//wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER |
		//wxPG_TOOLTIPS |
		//wxPG_HIDE_CATEGORIES |
		//wxPG_LIMITED_EDITING |
		wxPG_TOOLBAR |
		wxPG_DESCRIPTION;

	int extraStyle = 
		wxPG_EX_MODE_BUTTONS |
		//| wxPG_EX_AUTO_UNSPECIFIED_VALUES
		//| wxPG_EX_GREY_LABEL_WHEN_DISABLED
		//| wxPG_EX_NATIVE_DOUBLE_BUFFERING
		//| wxPG_EX_HELP_AS_TOOLTIPS
		wxPG_EX_MULTIPLE_SELECTION;

	m_pPropGridMan = new wxPropertyGridManager(
		panel,
		WIDGET_ID_PG, 
		wxDefaultPosition,
		wxSize(100, 100),
		style );
	m_pPropGridMan->SetExtraStyle( extraStyle );
	m_pPropGridMan->SetValidationFailureBehavior( wxPG_VFB_BEEP | wxPG_VFB_MARK_CELL | wxPG_VFB_SHOW_MESSAGE );
	m_pPropGridMan->GetGrid()->SetVerticalSpacing( 2 );
	// Register all editors (SpinCtrl etc.)
	m_pPropGridMan->RegisterAdditionalEditors();

	m_pPropGridMan->AddPage(wxT(NOTEBOOK_APPEARANCE_PAGE_NAME));
	m_pPropGridMan->AddPage(wxT(NOTEBOOK_IMAGE_PAGE_NAME));
	m_pPropGridMan->AddPage(wxT(NOTEBOOK_EVENT_PAGE_NAME));
	m_pPropGridMan->SelectPage(NOTEBOOK_PAGE_APPEARANCE);
	m_pPropGridMan->Refresh();

	//initialize image
	wxInitAllImageHandlers();

	//set sizer
	wxBoxSizer* topSizer = new wxBoxSizer ( wxVERTICAL );
	topSizer->Add( m_pPropGridMan, 1, wxEXPAND );
	panel->SetSizer( topSizer );
	topSizer->SetSizeHints( panel );

	return panel;
}
//------------------------------------------------------------------------------
WxEditorCanvasContainer* WxMainFrame::GetCanvasContainer()
{
	return m_pCanvas;
}
//------------------------------------------------------------------------------
void WxMainFrame::UpdateWidgetSizeAndPos()
{
	if( !m_pCurrentEditingWidget )
	{
		return;
	}
	const CGUIProperty& rSet = CPropertyConfigMgr::Instance()->GetPropertySet( m_pCurrentEditingWidget->GetType() );
	const CGUIProperty* pPropSize = rSet.GetProperty("size");
	SetPropertyByType( m_pPropGridMan, NULL, pPropSize, m_pCurrentEditingWidget );
	const CGUIProperty* pPropPos = rSet.GetProperty("position");
	SetPropertyByType( m_pPropGridMan, NULL, pPropPos, m_pCurrentEditingWidget );

	m_pCurrentEditingWidget->Refresh();
}
//------------------------------------------------------------------------------
void WxMainFrame::SetPropGridWidget(CGUIWidget* pWidget)
{
	m_pCurrentEditingWidget = pWidget;

	m_pPropGridMan->ClearPage(NOTEBOOK_PAGE_APPEARANCE);
	m_pPropGridMan->ClearPage(NOTEBOOK_PAGE_IMAGE);
	m_pPropGridMan->ClearPage(NOTEBOOK_PAGE_EVENT);

	if( m_pCurrentEditingWidget )
	{
		LoadWidgetConfig(m_pPropGridMan, m_pCurrentEditingWidget->GetType(), m_pCurrentEditingWidget);
	}

	m_pPropGridMan->Refresh();
}
//------------------------------------------------------------------------------
wxTreeCtrl*	WxMainFrame::CreateWidgetTreeCtrl()
{
	wxTreeCtrl* tree = new wxTreeCtrl(this, WIDGET_ID_TreeCtrl_Widget,
		wxPoint(0,0), wxSize(160,250),
		wxTR_DEFAULT_STYLE | wxNO_BORDER);

	//wxImageList* imglist = new wxImageList(16, 16, true, 2);
	//imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
	//imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
	//tree->AssignImageList(imglist);

	return tree;
}
//------------------------------------------------------------------------------
wxTreeCtrl*	WxMainFrame::CreateFileTreeCtrl()
{
	wxTreeCtrl* tree = new wxTreeCtrl(this, WIDGET_ID_TreeCtrl_File,
		wxPoint(0,0), wxSize(160,250),
		wxTR_DEFAULT_STYLE | wxNO_BORDER);

	wxImageList* imglist = new wxImageList(16, 16, true, 2);
	imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
	imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
	tree->AssignImageList(imglist);

	return tree;
}
//------------------------------------------------------------------------------
void			WxMainFrame::ResetFileTreeCtrl()
{
	m_pTreeCtrl_File->DeleteAllItems();

	wxTreeItemId root = m_pTreeCtrl_File->AddRoot(wxT("Scene"), 0);

	m_pTreeCtrl_File->AppendItem(root, Gui2wxString(TITLE_WIDGET_CONFIG), 0);
	m_pTreeCtrl_File->AppendItem(root, Gui2wxString(TITLE_SCRIPT), 0);
	m_pTreeCtrl_File->AppendItem(root, Gui2wxString(TITLE_RESOURCE_CONFIG), 0);

	m_pTreeCtrl_File->Expand(root);
}
//------------------------------------------------------------------------------
void			WxMainFrame::ResetWidgetTreeCtrl()
{
	m_pTreeCtrl_Widget->DeleteAllItems();
	m_mapTreeItem.clear();
}
//------------------------------------------------------------------------------
void			WxMainFrame::RefreshWidgetTreeCtrl()
{
	ResetWidgetTreeCtrl();
	
	if( !GSystem->IsInitialized())
	{
		//no editable page now
		return;
	}

	if( GSystem->GetOpenedPageNum() != 1 )
	{
		return;
	}
	CGUIWidget* pWidgetRoot = GSystem->GetOpenedPageByIndex(0);
	if( !pWidgetRoot)
	{
		//no root widget
		return;
	}

	std::string strItemId = pWidgetRoot->GetName() + " <"+pWidgetRoot->GetType() + ">";
	wxTreeItemId aItemId = m_pTreeCtrl_Widget->AddRoot(Gui2wxString( strItemId));
	m_mapTreeItem.insert( std::make_pair( pWidgetRoot, aItemId));

	CGUIWidget* pChild = pWidgetRoot->GetChild();
	while( pChild )
	{
		AddWidgetToTreeCtrl( pChild, aItemId );
		pChild = pChild->GetNextSibling();
	}
}
//------------------------------------------------------------------------------
void			WxMainFrame::AddWidgetToTreeCtrl(CGUIWidget* pWidget, wxTreeItemId aParentId )
{
	wxASSERT( aParentId.IsOk());

	std::string strItemId = pWidget->GetName() + " <" + pWidget->GetType() + ">";
	wxTreeItemId aItemId = m_pTreeCtrl_Widget->AppendItem(aParentId, Gui2wxString( strItemId));
	m_mapTreeItem.insert( std::make_pair( pWidget, aItemId));

	CGUIWidget* pChild = pWidget->GetChild();
	while( pChild )
	{
		if( !CGUIWidgetManager::IsInternalWidget( pChild->GetName()))
		{
			AddWidgetToTreeCtrl( pChild, aItemId );
		}
		pChild = pChild->GetNextSibling();
	}
}
//------------------------------------------------------------------------------
void			WxMainFrame::AddToFileTreeCtrl( const std::string& rFileName, const std::string& rType)
{
	wxTreeItemId nItemId = GetFileItemByName(rType);
	//if(nItemId.IsOk() == false);
	//{
	//	wxMessageBox( "unknown file type", "error", wxICON_ERROR);
	//	return;
	//}
	wxTreeItemId aFileId = m_pTreeCtrl_File->AppendItem(nItemId, Gui2wxString(rFileName), 1);
	CTreeNode* pNode = new CTreeNode;
	pNode->m_strFileType = rType;
	m_pTreeCtrl_File->SetItemData( aFileId, pNode );
	m_pTreeCtrl_File->Expand(nItemId);
}
//------------------------------------------------------------------------------
wxTreeItemId	WxMainFrame::GetFileItemByName(const std::string& rItemName)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId nItemId = m_pTreeCtrl_File->GetFirstChild( m_pTreeCtrl_File->GetRootItem(),cookie );
	
	while( nItemId.IsOk())
	{
		if( rItemName == m_pTreeCtrl_File->GetItemText(nItemId).char_str(wxConvUTF8).data() )
		{
			return nItemId;
		}

		nItemId = m_pTreeCtrl_File->GetNextSibling(nItemId);
	}

	wxASSERT(nItemId.IsOk());	
	return nItemId;
}
//------------------------------------------------------------------------------
wxToolBar*		WxMainFrame::CreateToolbar()
{
	//wxToolBar* tb1 = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
	//	wxTB_FLAT | wxTB_NODIVIDER);
	//tb1->SetToolBitmapSize(wxSize(16,16));
	//wxBitmap tb1_bmp1 = wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(16,16));
	//tb1->AddTool(101, wxT("Test"), tb1_bmp1);
	//tb1->AddSeparator();
	//tb1->AddTool(101, wxT("Test"), tb1_bmp1);
	//tb1->AddTool(101, wxT("Test"), tb1_bmp1);
	//tb1->AddTool(101, wxT("Test"), tb1_bmp1);
	//tb1->AddTool(101, wxT("Test"), tb1_bmp1);
	//tb1->AddSeparator();
	//tb1->AddTool(101, wxT("Test"), tb1_bmp1);
	//tb1->AddTool(101, wxT("Test"), tb1_bmp1);
	//tb1->Realize();

	wxToolBar* tb1 = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_NODIVIDER);
	//
	//tb1->AddTool(wxID_NEW, wxBitmap (new_xpm));
	tb1->AddTool(ID_Open, wxBitmap (open_xpm));
	tb1->AddTool(ID_Save, wxBitmap (save_xpm));
	tb1->AddSeparator();
	//tb1->AddTool(ID_EDIT_CUT, wxBitmap (cut_xpm));
	//tb1->AddTool(ID_EDIT_COPY, wxBitmap (copy_xpm));
	//tb1->AddTool(ID_EDIT_PASTE, wxBitmap (paste_xpm));
	//tb1->AddTool(ID_EDIT_CLEAR, wxBitmap (delete_xpm));
	tb1->AddTool(ID_CreateWidget, wxBitmap (new_xpm));
	tb1->AddTool(ID_DeleteWidget, wxBitmap (delete_xpm));
	tb1->AddTool(ID_WidgetUp, wxBitmap (bookmarkBluegem), _T("move widget up"));
	tb1->AddTool(ID_WidgetDown, wxBitmap (bookmarkRedgem), _T("move widget down"));
	tb1->AddSeparator();
	tb1->AddTool(ID_About, wxBitmap (help_xpm));

	// Don't forget this one!
	tb1->Realize();
	return tb1;
}
//------------------------------------------------------------------------------
wxAuiNotebook* WxMainFrame::CreateCanvasNotebook()
{
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create the notebook off-window to avoid flicker
	wxSize client_size = GetClientSize();

	wxAuiNotebook* ctrl = new wxAuiNotebook(this, WIDGET_ID_NoteBook_Canvas,
		wxPoint(client_size.x, client_size.y),
		wxSize(430,200),
		wxAUI_NB_DEFAULT_STYLE/* | wxAUI_NB_WINDOWLIST_BUTTON | wxAUI_NB_SCROLL_BUTTONS*/);
	ctrl->SetBackgroundColour( *wxLIGHT_GREY );
	//ctrl->SetNextHandler(this);
	return ctrl;
}
//------------------------------------------------------------------------------
//wxAuiNotebook* WxMainFrame::CreateConfigNotebook()
//{
//	wxSize client_size = GetClientSize();
//
//	wxAuiNotebook* ctrl = new wxAuiNotebook(this, WIDGET_ID_NoteBook_Config,
//		wxPoint(0,0), 
//		wxSize(160,250),
//		wxAUI_NB_BOTTOM | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE |wxAUI_NB_SCROLL_BUTTONS );
//	ctrl->SetBackgroundColour( *wxLIGHT_GREY );
//	ctrl->SetArtProvider(new wxAuiSimpleTabArt);
//
//	return ctrl;
//}
//------------------------------------------------------------------------------
int		WxMainFrame::OpenScene( const CGUIScene* pScene )
{
	m_strCurrentSceneName = pScene->GetSceneName();
	OutputString( std::string("Open Scene File: ") + m_strCurrentSceneName );

	//add widget file to tree
	const std::vector<CGUIString>& rWidgetFiles = pScene->GetWidgetFiles();
	for( unsigned i=0; i<rWidgetFiles.size(); ++i )
	{
		AddToFileTreeCtrl(rWidgetFiles[i], TITLE_WIDGET_CONFIG);

	}

	//add script file to tree
	const std::vector<CGUIString>& rScriptFiles = pScene->GetScriptFiles();
	for( unsigned i=0; i<rScriptFiles.size(); ++i )
	{
		AddToFileTreeCtrl(rScriptFiles[i], TITLE_SCRIPT);

	}

	//add resource list file to tree
	const std::vector<CGUIString>& rResourceFiles = pScene->GetResourceFiles();
	for( unsigned i=0; i<rResourceFiles.size(); ++i )
	{
		AddToFileTreeCtrl(rResourceFiles[i], TITLE_RESOURCE_CONFIG);
	}


	//load resource file
	try
	{	
		if( 0 != CGUISceneManager::Instance()->LoadResources(m_strCurrentSceneName))
		{
			return -1;
		}
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );
		return -1;
	}

	//update image list
	UpdateImageNameList();

	//update as list
	UpdateAsNameList();

	m_bIsSceneOpened = true;

	return 0;
}
//------------------------------------------------------------------------------
void	WxMainFrame::CloseScene( )
{
	if( m_bIsSceneOpened )
	{
		OutputString( "Close Scene...");

		ResetFileTreeCtrl();
		ResetWidgetTreeCtrl();

		CloseCanvas();

		GSystem->ReleaseAllResources();
		CGUISceneManager::Instance()->UnregisterAllScenes();

		m_bIsSceneOpened = false;
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::DoUpdate()
{
	m_mgr.Update();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemWidgetView(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );
	wxString strFilename = m_pTreeCtrl_File->GetItemText(id);

	std::string viewer_exe;
	viewer_exe = "libguiex_viewer_release.exe";

	std::string strRunCommand = viewer_exe + " " + GSystem->GetDataPath() + " " +m_strCurrentSceneName + " " + strFilename.char_str(wxConvUTF8).data();
	wxExecute(Gui2wxString(strRunCommand), wxEXEC_ASYNC);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemWidgetRender(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );

	RenderFile( m_pTreeCtrl_File->GetItemText(id).char_str(wxConvUTF8).data());
	RefreshWidgetTreeCtrl();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemWidgetEdit(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );

	EditFile( m_pTreeCtrl_File->GetItemText(id).char_str(wxConvUTF8).data(), EFT_WIDGET);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemEditExternal(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );

	EditFileExternal( m_pTreeCtrl_File->GetItemText(id).char_str(wxConvUTF8).data());
}

//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemImageEdit(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );

	EditFile( m_pTreeCtrl_File->GetItemText(id).char_str(wxConvUTF8).data(), EFT_IMAGE);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemScriptEdit(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );

	EditFile( m_pTreeCtrl_File->GetItemText(id).char_str(wxConvUTF8).data(), EFT_SCRIPT);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnPropertyGridChange( wxPropertyGridEvent& event )
{
	if( !m_pCurrentEditingWidget )
	{
		wxMessageBox( _T("[WxMainFrame::OnPropertyGridChange] no editing widget"), _T("error") );
		return;
	}
	wxPGProperty* pPGProperty = event.GetProperty();
	if( !pPGProperty)
	{
		wxMessageBox( _T("[WxMainFrame::OnPropertyGridChange] failed to get wxPGProperty"), _T("error") );
		return;
	}
	wxPGProperty* pPGTop = event.GetMainParent();
	if( !pPGTop)
	{
		wxMessageBox( _T("[WxMainFrame::OnPropertyGridChange] failed to get main parent of wxPGProperty"), _T("error") );
		return;
	}

	guiex::CGUIProperty aGuiProperty;
	GenerateGUIProperty(m_pPropGridMan, pPGTop, aGuiProperty);

	try
	{ 
		m_pCurrentEditingWidget->InsertProperty( aGuiProperty );
		m_pCurrentEditingWidget->ProcessProperty(aGuiProperty);
		m_pCurrentEditingWidget->Refresh();
		LoadWidgetConfig( m_pPropGridMan, m_pCurrentEditingWidget->GetType(), m_pCurrentEditingWidget );
	}
	catch(guiex::CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString( rError.what()), _T("OnPropertyChanged error") );
		CloseCanvas();
	}
	catch(...)
	{
		wxMessageBox(_T("unknown error"), _T("OnPropertyChanged error") );
		CloseCanvas();
	}

	m_pCanvas->UpdateWindowBox();
	m_pCanvas->SetSaveFlag(true);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnBookPageChanged(wxAuiNotebookEvent& event)
{
	SetPropGridWidget( NULL );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnBookPageClose(wxAuiNotebookEvent& evt)
{
	wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
	if( SaveFileProcess(evt.GetSelection()) ==false)
	{
		evt.Veto();
	}
	else
	{
		if(ctrl->GetPage(ctrl->GetSelection()) == m_pCanvas)
		{
			m_pCanvas = NULL;
			SetPropGridWidget( NULL );
			ResetWidgetTreeCtrl();
		}
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnWidgetTreeSelected(wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();

	if(item.IsOk())
	{
		wxString strLabel = m_pTreeCtrl_Widget->GetItemText(item);
		int nEndPos = strLabel.Find(_(" <"));
		if( nEndPos==wxNOT_FOUND)
		{
			m_pCanvas->SetSelectedWidget( strLabel.char_str(wxConvUTF8).data() );
		}
		else
		{
			m_pCanvas->SetSelectedWidget( strLabel.Mid(0,nEndPos).char_str(wxConvUTF8).data() );
		}
	}
}
//------------------------------------------------------------------------------
void	WxMainFrame::SelectWidgetTreeItem(CGUIWidget* pWidget)
{
	if( pWidget )
	{
		TTreeItemMap::iterator itor = m_mapTreeItem.find( pWidget);
		wxASSERT( itor != m_mapTreeItem.end());

		if( m_pTreeCtrl_Widget->IsSelected(itor->second) == false)
		{
			m_pTreeCtrl_Widget->SelectItem( itor->second);
		}
	}
	else
	{
		wxTreeItemId aItemId = m_pTreeCtrl_Widget->GetSelection();
		if( aItemId.IsOk())
		{
			m_pTreeCtrl_Widget->UnselectAll();
		}
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemMenu(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
	CHECK_ITEM(itemId);
	m_pTreeCtrl_File->SelectItem(itemId);
	CTreeNode* pNode = (CTreeNode*)m_pTreeCtrl_File->GetItemData(itemId);
	if( !pNode )
	{
		return;
	}

	if( pNode->m_strFileType == TITLE_WIDGET_CONFIG )
	{
		//xml file
		wxMenu menu(_("Widget Config File"));
		menu.Append(ID_ITEM_Widget_Render, wxT("&Render"));
		menu.Append(ID_ITEM_Widget_View, wxT("&View"));
		menu.Append(ID_ITEM_Widget_Edit, wxT("&Edit"));
		menu.Append(ID_ITEM_Edit_EXTERNAL, wxT("&Edit External"));
		//menu.Enable(ID_ITEM_Widget_View, m_pCanvas==NULL);
		m_pTreeCtrl_File->PopupMenu(&menu, event.GetPoint());
	}
	else if(pNode->m_strFileType == TITLE_SCRIPT )
	{
		//script
		wxMenu menu(_("Script File"));
		menu.Append(ID_ITEM_Script_Edit, wxT("&Edit"));
		menu.Append(ID_ITEM_Edit_EXTERNAL, wxT("&Edit External"));
		m_pTreeCtrl_File->PopupMenu(&menu, event.GetPoint());
	}
	else if(pNode->m_strFileType == TITLE_RESOURCE_CONFIG )
	{
		//image
		wxMenu menu(_("Resource File"));
		menu.Append(ID_ITEM_Image_Edit, wxT("&Edit"));
		menu.Append(ID_ITEM_Edit_EXTERNAL, wxT("&Edit External"));
		m_pTreeCtrl_File->PopupMenu(&menu, event.GetPoint());
	}
	else
	{
		wxASSERT_MSG(0,_T("unknown file type"));
	}

	event.Skip();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnClose(wxCommandEvent& evt)
{
	if( SaveFileProcess(-1))
	{
		CloseScene();
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateClose(wxUpdateUIEvent& event)
{
	event.Enable(m_bIsSceneOpened);
}
//------------------------------------------------------------------------------
CSaveFileBase* WxMainFrame::GetSaveFilePtr( const wxWindow* pWindows )
{
	CSaveFileBase* pSaveFile = NULL;

	if( pWindows->IsKindOf(CLASSINFO(WxEditorCanvasContainer)))
	{
		//widget render
		pSaveFile = (WxEditorCanvasContainer*)pWindows;
	}
	else if( pWindows->IsKindOf(CLASSINFO(WxTextEditor)))
	{
		//editor's file
		pSaveFile = (WxTextEditor*)pWindows;
	}
	wxASSERT(pSaveFile);
	return pSaveFile;
}
//------------------------------------------------------------------------------
void WxMainFrame::OnSave(wxCommandEvent& evt)
{
	if(m_pNoteBook_Canvas && m_pNoteBook_Canvas->GetSelection() >=0)
	{
		CSaveFileBase* pSave = GetSaveFilePtr(m_pNoteBook_Canvas->GetPage(m_pNoteBook_Canvas->GetSelection()));

		if( pSave->ShouldSaveFile())
		{
			pSave->SaveFile();
		}
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateSave(wxUpdateUIEvent& event)
{
	if( m_bIsSceneOpened && m_pNoteBook_Canvas && m_pNoteBook_Canvas->GetSelection() >=0 )
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnSaveAs(wxCommandEvent& evt)
{
	wxFileDialog aDlg(this, _T("Save file as"), _T(""), _T(""), _T("xml files (*.xml)|*.xml|lua files (*.lua)|*.lua"), wxFD_SAVE, wxDefaultPosition, wxDefaultSize);
	if( wxID_OK == aDlg.ShowModal())
	{
		//select file
		if(m_pNoteBook_Canvas && m_pNoteBook_Canvas->GetSelection() >=0)
		{
			CSaveFileBase* pSave = GetSaveFilePtr(m_pNoteBook_Canvas->GetPage(m_pNoteBook_Canvas->GetSelection()));

			pSave->SaveFileAs(aDlg.GetPath().char_str(wxConvUTF8).data());
		}
	}
	else
	{
		//do nothing
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateSaveAs(wxUpdateUIEvent& event)
{
	if( m_bIsSceneOpened && m_pNoteBook_Canvas && m_pNoteBook_Canvas->GetSelection() >=0 )
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnWidgetDown(wxCommandEvent& evt)
{
	CGUIWidget* pWidget = m_pCanvas->GetSelectedWidget();
	//empty
	if( !pWidget)
	{
		return;
	}

	pWidget->MoveDown();
	m_pCanvas->SetSaveFlag(true);

	RefreshWidgetTreeCtrl();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateWidgetDown(wxUpdateUIEvent& event)
{
	if( m_pCanvas && m_pCanvas->GetSelectedWidget())
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnWidgetUp(wxCommandEvent& evt)
{
	CGUIWidget* pWidget = m_pCanvas->GetSelectedWidget();
	//empty
	if( !pWidget)
	{
		return;
	}

	pWidget->MoveUp();
	m_pCanvas->SetSaveFlag(true);

	RefreshWidgetTreeCtrl();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateWidgetUp(wxUpdateUIEvent& event)
{
	if( m_pCanvas && m_pCanvas->GetSelectedWidget())
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnDeleteWidget(wxCommandEvent& evt)
{
	CGUIWidget* pWidget = m_pCanvas->GetSelectedWidget();
	//empty
	if( !pWidget)
	{
		return;
	}

	if( GSystem->GetOpenedPageNum() !=0 &&
		pWidget == GSystem->GetOpenedPageByIndex(0))
	{
		//is page
		GSystem->ClosePage(pWidget);
		CGUIWidgetManager::Instance()->ReleasePage(pWidget);
	}
	else
	{
		if( pWidget->IsOpen() )
		{
			pWidget->Close();
		}
		CGUIWidgetManager::Instance()->DestroyWidget(pWidget);
	}

	m_pCanvas->SetSelectedWidget(NULL);
	m_pCanvas->SetSaveFlag(true);

	RefreshWidgetTreeCtrl();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateDeleteWidget(wxUpdateUIEvent& event)
{
	if( m_pCanvas && m_pCanvas->GetSelectedWidget())
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnRunScript(wxCommandEvent& evt)
{
	bool bIsChecked = evt.IsChecked();

	if( GSystem->IsInitialized())
	{
		GSystem->SetRunScript(bIsChecked);
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
void WxMainFrame::OnToggleExtraInfo(wxCommandEvent& evt)
{
	bool bIsChecked = evt.IsChecked();

	if( CGUIInterfaceManager::Instance()->GetInterfaceRender())
	{
		GSystem->SetDrawExtraInfo(bIsChecked);
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
	GSystem->GetCurrentRootWidget()->Refresh();
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
void WxMainFrame::OnFullscreen(wxCommandEvent& evt)
{
	ShowFullScreen(true);
	Refresh();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateResolution(wxUpdateUIEvent& event)
{
	event.Enable( GSystem->IsInitialized());
}
//------------------------------------------------------------------------------
void WxMainFrame::OnCreateWidget(wxCommandEvent& evt)
{
	wxASSERT( m_pCanvas );

	//create wizard
	WxWizardCreateWidget wizard(this, m_pCanvas->GetSelectedWidget());
	if( !wizard.RunWizard(wizard.GetFirstPage()))
	{
		//cancel
		return;
	}
	
	//create widget
	CGUIWidget* pWidget = wizard.GetWidget();
	if( !pWidget )
	{
		return;
	}

	if( GSystem->GetOpenedPageNum() == 0 )
	{
		CGUIWidgetManager::Instance()->AddPage( pWidget);
		GSystem->OpenPage( pWidget);
	}
	else if( m_pCanvas->GetSelectedWidget())
	{
		pWidget->Open();
	}
	else
	{
		GUI_ASSERT( 0, "failed to create widget");
	}

	RefreshWidgetTreeCtrl();
	m_pCanvas->SetSaveFlag(true);
	m_pCanvas->SetSelectedWidget(pWidget);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateCreateWidget(wxUpdateUIEvent& event)
{
	if( m_pCanvas )
	{
		if( GSystem->GetOpenedPageNum() == 0 ||
			m_pCanvas->GetSelectedWidget())
		{
			event.Enable(true);
			return;
		}
	}
	event.Enable(false);
}
//------------------------------------------------------------------------------
void WxMainFrame::SetResolution( int width, int height )
{
	GSystem->SetScreenSize(width, height);
	if( m_pCanvas )
	{
		m_pCanvas->SetScreenSize(width, height);
	}
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
void WxMainFrame::OnKeyDown(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_ESCAPE && IsFullScreen())
	{
		ShowFullScreen(false);
		Refresh();
		event.Skip();
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnSaveAll(wxCommandEvent& evt)
{
	if(m_pNoteBook_Canvas)
	{
		for( unsigned i=0; i< m_pNoteBook_Canvas->GetPageCount(); ++i)
		{
			CSaveFileBase* pSave = GetSaveFilePtr(m_pNoteBook_Canvas->GetPage(i));
			
			if( pSave->ShouldSaveFile())
			{
				pSave->SaveFile();
			}
		}
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateSaveAll(wxUpdateUIEvent& event)
{
	event.Enable(m_bIsSceneOpened);

}

void WxMainFrame::OnRecentPaths( wxCommandEvent& In )
{
	if( SaveFileProcess(-1) == false)
	{
		//cancel
		return;
	}
	CloseScene();

	unsigned nFileIdx = In.GetId() - ID_RecentPathsBaseId;

	std::string	strPath = CToolCache::Instance()->m_pathHistory[nFileIdx];
	GSystem->SetDataPath(strPath);
	try
	{
		CGUISceneManager::Instance()->UnregisterAllScenes();
		if( 0 != CGUISceneManager::Instance()->RegisterScenesFromDir())
		{
			wxMessageBox( _T("failed to load scenes"), _T("error") );
			return;
		}
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );
		return;
	}

	//chose scene
	const std::vector<CGUIString>& vecScenes = CGUISceneManager::Instance()->GetSceneNames( );
	wxArrayString arrayScenes;
	for( unsigned i=0; i<vecScenes.size(); ++i )
	{
		arrayScenes.Add( Gui2wxString( vecScenes[i]));
	}
	wxSingleChoiceDialog aChoiceDlg( this, _T("select scene"), _T("select scene files"), arrayScenes );
	if( aChoiceDlg.ShowModal() != wxID_OK )
	{
		return;
	}
	CGUIString strSceneFileName = vecScenes[aChoiceDlg.GetSelection()];

	//get scene info
	const CGUIScene* pScene = CGUISceneManager::Instance()->GetScene( strSceneFileName );
	if( pScene->IsDependenciesLoaded())
	{
		if( 0 != OpenScene(pScene))
		{
			wxMessageBox( _T("failed to open scene"), _T("error") );
			CloseScene();
			return;
		}
	}
	else
	{
		wxMessageBox( _T("some dependent scene of this scene hasn't been loaded"), _T("error") );
		return;
	}

	CToolCache::Instance()->AddCache( strSceneFileName, strPath );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnRecentScenes( wxCommandEvent& In )
{
	if( SaveFileProcess(-1) == false)
	{
		//cancel
		return;
	}
	CloseScene();

	unsigned nFileIdx = In.GetId() - ID_RecentScenesBaseId;

	std::pair< std::string, std::string>	strScene = CToolCache::Instance()->m_sceneHistory[nFileIdx];
	GSystem->SetDataPath(strScene.second);
	try
	{
		CGUISceneManager::Instance()->UnregisterAllScenes();
		if( 0 != CGUISceneManager::Instance()->RegisterScenesFromDir())
		{
			wxMessageBox( _T("failed to load scenes"), _T("error") );
			return;
		}
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );
		return;
	}

	//get scene info
	const CGUIScene* pScene = CGUISceneManager::Instance()->GetScene( strScene.first );
	if( !pScene )
	{
		wxMessageBox( _T("failed to load scene"), _T("error") );
		return;
	}
	else if( pScene->IsDependenciesLoaded())
	{
		if( 0 != OpenScene(pScene))
		{
			wxMessageBox( _T("failed to open scene"), _T("error") );
			CloseScene();
			return;
		}
	}
	else
	{
		wxMessageBox( _T("some dependent scene of this scene hasn't been loaded"), _T("error") );
		return;
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnCloseWindow(wxCloseEvent& evt)
{
	if( false == SaveFileProcess(-1))
	{
		evt.SetCanVeto(true);
		evt.Veto();
	}
	else
	{
		evt.Skip();
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnNewWidgetFile(wxCommandEvent& evt)
{

}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateNewWidgetFile(wxUpdateUIEvent& event)
{
	event.Enable(m_bIsSceneOpened);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
	if( SaveFileProcess(-1) == false)
	{
		//cancel
		return;
	}

	CGUIString strPath = GSystem->GetDataPath();
	wxDirDialog aDlg( this, _T("Choose a libguiex root path"), Gui2wxString( strPath));
	if( wxID_OK != aDlg.ShowModal())
	{
		return;
	}

	CloseScene();

	std::string strDataPath = (aDlg.GetPath() + wxT("\\")).char_str(wxConvUTF8).data();
	GSystem->SetDataPath(strDataPath);

	try
	{
		CGUISceneManager::Instance()->UnregisterAllScenes();
		if( 0 != CGUISceneManager::Instance()->RegisterScenesFromDir())
		{
			wxMessageBox( _T("failed to load scenes"), _T("error") );
			return;
		}
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );
		return;
	}
	
	//chose scene
	const std::vector<CGUIString>& vecScenes = CGUISceneManager::Instance()->GetSceneNames( );
	wxArrayString arrayScenes;
	for( unsigned i=0; i<vecScenes.size(); ++i )
	{
		arrayScenes.Add( Gui2wxString( vecScenes[i]));
	}
	wxSingleChoiceDialog aChoiceDlg( this, _T("select scene"), _T("select scene files"), arrayScenes );
	if( aChoiceDlg.ShowModal() != wxID_OK )
	{
		return;
	}
	CGUIString strSceneFileName = vecScenes[aChoiceDlg.GetSelection()];

	//get scene info
	const CGUIScene* pScene = CGUISceneManager::Instance()->GetScene( strSceneFileName );
	if( pScene->IsDependenciesLoaded())
	{
		if( 0 != OpenScene(pScene))
		{
			wxMessageBox( _T("failed to open scene"), _T("error") );
			CloseScene();
			return;
		}
	}
	else
	{
		wxMessageBox( _T("some dependent scene of this scene hasn't been loaded"), _T("error") );
		return;
	}

	CToolCache::Instance()->AddCache( strSceneFileName, strDataPath );
}
//------------------------------------------------------------------------------
void	WxMainFrame::CloseCanvas()
{
	if( m_pCanvas)
	{
		int idx = m_pNoteBook_Canvas->GetPageIndex(m_pCanvas);
		if( wxNOT_FOUND != idx )
		{
			SaveFileProcess(idx);
			m_pNoteBook_Canvas->DeletePage(idx);
		}
		m_pCanvas = NULL;
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::RenderFile( const std::string& rFileName )
{
	CloseCanvas();
	CGUIScene* pScene = CGUISceneManager::Instance()->GetScene(m_strCurrentSceneName);

	std::string strAbsFileName = GSystem->GetDataPath() + pScene->GetScenePath() + rFileName;

	m_pCanvas = new WxEditorCanvasContainer(m_pNoteBook_Canvas, strAbsFileName);
	m_pNoteBook_Canvas->AddPage( m_pCanvas, Gui2wxString(rFileName), true );
	//m_pCanvas->SetNextHandler( m_pNoteBook_Canvas );
	try
	{
		//load xml widget by libguiex
		CGUIWidget* pWidget = CGUIWidgetManager::Instance()->LoadPage( rFileName, m_strCurrentSceneName );
		if( pWidget )
		{
			GSystem->OpenPage(pWidget);
		}
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );

		CloseCanvas();
	}
	catch(...)
	{
		wxMessageBox(_T("unknown error"), _T("error") );

		CloseCanvas();
	}
}
//------------------------------------------------------------------------------
void	WxMainFrame::EditFileExternal( const std::string& rFileName )
{
	CGUIScene* pScene = CGUISceneManager::Instance()->GetScene(m_strCurrentSceneName);

	std::string strAbsPath = GSystem->GetDataPath() + pScene->GetScenePath();
	std::string strAbsFileName = strAbsPath + rFileName;

	::ShellExecute( 
		NULL, 
		_T("open"),
		Gui2wxString( strAbsFileName).data(), 
		NULL, 
		Gui2wxString( strAbsPath).data(),
		SW_SHOWDEFAULT);
}
//------------------------------------------------------------------------------
void	WxMainFrame::EditFile( const std::string& rFileName, EFileType eFileType )
{
	CGUIScene* pScene = CGUISceneManager::Instance()->GetScene(m_strCurrentSceneName);

	std::string strAbsPath = 
		GSystem->GetDataPath() +
		pScene->GetScenePath() +
		rFileName;

	//check
	for( unsigned i=0; i< m_pNoteBook_Canvas->GetPageCount(); ++i)
	{
		CSaveFileBase* pSave = GetSaveFilePtr(m_pNoteBook_Canvas->GetPage(i));

		if( pSave->GetFileName() == strAbsPath)
		{
			m_pNoteBook_Canvas->SetSelection(i);
			return;
		}
	}


	WxTextEditor* pTextEditor = new WxTextEditor( m_pNoteBook_Canvas, strAbsPath );

	if( 0 != pTextEditor->InitEditor(strAbsPath, eFileType))
	{
		//error
		delete pTextEditor;
		return;
	}
	m_pNoteBook_Canvas->AddPage( pTextEditor, Gui2wxString(rFileName),true);
}
//------------------------------------------------------------------------------
bool	WxMainFrame::SaveFileProcess(int nIdx)
{
	wxArrayString aFileArray;
	std::vector<CSaveFileBase*> aSaveArray;

	if( nIdx == -1 )
	{
		//save all page
		for( unsigned i=0; i< m_pNoteBook_Canvas->GetPageCount(); ++i)
		{
			//get save file base class
			CSaveFileBase* pSave = GetSaveFilePtr(m_pNoteBook_Canvas->GetPage(i));
			
			if( pSave->ShouldSaveFile())
			{
				aFileArray.Add( Gui2wxString(pSave->GetFileName()));
				aSaveArray.push_back(pSave);
			}
		}
	}
	else
	{
		//save selected page
		wxASSERT(m_pNoteBook_Canvas->GetPageCount() > size_t(nIdx ));
		CSaveFileBase* pSave = GetSaveFilePtr(m_pNoteBook_Canvas->GetPage(nIdx));

		if( pSave->ShouldSaveFile())
		{
			aFileArray.Add( Gui2wxString( pSave->GetFileName()).data());
			aSaveArray.push_back(pSave);
		}
	}


	//open dlg
	if( aFileArray.IsEmpty())
	{
		return true;
	}
	else
	{
		WxSaveFileDialog aDlg(this, aFileArray);
		aDlg.ShowModal();
		switch( aDlg.GetReturnCode())
		{
		case wxID_OK:
			for( std::vector<CSaveFileBase*>::iterator itor = aSaveArray.begin();
				itor != aSaveArray.end();
				++itor)
			{
				int ret = (*itor)->SaveFile();
				wxASSERT(ret == 0);
			}
			return true;
		case wxID_NO:
			return true;
		case wxID_CANCEL:
			return false;
		}
	}

	return true;
}
//------------------------------------------------------------------------------
void WxMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(_("libguiex editor"), _("About"), wxOK, this);
}
//------------------------------------------------------------------------------
void	 WxMainFrame::OutputString( const std::string& rString)
{
	m_pOutput->AppendText(Gui2wxString(rString).data());
	m_pOutput->AppendText(wxString(_T("\n")));
}
//------------------------------------------------------------------------------
void WxMainFrame::UpdateStatusBar (wxChar *format, ...) 
{
	wxChar        buffer[1024] ;

	// initialize variable arguments list
	va_list arguments;
	va_start(arguments, format);
	// create the status bar text
	wxVsnprintf (buffer, sizeof(buffer), format, arguments) ;
	// cleanup arguments.
	va_end(arguments);

	GetStatusBar()->SetStatusText(buffer);
}
//------------------------------------------------------------------------------
void			WxMainFrame::CreateMenu()
{
	//update cache file
	CToolCache::Instance()->ParseCache(wxGetApp().GetBaseDir() + ".\\libguiex_editor_cache.xml");
	CToolCache::Instance()->SetMaxCacheSize(10);
	CToolCache::Instance()->SetPathsBaseId(ID_RecentPathsBaseId);
	CToolCache::Instance()->SetScenesBaseId(ID_RecentScenesBaseId);
	CToolCache::Instance()->UpdateMenu();

	wxMenuBar* mb = new wxMenuBar;

	//menu-file
	wxMenu* file_menu = new wxMenu;
	file_menu->Append(ID_Open, _("Open"));
	file_menu->AppendSeparator();
	file_menu->Append(ID_Save, _("Save"));
	file_menu->Append(ID_SaveAs, _("Save As"));
	file_menu->Append(ID_SaveAll, _("Save All"));
	file_menu->AppendSeparator();
	file_menu->Append(ID_Close, _("Close"));
	file_menu->AppendSeparator();
	file_menu->Append(ID_RecentScenes, _("Recent Scenes"), CToolCache::Instance()->m_pSceneMenu);
	file_menu->Append(ID_RecentPaths, _("Recent Paths"), CToolCache::Instance()->m_pPathMenu);
	file_menu->AppendSeparator();
	file_menu->Append(ID_Exit, _("Exit"));

	//menu-edit
	wxMenu*	edit_menu = new wxMenu;
	edit_menu->Append(ID_CreateWidget, _("Create Widget"));
	edit_menu->Append(ID_DeleteWidget, _("Delete Widget"));

	edit_menu->Append(ID_WidgetUp, _("Widget Up"));
	edit_menu->Append(ID_WidgetDown, _("Widget Down"));
	//edit_menu->Append(ID_RunScript, _("Run Script"),_T("Run script when render the page"), wxITEM_CHECK );

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
	view_menu->Append(ID_SetBGColor, wxT("Set BG Color"), wxT("set background color"));
	view_menu->Append(ID_Refresh, wxT("Refresh"), wxT("refresh widgets"));

	//menu-about
	wxMenu* help_menu = new wxMenu;
	help_menu->Append(ID_About, _("About..."));

	mb->Append(file_menu, _("File"));
	mb->Append(edit_menu, _("Edit"));
	mb->Append(view_menu, _("View"));
	mb->Append(help_menu, _("Help"));

	SetMenuBar(mb);
}
//------------------------------------------------------------------------------

