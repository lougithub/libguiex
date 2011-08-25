/** 
* @file tdwgttower.cpp
* @brief tower of td game
* @author ken
* @date 2011-08-17
*/


//============================================================================//
// include 
//============================================================================// 
#include "tdwgttower.h"
#include "tdgameworld.h"
#include "tdgametower.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CTDWgtTower);
	//------------------------------------------------------------------------------
	CTDWgtTower::CTDWgtTower( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitTower();
	}
	//------------------------------------------------------------------------------
	CTDWgtTower::~CTDWgtTower( )
	{
		if( m_pGameTower )
		{
			delete m_pGameTower;
			m_pGameTower = NULL;
		}
	}
	//------------------------------------------------------------------------------
	void CTDWgtTower::InitTower()
	{
		m_pImageTowerBase = NULL;

		m_bHovering = false;

		m_pGameTower = NULL;
		m_pGameWorld = NULL;
	}
	//------------------------------------------------------------------------------
	void CTDWgtTower::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "tower_base")
		{
			m_pImageTowerBase = pImage;
			if( GetSize().IsEqualZero() && m_pImageTowerBase )
			{
				SetPixelSize(m_pImageTowerBase->GetSize());
			}
		}
	}
	//------------------------------------------------------------------------------
	void CTDWgtTower::RenderSelf(IGUIInterfaceRender* pRender)
	{
		if( m_pGameTower )
		{
			m_pGameTower->OnRender( pRender );
		}
		else
		{
			if( m_pImageTowerBase )
			{
				if( m_bHovering )
				{
					m_pImageTowerBase->Draw( pRender, GetBoundArea(),m_fZ,CGUIColor( 1.0f,1.0f,0.5f,0.7f),GetDerivedAlpha() );
				}
				else
				{
					m_pImageTowerBase->Draw( pRender, GetBoundArea(),m_fZ,CGUIColor( 1.0f,1.0f,1.0f,1.0f),GetDerivedAlpha() );
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	void CTDWgtTower::OnUpdate( real fDeltaTime )
	{
		CGUIWidget::OnUpdate( fDeltaTime );

		if( m_pGameTower )
		{
			m_pGameTower->OnUpdate( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	uint32 CTDWgtTower::OnMouseEnter( CGUIEventMouse* pEvent )
	{
		m_bHovering = true;
		if( m_pGameTower )
		{
			m_pGameTower->OnMouseEnter();
		}

		return CGUIWidget::OnMouseEnter( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32 CTDWgtTower::OnMouseLeave( CGUIEventMouse* pEvent )
	{
		m_bHovering = false;
		if( m_pGameTower )
		{
			m_pGameTower->OnMouseLeave();
		}

		return CGUIWidget::OnMouseLeave( pEvent );
	}
	//------------------------------------------------------------------------------
	void CTDWgtTower::SetGameWorld( CTDGameWorld* pGameWorld )
	{
		m_pGameWorld = pGameWorld;

		if( m_pGameTower )
		{
			delete m_pGameTower;
			m_pGameTower = NULL;
		}
		if( m_pGameWorld )
		{
			m_pGameTower = new CTDGameTower( m_pGameWorld, GetAnchorPoint() );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex

