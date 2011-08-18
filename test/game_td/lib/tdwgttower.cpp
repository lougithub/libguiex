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
	GUI_WIDGET_GENERATOR_IMPLEMENT(TDWgtTower);
	//------------------------------------------------------------------------------
	TDWgtTower::TDWgtTower( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitTower();
	}
	//------------------------------------------------------------------------------
	void TDWgtTower::InitTower()
	{
		m_pImageTowerBase = NULL;
	}
	//------------------------------------------------------------------------------
	void TDWgtTower::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
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
	void TDWgtTower::RenderSelf(IGUIInterfaceRender* pRender)
	{
		DrawImage( pRender, m_pImageTowerBase, GetBoundArea( ));
	}
	//------------------------------------------------------------------------------

}//namespace guiex

