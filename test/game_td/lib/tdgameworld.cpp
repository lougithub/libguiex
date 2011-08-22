/** 
* @file tdgameworld.cpp
* @brief class of game world, contain all monster, bullet and thing in fighting game
* @author ken
* @date 2011-08-22
*/


//============================================================================//
// include
//============================================================================// 
#include "tdgameworld.h"

#include "tdgameobjectbullet.h"
#include "tdgameobjectmonster.h"

#include "tdwgtgameworldlayer.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CTDGameWorld::CTDGameWorld()
	{
		m_aObjectManager.SetGameWorld( this );
	}
	//------------------------------------------------------------------------------
	CTDGameWorld::~CTDGameWorld()
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::InitGameWorld( CGUIWidget* pGameRoot )
	{
		CTDGameWorldBase::InitGameWorld( pGameRoot );

		//load config data
		m_aConfigDataManager.LoadData( CGUISceneManager::Instance()->GetScenePath("game_td_map_base") + "configdata/");
	
		//find game layer
		CTDWgtGameWorldLayer* pGameLayer = static_cast<CTDWgtGameWorldLayer*>( pGameRoot->FindWidgetByType( CTDWgtGameWorldLayer::StaticGetType()));
		if( !pGameLayer )
		{
			GUI_THROW( "CTDGameWorld::InitGameWorld]: can't find CTDWgtGameWorldLayer in map" );
			return;
		}
		pGameLayer->ApplyGameWorld( this );

		//init object pool
		m_aObjectManager.InitObjects<CTDGameObjectMonster>( 100 );
		m_aObjectManager.InitObjects<CTDGameObjectBullet>( 100 );

		//=============================================
		//test here
		CTDGameObjectMonster* pMonster = AllocateMonster();
		//test here
		//==============================================

	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::DestroyGameWorld()
	{
		//deactive all object
		for( TBulletObjects::iterator itor = m_arrayObjectBullet.begin();
			itor != m_arrayObjectBullet.end();
			++itor )
		{
			m_aObjectManager.FreeObject( *itor );
		}
		m_arrayObjectBullet.empty();
		for( TMonsterObjects::iterator itor = m_arrayObjectMonster.begin();
			itor != m_arrayObjectMonster.end();
			++itor )
		{
			m_aObjectManager.FreeObject( *itor );
		}
		m_arrayObjectMonster.empty();
		m_aObjectManager.ReleaseAllObjects();

		CTDGameWorldBase::DestroyGameWorld();
	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::OnRender( IGUIInterfaceRender* pRender )
	{
		RenderAllObjects( pRender );
	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::OnUpdate( real fDeltaTime )
	{
		UpdateAllObjects( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::UpdateAllObjects( real fDeltaTime )
	{
		//bullet
		for( TBulletObjects::iterator itor = m_arrayObjectBullet.begin();
			itor != m_arrayObjectBullet.end();
			++itor )
		{
			(*itor)->OnUpdate( fDeltaTime );
		}

		//monster
		for( TMonsterObjects::iterator itor = m_arrayObjectMonster.begin();
			itor != m_arrayObjectMonster.end();
			++itor )
		{
			(*itor)->OnUpdate( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::RenderAllObjects( IGUIInterfaceRender* pRender )
	{
		//bullet
		for( TBulletObjects::iterator itor = m_arrayObjectBullet.begin();
			itor != m_arrayObjectBullet.end();
			++itor )
		{
			(*itor)->OnRender( pRender );
		}

		//monster
		for( TMonsterObjects::iterator itor = m_arrayObjectMonster.begin();
			itor != m_arrayObjectMonster.end();
			++itor )
		{
			(*itor)->OnRender( pRender );
		}
	}
	//------------------------------------------------------------------------------
	CTDGameObjectMonster* CTDGameWorld::AllocateMonster()
	{
		CTDGameObjectMonster* pMonster = m_aObjectManager.AllocateObject<CTDGameObjectMonster>( eGameObject_Monster );
		m_arrayObjectMonster.push_back( pMonster );
		pMonster->InitMonster();
		return pMonster;
	}
	//------------------------------------------------------------------------------

}

