/** 
* @file wxresourcepreviewcontainer.cpp
* @brief 
* @author ken
* @date 2011-09-14
*/

//============================================================================//
// include
//============================================================================//
#include "wxresourcepreviewcontainer.h"
#include "wxresourcepreviewpanel.h"
#include "toolsmisc.h"
#include "guiframeworkresource.h"
#include "propertyconvertor.h"

using namespace guiex;

//============================================================================//
// function
//============================================================================//
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(WxResourcePreviewContainer, wxPanel)

END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxResourcePreviewContainer::WxResourcePreviewContainer( wxWindow* parent )
:wxPanel(parent )
,m_pPreviewPanel(NULL)
{
	// tell wxAuiManager to manage this frame
	m_mgr.SetManagedWindow(this);

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
	m_pPropertyGridMgr = new wxPropertyGridManager( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	int extraStyle = 
		wxPG_EX_MODE_BUTTONS |
		//| wxPG_EX_AUTO_UNSPECIFIED_VALUES
		//| wxPG_EX_GREY_LABEL_WHEN_DISABLED
		//| wxPG_EX_NATIVE_DOUBLE_BUFFERING
		//| wxPG_EX_HELP_AS_TOOLTIPS
		wxPG_EX_MULTIPLE_SELECTION;
	m_pPropertyGridMgr->SetExtraStyle( extraStyle );
	m_pPropertyGridMgr->SetValidationFailureBehavior( wxPG_VFB_BEEP | wxPG_VFB_MARK_CELL | wxPG_VFB_SHOW_MESSAGE );
	m_pPropertyGridMgr->GetGrid()->SetVerticalSpacing( 2 );
	m_pPropertyGridMgr->RegisterAdditionalEditors();
	m_pPropertyGridMgr->Refresh();

	//add to aui manager
	m_mgr.AddPane(m_pPropertyGridMgr, wxAuiPaneInfo().MinSize(200, 400 ).
		Name(wxT("Property")).Caption(wxT("Property")).
		Right());

	m_mgr.Update();
}
//------------------------------------------------------------------------------
WxResourcePreviewContainer::~WxResourcePreviewContainer()
{
	if( m_pPreviewPanel )
	{
		CGUIFrameworkResource::ms_pFramework->UnregisterOpenglInterface();
	}
	m_mgr.UnInit();	
}
//------------------------------------------------------------------------------
void WxResourcePreviewContainer::CreatePreviewCanvas( const CGUIProperty* pResourceProperty )
{
	if( !m_pPreviewPanel )
	{
		uint32 uPropertyType = pResourceProperty->GetType();
		switch( uPropertyType )
		{
		case ePropertyType_FontDefine:
			m_pPreviewPanel = new WxFontPreviewPanel( this );
			break;
		case ePropertyType_ImageDefine:
			m_pPreviewPanel = new WxImagePreviewPanel( this );
			break;
		case ePropertyType_AnimationDefine:
			m_pPreviewPanel = new WxAnimationPreviewPanel( this );
			break;
		case ePropertyType_SoundDefine:
			m_pPreviewPanel = new WxSoundPreviewPanel( this );
			break;
		}

		if( m_pPreviewPanel )
		{
			assert( m_pPreviewPanel->GetResourceType() == pResourceProperty->GetTypeAsString());

			m_mgr.AddPane(m_pPreviewPanel, wxAuiPaneInfo().
				Name(wxT("Preview")).Caption(wxT("Preview")).
				CenterPane().PaneBorder(false));

			CGUIFrameworkResource::ms_pFramework->RegisterOpenglInterface();

			m_mgr.Update();
		}
	}
}
//------------------------------------------------------------------------------
void WxResourcePreviewContainer::DestroyPreviewCanvas()
{
	if( m_pPreviewPanel )
	{
		CGUIFrameworkResource::ms_pFramework->UnregisterOpenglInterface();

		//clear panel
		m_mgr.DetachPane(m_pPreviewPanel);
		m_pPreviewPanel->Destroy();
		m_pPreviewPanel = NULL;
	}
	m_pPropertyGridMgr->Clear();
}
//------------------------------------------------------------------------------
void WxResourcePreviewContainer::SetGUIProperty( const CGUIProperty* pResourceProperty )
{
	if( !pResourceProperty )
	{
		DestroyPreviewCanvas();
		return;
	}

	//set property
	m_pPropertyGridMgr->Clear();
	for(unsigned int i=0; i<pResourceProperty->GetPropertyCount();++i)
	{
		CPropertyConvertorMgr::Instance()->GuiProperty2GridPropertyRow( m_pPropertyGridMgr, NULL, *pResourceProperty->GetProperty(i) );
	}

	//set preview
	if( m_pPreviewPanel )
	{
		if( m_pPreviewPanel->GetResourceType() != pResourceProperty->GetTypeAsString() )
		{
			DestroyPreviewCanvas();
		}
	}

	if( !m_pPreviewPanel )
	{
		CreatePreviewCanvas(pResourceProperty);
	}

	if( !m_pPreviewPanel )
	{
		wxMessageBox( _T("resource doesn't support preview"), _T("error") );
		return;
	}

	m_pPreviewPanel->SetResourceName( Gui2wxString(pResourceProperty->GetName()));
}
//------------------------------------------------------------------------------
