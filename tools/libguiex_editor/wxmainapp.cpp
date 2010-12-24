/** 
* @file mainapp.cpp
* @brief entry of the application
* @author ken
* @date 2007-09-10
*/

//============================================================================//
// include
//============================================================================// 
#include "wxmainapp.h"
#include "wxmainframe.h"
#include "propertyconfigmgr.h"


//============================================================================//
// function
//============================================================================// 

//------------------------------------------------------------------------------
IMPLEMENT_APP(WxMainApp)
//------------------------------------------------------------------------------
WxMainApp::WxMainApp()
:wxApp()
,m_hSciDll(NULL)
{
}
//------------------------------------------------------------------------------
bool WxMainApp::OnInit()
{
	//get base dir
    wxString strDir;
	wxSplitPath(wxGetFullModuleName(), &strDir, NULL, NULL);
	m_strBaseDir = strDir.char_str(wxConvUTF8).data();
	m_strBaseDir += "\\";

	//load scintilla dll
	m_hSciDll = LoadLibrary(_T("SciLexer.DLL"));
	if (m_hSciDll==NULL)
	{
		wxMessageBox(_T("The Scintilla DLL could not be loaded."),
			_T("Error loading Scintilla"), wxOK|wxCENTER|wxICON_ERROR,NULL);
		return false;
	}

	//create frame
	wxFrame* frame = new WxMainFrame(NULL,
		wxID_ANY,
		wxT("liguiex editor"),
		wxDefaultPosition,
		wxSize(1024, 768));
	SetTopWindow(frame);
	frame->Show();

	//load config file
	if( 0 != CPropertyConfigMgr::Instance()->ReadPropertyConfig(GetBaseDir() + "../editorconfig/libguiex_editor_config.xml"))
	{
		wxMessageBox(_T("failed to read property config file!"), _T("error"));
		return false;
	}

	wxToolTip::Enable(true);
	wxToolTip::SetDelay(1000);

	return true;
}
//------------------------------------------------------------------------------
int WxMainApp::OnExit()
{
	//free scintilla
	if (m_hSciDll != NULL)
	{
		FreeLibrary(m_hSciDll);
		m_hSciDll = NULL;
	}

	return wxApp::OnExit();

}
//------------------------------------------------------------------------------
const std::string& WxMainApp::GetBaseDir( ) const 
{
	return m_strBaseDir;
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
WxMainFrame* GetMainFrame()
{
	return (WxMainFrame*)wxGetApp().GetTopWindow();
}
//------------------------------------------------------------------------------
