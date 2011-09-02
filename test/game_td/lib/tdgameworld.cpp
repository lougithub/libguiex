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
#include "tdwgttower.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	CTDGameWorld* GGameWorld = 0;
}

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CTDGameWorld* GetGameWorld()
	{
		return GGameWorld;
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	CTDGameWorld::CTDGameWorld()
		:m_pCurrentFocusTower(NULL)
		,m_pTowerSelectPanel(NULL)
		,m_pTowerUpgradePanel(NULL)
		,m_uGold(0)
	{
		m_aObjectManager.SetGameWorld( this );
		GGameWorld = this;
	}
	//------------------------------------------------------------------------------
	CTDGameWorld::~CTDGameWorld()
	{
		GGameWorld = NULL;
	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::InitGameWorld( const CGUIString& rRound, CGUIWidget* pGameRoot, CGUIWidget* pTowerSelectPanel, CGUIWidget* pTowerUpgradePanel )
	{
		CTDGameWorldBase::InitGameWorld( pGameRoot );

		//load config data
		m_aConfigDataManager.LoadData( CGUISceneManager::Instance()->GetScenePath("game_td_map_base") + "configdata/");
	
		//find game layer
		CTDWgtGameWorldLayer* pGameLayer = static_cast<CTDWgtGameWorldLayer*>( pGameRoot->FindWidgetByType( CTDWgtGameWorldLayer::StaticGetType()));
		if( !pGameLayer )
		{
			GUI_THROW( "[CTDGameWorld::InitGameWorld]: can't find CTDWgtGameWorldLayer in map" );
			return;
		}
		pGameLayer->ApplyGameWorld( this );

		//get start path node
		m_pSimplePathNodeMgr = static_cast<CGUIWgtSimplePathNodeMgr*>( pGameRoot->FindWidgetByType( CGUIWgtSimplePathNodeMgr::StaticGetType()));
		if( !m_pSimplePathNodeMgr )
		{
			GUI_THROW( "CTDGameWorld::InitGameWorld]: can't find CGUIWgtSimplePathNodeMgr in map" );
			return;
		}

		//init tower
		m_pTowerSelectPanel = pTowerSelectPanel;
		m_pTowerUpgradePanel = pTowerUpgradePanel;

		CGUIWidget* pTowerNode = pGameRoot->FindWidgetByName( "node_towers" );
		if( !pTowerNode )
		{
			GUI_THROW( "CTDGameWorld::InitGameWorld]: can't find widget <node_towers> in map" );
		}
		CGUIWidget* pTower = pTowerNode->GetChild();
		while( pTower )
		{
			static_cast<CTDWgtTower*>(pTower)->SetGameWorld( this );
			pTower = pTower->GetNextSibling();
		}

		//init round data
		InitRound( rRound );

		//init object pool
		m_aObjectManager.InitObjects<CTDGameObjectMonster>( 100 );
		m_aObjectManager.InitObjects<CTDGameObjectBullet>( 100 );

		//=============================================
		//test here
		AllocateMonster( "monster001", "pathnode_1_start" );
		AllocateMonster( "monster002", "pathnode_2_start" );
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
		
		//clear path node
		m_pSimplePathNodeMgr = NULL;

		//clear config data
		m_aConfigDataManager.ClearData();


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
		RemoveDeadObject();
	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::InitRound( const CGUIString& rRound )
	{
		//get data property
		const CGUIProperty* pAllRoundProperty = GetGameWorld()->GetDataProperty("round.xml");
		const CGUIProperty& rRoundProp = *pAllRoundProperty->GetProperty(rRound);

		m_uGold = rRoundProp["Gold"]->GetSpecifiedValue<uint32>();
		m_uHP = rRoundProp["HP"]->GetSpecifiedValue<uint32>();
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
	void CTDGameWorld::RemoveDeadObject()
	{
		//bullet
		for( TBulletObjects::iterator itor = m_arrayObjectBullet.begin();
			itor != m_arrayObjectBullet.end(); )
		{
			CTDGameObject* pObject = *itor;
			if( pObject->IsAlive() == false )
			{
				m_aObjectManager.FreeObject( pObject );
				m_arrayObjectBullet.erase( itor );
			}
			else
			{
				++itor;
			}
		}

		//monster
		for( TMonsterObjects::iterator itor = m_arrayObjectMonster.begin();
			itor != m_arrayObjectMonster.end(); )
		{
			CTDGameObject* pObject = *itor;
			if( pObject->IsAlive() == false )
			{
				m_aObjectManager.FreeObject( pObject );
				itor = m_arrayObjectMonster.erase( itor );
			}
			else
			{
				++itor;
			}
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
	void CTDGameWorld::AllocateMonster( const CGUIString& rMonsterType, const CGUIString& rStartPathNode )
	{
		CTDGameObjectMonster* pMonster = m_aObjectManager.AllocateObject<CTDGameObjectMonster>( eGameObject_Monster );
		m_arrayObjectMonster.push_back( pMonster );
		pMonster->InitMonster( rMonsterType, rStartPathNode );
	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::AllocateBullet( const CGUIString& rBulletType, const CGUIVector2& rStartPos, CTDGameObjectMonster* pTarget)
	{
		CTDGameObjectBullet* pBullet = m_aObjectManager.AllocateObject<CTDGameObjectBullet>( eGameObject_Bullet );
		m_arrayObjectBullet.push_back( pBullet );
		pBullet->InitBullet( rBulletType, rStartPos, pTarget );
	}
	//------------------------------------------------------------------------------
	const CGUIProperty* CTDGameWorld::GetDataProperty( const CGUIString& rFilename ) const
	{
		const CGUIProperty* pProperty = m_aConfigDataManager.GetProperty( rFilename );
		if( !pProperty )
		{
			GUI_THROW( GUI_FORMAT("CTDGameWorld::GetDataProperty]: can't find any data property from file %s", rFilename.c_str()) );
		}
		return pProperty;
	}
	//------------------------------------------------------------------------------
	CGUIWgtSimplePathNode* CTDGameWorld::GetSimplePathNode( const CGUIString& rPathNodeName ) const
	{
		if( !m_pSimplePathNodeMgr )
		{
			GUI_THROW( "CTDGameWorld::GetSimplePathNode]: no simple path node manager" );
			return NULL;
		}
		CGUIWgtSimplePathNode * pPathNode = m_pSimplePathNodeMgr->FindPathNode( rPathNodeName );
		if( !pPathNode )
		{
			GUI_THROW( GUI_FORMAT("CTDGameWorld::GetSimplePathNode]: not find simple path node %s", rPathNodeName.c_str()));
			return NULL;
		}
		return pPathNode;
	}
	//------------------------------------------------------------------------------
	CTDGameTower* CTDGameWorld::GetCurrentFocusTower() const
	{
		return m_pCurrentFocusTower;
	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::SetCurrentFocusTower( CTDGameTower* pTower )
	{
		m_pCurrentFocusTower = pTower;
	}
	//------------------------------------------------------------------------------
	CGUIWidget* CTDGameWorld::GetTowerSelectPanel() const
	{
		return m_pTowerSelectPanel;
	}
	//------------------------------------------------------------------------------
	CGUIWidget* CTDGameWorld::GetTowerUpgradePanel() const
	{
		return m_pTowerUpgradePanel;
	}
	//------------------------------------------------------------------------------
	uint32 CTDGameWorld::GetGold() const
	{
		return m_uGold;
	}
	//------------------------------------------------------------------------------
	void CTDGameWorld::CostGold( uint32 uGold )
	{
		if( m_uGold < uGold )
		{
			GUI_THROW( GUI_FORMAT( "[CTDGameWorld::CostGold]: cost gold %d, but we only have %d.", uGold, m_uGold ));
			return;
		}
		m_uGold -= uGold;
	}
	//------------------------------------------------------------------------------
	uint32 CTDGameWorld::GetHP() const
	{
		return m_uHP;
	}
	//------------------------------------------------------------------------------

}

