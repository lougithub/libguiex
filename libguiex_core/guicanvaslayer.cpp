/** 
 * @file guicanvaslayer.cpp
 * @brief canvas layer used in system
 * @author ken
 * @date 2010-12-22
 */

//============================================================================//
// include 
//============================================================================// 
#include "guicanvaslayer.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUICanvasLayer::CGUICanvasLayer( const char* szLayerName, bool bTopMost /*= false*/, const CGUISize& rCanvasSize /*= GSystem->GetScreenSize()*/ )
		:CGUIWidget( "CGUICanvasLayer", szLayerName, "" )
		,m_strLayerName( szLayerName )
		,m_bTopMost( bTopMost )
	{
		SetPixelSize( rCanvasSize );
		SetFocusable(false);
		SetActivable(false);
		SetMouseConsumed(false);
		SetHitable( false );
	}
	//------------------------------------------------------------------------------
	CGUICanvasLayer::~CGUICanvasLayer()
	{
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayer::Initialize( )
	{
		Create();
		Open();
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayer::Finalize( )
	{
		Close();
		OnDestroy();
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUICanvasLayer::GetLayerName( ) const
	{
		return m_strLayerName;
	}
	//------------------------------------------------------------------------------
	bool CGUICanvasLayer::IsTopMost( ) const
	{
		return m_bTopMost;
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayer::Update( real fDeltaTime )
	{
		//update widget
		CGUIWidget::Update( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayer::Render( IGUIInterfaceRender* pRender )
	{
		CGUIWidget::Render( pRender );
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayer::RenderExtraInfo( IGUIInterfaceRender* pRender )
	{
		CGUIWidget::RenderExtraInfo( pRender );
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayer::BeginRender( IGUIInterfaceRender* /*pRender*/ )
	{
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayer::EndRender( IGUIInterfaceRender* /*pRender*/ )
	{
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayer::Refresh( )
	{
		CGUIWidget::Refresh( );
	}
	//------------------------------------------------------------------------------
	CGUIWidget* CGUICanvasLayer::GetWidgetUnderPoint( const CGUIVector2& rPos )
	{
		return GetWidgetAtPoint(rPos);
	}
	//------------------------------------------------------------------------------
}
