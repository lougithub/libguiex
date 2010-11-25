/** 
* @file wizardcreatewidget.cpp
* @brief a wizard dialog to create widget
* @author ken
* @date 2007-09-27
*/



//============================================================================//
// include
//============================================================================// 
#include "libguiex_editor.h"






#include "bitmaps/wizard.xpm"
#include "bitmaps/wizard2.xpm"

//============================================================================//
// declare
//============================================================================// 




//============================================================================//
// function
//============================================================================// 


BEGIN_EVENT_TABLE(WxWizardCreateWidget, wxWizard)
EVT_WIZARD_PAGE_CHANGING(wxID_ANY, WxWizardCreateWidget::OnWizardPageChanging)
EVT_WIZARD_FINISHED(wxID_ANY, WxWizardCreateWidget::OnWizardFinished)
END_EVENT_TABLE()


//------------------------------------------------------------------------------
WxWizardCreateWidget::WxWizardCreateWidget(wxFrame *frame, guiex::CGUIWidget* pParent )
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
	const CPropertyConfigMgr::TMapPropertySet& rTypeMap = CPropertyConfigMgr::Instance()->GetPropertySetMap();
	wxASSERT(rTypeMap.empty() == false);
	for( CPropertyConfigMgr::TMapPropertySet::const_iterator itor = rTypeMap.begin();
		itor != rTypeMap.end();
		++itor)
	{
		m_pComboBoxType->Append(wxConvUTF8.cMB2WC( itor->first.c_str()));
	}
	m_pComboBoxType->Select(0);

	//editbox
	wxString widgetname=m_pParent ? wxConvUTF8.cMB2WC((m_pParent->GetName()+"\\").c_str()) : wxString();
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
	//wxWindow* panel = new wxPanel(page2,-1,wxPoint(0,0),wxSize(500,400));
	m_pPropGridMgr = new wxPropertyGridManager(
		page2, 
		wxID_ANY, 
		wxDefaultPosition, 
		wxDefaultSize,
		wxPG_BOLD_MODIFIED |
		wxPG_SPLITTER_AUTO_CENTER |
		//wxPG_AUTO_SORT |
		//wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER |
		//wxPG_TOOLTIPS |
		//wxPG_HIDE_CATEGORIES |
		//wxPG_LIMITED_EDITING |
		wxTAB_TRAVERSAL |
		wxPG_TOOLBAR |
		wxPG_DESCRIPTION
		//wxPG_COMPACTOR
		);
	//add page
	m_pPropGridMgr->SetExtraStyle(
		wxPG_EX_NO_FLAT_TOOLBAR |
		wxPG_EX_MODE_BUTTONS |
		wxPG_EX_HELP_AS_TOOLTIPS |
		wxPG_EX_ENABLE_TLP_TRACKING |
		wxPG_EX_UNFOCUS_ON_ENTER 
		);
	m_pPropGridMgr->SetValidationFailureBehavior( wxPG_VFB_BEEP | wxPG_VFB_MARK_CELL | wxPG_VFB_SHOW_MESSAGE );

	m_pPropGridMgr->GetGrid()->SetVerticalSpacing( 2 );
    // Register all editors (SpinCtrl etc.)
    //m_pPropGridMgr->RegisterAdditionalEditors();

	m_pPropGridMgr->AddPage(wxT("Property"));
	m_pPropGridMgr->AddPage(wxT("Image"));
	m_pPropGridMgr->AddPage(wxT("Event"));
	m_pPropGridMgr->SelectPage(NOTEBOOK_PAGE_PROP);
	m_pPropGridMgr->Refresh();


	wxBoxSizer* topSizer = new wxBoxSizer ( wxVERTICAL );
	topSizer->Add( m_pPropGridMgr, 1, wxEXPAND );
	page2->SetSizer( topSizer );
	topSizer->SetSizeHints( page2 );

	//wxBoxSizer* panelSizer = new wxBoxSizer( wxHORIZONTAL );
	//panelSizer->Add( panel, 1, wxEXPAND|wxFIXED_MINSIZE );
	//page2->SetSizer( panelSizer );
	//panelSizer->SetSizeHints( page2 );


	//wxBoxSizer* panelSizer = new wxBoxSizer( wxHORIZONTAL );
	//panelSizer->Add( m_pPropGridMgr, 1, wxEXPAND );
	//page2->SetSizer(panelSizer);
	//panelSizer->SetSizeHints( page2 );

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
			wxMessageBox(_T("the widget name is empty!"), _T("Warning"),wxICON_WARNING | wxOK, this);
			event.Veto();
		}
		else if (guiex::CGUIWidgetManager::Instance()->HasWidget(wxConvUTF8.cWC2MB( m_pEditName->GetValue().c_str()).data(), GetMainFrame()->GetCurrentSceneName()))
		{
			wxMessageBox(_T("the widget name has existed!"), _T("Warning"),wxICON_WARNING | wxOK, this);
			event.Veto();
		}

		if( m_strWidgetType != m_pComboBoxType->GetValue())
		{
			m_strWidgetType = m_pComboBoxType->GetValue();
			m_pPropGridMgr->ClearPage(NOTEBOOK_PAGE_PROP);
			m_pPropGridMgr->ClearPage(NOTEBOOK_PAGE_IMAGE);
			m_pPropGridMgr->ClearPage(NOTEBOOK_PAGE_EVENT);
			LoadWidgetConfig( m_pPropGridMgr, m_strWidgetType.char_str(wxConvUTF8).data());
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
	m_pWidget = CreateWidget( m_pPropGridMgr, m_pComboBoxType->GetValue().char_str(wxConvUTF8).data(), m_pEditName->GetValue().char_str(wxConvUTF8).data(), m_pParent );
	if( !m_pWidget )
	{
		event.Veto();
	}
}
//------------------------------------------------------------------------------
