/** 
* @file guiwgtbox2dstaticbody.cpp
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtbox2dstaticbody.h"
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
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtBox2DStaticBody);
	//------------------------------------------------------------------------------
	CGUIWgtBox2DStaticBody::CGUIWgtBox2DStaticBody( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBase(StaticGetType(), rName, rSceneName)
	{
		InitBox2DStaticBody();
	}
	//------------------------------------------------------------------------------
	CGUIWgtBox2DStaticBody::CGUIWgtBox2DStaticBody( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBase(rType, rName, rSceneName)
	{
		InitBox2DStaticBody();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DStaticBody::InitBox2DStaticBody()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DStaticBody::OnCreate()
	{
		CGUIWgtBox2DBase::OnCreate();

		b2World * pWorld = CGUIInterfaceManager::Instance()->GetInterfacePhysicsTyped<IGUIPhysics_box2d>()->GetWorld();

		//create static body
		b2BodyDef bd;
		bd.type = b2_staticBody;

		//set position
		CGUIVector2 vPos = GetPosition();
		LocalToWorld( vPos );
		bd.position.Set( vPos.x, vPos.y );
		
		//create body
		b2Body* body = pWorld->CreateBody(&bd);

		//b2Body * groundBody = m_pWorld->CreateBody( &bd );
		//b2PolygonShape groundBox;
		//groundBox.SetAsBox( 80.0f, 4.0f );
		//groundBody->CreateFixture( &groundBox, 0.0f );
	}
	//------------------------------------------------------------------------------

}//namespace guiex

