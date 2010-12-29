/** 
* @file wizardcreatewidget.cpp
* @brief a wizard dialog to create widget
* @author ken
* @date 2007-09-27
*/

//============================================================================//
// include
//============================================================================// 
#include "wxwizardcreatewidget.h"
#include "editorutility.h"
#include "propertysheetfunc.h"
#include "propertyconfigmgr.h"
#include "wxmainapp.h"
#include "wxmainframe.h"

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

#include <libguiex_core/guiex.h>

#include "bitmaps/wizard.xpm"
#include "bitmaps/wizard2.xpm"

using namespace guiex;

//============================================================================//
// function
//============================================================================// 
BEGIN_EVENT_TABLE(WxWizardCreateWidget, wxWizard)
EVT_WIZARD_PAGE_CHANGING(wxID_ANY, WxWizardCreateWidget::OnWizardPageChanging)
EVT_WIZARD_FINISHED(wxID_ANY, WxWizardCreateWidget::OnWizardFinished)
END_EVENT_TABLE()

//------------------------------------------------------------------------------
WxWizardCreateWidget::WxWizardCreateWidget(wxFrame *frame, CGUIWidget* pParent )
: wxWizard(frame,wxID_ANY,_T("Create Widget"),wxBitmap(wiztest_xpm),wxDefaultPosition,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
,m_pWidget(NULL)
,m_pParent(pParent)
{
	m_pPage1 = InitPage1();
	m_pPage2 = InitPage2();

	wxWizardPageSimple::Chain(m_pPage1, m_pPage2);
	GetPageAreaSizer()->Add(m_pPage1);
}
//------------------------------------------------------------------------------
wxWizardPageSimple* WxWizardCreateWidget::InitPage1()
{
	/////////////////////////////////////////////////////////////////////////
	//page 1, select widget type and name
	wxWizardPageSimple* page1 = new wxWizardPageSimple(this);
	//combobox
	m_pComboBoxType = new wxComboBox( page1, wxID_ANY, wxEmptyString, wxDefaultPosition,wxSize(240,20),0,NULL, wxCB_DROPDOWN|wxCB_READONLY|wxCB_SORT );
	wxStaticBoxSizer* pSizerComboxType = new wxStaticBoxSizer(new wxStaticBox(page1, -1, wxT("Select Widget Type:")),wxVERTICAL);
	pSizerComboxType->Add(m_pComboBoxType);
	const CPropertyConfigMgr::TSetType& rTypeMap = CPropertyConfigMgr::Instance()->GetWidgetTypes();
	for( CPropertyConfigMgr::TSetType::const_iterator itor = rTypeMap.begin();
		itor != rTypeMap.end();
		++itor)
	{
		m_pComboBoxType->Append(Gui2wxString( *itor ));
	}
	m_pComboBoxType->Select(0);

	//editbox
	wxString widgetname=m_pParent ? Gui2wxString((m_pParent->GetName()+"\\")) : wxString();
	m_pEditName = new wxTextCtrl(page1, wxID_ANY, widgetname,wxDefaultPosition,wxSize(240,20));
	wxStaticBoxSizer* pSizerEditName = new wxStaticBoxSizer(new wxStaticBox(page1, -1, wxT("Set Widget Name:")),wxVERTICAL);
	pSizerEditName->Add(m_pEditName);

	//sizer
	wxBoxSizer *pPageSizer1 = new wxBoxSizer(wxVERTICAL);
	pPageSizer1->AddSpacer(20);
	pPageSizer1->Add(pSizerComboxType);
	pPageSizer1->AddSpacer(20);
	pPageSizer1->Add(pSizerEditName);

	page1->SetSizer(pPageSizer1);

	return page1;
}
//------------------------------------------------------------------------------
wxWizardPageSimple* WxWizardCreateWidget::InitPage2()
{
	/////////////////////////////////////////////////////////////////////////
	//page 2, select widget parameter
	wxWizardPageSimple* page2 = new wxWizardPageSimple(this);

	//create property grid manager
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

	m_pPropGridMgr = new wxPropertyGridManager(
		page2,
		wxID_ANY, 
		wxDefaultPosition,
		wxSize(100, 100),
		style );

	//add page
	m_pPropGridMgr->SetExtraStyle( extraStyle );
	m_pPropGridMgr->SetValidationFailureBehavior( wxPG_VFB_BEEP | wxPG_VFB_MARK_CELL | wxPG_VFB_SHOW_MESSAGE );
	m_pPropGridMgr->GetGrid()->SetVerticalSpacing( 2 );
	// Register all editors (SpinCtrl etc.)
	m_pPropGridMgr->RegisterAdditionalEditors();

	m_pPropGridMgr->AddPage(wxT(NOTEBOOK_APPEARANCE_PAGE_NAME));
	m_pPropGridMgr->AddPage(wxT(NOTEBOOK_IMAGE_PAGE_NAME));
	m_pPropGridMgr->AddPage(wxT(NOTEBOOK_EVENT_PAGE_NAME));
	m_pPropGridMgr->SelectPage(NOTEBOOK_PAGE_APPEARANCE);
	m_pPropGridMgr->Refresh();

	wxBoxSizer* topSizer = new wxBoxSizer ( wxVERTICAL );
	topSizer->Add( m_pPropGridMgr, 1, wxEXPAND );
	page2->SetSizer( topSizer );
	topSizer->SetSizeHints( page2 );

	return page2;
}
//------------------------------------------------------------------------------
void WxWizardCreateWidget::OnWizardPageChanging(wxWizardEvent& event)
{
	wxWizardPage* pCurPage = event.GetPage();

	if( pCurPage == m_pPage1)
	{
		//page 1
		if( m_pEditName->GetValue().empty())
		{
			wxMessageBox(_T("the widget name is empty!"), _T("Warning") );
			event.Veto();
		}
		else if (CGUIWidgetManager::Instance()->HasWidget(wx2GuiString( m_pEditName->GetValue()), GetMainFrame()->GetCurrentSceneName()))
		{
			wxMessageBox(_T("the widget name has existed!"), _T("Warning") );
			event.Veto();
		}

		if( m_strWidgetType != m_pComboBoxType->GetValue())
		{
			m_strWidgetType = m_pComboBoxType->GetValue();
			m_pPropGridMgr->ClearPage(NOTEBOOK_PAGE_APPEARANCE);
			m_pPropGridMgr->ClearPage(NOTEBOOK_PAGE_IMAGE);
			m_pPropGridMgr->ClearPage(NOTEBOOK_PAGE_EVENT);
			LoadWidgetConfig( m_pPropGridMgr, m_strWidgetType.char_str(wxConvUTF8).data() );
		}
	}
	else if( pCurPage == m_pPage2)
	{
		//page 2
	}
}
//------------------------------------------------------------------------------
void WxWizardCreateWidget::OnWizardFinished(wxWizardEvent& event)
{
	CGUIString strType = wx2GuiString( m_pComboBoxType->GetValue());
	CGUIString strName = wx2GuiString( m_pEditName->GetValue());
	try
	{
		CGUIProperty aSet;
		GenerateGUIProperty( m_pPropGridMgr,aSet);

		m_pWidget = CGUIWidgetManager::Instance()->CreateWidget(strType, strName, GetMainFrame()->GetCurrentSceneName());
		if( m_pParent )
		{
			m_pWidget->SetParent( m_pParent );
			aSet.GetProperty("parent")->SetValue( m_pParent->GetName() );
		}
		m_pWidget->SetProperty(aSet);
		m_pWidget->LoadProperty();
		m_pWidget->Create();
	}
	catch (CGUIBaseException& rError)
	{
		wxMessageBox( Gui2wxString(rError.what()), _T("error") );

		if( m_pWidget )
		{
			CGUIWidgetManager::Instance()->DestroyWidget(m_pWidget);
			m_pWidget = NULL;
		}
		event.Veto();
	}
}
//------------------------------------------------------------------------------
