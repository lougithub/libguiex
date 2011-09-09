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
#include "guiframeworkresource.h"
#include "wxmainapp.h"
#include "editorid.h"

#include <wx/treectrl.h>

//============================================================================//
// function
//============================================================================// 

BEGIN_EVENT_TABLE(WxMainFrame, wxFrame)

END_EVENT_TABLE()

//------------------------------------------------------------------------------
WxMainFrame::WxMainFrame(wxWindow* parent,
				 wxWindowID id,
				 const wxString& title,
				 const wxPoint& pos,
				 const wxSize& size,
				 long style)
				 : wxFrame(parent, id, title, pos, size, style)
				 ,m_pTreeCtrl_Resource(NULL)
{
	// tell wxAuiManager to manage this frame
	m_mgr.SetManagedWindow(this);

	// create widget tree
	m_pTreeCtrl_Resource = new wxTreeCtrl( this, ID_TreeCtrl_Resource, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxNO_BORDER );

	m_mgr.AddPane(m_pTreeCtrl_Resource, wxAuiPaneInfo().BestSize(150,600).
		Name(wxT("Widget")).Caption(wxT("Widget")).
		Left().Layer(1).Position(2));

	Show(true);

	// "commit" all changes made to wxAuiManager
	m_mgr.Update();
}
//------------------------------------------------------------------------------
WxMainFrame::~WxMainFrame()
{
	m_mgr.UnInit();	
}
//------------------------------------------------------------------------------
