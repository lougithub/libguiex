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
	CGUICanvasLayer::CGUICanvasLayer( const char* szLayerName, bool bTopMost /*= false*/ )
		:m_strLayerName( szLayerName )
		,m_bTopMost( bTopMost )
	{
	}
	//------------------------------------------------------------------------------
	CGUICanvasLayer::~CGUICanvasLayer()
	{
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
	void CGUICanvasLayer::Update( real /*fDeltaTime*/ )
	{

	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayer::Render( IGUIInterfaceRender* pRender )
	{
	}
	void CGUICanvasLayer::RenderExtraInfo( IGUIInterfaceRender* pRender )
	{
	}
	//------------------------------------------------------------------------------
	void CGUICanvasLayer::Refresh( )
	{

	}
	//------------------------------------------------------------------------------
	CGUIWidget* CGUICanvasLayer::GetWidgetUnderPoint( const CGUIVector2& rPos )
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
}
