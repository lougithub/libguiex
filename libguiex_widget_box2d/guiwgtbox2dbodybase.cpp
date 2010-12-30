/** 
* @file guiwgtbox2dbodybase.cpp
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtbox2dbodybase.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_module/physics_box2d/guiphysics_box2d.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIWgtBox2DBodyBase::CGUIWgtBox2DBodyBase( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBase(rType, rName, rSceneName)
	{
		InitBox2DBodyBase();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBodyBase::InitBox2DBodyBase()
	{
		m_pBody = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBodyBase::ReleaseBox2D()
	{
		if( m_pBody )
		{
			b2World * pWorld = CGUIInterfaceManager::Instance()->GetInterfacePhysicsTyped<IGUIPhysics_box2d>()->GetWorld();
			if( pWorld )
			{
				pWorld->DestroyBody( m_pBody );
				m_pBody = NULL;
			}
		}

		CGUIWgtBox2DBase::ReleaseBox2D();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DBodyBase::RenderExtraSelfInfo(IGUIInterfaceRender* pRender)
	{
		CGUIWgtBox2DBase::RenderExtraSelfInfo( pRender );

		const b2Vec2& rPos = m_pBody->GetPosition();
		CGUIVector2 aPos( IGUIPhysics_box2d::Meter2Pixel(rPos.x), IGUIPhysics_box2d::Meter2Pixel(rPos.y) );
		CGUIRect aRect( aPos.x-3, aPos.y-3, aPos.x+3, aPos.y+3 );
		CGUIColor aColor( 1.0f, 0.0f, 0.0f, 1.0f );

		pRender->DrawRect( CGUIMatrix4::IDENTITY, aRect, 3, pRender->GetAndIncZ(),
			aColor.GetARGB(), aColor.GetARGB(), aColor.GetARGB(), aColor.GetARGB() );
	}
	//------------------------------------------------------------------------------
}//namespace guiex

