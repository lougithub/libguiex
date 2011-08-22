/** 
* @file tdgameobjectmonster.cpp
* @brief monster
* @author ken
* @date 2011-08-22
*/


//============================================================================//
// include
//============================================================================// 
#include "tdgameobjectmonster.h"
#include "tdgameobjectmanager.h"
#include "tdgameworld.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CTDGameObjectMonster::CTDGameObjectMonster( CTDGameWorld* pGameWorld )
		:CTDGameObject( eGameObject_Monster, pGameWorld )
	{
		memset( m_arrayAnimations, 0, sizeof( m_arrayAnimations ));
	}
	//------------------------------------------------------------------------------
	CTDGameObjectMonster::~CTDGameObjectMonster()
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::InitMonster()
	{
		InitAnimation();
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::OnActive()
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::OnDeactive()
	{
		ReleaseAnimation();
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::OnRender( IGUIInterfaceRender* pRender )
	{
		m_arrayAnimations[eAnimState_Idle]->Draw( pRender, CGUIRect( 0,0,100,100 ), 0, 1 );
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::OnUpdate( real fDeltaTime )
	{
		m_arrayAnimations[eAnimState_Idle]->Update( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::InitAnimation()
	{
		m_arrayAnimations[eAnimState_Idle] = CGUIAnimationManager::Instance()->AllocateResource( "Monster001_Idle" );
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::ReleaseAnimation()
	{
		for( uint32 i=0; i<__eAnimState_MAX__; ++i )
		{
			if( m_arrayAnimations[i] )
			{
				m_arrayAnimations[i]->RefRelease();
				m_arrayAnimations[i] = NULL;
			}
		}
	}
	//------------------------------------------------------------------------------
}

