/** 
* @file mainframe.cpp
* @brief main frame used by application
* @author ken
* @date 2011-09-08
*/

//============================================================================//
// include
//============================================================================// 
#include "wxmainframe.h"
#include "wxmainapp.h"
#include "wxresourcepreviewcontainer.h"
#include <wx/artprov.h>

#include "guiframeworkresource.h"
#include "editorid.h"
#include "toolshistory.h"
#include "toolsmisc.h"

#include <wx/treectrl.h>


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


//============================================================================//
// function
//============================================================================// 

BEGIN_EVENT_TABLE(WxMainFrame, wxFrame)
	EVT_MENU(ID_Open, WxMainFrame::OnOpen)

	EVT_MENU_RANGE( ID_RecentPathsBaseId, ID_RecentPathsEndId, WxMainFrame::OnRecentPaths )
	EVT_MENU_RANGE( ID_RecentScenesBaseId, ID_RecentScenesEndId, WxMainFrame::OnRecentScenes )

	//combobox
	EVT_COMBOBOX(ID_ResourceFileCtrl, WxMainFrame::OnFileSelected )

	//note book ctrl
	EVT_AUINOTEBOOK_PAGE_CLOSE(ID_NoteBookCtrl, OnBookPageClose)

	//tree ctrl
	EVT_TREE_SEL_CHANGED(ID_ResourceItemCtrl, WxMainFrame::OnResourceSelected) 

END_EVENT_TABLE()

//------------------------------------------------------------------------------
WxMainFrame::WxMainFrame(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style)
: wxFrame(parent, id, title, pos, size, style)
,m_pResourceFiles(NULL)
,m_pResourceItemCtrl(NULL)
,m_pAuiNoteBook(NULL)
,m_pToolbar(NULL)
,m_pPreviewContainer(NULL)
{
	// tell wxAuiManager to manage this frame
	m_mgr.SetManagedWindow(this);

	// create main menu
	CreateMenu();

	//STATUS
	CreateStatusBar();
	GetStatusBar()->SetStatusText(_("Ready"));

	// create some toolbars
	CreateToolbar();

	// create notebook
	CreateCanvasNotebook();

	SetMinSize(wxSize(400,300));


	// create resource tree
	m_pResourceFiles = new wxComboBox( this, ID_ResourceFileCtrl,wxEmptyString, wxDefaultPosition,wxDefaultSize, 0, NULL, wxCB_READONLY | wxCB_SORT );

	//create resource list 
	m_pResourceItemCtrl = new wxTreeCtrl( this, ID_ResourceItemCtrl, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxNO_BORDER );
	wxImageList* imglist = new wxImageList(16, 16, true, 2);
	imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
	m_pResourceItemCtrl->AssignImageList(imglist);

	//add to aui manager
	m_mgr.AddPane(m_pResourceFiles, wxAuiPaneInfo().
		Name(wxT("ResourceFile")).Caption(wxT("Files")).
		ToolbarPane().Top().Row(1).Position(1).
		LeftDockable(false).RightDockable(false));

	m_mgr.AddPane(m_pToolbar, wxAuiPaneInfo().
		Name(wxT("Toolbar")).Caption(wxT("Toolbar")).
		ToolbarPane().Top().Row(1).Position(2).
		LeftDockable(false).RightDockable(false));

	m_mgr.AddPane(m_pResourceItemCtrl, wxAuiPaneInfo().BestSize( 250, 400 ).
		Name(wxT("ResourceItem")).Caption(wxT("Items")).
		Left().Layer(1).Position(1));

	m_mgr.AddPane(m_pAuiNoteBook, wxAuiPaneInfo().Name(wxT("notebook_content")).
		CenterPane().PaneBorder(false));

	Show(true);

	// "commit" all changes made to wxAuiManager
	m_mgr.Update();
}
//------------------------------------------------------------------------------
WxMainFrame::~WxMainFrame()
{
	CloseScene();
	m_mgr.UnInit();	
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
	
	//menu-tool
	wxMenu*	tool_menu = new wxMenu;
	tool_menu->Append(ID_SetDefaultEditor, wxT("Set Default Editor"), wxT("Set Default Editor"));

	//menu-about
	wxMenu* help_menu = new wxMenu;
	help_menu->Append(ID_About, _("About..."));

	mb->Append(file_menu, _("File"));
	mb->Append(edit_menu, _("Edit"));
	mb->Append(tool_menu, _("Tool"));
	mb->Append(help_menu, _("Help"));

	SetMenuBar(mb);
}
//------------------------------------------------------------------------------
void WxMainFrame::CreateToolbar()
{
	m_pToolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_NODIVIDER);

	m_pToolbar->AddTool(ID_Open, wxBitmap (open_xpm), _T("open"));
	m_pToolbar->AddTool(ID_Save, wxBitmap (save_xpm), _T("save"));
	m_pToolbar->AddSeparator();
	m_pToolbar->AddTool(ID_About, wxBitmap (help_xpm));

	// Don't forget this one!
	m_pToolbar->Realize();
}
//------------------------------------------------------------------------------
void WxMainFrame::CreateCanvasNotebook()
{
	m_pAuiNoteBook = new wxAuiNotebook(this, ID_NoteBookCtrl,
		wxDefaultPosition,
		wxDefaultSize,
		wxAUI_NB_DEFAULT_STYLE/* | wxAUI_NB_WINDOWLIST_BUTTON | wxAUI_NB_SCROLL_BUTTONS*/);
	m_pAuiNoteBook->SetBackgroundColour( *wxLIGHT_GREY );
}
//------------------------------------------------------------------------------
void WxMainFrame::SetPreview( const CGUIProperty* pResourceProperty )
{
	if( !m_pPreviewContainer )
	{
		//create default preview container
		m_pPreviewContainer = new WxResourcePreviewContainer( m_pAuiNoteBook );
		m_pAuiNoteBook->AddPage( m_pPreviewContainer, L"Preview", true );
	}
	m_pAuiNoteBook->SetPageText( m_pAuiNoteBook->GetPageIndex(m_pPreviewContainer), Gui2wxString( CGUIString("Preview: ") + pResourceProperty->GetName()));

	m_pPreviewContainer->SetGUIProperty( pResourceProperty );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnBookPageClose(wxAuiNotebookEvent& evt)
{
	wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
	if(ctrl->GetPage(ctrl->GetSelection()) == m_pPreviewContainer)
	{
		m_pPreviewContainer = NULL;
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnOpen(wxCommandEvent& evt)
{
	LoadScene();
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
		strDataPath = (aDlg.GetPath() + wxT("\\")).char_str(wxConvUTF8).data();
	}

	GSystem->SetDataPath(strDataPath);

	try
	{
		CGUISceneManager::Instance()->ReleaseAllResources();
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

	//load resource
	CGUISceneManager::Instance()->LoadResources( strSceneName );

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
void WxMainFrame::OnRecentPaths( wxCommandEvent& In )
{
	unsigned nFileIdx = In.GetId() - ID_RecentPathsBaseId;
	std::string	strPath = CToolsHistory::Instance()->GetHistoryPaths()[nFileIdx];
	LoadScene( strPath );
}
//------------------------------------------------------------------------------
void WxMainFrame::OnRecentScenes( wxCommandEvent& In )
{
	unsigned nFileIdx = In.GetId() - ID_RecentScenesBaseId;
	std::pair< std::string, std::string> strScene = CToolsHistory::Instance()->GetHistoryScenes()[nFileIdx];
	LoadScene( strScene.second, strScene.first );
}
//------------------------------------------------------------------------------
int	WxMainFrame::OpenScene(const CGUIScene* pSceneInfo )
{
	m_strCurrentSceneName = pSceneInfo->GetSceneName();

	//add resource list file to tree
	const std::vector<CGUIString>& rResourceFiles = pSceneInfo->GetResourceFiles();
	for( unsigned i=0; i<rResourceFiles.size(); ++i )
	{
		m_pResourceFiles->Append( Gui2wxString(rResourceFiles[i] ));
	}

	if( m_pResourceFiles->GetCount() > 0 )
	{
		m_pResourceFiles->Select( 0 );
		SelectFile( m_pResourceFiles->GetValue() );
	}
	return 0;
}
//------------------------------------------------------------------------------
void WxMainFrame::CloseScene( )
{
	m_strCurrentSceneName.clear();

	m_pResourceFiles->Clear();
	m_pResourceItemCtrl->DeleteAllItems();
	m_mapItemFolder.clear();
	if( m_pPreviewContainer )
	{
		m_pAuiNoteBook->DeletePage( m_pAuiNoteBook->GetPageIndex(m_pPreviewContainer));
	}
}
//------------------------------------------------------------------------------
void WxMainFrame::OnFileSelected(wxCommandEvent& event)
{
	SelectFile( m_pResourceFiles->GetValue());
}
//------------------------------------------------------------------------------
wxString WxMainFrame::GetFileFullPath( const wxString& rFileName )
{
	CGUIScene* pScene = CGUISceneManager::Instance()->GetScene(m_strCurrentSceneName);

	CGUIString strPath = GSystem->GetDataPath() + pScene->GetScenePath() + wx2GuiString(rFileName);
	return Gui2wxString(strPath);
}
//------------------------------------------------------------------------------
wxString WxMainFrame::GetFileRelativePath( const wxString& rFileName )
{
	CGUIScene* pScene = CGUISceneManager::Instance()->GetScene(m_strCurrentSceneName);

	CGUIString strPath = pScene->GetScenePath() + wx2GuiString(rFileName);
	return Gui2wxString(strPath);
}
//------------------------------------------------------------------------------
void WxMainFrame::SelectFile( const wxString& rFileName )
{
	m_aResourcePropertys.Clear();
	m_pResourceItemCtrl->DeleteAllItems();
	m_mapItemFolder.clear();

	m_pResourceItemCtrl->AddRoot(rFileName);

	IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
	if( 0 != pConfigFile->LoadConfigFile(wx2GuiString(GetFileRelativePath(rFileName)), m_aResourcePropertys) )
	{
		wxMessageBox( _T("failed to load config file"), _T("error") );
		return;
	}

	for( uint32 i=0; i<m_aResourcePropertys.GetPropertyNum(); ++i )
	{
		const CGUIProperty* pProperty = m_aResourcePropertys.GetProperty( i );

		AddProperty2Tree( pProperty );
	}

	m_pResourceItemCtrl->ExpandAll();
}
//------------------------------------------------------------------------------
void WxMainFrame::AddProperty2Tree( const guiex::CGUIProperty* pProperty )
{
	std::map<guiex::uint32, wxTreeItemId>::iterator itor = m_mapItemFolder.find( pProperty->GetType() );
	wxTreeItemId idFolder;
	if( itor == m_mapItemFolder.end() )
	{
		idFolder = m_pResourceItemCtrl->AppendItem( m_pResourceItemCtrl->GetRootItem(), Gui2wxString( pProperty->GetTypeAsString()), 0);
		m_mapItemFolder.insert( std::make_pair( uint32( pProperty->GetType()), idFolder));
	}
	else
	{
		idFolder = itor->second;
	}

	m_pResourceItemCtrl->AppendItem( idFolder, Gui2wxString(pProperty->GetName()));
	
}
//------------------------------------------------------------------------------
void WxMainFrame::OnResourceSelected(wxTreeEvent& event)
{
	wxTreeItemId aSelectItem = event.GetItem();
	if( !aSelectItem.IsOk())
	{
		return;
	}

	wxTreeItemId aParentItem = m_pResourceItemCtrl->GetItemParent(aSelectItem);
	if( !aParentItem.IsOk())
	{
		return;
	}

	if( aParentItem == m_pResourceItemCtrl->GetRootItem() )
	{
		//folder, ignore it
		return;
	}

	CGUIString strItemName = wx2GuiString( m_pResourceItemCtrl->GetItemText( aSelectItem ));
	CGUIString strItemType = wx2GuiString( m_pResourceItemCtrl->GetItemText( aParentItem ));

	const CGUIProperty* pProperty = m_aResourcePropertys.GetProperty( strItemName, strItemType );
	SetPreview( pProperty );
}
//------------------------------------------------------------------------------
