/** 
* @file wxanimationpreviewpanel.cpp
* @brief 
* @author ken
* @date 2011-09-10
*/


//============================================================================//
// include
//============================================================================// 
#include "wxanimationpreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"
#include <wx/filename.h> 

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
WxAnimationCanvas::WxAnimationCanvas( wxWindow *parent )
	:WxResourceCanvasBase(parent)
	,m_pGUIAnimation(NULL)
{

}
//------------------------------------------------------------------------------
WxAnimationCanvas::~WxAnimationCanvas()
{
	ClearAnimation();
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::OnUpdate( float fDeltaTime )
{
	if( m_pGUIAnimation )
	{
		m_pGUIAnimation->Update( fDeltaTime );
	}
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::OnRender( guiex::IGUIInterfaceRender* pRender )
{
	if( m_pGUIAnimation )
	{
		const CGUISize& rSize = m_pGUIAnimation->GetSize();

		CGUIRect aRenderRect( 
			-rSize.GetWidth()/2,
			-rSize.GetHeight()/2,
			rSize.GetWidth()/2,
			rSize.GetHeight()/2
			);
		m_pGUIAnimation->Draw( pRender, aRenderRect, 0, 1 );
	}
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::ClearAnimation()
{
	if( m_pGUIAnimation )
	{
		m_pGUIAnimation->RefRelease();
		m_pGUIAnimation = NULL;
	}
}		
//------------------------------------------------------------------------------
void WxAnimationCanvas::SetAnimationName( const guiex::CGUIString& rImageName )
{
	if( m_strAnimationName != rImageName )
	{
		ClearAnimation();
		m_strAnimationName = rImageName;
		if( !m_strAnimationName.empty() )
		{
			m_pGUIAnimation = CGUIAnimationManager::Instance()->AllocateResource( m_strAnimationName );
			if( m_pGUIAnimation )
			{
				m_pGUIAnimation->SetLooping( true );
			}
		}
	}
}
//------------------------------------------------------------------------------



BEGIN_EVENT_TABLE(WxAnimationPreviewPanel, WxResourcePreviewPanelBase)
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxAnimationPreviewPanel::WxAnimationPreviewPanel( wxWindow *parent )
: WxResourcePreviewPanelBase( parent )
, m_pAnimationCanvas( NULL )
{
	m_pAnimationCanvas = new WxAnimationCanvas( this ); 

	wxSizer *sizerAnimation = new wxBoxSizer( wxVERTICAL );
	sizerAnimation->Add( m_pAnimationCanvas, 1, wxALL|wxEXPAND );
	SetSizer( sizerAnimation );

	Show(true);
}
//------------------------------------------------------------------------------
void WxAnimationPreviewPanel::SetResourceName( const wxString& rResourceName )
{
	if( m_strResourceName != rResourceName )
	{
		m_strResourceName = rResourceName;

		m_pAnimationCanvas->SetAnimationName( wx2GuiString( m_strResourceName) );

		m_pAnimationCanvas->Refresh();
	}
}
//------------------------------------------------------------------------------
