/** 
* @file wximageepreviewpanelbase.cpp
* @brief 
* @author ken
* @date 2011-09-10
*/

//============================================================================//
// include
//============================================================================// 
#include "wximagepreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"
#include <wx/filename.h> 

////============================================================================//
//// function
////============================================================================// 
//------------------------------------------------------------------------------
WxImageCanvas::WxImageCanvas( wxWindow *parent )
	:WxResourceCanvasBase(parent)
	,m_pGUIImage(NULL)
{

}
//------------------------------------------------------------------------------
WxImageCanvas::~WxImageCanvas()
{
	ClearImage();
}
//------------------------------------------------------------------------------
void WxImageCanvas::OnUpdate( float fDeltaTime )
{

}
//------------------------------------------------------------------------------
void WxImageCanvas::OnRender( guiex::IGUIInterfaceRender* pRender )
{
	if( m_pGUIImage )
	{
		const CGUISize& rSize = m_pGUIImage->GetSize();

		CGUIRect aRenderRect( 
			-rSize.GetWidth()/2,
			-rSize.GetHeight()/2,
			rSize.GetWidth()/2,
			rSize.GetHeight()/2
			);
		m_pGUIImage->Draw( pRender, aRenderRect, 0, 1 );
	}
}
//------------------------------------------------------------------------------
void WxImageCanvas::ClearImage()
{
	if( m_pGUIImage )
	{
		m_pGUIImage->RefRelease();
		m_pGUIImage = NULL;
	}
}		
//------------------------------------------------------------------------------
void WxImageCanvas::SetImageName( const guiex::CGUIString& rImageName )
{
	if( m_strImageName != rImageName )
	{
		ClearImage();
		m_strImageName = rImageName;
		if( !m_strImageName.empty() )
		{
			m_pGUIImage = CGUIImageManager::Instance()->AllocateResource( m_strImageName );
		}
	}
}
//------------------------------------------------------------------------------
void WxImageCanvas::SetUVRect( const wxRect& rTargetRect )
{
	m_aUVRect = rTargetRect;
}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxImagePreviewPanel, WxResourcePreviewPanelBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxImagePreviewPanel::WxImagePreviewPanel( wxWindow* parent )
:WxResourcePreviewPanelBase( parent )
{
	m_pImageCanvas = new WxImageCanvas( this ); 
	m_pFullImageCanvas = new WxImageCanvas( this ); 

	wxSizer *sizerImages = new wxBoxSizer( wxVERTICAL );
	sizerImages->Add( m_pImageCanvas, 1, wxALL|wxEXPAND );
	sizerImages->Add( m_pFullImageCanvas, 1, wxALL|wxEXPAND );
	SetSizer( sizerImages );

	Show(true);
}
//------------------------------------------------------------------------------
void WxImagePreviewPanel::SetResourceName( const wxString& rResourceName ) 
{
	if( m_strResourceName == rResourceName )
	{
		return;
	}
	m_strResourceName = rResourceName;

	m_pImageCanvas->SetImageName( wx2GuiString( m_strResourceName) );
	m_pFullImageCanvas->SetImageName( wx2GuiString( m_strResourceName) );


	////thumbnail
	//wxBitmap* pBitmap = CGUIResourcePool::Instance()->GenerateOriginalImageThumbnail(  m_strResourceName );
	//m_pImageCanvas->SetBitmap( pBitmap );

	////full image
	//m_pFullImageCanvas->SetBitmap( NULL );
	//if( !m_strResourceName.empty() )
	//{
	//	CGUIImage* pImage = CGUIImageManager::Instance()->AllocateResource( wx2GuiString( m_strResourceName ) );
	//	if( pImage && pImage->GetImageType() == eImageType_FromFile )
	//	{
	//		wxImage* pWxImage = LoadwxImageByGuiImage( pImage );
	//		if( pWxImage )
	//		{
	//			wxBitmap* pOriginalBitmap = new wxBitmap( *pWxImage );
	//			delete pWxImage;
	//			pWxImage = NULL;

	//			m_pFullImageCanvas->SetBitmap( pOriginalBitmap );
	//			const CGUIRect& rRect = pImage->GetUVRect();
	//			wxSize newSize( 
	//				pOriginalBitmap->GetSize().GetWidth() * rRect.GetWidth(), 
	//				pOriginalBitmap->GetSize().GetHeight()* rRect.GetHeight() );
	//			wxPoint newPoint( 
	//				rRect.GetPosition().x * pOriginalBitmap->GetSize().GetWidth(), 
	//				rRect.GetPosition().y * pOriginalBitmap->GetSize().GetHeight() );
	//			wxRect aUVRect( newPoint, newSize );
	//			m_pFullImageCanvas->SetUVRect( aUVRect );
	//		}
	//	}
	//	if( pImage )
	//	{
	//		pImage->RefRelease();
	//	}
	//}

	m_pImageCanvas->Refresh();
	m_pFullImageCanvas->Refresh();
}
//------------------------------------------------------------------------------
