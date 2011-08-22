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
	void CTDWgtTower::InitTower()
	{
		m_pImageTowerBase = NULL;

		m_bHovering = false;
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
	//------------------------------------------------------------------------------
	uint32 CTDWgtTower::OnMouseEnter( CGUIEventMouse* pEvent )
	{
		m_bHovering = true;

		return CGUIWidget::OnMouseEnter( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32 CTDWgtTower::OnMouseLeave( CGUIEventMouse* pEvent )
	{
		m_bHovering = false;

		return CGUIWidget::OnMouseLeave( pEvent );
	}
	//------------------------------------------------------------------------------
}//namespace guiex

