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
#include "wxtexteditor.h"
#include "guiframeworkeditor.h"

#include <wx/wxprec.h>
#include <wx/aui/aui.h>
#include <wx/treebase.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>
#include <libguiex_core/guiex.h>

#include <map>

//============================================================================//
// declare
//============================================================================// 
using namespace guiex;
class WxEditorCanvasContainer;
class wxTreeCtrl;
class wxPropertyGridEvent;
class WxOutputPanel;
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

	void OutputString( const std::string& rString);
	void UpdateStatusBar (wxChar *format, ...) ;

	void UpdateWidgetSizeAndPos();

	//set property grid related widget
	void SetPropGridWidget(CGUIWidget* pWidget, bool bForceRefresh=false);

	//get current canvas
	WxEditorCanvasContainer* GetCanvasContainer();

	//select a widget item in tree
	void SelectWidgetTreeItem(CGUIWidget* pWidget);

	const wxColor& GetBGColor()
	{
		return m_aBGColor;
	}

	const CGUIString& GetCurrentSceneName() const
	{
		return m_strCurrentSceneName;
	}

protected:
	void CreateMenu();
	wxToolBar* CreateToolbar();
	wxAuiNotebook*	CreateCanvasNotebook();
	//wxAuiNotebook* CreateConfigNotebook();

	//file tree ctrl
	wxTreeCtrl*	CreateFileTreeCtrl();
	void ResetFileTreeCtrl();
	void AddToFileTreeCtrl( const std::string& rFileName, const std::string& rType);
	wxTreeItemId GetFileItemByName(const std::string& rItemName);

	//widget tree ctrl
	void ResetWidgetTreeCtrl();
	void RefreshWidgetTreeCtrl();
	void AddWidgetToTreeCtrl(CGUIWidget* pWidget, wxTreeItemId aParentId );

	WxOutputPanel* CreateOutput();

	//property ctrl
	wxPanel* CreatePropGridPanel();
	void LoadPropConfigByWidget( );

	/** 
	 * @brief open scene
	 * @return 0 for success
	 */
	int	OpenScene(const CGUIScene* pSceneInfo );

	/** 
	* @brief close scene
	*/
	void CloseScene( );

	/** 
	 * @brief render given file
	 */
	void RenderFile( const std::string& rFileName );

	/** 
	* @brief edit given file
	*/
	void EditFile( const std::string& rFileName, EFileType eFileType );

	void EditFileExternal( const std::string& rFileName );

	/** 
	 * @brief save files before close it
	 * @param nIdx the index of page, -1 for all page
	 * @return false for user cancel the process.
	 */
	bool SaveFileProcess( int nIdx);

	//remove old canvas
	void CloseCanvas();

protected:
	void SetResolution( int width, int height );


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
	void OnWidgetChangeParent(wxCommandEvent& evt);
	void OnUpdateWidgetChangeParent(wxUpdateUIEvent& event);
	void OnWidgetCopy(wxCommandEvent& evt);
	void OnUpdateWidgetCopy(wxUpdateUIEvent& event);
	void OnWidgetPaste(wxCommandEvent& evt);
	void OnUpdateWidgetPaste(wxUpdateUIEvent& event);
	void OnToggleScissor(wxCommandEvent& evt);
	void OnToggleExtraInfo(wxCommandEvent& evt);
	void OnToggleWireframe(wxCommandEvent& evt);
	void OnTogglePlayingAs(wxCommandEvent& evt);
	void OnSetBGColor(wxCommandEvent& evt);
	void OnRefresh(wxCommandEvent& evt);
	void OnFullscreen(wxCommandEvent& evt);
	void OnUpdateParseScript(wxUpdateUIEvent& event);
	void OnParseScript(wxCommandEvent& evt);

	void OnSetDefaultEditor(wxCommandEvent& evt);


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
	void OnSetLocalization( wxCommandEvent& event );

	void OnRecentPaths( wxCommandEvent& In );
	void OnRecentScenes( wxCommandEvent& In );

	//file tree
	void OnTreeItemMenu(wxTreeEvent& event);
	void OnTreeItemWidgetRender(wxCommandEvent& event);
	void OnTreeItemWidgetView(wxCommandEvent& event);
	void OnTreeItemWidgetEdit(wxCommandEvent& event);
	void OnTreeItemScriptEdit(wxCommandEvent& event);
	void OnTreeItemScriptCheck(wxCommandEvent& event);
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
	WxEditorCanvasContainer*m_pCanvas;
	wxAuiNotebook*m_pAuiNoteBook;
	wxTreeCtrl*m_pTreeCtrl_File;
	
	//widget tree
	wxTreeCtrl*m_pTreeCtrl_Widget;
	typedef std::map<CGUIWidget*, wxTreeItemId>	TTreeItemMap;
	TTreeItemMap m_mapTreeItem;

	wxToolBar*m_pToolbar;
	WxOutputPanel*m_pOutput;

	class WxToolsPGManager* m_pPropGridMan;

	bool m_bIsSceneOpened;

	CGUIString m_strCurrentSceneName;
	wxColor m_aBGColor;

	CGUIWidget* m_pCurrentEditingWidget;

	struct
	{
		CGUIProperty m_aProperty;
		CGUIString m_aWidgetType;
	} m_aPasteCache;

	DECLARE_EVENT_TABLE()
};


#endif //__KEN_MAINFRAME_20070910_H__

