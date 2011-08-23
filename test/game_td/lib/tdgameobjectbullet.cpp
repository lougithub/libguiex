/** 
* @file tdgameobjectbullet.cpp
* @brief bullet
* @author ken
* @date 2011-08-22
*/


//============================================================================//
// include
//============================================================================// 
#include "tdgameobjectbullet.h"
#include "tdgameobjectmanager.h"
#include "tdgameworld.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CTDGameObjectBullet::CTDGameObjectBullet( CTDGameWorld* pGameWorld )
		:CTDGameObject( eGameObject_Bullet, pGameWorld )
	{
	}
	//------------------------------------------------------------------------------
	CTDGameObjectBullet::~CTDGameObjectBullet()
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::OnActive()
	{
		CTDGameObject::OnActive();
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::OnDeactive()
	{
		CTDGameObject::OnDeactive();
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::OnRender( IGUIInterfaceRender* pRender )
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::OnUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
}

