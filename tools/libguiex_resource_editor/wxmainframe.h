/** 
* @file mainframe.h
* @brief main frame used by application
* @author ken
* @date 2011-09-08
*/

#ifndef __KEN_MAINFRAME_20110908_H__
#define __KEN_MAINFRAME_20110908_H__


//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include <wx/aui/aui.h>
#include <wx/treebase.h>
#include <wx/listctrl.h>

#include <libguiex_core/guiex.h>

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

protected:
	void CreateMenu();
	void CreateToolbar();
	void CreateCanvasNotebook();

	void SetPreview( const guiex::CGUIProperty* pResourceProperty );

	int LoadScene( std::string strDataPath = "", std::string strSceneName = "" );
	int	OpenScene(const guiex::CGUIScene* pSceneInfo );
	void CloseScene( );

	wxString GetFileFullPath( const wxString& rFileName );
	wxString GetFileRelativePath( const wxString& rFileName );
	void SelectFile( const wxString& rFileName );

	void AddProperty2Tree( const guiex::CGUIProperty* pProperty );

protected:
	void OnOpen(wxCommandEvent& evt);
	void OnRecentPaths( wxCommandEvent& In );
	void OnRecentScenes( wxCommandEvent& In );

	//tree ctrl
	void OnResourceSelected(wxTreeEvent& event);

	//combobox
	void OnFileSelected(wxListEvent& event);

	//book ctrl
	void OnBookPageClose(wxAuiNotebookEvent& evt);

protected:

	//widget tree
	wxToolBar* m_pToolbar;
	wxTreeCtrl* m_pResourceItemCtrl;
	wxListCtrl* m_pResourceFiles;
	wxAuiNotebook* m_pAuiNoteBook;
	class WxResourcePreviewContainer* m_pPreviewContainer;
	wxAuiManager m_mgr;

	//list of items
	guiex::CGUIString m_strCurrentSceneName;
	guiex::CGUIProperty m_aResourcePropertys;
	std::map<guiex::uint32, wxTreeItemId> m_mapItemFolder;

	DECLARE_EVENT_TABLE()
};


#endif //__KEN_MAINFRAME_20110908_H__

