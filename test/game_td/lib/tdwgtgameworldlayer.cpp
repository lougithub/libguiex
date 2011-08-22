/** 
* @file tdwgtgameworldlayer.cpp
* @brief used to render monster and other things.
* @author ken
* @date 2011-08-19
*/



//============================================================================//
// include 
//============================================================================// 
#include "tdwgtgameworldlayer.h"
#include "tdgameworldbase.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CTDWgtGameWorldLayer);
	//------------------------------------------------------------------------------
	CTDWgtGameWorldLayer::CTDWgtGameWorldLayer( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitGameLayer();
	}
	//------------------------------------------------------------------------------
	void CTDWgtGameWorldLayer::InitGameLayer()
	{
		m_pGameWorld = NULL;
	}
	//------------------------------------------------------------------------------
	void CTDWgtGameWorldLayer::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIWidget::RenderSelf( pRender );
		if( m_pGameWorld )
		{
			m_pGameWorld->OnRender( pRender );
		}
	}
	//------------------------------------------------------------------------------
	void CTDWgtGameWorldLayer::OnUpdate( real fDeltaTime )
	{
		CGUIWidget::OnUpdate( fDeltaTime );
		if( m_pGameWorld )
		{
			m_pGameWorld->OnUpdate( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	CTDGameWorldBase* CTDWgtGameWorldLayer::GetGameWorld( ) const
	{
		return m_pGameWorld;
	}
	//------------------------------------------------------------------------------
	void CTDWgtGameWorldLayer::ApplyGameWorld( class CTDGameWorldBase* pWorld )
	{
		if( m_pGameWorld && pWorld)
		{
			GUI_THROW( "CTDWgtGameWorldLayer::ApplyGameWorld]: game world has been applied" );
			return;
		}
		m_pGameWorld = pWorld;
	}
	//------------------------------------------------------------------------------

}//namespace guiex

