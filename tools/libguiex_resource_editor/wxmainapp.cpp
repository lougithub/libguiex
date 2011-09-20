/** 
* @file mainapp.cpp
* @brief entry of the application
* @author ken
* @date 2011-09-08
*/

//============================================================================//
// include
//============================================================================// 
#include "wxmainapp.h"
#include "wxmainframe.h"
#include <wx/filename.h>

#include "guiframeworkresource.h"
#include "propertyconfigmgr.h"
#include "toolsmisc.h"

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

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//initialize guiex system
	try
	{
		CGUIFrameworkResource::ms_pFramework = new CGUIFrameworkResource( );
		CGUIFrameworkResource::ms_pFramework->Initialize( CGUIIntSize(1024,768), "" );

		GSystem->SetDrawExtraInfo( false );
		GSystem->SetPlayingAs( false );
		GSystem->SetEditorMode( true );
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );

		if( CGUIFrameworkResource::ms_pFramework )
		{
			CGUIFrameworkResource::ms_pFramework->Release();
			delete CGUIFrameworkResource::ms_pFramework;
			CGUIFrameworkResource::ms_pFramework = NULL;
		}
	}

	//load scintilla dll
	m_hSciDll = LoadLibrary(_T("SciLexer.DLL"));
	if (m_hSciDll==NULL)
	{
		wxMessageBox(_T("The Scintilla DLL could not be loaded."),
			_T("Error loading Scintilla"), wxOK|wxCENTER|wxICON_ERROR,NULL);
		return false;
	}

	//load base config file
	std::vector<wxFileName> vecBaseConfigFile;
	vecBaseConfigFile.push_back( Gui2wxString(GetBaseDir() + "../editorconfig/libguiex_editor_config.xml"));

	for( uint32 i=0; i<vecBaseConfigFile.size(); ++i )
	{
		if( 0 != CPropertyConfigMgr::Instance()->ReadPropertyConfig( vecBaseConfigFile[i].GetFullPath()))
		{
			wxMessageBox( wxString::Format( _T("failed to read property config file: %s"), vecBaseConfigFile[i].GetFullPath() ), _T("error"));
			return false;
		}
	}
	CPropertyConfigMgr::Instance()->AddType( "array" );

	//create frame
	wxFrame* frame = new WxMainFrame(NULL,
		wxID_ANY,
		wxT("libguiex editor"),
		wxDefaultPosition,
		wxSize(1024, 768));
	SetTopWindow(frame);
	frame->Show();

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

	//release libguiex system
	if( CGUIFrameworkResource::ms_pFramework )
	{
		CGUIFrameworkResource::ms_pFramework->Release();
		delete CGUIFrameworkResource::ms_pFramework;
		CGUIFrameworkResource::ms_pFramework = NULL;
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
