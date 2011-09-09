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
	//widget tree
	wxTreeCtrl*m_pTreeCtrl_Resource;

	wxAuiManager m_mgr;

	DECLARE_EVENT_TABLE()
};


#endif //__KEN_MAINFRAME_20110908_H__

