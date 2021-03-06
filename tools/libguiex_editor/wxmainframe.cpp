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
#include "guiframeworkeditor.h"
#include "wxmainapp.h"
#include "wxsavefiledlg.h"
#include "wxeditorcanvascontainer.h"
#include "wxoutputpanel.h"
#include "wxwizardcreatewidget.h"
#include "propertysheetfunc.h"
#include "propertyconfigmgr.h"
#include "toolsmisc.h"
#include "toolshistory.h"
#include "guiresourcepool.h"
#include "wxeditorid.h"
#include "propertyconvertor.h"
#include "command.h"
#include "commandmanager.h"

//lua

#ifdef __cplusplus
extern "C" {
#endif //#ifdef __cplusplus
#include "lua.h"
#include "lauxlib.h"
#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

//wxwidgets
#include <wx/treectrl.h>
#include <wx/artprov.h>
#include <wx/colordlg.h>

//libguiex module
#include <libguiex_widget/guiwgt.h>

//images
#include "../Resource/bitmaps/new.xpm"
#include "../Resource/bitmaps/open.xpm"
#include "../Resource/bitmaps/save.xpm"
#include "../Resource/bitmaps/cut.xpm"
#include "../Resource/bitmaps/copy.xpm"
#include "../Resource/bitmaps/paste.xpm"
#include "../Resource/bitmaps/delete.xpm"
#include "../Resource/bitmaps/help.xpm"
#include "../Resource/bitmaps/icon.xpm"
#include "../Resource/bitmaps/gem_blue.xpm"
#include "../Resource/bitmaps/gem_red.xpm"
#include "../Resource/bitmaps/changeparent.xpm"
#include "../Resource/bitmaps/undo.xpm"
#include "../Resource/bitmaps/redo.xpm"

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



EVT_UPDATE_UI(ID_Undo, WxMainFrame::OnUpdateUndo)
EVT_MENU(ID_Undo, WxMainFrame::OnUndo)
EVT_UPDATE_UI(ID_Redo, WxMainFrame::OnUpdateRedo)
EVT_MENU(ID_Redo, WxMainFrame::OnRedo)

EVT_UPDATE_UI(ID_CreateWidget, WxMainFrame::OnUpdateCreateWidget)
EVT_MENU(ID_CreateWidget, WxMainFrame::OnCreateWidget)
EVT_MENU_RANGE(ID_Localization_begin, ID_Localization_end, WxMainFrame::OnSetLocalization)

EVT_UPDATE_UI(ID_DeleteWidget, WxMainFrame::OnUpdateDeleteWidget)
EVT_MENU(ID_DeleteWidget, WxMainFrame::OnDeleteWidget)

EVT_UPDATE_UI(ID_WidgetUp, WxMainFrame::OnUpdateWidgetUp)
EVT_MENU(ID_WidgetUp, WxMainFrame::OnWidgetUp)
EVT_UPDATE_UI(ID_WidgetDown, WxMainFrame::OnUpdateWidgetDown)
EVT_MENU(ID_WidgetDown, WxMainFrame::OnWidgetDown)
EVT_UPDATE_UI(ID_WidgetChangeParent, WxMainFrame::OnUpdateWidgetChangeParent)
EVT_MENU(ID_WidgetChangeParent, WxMainFrame::OnWidgetChangeParent)
EVT_UPDATE_UI(ID_WidgetCopy, WxMainFrame::OnUpdateWidgetCopy)
EVT_MENU(ID_WidgetCopy, WxMainFrame::OnWidgetCopy)
EVT_UPDATE_UI(ID_WidgetPaste, WxMainFrame::OnUpdateWidgetPaste)
EVT_MENU(ID_WidgetPaste, WxMainFrame::OnWidgetPaste)

EVT_UPDATE_UI(ID_ParseScript, WxMainFrame::OnUpdateParseScript)
EVT_MENU(ID_ParseScript, WxMainFrame::OnParseScript)


EVT_MENU(ID_SetDefaultEditor, WxMainFrame::OnSetDefaultEditor)

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
EVT_MENU(ID_TogglePlayingAs, WxMainFrame::OnTogglePlayingAs)
EVT_MENU(ID_SetBGColor, WxMainFrame::OnSetBGColor)
EVT_MENU(ID_Refresh, WxMainFrame::OnRefresh)

EVT_KEY_DOWN(WxMainFrame::OnKeyDown)

EVT_CLOSE(WxMainFrame::OnCloseWindow)

EVT_MENU_RANGE( ID_RecentPathsBaseId, ID_RecentPathsEndId, WxMainFrame::OnRecentPaths )
EVT_MENU_RANGE( ID_RecentScenesBaseId, ID_RecentScenesEndId, WxMainFrame::OnRecentScenes )


//file tree
EVT_TREE_ITEM_MENU(ID_ID_TreeCtrl_File, WxMainFrame::OnTreeItemMenu)
EVT_MENU(ID_ITEM_Widget_Render, WxMainFrame::OnTreeItemWidgetRender)
EVT_MENU(ID_ITEM_Widget_View, WxMainFrame::OnTreeItemWidgetView)
EVT_MENU(ID_ITEM_Widget_Edit, WxMainFrame::OnTreeItemWidgetEdit)
EVT_MENU(ID_ITEM_Edit_External, WxMainFrame::OnTreeItemEditExternal)
EVT_MENU(ID_ITEM_Script_Edit, WxMainFrame::OnTreeItemScriptEdit)
EVT_MENU(ID_ITEM_Script_Check, WxMainFrame::OnTreeItemScriptCheck)
EVT_MENU(ID_ITEM_Image_Edit, WxMainFrame::OnTreeItemImageEdit)

//widget tree
EVT_TREE_SEL_CHANGED(ID_TreeCtrl_Widget, WxMainFrame::OnWidgetTreeSelected) 

//notebook
EVT_AUINOTEBOOK_PAGE_CHANGED(ID_NoteBookCtrl, OnBookPageChanged)
EVT_AUINOTEBOOK_PAGE_CLOSE(ID_NoteBookCtrl, OnBookPageClose)

//property sheet
EVT_PG_CHANGED( ID_GridManager, WxMainFrame::OnPropertyGridChange )

END_EVENT_TABLE()

//------------------------------------------------------------------------------
WxMainFrame::WxMainFrame(wxWindow* parent,
				 wxWindowID id,
				 const wxString& title,
				 const wxPoint& pos,
				 const wxSize& size,
				 long style)
				 : wxFrame(parent, id, title, pos, size, style )
				 ,m_bIsSceneOpened(false)
				 ,m_pCanvasContainer(NULL)
				 ,m_pPropGridMan(NULL)
				 ,m_pOutput(NULL)
				 ,m_pToolbar(NULL)
				 ,m_pTreeCtrl_Widget(NULL)
				 ,m_pTreeCtrl_File(NULL)
				 ,m_pAuiNoteBook(NULL)
				 ,m_aBGColor(128,128,128,255)
				 ,m_pCurrentEditingWidget( NULL )
{
	// tell wxAuiManager to manage this frame
	m_mgr.SetManagedWindow(this);

	//set icon
	SetIcon(wxIcon(icon_xpm));

	// create main menu
	CreateMenu();

	//STATUS
	CreateStatusBar();
	GetStatusBar()->SetStatusText(_("Ready"));

	SetMinSize(wxSize(400,300));

	// create some toolbars
	m_pToolbar = CreateToolbar();

	// create notebook
	m_pAuiNoteBook = CreateCanvasNotebook();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//// create property sheet
	wxPanel* pGridPanel = CreatePropGridPanel();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// create file tree
	m_pTreeCtrl_File = CreateFileTreeCtrl();
	ResetFileTreeCtrl();

	// create widget tree
	m_pTreeCtrl_Widget = new wxTreeCtrl(this, ID_TreeCtrl_Widget, wxPoint(0,0), wxSize(160,250), wxTR_DEFAULT_STYLE | wxNO_BORDER);

	// create putput
	m_pOutput = CreateOutput();

	//add to aui manager
	m_mgr.AddPane(m_pToolbar, wxAuiPaneInfo().
		Name(wxT("tb1")).Caption(wxT("Toolbar")).
		ToolbarPane().Top().Row(1).
		LeftDockable(false).RightDockable(false));

	m_mgr.AddPane(m_pAuiNoteBook, wxAuiPaneInfo().Name(wxT("notebook_content")).
		CenterPane().PaneBorder(false));

	m_mgr.AddPane(m_pOutput, wxAuiPaneInfo().
		Name(wxT("Output")).Caption(wxT("Output")).
		Bottom().
		Dockable(true));

	m_mgr.AddPane(m_pTreeCtrl_File, wxAuiPaneInfo().BestSize(150,600).
		Name(wxT("Scene")).Caption(wxT("Scene")).
		Left().Layer(1).Position(1));

	m_mgr.AddPane(m_pTreeCtrl_Widget, wxAuiPaneInfo().BestSize(150,600).
		Name(wxT("Widget")).Caption(wxT("Widget")).
		Left().Layer(1).Position(2));

	m_mgr.AddPane(pGridPanel, wxAuiPaneInfo().BestSize(250,600).
		Name(wxT("Property")).Caption(wxT("Property")).
		Right().Layer(1).Position(1));

	Show(true);

	// "commit" all changes made to wxAuiManager
	m_mgr.Update();

	CGUIFrameworkEditor::ms_pFramework->EditorSetupLogSystem();
	CGUIAssert::SetWarningCB( EditorWarningCB, NULL );
}
//------------------------------------------------------------------------------
WxMainFrame::~WxMainFrame()
{
	CloseCanvas();
	CloseScene();
	m_mgr.UnInit();	
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
		this,
		ID_GridManager, 
		wxDefaultPosition,
		wxDefaultSize,
		style );
	m_pPropGridMan->SetExtraStyle( extraStyle );
	m_pPropGridMan->SetValidationFailureBehavior( wxPG_VFB_BEEP | wxPG_VFB_MARK_CELL | wxPG_VFB_SHOW_MESSAGE );
	m_pPropGridMan->GetGrid()->SetVerticalSpacing( 2 );
	// Register all editors (SpinCtrl etc.)
	m_pPropGridMan->RegisterAdditionalEditors();
	m_pPropGridMan->Refresh();

	//initialize image
	wxInitAllImageHandlers();

	return m_pPropGridMan;
}
//------------------------------------------------------------------------------
WxEditorCanvasContainer* WxMainFrame::GetCanvasContainer()
{
	return m_pCanvasContainer;
}
//------------------------------------------------------------------------------
void WxMainFrame::UpdateWidgetSizeAndPos()
{
	if( !m_pCurrentEditingWidget )
	{
		return;
	}
	m_pCurrentEditingWidget->Refresh();

	UpdateGridProperty( m_pPropGridMan, m_pCurrentEditingWidget, "size", "CGUIWidgetSize" );
	UpdateGridProperty( m_pPropGridMan, m_pCurrentEditingWidget, "position", "CGUIWidgetPosition" );
}
//------------------------------------------------------------------------------
void WxMainFrame::SetPropGridWidget(CGUIWidget* pWidget, bool bForceRefresh/*=false*/)
{
	if( bForceRefresh || m_pCurrentEditingWidget != pWidget )
	{
		m_pPropGridMan->Clear();
		m_pCurrentEditingWidget = pWidget;
		if( m_pCurrentEditingWidget )
		{
			UpdateGridProperties( m_pPropGridMan, m_pCurrentEditingWidget->GetType(), m_pCurrentEditingWidget );
			m_pPropGridMan->SelectPage( 0 );
		}
		m_pPropGridMan->Refresh();
	}
}
//------------------------------------------------------------------------------
wxTreeCtrl*	WxMainFrame::CreateFileTreeCtrl()
{
	wxTreeCtrl* tree = new wxTreeCtrl(this, ID_ID_TreeCtrl_File,
		wxPoint(0,0), wxSize(160,250),
		wxTR_DEFAULT_STYLE | wxNO_BORDER);

	wxImageList* imglist = new wxImageList(16, 16, true, 2);
	imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
	imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
	tree->AssignImageList(imglist);

	return tree;
}
//------------------------------------------------------------------------------
void WxMainFrame::ResetFileTreeCtrl()
{
	m_pTreeCtrl_File->DeleteAllItems();

	wxTreeItemId root = m_pTreeCtrl_File->AddRoot(wxT("Scene"), 0);

	m_pTreeCtrl_File->AppendItem(root, Gui2wxString(TITLE_WIDGET_CONFIG), 0);
	m_pTreeCtrl_File->AppendItem(root, Gui2wxString(TITLE_SCRIPT), 0);
	m_pTreeCtrl_File->AppendItem(root, Gui2wxString(TITLE_RESOURCE_CONFIG), 0);

	m_pTreeCtrl_File->Expand(root);
}
//------------------------------------------------------------------------------
void WxMainFrame::ResetWidgetTreeCtrl()
{
	m_pTreeCtrl_Widget->DeleteAllItems();
	m_mapTreeItem.clear();
}
//------------------------------------------------------------------------------
void WxMainFrame::RefreshWidgetTreeCtrl()
{
	ResetWidgetTreeCtrl();
	
	if( !GSystem->IsInitialized())
	{
		//no editable page now
		return;
	}

	if( GSystem->GetUICanvas()->GetOpenedPageNum() != 1 )
	{
		return;
	}
	CGUIWidget* pWidgetRoot = GSystem->GetUICanvas()->GetOpenedPageByIndex(0);
	if( !pWidgetRoot)
	{
		//no root widget
		return;
	}

	AddWidgetToTreeCtrl( pWidgetRoot, wxTreeItemId() );
}
//------------------------------------------------------------------------------
void WxMainFrame::AddWidgetToTreeCtrl(CGUIWidget* pWidget, wxTreeItemId aParentId )
{
	std::string strItemId = pWidget->GetName() + " <"+pWidget->GetType() + ">";
	wxTreeItemId aItemId;
	if( !aParentId.IsOk())
	{
		//root
		aItemId = m_pTreeCtrl_Widget->AddRoot(Gui2wxString( strItemId));
	}
	else
	{
		//not root
		aItemId = m_pTreeCtrl_Widget->AppendItem(aParentId, Gui2wxString( strItemId));
	}
	m_mapTreeItem.insert( std::make_pair( pWidget, aItemId));


	CGUIWidget* pChild = pWidget->GetChild();
	while( pChild )
	{
		if( !CGUIWidgetManager::IsInternalName( pChild->GetName()))
		{
			AddWidgetToTreeCtrl( pChild, aItemId );
		}
		pChild = pChild->GetNextSibling();
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::AddToFileTreeCtrl( const std::string& rFileName, const std::string& rType)
{
	wxTreeItemId nItemId = GetFileItemByName(rType);
	wxTreeItemId aFileId = m_pTreeCtrl_File->AppendItem(nItemId, Gui2wxString(rFileName), 1);
	CTreeNode* pNode = new CTreeNode;
	pNode->m_strFileType = rType;
	m_pTreeCtrl_File->SetItemData( aFileId, pNode );
	m_pTreeCtrl_File->Expand(nItemId);
}
//------------------------------------------------------------------------------
wxTreeItemId WxMainFrame::GetFileItemByName(const std::string& rItemName)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId nItemId = m_pTreeCtrl_File->GetFirstChild( m_pTreeCtrl_File->GetRootItem(),cookie );
	
	while( nItemId.IsOk())
	{
		if( rItemName == wx2GuiString(m_pTreeCtrl_File->GetItemText(nItemId)) )
		{
			return nItemId;
		}

		nItemId = m_pTreeCtrl_File->GetNextSibling(nItemId);
	}

	wxASSERT(nItemId.IsOk());	
	return nItemId;
}
//------------------------------------------------------------------------------
wxToolBar* WxMainFrame::CreateToolbar()
{
	wxToolBar* tb1 = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_NODIVIDER);

	//tb1->AddTool(wxID_NEW, wxBitmap (new_xpm));
	tb1->AddTool(ID_Undo, wxBitmap (undo_xpm), _T("undo"));
	tb1->AddTool(ID_Redo, wxBitmap (redo_xpm), _T("redo"));
	tb1->AddSeparator();
	tb1->AddTool(ID_Open, wxBitmap (open_xpm), _T("open"));
	tb1->AddTool(ID_Save, wxBitmap (save_xpm), _T("save"));
	tb1->AddSeparator();
	tb1->AddTool(ID_CreateWidget, wxBitmap (new_xpm), _T("new widget"));
	tb1->AddTool(ID_DeleteWidget, wxBitmap (delete_xpm), _T("delete widget"));
	tb1->AddSeparator();
	tb1->AddTool(ID_WidgetChangeParent, wxBitmap (changeparent_xpm), _T("change parent"));
	tb1->AddTool(ID_WidgetCopy, wxBitmap (copy_xpm), _T("copy widget"));
	tb1->AddTool(ID_WidgetPaste, wxBitmap (paste_xpm), _T("paste widget"));
	tb1->AddSeparator();
	tb1->AddTool(ID_WidgetUp, wxBitmap (bookmarkBluegem), _T("move widget up"));
	tb1->AddTool(ID_WidgetDown, wxBitmap (bookmarkRedgem), _T("move widget down"));
	tb1->AddSeparator();
	tb1->AddTool(ID_ParseScript, wxBitmap (icon_xpm), _T("parse script"));
	tb1->AddTool(ID_About, wxBitmap (help_xpm));

	// Don't forget this one!
	tb1->Realize();
	return tb1;
}
//------------------------------------------------------------------------------
wxAuiNotebook* WxMainFrame::CreateCanvasNotebook()
{
	wxAuiNotebook* ctrl = new wxAuiNotebook(this, ID_NoteBookCtrl,
		wxDefaultPosition,
		wxDefaultSize,
		wxAUI_NB_DEFAULT_STYLE/* | wxAUI_NB_WINDOWLIST_BUTTON | wxAUI_NB_SCROLL_BUTTONS*/);
	ctrl->SetBackgroundColour( *wxLIGHT_GREY );
	return ctrl;
}
//------------------------------------------------------------------------------
int WxMainFrame::LoadScene( std::string strDataPath, std::string strSceneName )
{
	CloseScene();

	//select data path
	if( strDataPath.empty() )
	{
		CGUIString strPath = GSystem->GetDataPath();
		wxDirDialog aDlg( this, _T("Choose a libguiex root path"), Gui2wxString( strPath));
		if( wxID_OK != aDlg.ShowModal())
		{
			return -1;
		}
		strDataPath = wx2GuiString(aDlg.GetPath() + wxT("\\"));
	}

	GSystem->SetDataPath(strDataPath);

	try
	{
		CGUISceneManager::Instance()->UnregisterAllScenes();
		if( 0 != CGUISceneManager::Instance()->RegisterScenesFromDir())
		{
			wxMessageBox( _T("failed to load scenes"), _T("error") );
			return -1;
		}
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );
		return -1;
	}

	//choose scene
	if( strSceneName.empty() )
	{
		const std::vector<CGUIString>& vecScenes = CGUISceneManager::Instance()->GetSceneNames( );
		wxArrayString arrayScenes;
		for( unsigned i=0; i<vecScenes.size(); ++i )
		{
			arrayScenes.Add( Gui2wxString( vecScenes[i]));
		}
		wxSingleChoiceDialog aChoiceDlg( this, _T("select scene"), _T("select scene files"), arrayScenes );
		if( aChoiceDlg.ShowModal() != wxID_OK )
		{
			return -1;
		}
		strSceneName = vecScenes[aChoiceDlg.GetSelection()];
	}

	//get scene info
	const CGUIScene* pScene = CGUISceneManager::Instance()->GetScene( strSceneName );
	if( pScene->IsDependenciesLoaded())
	{
		if( 0 != OpenScene(pScene))
		{
			wxMessageBox( _T("failed to open scene"), _T("error") );
			CloseScene();
			return -1;
		}
	}
	else
	{
		wxMessageBox( _T("some dependent scene of this scene hasn't been loaded"), _T("error") );
		return -1;
	}

	CToolsHistory::Instance()->AddScenePath( strSceneName, strDataPath );

	return 0;
}
//------------------------------------------------------------------------------
/** 
* @brief open scene
* @return 0 for success
*/
int WxMainFrame::OpenScene( const CGUIScene* pScene )
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

	m_bIsSceneOpened = true;

	return 0;
}
//------------------------------------------------------------------------------
/** 
* @brief close scene
*/
void WxMainFrame::CloseScene( )
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
	viewer_exe = "tools_viewer_debug.exe";

	std::string strRunCommand = viewer_exe + " " + GSystem->GetDataPath() + " " +m_strCurrentSceneName + " " + wx2GuiString(strFilename);
	wxExecute(Gui2wxString(strRunCommand), wxEXEC_ASYNC);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemWidgetRender(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );

	RenderFile( wx2GuiString(m_pTreeCtrl_File->GetItemText(id)));
	RefreshWidgetTreeCtrl();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemWidgetEdit(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );

	EditFile( wx2GuiString(m_pTreeCtrl_File->GetItemText(id)), EFT_WIDGET);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemEditExternal(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );

	EditFileExternal( wx2GuiString( m_pTreeCtrl_File->GetItemText(id)));
}

//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemImageEdit(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );

	EditFile( wx2GuiString( m_pTreeCtrl_File->GetItemText(id)), EFT_RESOURCE);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemScriptCheck(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );


	bool bHasError = false;
	guiex::IGUIInterfaceScript* pInterfaceScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
	try
	{
		// create script
		pInterfaceScript->CreateScript( m_strCurrentSceneName );

		// load script
		std::string strScriptFile = wx2GuiString(m_pTreeCtrl_File->GetItemText(id));
		const CGUIScene* pScene = CGUISceneManager::Instance()->GetScene(m_strCurrentSceneName);
		CGUIString strScriptFilePath = pScene->GetScenePath() + strScriptFile;	
		pInterfaceScript->ExecuteFile(strScriptFilePath, pScene->GetSceneName());
		pInterfaceScript->DestroyScript( m_strCurrentSceneName );
	}
	catch (CGUIBaseException rError)
	{
		bHasError = true;
		pInterfaceScript->DestroyScript( m_strCurrentSceneName );
		wxMessageBox( Gui2wxString( rError.what()), _T("script error"), wxICON_ERROR | wxOK | wxCENTRE );
	}
	if( !bHasError )
	{
		wxMessageBox( _T("there is no error in script!"), _T("success") );
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTreeItemScriptEdit(wxCommandEvent& event)
{
	wxTreeItemId id = m_pTreeCtrl_File->GetSelection();
	CHECK_ITEM( id );

	EditFile( wx2GuiString(m_pTreeCtrl_File->GetItemText(id)), EFT_SCRIPT);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnPropertyGridChange( wxPropertyGridEvent& event )
{
	if( !m_pCurrentEditingWidget )
	{
		wxMessageBox( _T("[WxMainFrame::OnPropertyGridChange] no editing widget"), _T("error") );
		return;
	}

	wxPGProperty* pPGTop = event.GetMainParent();
	if( !pPGTop)
	{
		wxMessageBox( _T("[WxMainFrame::OnPropertyGridChange] failed to get main parent of wxPGProperty"), _T("error") );
		return;
	}

	guiex::CGUIProperty aGuiProperty;
	CPropertyConvertorMgr::Instance()->GridProperty2GuiProperty(m_pPropGridMan, pPGTop, aGuiProperty);

	//do some dirty check for make edit convenicence
	if( aGuiProperty.GetName() == "position" )
	{
		guiex::CGUIWidgetPosition aPosition;
		PropertyToValue( aGuiProperty, aPosition );
		if( aPosition.m_eType != m_pCurrentEditingWidget->GetPositionType() )
		{
			//change position type
			CGUIVector2 aPixelPos = m_pCurrentEditingWidget->GetPixelPosition();
			m_pCurrentEditingWidget->SetPositionType( aPosition.m_eType );
			m_pCurrentEditingWidget->SetPixelPosition( aPixelPos );
			aPosition.m_aValue = m_pCurrentEditingWidget->GetPosition();
			ValueToProperty( aPosition, aGuiProperty );
		}
	}
	else if( aGuiProperty.GetName() == "size" )
	{
		guiex::CGUIWidgetSize aSize;
		PropertyToValue( aGuiProperty, aSize );
		if( aSize.m_eType != m_pCurrentEditingWidget->GetSizeType() )
		{
			//change Size type
			CGUISize aPixelSize = m_pCurrentEditingWidget->GetPixelSize();
			m_pCurrentEditingWidget->SetSizeType( aSize.m_eType );
			m_pCurrentEditingWidget->SetPixelSize( aPixelSize );
			aSize.m_aValue = m_pCurrentEditingWidget->GetSize();
			ValueToProperty( aSize, aGuiProperty );
		}
	}

	try
	{ 
		CCommand_SetProperty* pCommand = new CCommand_SetProperty( m_pCurrentEditingWidget, aGuiProperty );
		CCommandManager::Instance()->StoreCommand( pCommand );
		pCommand->Execute();

		UpdateGridProperties( m_pPropGridMan, m_pCurrentEditingWidget->GetType(), m_pCurrentEditingWidget );
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

	m_pCanvasContainer->UpdateWindowBox();
	m_pCanvasContainer->SetSaveFlag(true);
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
		if(ctrl->GetPage(ctrl->GetSelection()) == m_pCanvasContainer)
		{
			m_pCanvasContainer = NULL;
			SetPropGridWidget( NULL );
			ResetWidgetTreeCtrl();
			CCommandManager::Instance()->ClearAllCommand();
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
			m_pCanvasContainer->SetSelectedWidget( wx2GuiString(strLabel) );
		}
		else
		{
			m_pCanvasContainer->SetSelectedWidget( wx2GuiString(strLabel.Mid(0,nEndPos)) );
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
		menu.Append(ID_ITEM_Edit_External, wxT("&Edit External"));
		//menu.Enable(ID_ITEM_Widget_View, m_pCanvasContainer==NULL);
		m_pTreeCtrl_File->PopupMenu(&menu, event.GetPoint());
	}
	else if(pNode->m_strFileType == TITLE_SCRIPT )
	{
		//script
		wxMenu menu(_("Script File"));
		menu.Append(ID_ITEM_Script_Edit, wxT("&Edit"));
		menu.Append(ID_ITEM_Edit_External, wxT("&Edit External"));
		menu.Append(ID_ITEM_Script_Check, wxT("&Check"));
		m_pTreeCtrl_File->PopupMenu(&menu, event.GetPoint());
	}
	else if(pNode->m_strFileType == TITLE_RESOURCE_CONFIG )
	{
		//image
		wxMenu menu(_("Resource File"));
		menu.Append(ID_ITEM_Image_Edit, wxT("&Edit"));
		menu.Append(ID_ITEM_Edit_External, wxT("&Edit External"));
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
	if(m_pAuiNoteBook && m_pAuiNoteBook->GetSelection() >=0)
	{
		CSaveFileBase* pSave = GetSaveFilePtr(m_pAuiNoteBook->GetPage(m_pAuiNoteBook->GetSelection()));

		//if( pSave->ShouldSaveFile())
		{
			pSave->SaveFile();
		}
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateSave(wxUpdateUIEvent& event)
{
	if( m_bIsSceneOpened && m_pAuiNoteBook && m_pAuiNoteBook->GetSelection() >=0 )
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
		if(m_pAuiNoteBook && m_pAuiNoteBook->GetSelection() >=0)
		{
			CSaveFileBase* pSave = GetSaveFilePtr(m_pAuiNoteBook->GetPage(m_pAuiNoteBook->GetSelection()));

			pSave->SaveFileAs(wx2GuiString(aDlg.GetPath()));
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
	if( m_bIsSceneOpened && m_pAuiNoteBook && m_pAuiNoteBook->GetSelection() >=0 )
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
	CGUIWidget* pWidget = m_pCanvasContainer->GetSelectedWidget();
	//empty
	if( !pWidget)
	{
		return;
	}

	pWidget->MoveDown();
	m_pCanvasContainer->SetSaveFlag(true);

	RefreshWidgetTreeCtrl();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateWidgetDown(wxUpdateUIEvent& event)
{
	if( m_pCanvasContainer && m_pCanvasContainer->GetSelectedWidget())
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}
//------------------------------------------------------------------------------
static void DoGetWidgetName( const CGUIWidget* pWidget, wxArrayString& rArray )
{
	rArray.push_back( Gui2wxString(pWidget->GetName()) );
	CGUIWidget* pChild = pWidget->GetChild();
	while( pChild )
	{
		if( !CGUIWidgetManager::IsInternalName( pChild->GetName()))
		{
			DoGetWidgetName( pChild, rArray );
		}
		pChild = pChild->GetNextSibling();
	}
}
void WxMainFrame::OnWidgetChangeParent(wxCommandEvent& evt)
{
	CGUIWidget* pWidget = m_pCanvasContainer->GetSelectedWidget();
	//empty
	if( !pWidget)
	{
		return;
	}

	//change parent here
	wxArrayString arrayWidgetName;
	if( GSystem->GetUICanvas()->GetOpenedPageNum() != 1 )
	{
		return;
	}
	CGUIWidget* pWidgetRoot = GSystem->GetUICanvas()->GetOpenedPageByIndex(0);
	if( !pWidgetRoot)
	{
		return;
	}
	DoGetWidgetName(pWidgetRoot, arrayWidgetName);
	arrayWidgetName.push_back( pWidgetRoot->GetName() );

	wxSingleChoiceDialog aChoiceDlg( this, _T("select parent"), _T("select parent"), arrayWidgetName );
	if( aChoiceDlg.ShowModal() != wxID_OK )
	{
		return;
	}
	CGUIString strWidgetName = arrayWidgetName[aChoiceDlg.GetSelection()];
	if( strWidgetName == pWidget->GetName() )
	{
		return;
	}

	CCommand_WidgetChangeParent* pCommand = new CCommand_WidgetChangeParent( pWidget, pWidget->GetParent()->GetName(), strWidgetName );
	CCommandManager::Instance()->StoreCommand( pCommand );

	pCommand->Execute();

	m_pCanvasContainer->UpdateWindowBox();
	SetPropGridWidget( pWidget, true );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateWidgetChangeParent(wxUpdateUIEvent& event)
{
	if( m_pCanvasContainer && m_pCanvasContainer->GetSelectedWidget() )
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnWidgetCopy(wxCommandEvent& evt)
{
	CGUIWidget* pWidget = m_pCanvasContainer->GetSelectedWidget();
	//empty
	if( !pWidget)
	{
		return;
	}

	m_aWidgetPasteCache.SetCache( pWidget );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateWidgetCopy(wxUpdateUIEvent& event)
{
	if( m_pCanvasContainer && m_pCanvasContainer->GetSelectedWidget() )
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnWidgetPaste(wxCommandEvent& evt)
{
	CGUIWidget* pWidget = m_pCanvasContainer->GetSelectedWidget();
	//empty
	if( !pWidget)
	{
		return;
	}
	if( !m_aWidgetPasteCache.HasCache())
	{
		return;
	}

	//get name of new widget.
	wxTextEntryDialog aTextDlg( this, _T("input widget name"),wxString::Format( _T("input name of widget <%s>"), Gui2wxString( m_aWidgetPasteCache.GetWidgetType()).c_str()) );
	if(  wxID_OK != aTextDlg.ShowModal())
	{
		return;
	}

	CGUIWidget* pNewWidget = NULL;
	try
	{
		pNewWidget = m_aWidgetPasteCache.GenerateWidget( wx2GuiString(aTextDlg.GetValue()), pWidget->GetName() );
		pNewWidget->Open();
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );

		if( pNewWidget )
		{
			CGUIWidgetManager::Instance()->DestroyWidget(pNewWidget);
			pNewWidget = NULL;
		}
	}

	if( pNewWidget )
	{
		OnWidgetModified();
		m_pCanvasContainer->SetSelectedWidget(pNewWidget);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateWidgetPaste(wxUpdateUIEvent& event)
{
		if( m_pCanvasContainer && m_pCanvasContainer->GetSelectedWidget() )
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateParseScript(wxUpdateUIEvent& event)
{
	if(m_pAuiNoteBook && m_pAuiNoteBook->GetSelection() >=0)
	{
		CSaveFileBase* pSave = GetSaveFilePtr(m_pAuiNoteBook->GetPage(m_pAuiNoteBook->GetSelection()));
		if( pSave)
		{
			if( pSave->GetSaveFileMode() == ESaveFileMode_Text )
			{
				WxTextEditor* pTextEditor = (WxTextEditor*)(pSave);
				if( pTextEditor->GetFileType() == EFT_SCRIPT )
				{
					event.Enable(true);
					return;
				}
			}
		}
	}
	event.Enable(false);
}
//------------------------------------------------------------------------------
void WxMainFrame::OnParseScript(wxCommandEvent& evt)
{
	if(m_pAuiNoteBook && m_pAuiNoteBook->GetSelection() >=0)
	{
		CSaveFileBase* pSave = GetSaveFilePtr(m_pAuiNoteBook->GetPage(m_pAuiNoteBook->GetSelection()));
		if( pSave)
		{
			if( pSave->GetSaveFileMode() == ESaveFileMode_Text )
			{
				WxTextEditor* pTextEditor = (WxTextEditor*)(pSave);
				if( pTextEditor->GetFileType() == EFT_SCRIPT )
				{
					std::string strContent = pTextEditor->GetFileContent();
					lua_State* L=lua_open();
					if (L==NULL)
					{
						wxMessageBox( _T("failed to init lua"), _T("error") );
						return;
					}

					int error =	luaL_loadbuffer(L,strContent.c_str(),strContent.size(),"script check");
					if ( error )
					{
						std::string strError = lua_tostring(L,-1);
						wxMessageBox( Gui2wxString( strError ).c_str(), _T("check script") );
					}
					else
					{
						wxMessageBox( _T("no error"), _T("check script") );
					}
					lua_close(L);
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnWidgetUp(wxCommandEvent& evt)
{
	CGUIWidget* pWidget = m_pCanvasContainer->GetSelectedWidget();
	//empty
	if( !pWidget)
	{
		return;
	}

	pWidget->MoveUp();
	m_pCanvasContainer->SetSaveFlag(true);

	RefreshWidgetTreeCtrl();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateWidgetUp(wxUpdateUIEvent& event)
{
	if( m_pCanvasContainer && m_pCanvasContainer->GetSelectedWidget())
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
	CGUIWidget* pWidget = m_pCanvasContainer->GetSelectedWidget();
	//empty
	if( !pWidget)
	{
		return;
	}

	CCommand_DeleteWidget* pCommand = new CCommand_DeleteWidget( pWidget );
	pCommand->Execute();
	CCommandManager::Instance()->StoreCommand( pCommand );

	OnWidgetDeleted();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnWidgetDeleted()
{
	m_pCanvasContainer->SetSelectedWidget(NULL);
	m_pCanvasContainer->SetSaveFlag(true);

	RefreshWidgetTreeCtrl();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnWidgetModified()
{
	m_pCanvasContainer->SetSaveFlag(true);
	RefreshWidgetTreeCtrl();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateDeleteWidget(wxUpdateUIEvent& event)
{
	if( m_pCanvasContainer && m_pCanvasContainer->GetSelectedWidget())
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnToggleScissor(wxCommandEvent& evt)
{
	bool bIsChecked = evt.IsChecked();

	if( CGUIInterfaceManager::Instance()->GetInterfaceRender())
	{
		CGUIInterfaceManager::Instance()->GetInterfaceRender()->EnableClip(!bIsChecked);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnTogglePlayingAs(wxCommandEvent& evt)
{
	bool bIsChecked = evt.IsChecked();

	if( GSystem )
	{
		GSystem->SetPlayingAs(bIsChecked);
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnToggleExtraInfo(wxCommandEvent& evt)
{
	bool bIsChecked = evt.IsChecked();

	if( GSystem )
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
	GSystem->GetUICanvas()->Refresh();
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
void WxMainFrame::OnSetDefaultEditor(wxCommandEvent& evt)
{
	wxFileDialog aDialog( this, wxT("select editor"), wxT(""), wxT(""), wxT("exe files (*.exe)|*.exe"), wxFD_OPEN );
	if( wxID_OK  == aDialog.ShowModal() )
	{
		CToolsHistory::Instance()->SetDefaultEditor( wx2GuiString( aDialog.GetPath() ));
	}
	else
	{
		CToolsHistory::Instance()->SetDefaultEditor( "" );
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateResolution(wxUpdateUIEvent& event)
{
	event.Enable( GSystem->IsInitialized());
}
//------------------------------------------------------------------------------
void WxMainFrame::OnCreateWidget(wxCommandEvent& evt)
{
	wxASSERT( m_pCanvasContainer );

	//create wizard
	WxWizardCreateWidget wizard(this, m_pCanvasContainer->GetSelectedWidget());
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

	if( pWidget->IsPageRoot() )
	{
		CGUIWidgetManager::Instance()->AddPage( pWidget);
		GSystem->GetUICanvas()->OpenUIPage( pWidget);
	}
	else
	{
		pWidget->Open();
	}

	pWidget->Refresh();

	OnWidgetModified();
	m_pCanvasContainer->SetSelectedWidget(pWidget);

	CCommand_CreateWidget* pCommand = new CCommand_CreateWidget( pWidget );
	CCommandManager::Instance()->StoreCommand( pCommand );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnSetLocalization( wxCommandEvent& event )
{
	int nId = event.GetId();
	uint32 nIdx = nId - ID_Localization_begin;

	CGUILocalizationManager::Instance()->SetCurrentLocalConfig( wx2GuiString( CPropertyConfigMgr::Instance()->GetLocalizations()[nIdx]));
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUndo(wxCommandEvent& evt)
{
	CCommandManager::Instance()->Undo();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateUndo(wxUpdateUIEvent& event)
{
	event.Enable(CCommandManager::Instance()->HasUndoCommand());
}
//------------------------------------------------------------------------------
void WxMainFrame::OnRedo(wxCommandEvent& evt)
{
	CCommandManager::Instance()->Redo();
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateRedo(wxUpdateUIEvent& event)
{
	event.Enable(CCommandManager::Instance()->HasRedoCommand());
}
//------------------------------------------------------------------------------
void WxMainFrame::OnUpdateCreateWidget(wxUpdateUIEvent& event)
{
	if( m_pCanvasContainer )
	{
		if( GSystem->GetUICanvas()->GetOpenedPageNum() == 0 ||
			m_pCanvasContainer->GetSelectedWidget())
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
	if( m_pCanvasContainer )
	{
		m_pCanvasContainer->SetScreenSize(width, height);
	}
	else
	{
		GSystem->SetRawScreenSize(width, height);
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
	if(m_pAuiNoteBook)
	{
		for( unsigned i=0; i< m_pAuiNoteBook->GetPageCount(); ++i)
		{
			CSaveFileBase* pSave = GetSaveFilePtr(m_pAuiNoteBook->GetPage(i));
			
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
//------------------------------------------------------------------------------
void WxMainFrame::OnRecentPaths( wxCommandEvent& In )
{
	if( SaveFileProcess(-1) == false)
	{
		//cancel
		return;
	}

	unsigned nFileIdx = In.GetId() - ID_RecentPathsBaseId;
	std::string	strPath = CToolsHistory::Instance()->GetHistoryPaths()[nFileIdx];
	LoadScene( strPath );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnRecentScenes( wxCommandEvent& In )
{
	if( SaveFileProcess(-1) == false)
	{
		//cancel
		return;
	}

	unsigned nFileIdx = In.GetId() - ID_RecentScenesBaseId;
	std::pair< std::string, std::string> strScene = CToolsHistory::Instance()->GetHistoryScenes()[nFileIdx];
	LoadScene( strScene.second, strScene.first );
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

	LoadScene();
}
//------------------------------------------------------------------------------
void WxMainFrame::CloseCanvas()
{
	if( m_pCanvasContainer)
	{
		int idx = m_pAuiNoteBook->GetPageIndex(m_pCanvasContainer);
		if( wxNOT_FOUND != idx )
		{
			SaveFileProcess(idx);
			m_pAuiNoteBook->DeletePage(idx);
		}
		m_pCanvasContainer = NULL;
	}
}
//------------------------------------------------------------------------------
/** 
* @brief render given file
*/
void WxMainFrame::RenderFile( const std::string& rFileName )
{
	CloseCanvas();
	CGUIScene* pScene = CGUISceneManager::Instance()->GetScene(m_strCurrentSceneName);

	std::string strAbsFileName = GSystem->GetDataPath() + pScene->GetScenePath() + rFileName;

	m_pCanvasContainer = new WxEditorCanvasContainer(m_pAuiNoteBook, strAbsFileName);
	m_pAuiNoteBook->AddPage( m_pCanvasContainer, Gui2wxString(rFileName), true );

	//update resource for editor
	CGUIResourcePool::Instance()->UpdateResourceList();

	try
	{
		//load xml widget by libguiex
		CGUIWidget* pWidget = CGUIWidgetManager::Instance()->LoadPage( rFileName, m_strCurrentSceneName );
		if( pWidget )
		{
			GSystem->GetUICanvas()->OpenUIPage(pWidget);
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
void WxMainFrame::EditFileExternal( const std::string& rFileName )
{
	CGUIScene* pScene = CGUISceneManager::Instance()->GetScene(m_strCurrentSceneName);

	std::string strAbsPath = GSystem->GetDataPath() + pScene->GetScenePath();
	std::string strAbsFileName = strAbsPath + rFileName;

	wxString strCommand = wxString::Format( _T("%s %s"), _T("notepad"), Gui2wxString( strAbsFileName).c_str() );

	if( !CToolsHistory::Instance()->GetDefaultEditor().empty() )
	{
		wxString strEditor = Gui2wxString(CToolsHistory::Instance()->GetDefaultEditor());
		if( wxFileExists( strEditor ))
		{
			strCommand = wxString::Format( _T("%s %s"), strEditor.c_str(), Gui2wxString( strAbsFileName).c_str() );
		}
		else
		{
			CToolsHistory::Instance()->SetDefaultEditor("");
		}
	}

	wxExecute( strCommand );
	//::ShellExecute( 
	//	NULL, 
	//	_T("open"),
	//	Gui2wxString( strAbsFileName).c_str(), 
	//	NULL, 
	//	Gui2wxString( strAbsPath).c_str(),
	//	SW_SHOWDEFAULT);
}
//------------------------------------------------------------------------------
/** 
* @brief edit given file
*/
void WxMainFrame::EditFile( const std::string& rFileName, EFileType eFileType )
{
	CGUIScene* pScene = CGUISceneManager::Instance()->GetScene(m_strCurrentSceneName);

	std::string strAbsPath = GSystem->GetDataPath() + pScene->GetScenePath() + rFileName;

	//check
	for( unsigned i=0; i< m_pAuiNoteBook->GetPageCount(); ++i)
	{
		CSaveFileBase* pSave = GetSaveFilePtr(m_pAuiNoteBook->GetPage(i));

		if( pSave->GetFileName() == strAbsPath)
		{
			m_pAuiNoteBook->SetSelection(i);
			return;
		}
	}


	WxTextEditor* pTextEditor = new WxTextEditor( m_pAuiNoteBook, strAbsPath );

	if( 0 != pTextEditor->InitEditor(strAbsPath, eFileType))
	{
		//error
		delete pTextEditor;
		return;
	}
	m_pAuiNoteBook->AddPage( pTextEditor, Gui2wxString(rFileName),true);
}
//------------------------------------------------------------------------------
/** 
* @brief save files before close it
* @param nIdx the index of page, -1 for all page
* @return false for user cancel the process.
*/
bool WxMainFrame::SaveFileProcess(int nIdx)
{
	wxArrayString aFileArray;
	std::vector<CSaveFileBase*> aSaveArray;

	if( nIdx == -1 )
	{
		//save all page
		for( unsigned i=0; i< m_pAuiNoteBook->GetPageCount(); ++i)
		{
			//get save file base class
			CSaveFileBase* pSave = GetSaveFilePtr(m_pAuiNoteBook->GetPage(i));
			
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
		wxASSERT(m_pAuiNoteBook->GetPageCount() > size_t(nIdx ));
		CSaveFileBase* pSave = GetSaveFilePtr(m_pAuiNoteBook->GetPage(nIdx));

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
void WxMainFrame::OutputString( const std::string& rString)
{
	m_pOutput->AppendText(Gui2wxString(rString).data());
	m_pOutput->AppendText(wxString(_T("\n")));
}
//------------------------------------------------------------------------------
void WxMainFrame::UpdateStatusBar (wxChar *format, ...) 
{
	wxChar buffer[1024] ;

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
void WxMainFrame::CreateMenu()
{
	//update cache file
	CToolsHistory::Instance()->ParseHistoryFile(wxGetApp().GetBaseDir() + ".\\libguiex_editor_cache.xml");
	CToolsHistory::Instance()->SetMaxCacheSize(10);
	CToolsHistory::Instance()->SetPathsBaseId(ID_RecentPathsBaseId);
	CToolsHistory::Instance()->SetScenesBaseId(ID_RecentScenesBaseId);
	CToolsHistory::Instance()->UpdateMenu();

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
	file_menu->Append(ID_RecentScenes, _("Recent Scenes"), CToolsHistory::Instance()->GetSceneMenu());
	file_menu->Append(ID_RecentPaths, _("Recent Paths"), CToolsHistory::Instance()->GetPathMenu());
	file_menu->AppendSeparator();
	file_menu->Append(ID_Exit, _("Exit"));

	//menu-edit
	wxMenu*	edit_menu = new wxMenu;

	wxMenu*	localization_menu = new wxMenu;
	edit_menu->AppendSubMenu(localization_menu, wxT("Localization"));
	const std::vector<wxString>& rLocs = CPropertyConfigMgr::Instance()->GetLocalizations();
	for( uint32 i=0; i<rLocs.size(); ++i )
	{
		localization_menu->Append(ID_Localization_begin+i, rLocs[i]);
	}
	edit_menu->Append(ID_Undo, _("Undo"));
	edit_menu->Append(ID_Redo, _("Redo"));

	edit_menu->Append(ID_CreateWidget, _("Create Widget"));
	edit_menu->Append(ID_DeleteWidget, _("Delete Widget"));

	edit_menu->Append(ID_WidgetChangeParent, _("Change parent"));
	edit_menu->Append(ID_WidgetCopy, _("Copy Widget"));
	edit_menu->Append(ID_WidgetPaste, _("Paste Widget"));
	file_menu->AppendSeparator();
	edit_menu->Append(ID_WidgetUp, _("Widget Up"));
	edit_menu->Append(ID_WidgetDown, _("Widget Down"));
	
	//menu-tool
	wxMenu*	tool_menu = new wxMenu;
	tool_menu->Append(ID_SetDefaultEditor, wxT("Set Default Editor"), wxT("Set Default Editor"));

	//menu-view
	wxMenu*	view_menu = new wxMenu;
	wxMenu*	resolution_menu = new wxMenu;
	resolution_menu->Append(ID_VIEW_Fullscreen, _("Fullscreen"));
	resolution_menu->Append(ID_VIEW_800x600, wxT("800 x 600"), wxT("Convenience resizer for 800 x 600."));
	resolution_menu->Append(ID_VIEW_1024x786, wxT("1024 x 768"), wxT("Convenience resizer for 1024 x 768."));
	resolution_menu->Append(ID_VIEW_1280x800, wxT("1280 x 800"), wxT("Convenience resizer for 1280 x 800."));
	resolution_menu->Append(ID_VIEW_Iphone480x320, wxT("Iphone480x320"), wxT("Convenience resizer for Iphone480x320."));
	resolution_menu->Append(ID_VIEW_Iphone320x480, wxT("Iphone320x480"), wxT("Convenience resizer for Iphone320x480."));
	resolution_menu->Append(ID_VIEW_Iphone960x640, wxT("Iphone960x640"), wxT("Convenience resizer for Iphone960x640."));
	resolution_menu->Append(ID_VIEW_Iphone640x960, wxT("Iphone640x960"), wxT("Convenience resizer for Iphone640x960."));
	resolution_menu->Append(ID_VIEW_Ipad1024x768, wxT("Ipad1024x768"), wxT("Convenience resizer for Ipad1024x768."));
	resolution_menu->Append(ID_VIEW_Ipad768x1024, wxT("Ipad768x1024"), wxT("Convenience resizer for Ipad768x1024."));

	view_menu->AppendSubMenu(resolution_menu, wxT("Resolution"));
	view_menu->AppendSeparator();
	view_menu->Append(ID_ToggleScissor, wxT("Toggle Scissor"), wxT("enable or disable scissor"), wxITEM_CHECK);
	view_menu->Append(ID_ToggleWireframe, wxT("Toggle Wireframe"), wxT("enable or disable wireframe"), wxITEM_CHECK);
	view_menu->Append(ID_ToggleExtraInfo, wxT("Toggle render extra info"), wxT("enable or disable render extra info"), wxITEM_CHECK);
	view_menu->Append(ID_TogglePlayingAs, wxT("Toggle playing as"), wxT("enable or disable playing as"), wxITEM_CHECK);
	view_menu->Append(ID_SetBGColor, wxT("Set BG Color"), wxT("set background color"));
	view_menu->Append(ID_Refresh, wxT("Refresh"), wxT("refresh widgets"));

	//menu-about
	wxMenu* help_menu = new wxMenu;
	help_menu->Append(ID_About, _("About..."));

	mb->Append(file_menu, _("File"));
	mb->Append(edit_menu, _("Edit"));
	mb->Append(view_menu, _("View"));
	mb->Append(tool_menu, _("Tool"));
	mb->Append(help_menu, _("Help"));

	SetMenuBar(mb);
}
//------------------------------------------------------------------------------

