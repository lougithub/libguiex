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
	m_pSizer = new wxBoxSizer( wxVERTICAL );
	SetSizer( m_pSizer );
}
//------------------------------------------------------------------------------
WxResourcePreviewContainer::~WxResourcePreviewContainer()
{
	if( m_pPreviewPanel )
	{
		CGUIFrameworkResource::ms_pFramework->UnregisterOpenglInterface();
	}
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

			m_pSizer->Add( m_pPreviewPanel, 1, wxALL|wxEXPAND );
			Layout();

			CGUIFrameworkResource::ms_pFramework->RegisterOpenglInterface();
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
		m_pSizer->Clear( true );
		//RemoveChild( m_pPreviewPanel );
		m_pPreviewPanel = NULL;
	}
}
//------------------------------------------------------------------------------
void WxResourcePreviewContainer::SetGUIProperty( const CGUIProperty* pResourceProperty )
{
	if( !pResourceProperty )
	{
		DestroyPreviewCanvas();
		return;
	}

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
