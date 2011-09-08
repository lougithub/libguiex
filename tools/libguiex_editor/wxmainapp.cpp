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
#include "editorutility.h"
#include <algorithm>


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
		CGUIFrameworkEditor::ms_pFramework = new CGUIFrameworkEditor( );
		CGUIFrameworkEditor::ms_pFramework->Initialize( CGUIIntSize(1024,768), "" );

		GSystem->SetDrawExtraInfo( false );
		GSystem->SetPlayingAs( false );
		GSystem->SetEditorMode( true );
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );

		if( CGUIFrameworkEditor::ms_pFramework )
		{
			CGUIFrameworkEditor::ms_pFramework->Release();
			delete CGUIFrameworkEditor::ms_pFramework;
			CGUIFrameworkEditor::ms_pFramework = NULL;
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

	//load localization config file
	if( 0 != CPropertyConfigMgr::Instance()->ReadLocalizationConfig(GetBaseDir() + "../editorconfig/localization_config.xml"))
	{
		wxMessageBox(_T("failed to read config file localization_config.xml!"), _T("error"));
		return false;
	}

	//load base config file
	std::vector<wxFileName> vecBaseConfigFile;
	vecBaseConfigFile.push_back( Gui2wxString(GetBaseDir() + "../editorconfig/libguiex_editor_config.xml"));
	vecBaseConfigFile.push_back( Gui2wxString(GetBaseDir() + "../editorconfig/libguiex_editor_config_box2d.xml"));
	vecBaseConfigFile.push_back( Gui2wxString(GetBaseDir() + "../editorconfig/libguiex_editor_config_game.xml"));

	for( uint32 i=0; i<vecBaseConfigFile.size(); ++i )
	{
		if( 0 != CPropertyConfigMgr::Instance()->ReadPropertyConfig( vecBaseConfigFile[i].GetFullPath()))
		{
			wxMessageBox( wxString::Format( _T("failed to read property config file: %s"), vecBaseConfigFile[i].GetFullPath() ), _T("error"));
			return false;
		}
	}

	//load extend config file
	if( wxMessageBox( _T("Do you want to load extend widget config file?"), _T("Questing"), wxYES_NO | wxICON_QUESTION) == wxYES )
	{
		wxFileDialog aDlg(NULL, _T("Choose widget config files"), Gui2wxString(GetBaseDir() + "../editorconfig/"), wxEmptyString, _T("widget config files (*.xml)|*.xml"), wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST );
		if( wxID_OK == aDlg.ShowModal())
		{
			//try load
			wxArrayString arrayFiles;
			aDlg.GetPaths( arrayFiles );
			for( uint32 i=0; i<arrayFiles.size(); ++i )
			{
				if( std::find(vecBaseConfigFile.begin(), vecBaseConfigFile.end(), arrayFiles[i]) != vecBaseConfigFile.end() )
				{
					//base config file, ignore it
					continue;
				}

				if( 0 != CPropertyConfigMgr::Instance()->ReadPropertyConfig( arrayFiles[i]))
				{
					wxMessageBox( wxString::Format( _T("failed to read property config file: %s"), arrayFiles[i] ), _T("error"));
					return false;
				}
			}
		}
	}

	//check default property
	if( false == CheckProperty() )
	{
		wxMessageBox(_T("check default property!"), _T("error"));
		return false;
	}

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
	if( CGUIFrameworkEditor::ms_pFramework )
	{
		CGUIFrameworkEditor::ms_pFramework->Release();
		delete CGUIFrameworkEditor::ms_pFramework;
		CGUIFrameworkEditor::ms_pFramework = NULL;
	}

	return wxApp::OnExit();
}
//------------------------------------------------------------------------------
const std::string& WxMainApp::GetBaseDir( ) const 
{
	return m_strBaseDir;
}
//------------------------------------------------------------------------------
bool WxMainApp::CheckProperty() const
{
	try
	{
		const CPropertyConfigMgr::TSetType& rTypes = CPropertyConfigMgr::Instance()->GetWidgetTypes( );
		for( CPropertyConfigMgr::TSetType::const_iterator itor = rTypes.begin();
			itor != rTypes.end();
			++itor )
		{
			const std::string& rWidgetType = *itor;
			guiex::CGUIWidget* pWidget = guiex::CGUIWidgetManager::Instance()->CreateWidget( rWidgetType,"__template4check__", "" );
			const CGUIProperty& rDefaultProperty = CPropertyConfigMgr::Instance()->GetPropertySet( rWidgetType );

			for( uint32 i=0; i<rDefaultProperty.GetPropertyNum(); ++i )
			{
				const guiex::CGUIProperty* pDefaultProperty = rDefaultProperty.GetProperty(i);
				if( CPropertyData::GetPropertyData(*pDefaultProperty)->IsAlternaitiveSave() == false )
				{
					continue;
				}

				guiex::CGUIProperty aWidgetProperty;
				aWidgetProperty.SetName( pDefaultProperty->GetName() );
				aWidgetProperty.SetType( pDefaultProperty->GetTypeAsString() );
				if( 0 != pWidget->GenerateProperty( aWidgetProperty ))
				{
					wxString strError = wxString::Format( 
						_T("failed to create property <%s:%s> in widget <%s>!"), 
						Gui2wxString(aWidgetProperty.GetName()).c_str(),
						Gui2wxString(aWidgetProperty.GetTypeAsString()).c_str(),
						Gui2wxString(pWidget->GetType()).c_str()
						);
					wxMessageBox(strError, _T("error"));
					return false;
				}

				if( aWidgetProperty != *pDefaultProperty )
				{
					wxString strError = wxString::Format( 
						_T("failed to check property <%s:%s> in widget <%s>!\r\nwidget value <%s>\r\ndefault value <%s>"), 
						Gui2wxString(aWidgetProperty.GetName()).c_str(),
						Gui2wxString(aWidgetProperty.GetTypeAsString()).c_str(),
						Gui2wxString(pWidget->GetType()).c_str(),
						Gui2wxString(aWidgetProperty.GetValue()).c_str(),
						Gui2wxString((*pDefaultProperty).GetValue()).c_str()
						);
					wxMessageBox(strError, _T("error"));
					return false;
				}
			}

			guiex::CGUIWidgetManager::Instance()->DestroyWidget( pWidget );
		}

	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );

		return false;
	}
	return true;
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
WxMainFrame* GetMainFrame()
{
	return (WxMainFrame*)wxGetApp().GetTopWindow();
}
//------------------------------------------------------------------------------
