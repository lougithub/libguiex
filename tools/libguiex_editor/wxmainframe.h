/** 
* @file mainframe.h
* @brief main frame used by application
* @author ken
* @date 2007-09-10
*/

#ifndef __KEN_MAINFRAME_20070910_H__
#define __KEN_MAINFRAME_20070910_H__


//============================================================================//
// include
//============================================================================// 

#include <map>

//============================================================================//
// declare
//============================================================================// 
class WxEditorCanvasContainer;
class wxTreeCtrl;
class wxPropertyGridEvent;
class WxOutputPanel;
class WxWidgetPropertySheet;
class wxTreeItemId;
class CSaveFileBase;
namespace guiex
{
	class CGUIWidget;
}

//============================================================================//
// class
//============================================================================// 
class WxMainFrame : public wxFrame
{
public:
	WxMainFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

	~WxMainFrame();

	void DoUpdate();

	/** 
	 * @brief show string in output panel
	 */
	void	OutputString( const std::string& rString);

	/** 
	 * @brief set status bar content
	 */
	void	UpdateStatusBar (wxChar *format, ...) ;


	enum
	{
		//file
		ID_Exit = wxID_HIGHEST+1,
		ID_About,
		ID_Open,
		ID_NewWidgetFile,
		ID_NewScriptFile,
		ID_AddImageFile,
		ID_Close,
		ID_Save,
		ID_SaveAs,
		ID_SaveAll,
		ID_RecentPaths,
		ID_RecentPathsBaseId,
		ID_RecentPathsEndId = ID_RecentPathsBaseId+10-1,
		ID_RecentScenes,
		ID_RecentScenesBaseId,
		ID_RecentScenesEndId = ID_RecentScenesBaseId+10-1,

		//edit
		ID_CreateWidget,
		ID_DeleteWidget,
		ID_WidgetUp,
		ID_WidgetDown,
		ID_RunScript,
		
		//view
		ID_VIEW_Fullscreen,
		ID_VIEW_800x600,
		ID_VIEW_1024x786,
		ID_VIEW_1280x800,
		ID_VIEW_Iphone480x320,
		ID_VIEW_Iphone320x480,
		ID_VIEW_Iphone960x640,
		ID_VIEW_Iphone640x960,
		ID_VIEW_Ipad1024x768,
		ID_VIEW_Ipad768x1024,

		ID_ToggleScissor,
		ID_ToggleWireframe,
		ID_SetBGColor,
		ID_Refresh,
		ID_ToggleExtraInfo,



		ID_ITEM_Widget_Render,
		ID_ITEM_Widget_View,
		ID_ITEM_Widget_Edit,
		ID_ITEM_Edit_EXTERNAL,
		ID_ITEM_Script_Edit,
		ID_ITEM_Image_Edit,

		WIDGET_ID_PG,		//id of grid manager
		WIDGET_ID_TreeCtrl_File,	//id of tree ctrl
		WIDGET_ID_TreeCtrl_Widget,	//id of tree ctrl
		WIDGET_ID_NoteBook_Canvas,	
		WIDGET_ID_NoteBook_Config,	
	};


	////initialize libguiex,return 0 for success
	//int	InitializeLibguiex();

	////release libguiex
	//void	ReleaseLibguiex();

	//set property grid related widget
	void			SetPropGridWidget(guiex::CGUIWidget* pWidget);

	WxWidgetPropertySheet*		GetPropGrid();

	//get current canvas
	WxEditorCanvasContainer*			GetCanvasContainer();

	//select a widget item in tree
	void			SelectWidgetTreeItem(guiex::CGUIWidget* pWidget);

	const wxColor& GetBGColor()
	{
		return m_aBGColor;
	}

	const guiex::CGUIString& GetCurrentSceneName() const
	{
		return m_strCurrentSceneName;
	}

protected:
	void			CreateMenu();
	wxToolBar*		CreateToolbar();
	wxAuiNotebook*	CreateCanvasNotebook();
	//wxAuiNotebook*	CreateConfigNotebook();

	//file tree ctrl
	wxTreeCtrl*		CreateFileTreeCtrl();
	void			ResetFileTreeCtrl();
	void			AddToFileTreeCtrl( const std::string& rFileName, const std::string& rType);
	wxTreeItemId	GetFileItemByName(const std::string& rItemName);

	//widget tree ctrl
	wxTreeCtrl*		CreateWidgetTreeCtrl();
	void			ResetWidgetTreeCtrl();
	void			RefreshWidgetTreeCtrl();
	void			AddWidgetToTreeCtrl(guiex::CGUIWidget* pWidget, wxTreeItemId aParentId );

	WxOutputPanel*	CreateOutput();

	//property ctrl
	wxPanel*		CreatePropGridPanel();
	void			ResetPropGridPanel();
	void			LoadPropConfigByWidget( );

	/** 
	 * @brief open scene
	 * @return 0 for success
	 */
	int		OpenScene(const guiex::CGUISceneInfo* pSceneInfo );

	/** 
	* @brief close scene
	*/
	void	CloseScene( );

	/** 
	 * @brief render given file
	 */
	void	RenderFile( const std::string& rFileName );

	/** 
	* @brief edit given file
	*/
	void	EditFile( const std::string& rFileName, EFileType eFileType );

	void	EditFileExternal( const std::string& rFileName );

	/** 
	 * @brief save files before close it
	 * @param nIdx the index of page, -1 for all page
	 * @return false for user cancel the process.
	 */

	bool	SaveFileProcess( int nIdx);

	//remove old canvas
	void	CloseCanvas();


protected:
	//callback function
	void OnCloseWindow(wxCloseEvent& evt);

	//key
	void OnKeyDown(wxKeyEvent& event);

	//menu
	void OnExit(wxCommandEvent& evt);

	void OnAbout(wxCommandEvent& evt);

	void OnOpen(wxCommandEvent& evt);

	void OnNewWidgetFile(wxCommandEvent& evt);
	void OnUpdateNewWidgetFile(wxUpdateUIEvent& event);
	

	void OnClose(wxCommandEvent& evt);
	void OnUpdateClose(wxUpdateUIEvent& event);

	void OnSave(wxCommandEvent& evt);
	void OnUpdateSave(wxUpdateUIEvent& event);
	void OnSaveAs(wxCommandEvent& evt);
	void OnUpdateSaveAs(wxUpdateUIEvent& event);

	void OnSaveAll(wxCommandEvent& evt);
	void OnUpdateSaveAll(wxUpdateUIEvent& event);

	void OnDeleteWidget(wxCommandEvent& evt);
	void OnUpdateDeleteWidget(wxUpdateUIEvent& event);
	
	void OnWidgetUp(wxCommandEvent& evt);
	void OnUpdateWidgetUp(wxUpdateUIEvent& event);
	void OnWidgetDown(wxCommandEvent& evt);
	void OnUpdateWidgetDown(wxUpdateUIEvent& event);

	void OnRunScript(wxCommandEvent& evt);
	void OnToggleScissor(wxCommandEvent& evt);
	void OnToggleExtraInfo(wxCommandEvent& evt);
	void OnToggleWireframe(wxCommandEvent& evt);
	void OnSetBGColor(wxCommandEvent& evt);
	void OnRefresh(wxCommandEvent& evt);

	void OnFullscreen(wxCommandEvent& evt);

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

	void OnUpdateResolution(wxUpdateUIEvent& event);


	void OnCreateWidget(wxCommandEvent& evt);
	void OnUpdateCreateWidget(wxUpdateUIEvent& event);

	void OnRecentPaths( wxCommandEvent& In );
	void OnRecentScenes( wxCommandEvent& In );



	//file tree
	void OnTreeItemMenu(wxTreeEvent& event);
	void OnTreeItemWidgetRender(wxCommandEvent& event);
	void OnTreeItemWidgetView(wxCommandEvent& event);
	void OnTreeItemWidgetEdit(wxCommandEvent& event);
	void OnTreeItemScriptEdit(wxCommandEvent& event);
	void OnTreeItemImageEdit(wxCommandEvent& event);
	void OnTreeItemEditExternal(wxCommandEvent& event);
	

	//notebook
	void OnBookPageChanged(wxAuiNotebookEvent& event);
	void OnBookPageClose(wxAuiNotebookEvent& event);
	
	//widget tree
	void OnWidgetTreeSelected(wxTreeEvent& event);
	

	//property sheet
	void OnPropertyGridChange( wxPropertyGridEvent& event );

	//get ptr of save file
	CSaveFileBase* GetSaveFilePtr( const wxWindow* pWindows );

protected:
	wxAuiManager m_mgr;
	WxEditorCanvasContainer*		m_pCanvas;
	wxAuiNotebook*			m_pNoteBook_Canvas;
	//wxAuiNotebook*			m_pNoteBook_Config;
	wxTreeCtrl*				m_pTreeCtrl_File;
	
	//widget tree
	wxTreeCtrl*				m_pTreeCtrl_Widget;
	typedef std::map<guiex::CGUIWidget*, wxTreeItemId>	TTreeItemMap;
	TTreeItemMap			m_mapTreeItem;


	wxToolBar*				m_pToolbar;
	WxOutputPanel*			m_pOutput;

	WxWidgetPropertySheet*	m_pPropGridMan;

	bool					m_bIsSceneOpened;

	wxSize					m_aScreenSize;			//!< screen size
	guiex::CGUIString		m_strCurrentSceneName;
	wxColor					m_aBGColor;

	DECLARE_EVENT_TABLE()
};







#endif //__KEN_MAINFRAME_20070910_H__

