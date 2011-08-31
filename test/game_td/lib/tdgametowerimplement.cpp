/** 
* @file tdgametowerimplement.cpp
* @brief tower of td game
* @author ken
* @date 2011-08-29
*/


//============================================================================//
// include 
//============================================================================// 
#include "tdgametowerimplement.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	// CTDGameTowerImplement
	//------------------------------------------------------------------------------
	CTDGameTowerImplement::CTDGameTowerImplement( CTDGameTower* pGameTower, const CGUIString& rTowerType )
		:m_pGameTower( pGameTower )
		,m_strTowerType( rTowerType )
		,m_uLevel(0)
	{
		const CGUIProperty* pAllTowerProperty = GetGameWorld()->GetDataProperty("tower.xml");
		const CGUIProperty* pTowerProp = pAllTowerProperty->GetProperty(m_strTowerType);

		//animations
		for( uint32 i=0; i<pTowerProp->GetPropertyNum(); ++i )
		{
			const CGUIProperty* pProp = pTowerProp->GetProperty( i );
			if( pProp->GetType() == ePropertyType_Animation )
			{
				GUI_ASSERT( m_mapAnimations.find(pProp->GetName()) == m_mapAnimations.end(), "[CTDGameTowerImplement::]: duplicate animation name");
				m_mapAnimations[pProp->GetName()] = CGUIAnimationManager::Instance()->AllocateResource( pProp->GetValue() );
			}
		}
	}
	//------------------------------------------------------------------------------
	CTDGameTowerImplement::~CTDGameTowerImplement()
	{
		for( std::map<CGUIString, CGUIAnimation*>::iterator itor = m_mapAnimations.begin();
			itor != m_mapAnimations.end();
			++itor )
		{
			itor->second->RefRelease();
		}
		m_mapAnimations.clear();
	}
	//------------------------------------------------------------------------------
	const CGUIString& CTDGameTowerImplement::GetTowerType() const
	{
		return m_strTowerType;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CTDGameTowerImplement::GetAnimation( const CGUIString& rAnimName )
	{
		std::map<CGUIString, CGUIAnimation*>::iterator itor = m_mapAnimations.find( rAnimName );
		if( itor == m_mapAnimations.end() )
		{
			GUI_THROW( GUI_FORMAT("[CTDGameTowerImplement::GetAnimation]: can't find animation by name: %s", rAnimName.c_str()) );
			return NULL;
		}
		return itor->second;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement::OnUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement::OnRender( IGUIInterfaceRender* pRender )
	{
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Base
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Base::CTDGameTowerImplement_Base( CTDGameTower* pGameTower )
		:CTDGameTowerImplement( pGameTower, "tower_base")
		,m_pAnimTowerBase(NULL)
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Base::OnBuild()
	{
		m_pAnimTowerBase = GetAnimation( "AnimIdle" );
		GetGameTower()->SetSize( m_pAnimTowerBase->GetSize());
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Base::OnUpdate( real fDeltaTime )
	{
		CTDGameTowerImplement::OnUpdate( fDeltaTime );

		m_pAnimTowerBase->Update( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Base::OnRender( IGUIInterfaceRender* pRender )
	{
		CTDGameTowerImplement::OnRender( pRender );

		m_pAnimTowerBase->Draw( pRender, GetRenderRect(), 0.0f, GetRenderColor(), 1.0f );
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_TowerBase
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_TowerBase::CTDGameTowerImplement_TowerBase( CTDGameTower* pGameTower, const CGUIString& rTowerType )
		:CTDGameTowerImplement( pGameTower, rTowerType)
		,m_eTowerState(eTowerState_Unknown)
		,m_fBuildingTimeElapsed(0.0f)
	{
		const CGUIProperty* pAllTowerProperty = GetGameWorld()->GetDataProperty("tower.xml");
		const CGUIProperty* pTowerProp = pAllTowerProperty->GetProperty(GetTowerType());

		m_fBuildingTime = pTowerProp->GetProperty("building_time")->GetSpecifiedValue<real>();

		char szLevelName[10];
		uint32 nLevel = 1;
		snprintf( szLevelName, 10, "level%03d", nLevel );
		const CGUIProperty* pLevelProp = pTowerProp->GetProperty( szLevelName );
		while( pLevelProp )
		{
			m_vecLevelInfos.push_back( STowerLevelInfo() );
			STowerLevelInfo& rInfo = m_vecLevelInfos.back();
			rInfo.m_uPrice = pLevelProp->GetProperty("price")->GetSpecifiedValue<uint32>();
			rInfo.m_uSellPrice = pLevelProp->GetProperty("sell_price")->GetSpecifiedValue<uint32>();
			rInfo.m_pLevelProperty = pLevelProp;

			for( uint32 i=0; i<pLevelProp->GetPropertyNum(); ++i )
			{
				const CGUIProperty* pProp = pLevelProp->GetProperty( i );
				if( pProp->GetType() == ePropertyType_Animation )
				{
					GUI_ASSERT( rInfo.m_mapAnimations.find(pProp->GetName()) == rInfo.m_mapAnimations.end(), "[CTDGameTowerImplement::]: duplicate animation name");
					rInfo.m_mapAnimations[pProp->GetName()] = CGUIAnimationManager::Instance()->AllocateResource( pProp->GetValue() );
				}
			}

			++nLevel;
			snprintf( szLevelName, 10, "level%03d", nLevel );
			pLevelProp = pTowerProp->GetProperty( szLevelName );
		}
	}
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_TowerBase::~CTDGameTowerImplement_TowerBase()
	{
		for( std::vector<STowerLevelInfo>::iterator itorLevel = m_vecLevelInfos.begin();
			itorLevel != m_vecLevelInfos.end();
			++itorLevel )
		{
			STowerLevelInfo& rInfo = *itorLevel;

			for( std::map<CGUIString, CGUIAnimation*>::iterator itor = rInfo.m_mapAnimations.begin();
				itor != rInfo.m_mapAnimations.end();
				++itor )
			{
				itor->second->RefRelease();
			}
			rInfo.m_mapAnimations.clear();
		}
		m_vecLevelInfos.clear();
	}
	//------------------------------------------------------------------------------
	uint32 CTDGameTowerImplement_TowerBase::GetUpgradeCost() const
	{
		if( m_vecLevelInfos.size() == 0 || m_uLevel >= m_vecLevelInfos.size()-1 )
		{
			GUI_THROW( "[CTDGameTowerImplement_TowerBase::GetUpgradeCost]: invalid tower level");
			return 0;
		}
		return m_vecLevelInfos[m_uLevel+1].m_uPrice;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::Upgrade()
	{
		if( m_vecLevelInfos.size() == 0 || m_uLevel >= m_vecLevelInfos.size()-1 )
		{
			GUI_THROW( "[CTDGameTowerImplement_TowerBase::Upgrade]: invalid tower level");
			return;
		}
		m_uLevel++;
		UpdateTowerDataByLevel();
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::SetTowerState( ETowerState eState )
	{
		if( m_eTowerState != eState )
		{
			switch( m_eTowerState )
			{
			case eTowerState_Construct:
				OnLeaveState_Construct();
				break;
			case eTowerState_Idle:
				OnLeaveState_Idle();
				break;
			}

			m_eTowerState = eState;
			switch( m_eTowerState )
			{
			case eTowerState_Construct:
				OnEnterState_Construct();
				break;
			case eTowerState_Idle:
				OnEnterState_Idle();
				break;
			}
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnBuild()
	{
		CTDGameTowerImplement::OnBuild();

		//set tower state
		SetTowerState( eTowerState_Construct );

		m_uLevel = 0;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnDestruct()
	{
		SetTowerState( eTowerState_Unknown );
		m_uLevel = 0;

		CTDGameTowerImplement::OnDestruct();
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::UpdateTowerDataByLevel( )
	{

	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnEnterState_Construct()
	{
		m_pAnimBuilding = GetAnimation("AnimTower_Building");
		GetGameTower()->SetSize( m_pAnimBuilding->GetSize());

		m_fBuildingTimeElapsed = 0.0f;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnLeaveState_Construct()
	{
		m_pAnimBuilding = NULL;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnEnterState_Idle()
	{
		//set tower data
		UpdateTowerDataByLevel();
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnLeaveState_Idle()
	{

	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnUpdateState_Construct(real fDeltaTime)
	{
		m_pAnimBuilding->Update( fDeltaTime );

		m_fBuildingTimeElapsed += fDeltaTime;
		if( m_fBuildingTimeElapsed >= m_fBuildingTime )
		{
			SetTowerState( eTowerState_Idle );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnRenderState_Construct(IGUIInterfaceRender* pRender)
	{
		m_pAnimBuilding->Draw( pRender, GetRenderRect(), 0.0f, GetRenderColor(), 1.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnUpdateState_Idle(real fDeltaTime)
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnUpdate( real fDeltaTime )
	{
		switch( m_eTowerState )
		{
		case eTowerState_Construct:
			OnUpdateState_Construct( fDeltaTime );
			break;
		case eTowerState_Idle:
			OnUpdateState_Idle( fDeltaTime );
			break;
		}

		CTDGameTowerImplement::OnUpdate( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_TowerBase::OnRender( IGUIInterfaceRender* pRender )
	{
		CTDGameTowerImplement::OnRender( pRender );

		switch( m_eTowerState )
		{
		case eTowerState_Construct:
			OnRenderState_Construct( pRender );
			break;
		case eTowerState_Idle:
			OnRenderState_Idle( pRender );
			break;
		}
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_ArcherTower
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_ArcherTower::CTDGameTowerImplement_ArcherTower( CTDGameTower* pGameTower )
		:CTDGameTowerImplement_TowerBase( pGameTower, "tower_archertower")
		,m_pAnimTowerIdle(NULL)
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_ArcherTower::UpdateTowerDataByLevel( )
	{
		CTDGameTowerImplement_TowerBase::UpdateTowerDataByLevel();

		//update archer data
		STowerLevelInfo& rInfo = m_vecLevelInfos[m_uLevel];
		for( uint32 i=0; i<2; ++i )
		{
			m_aArcherData[i].m_pAnimArcher[eArcherState_Idle][eArcherDir_UpRight] = rInfo.m_mapAnimations["AnimArcher_Idle_UpRight"];
			m_aArcherData[i].m_pAnimArcher[eArcherState_Idle][eArcherDir_UpLeft] = rInfo.m_mapAnimations["AnimArcher_Idle_UpLeft"];
			m_aArcherData[i].m_pAnimArcher[eArcherState_Idle][eArcherDir_DownRight] = rInfo.m_mapAnimations["AnimArcher_Idle_DownRight"];
			m_aArcherData[i].m_pAnimArcher[eArcherState_Idle][eArcherDir_DownLeft] = rInfo.m_mapAnimations["AnimArcher_Idle_DownLeft"];
			m_aArcherData[i].m_pAnimArcher[eArcherState_Attack][eArcherDir_UpRight] = rInfo.m_mapAnimations["AnimArcher_Attack_UpRight"];
			m_aArcherData[i].m_pAnimArcher[eArcherState_Attack][eArcherDir_UpLeft] = rInfo.m_mapAnimations["AnimArcher_Attack_UpLeft"];
			m_aArcherData[i].m_pAnimArcher[eArcherState_Attack][eArcherDir_DownRight] = rInfo.m_mapAnimations["AnimArcher_Attack_DownRight"];
			m_aArcherData[i].m_pAnimArcher[eArcherState_Attack][eArcherDir_DownLeft] = rInfo.m_mapAnimations["AnimArcher_Attack_DownLeft"];

			m_aArcherData[i].m_eArcherState = eArcherState_Idle;
			m_aArcherData[i].m_eArcherDir = eArcherDir_DownRight;

			char posName[16];
			snprintf( posName, 16, "archer_pos_%d", i );
			CGUIVector2 pos = rInfo.m_pLevelProperty->GetProperty(posName)->GetSpecifiedValue<CGUIVector2>();
			const CGUISize& size = m_aArcherData[i].m_pAnimArcher[eArcherState_Idle][eArcherDir_UpRight]->GetSize();
			m_aArcherData[i].m_rectArcherRender = CGUIRect( CGUIVector2( pos.x-size.m_fWidth/2, pos.y-size.m_fHeight/2 ), size );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_ArcherTower::OnEnterState_Idle()
	{
		CTDGameTowerImplement_TowerBase::OnEnterState_Idle();

		//update tower data
		m_pAnimTowerIdle = GetLevelInfo(m_uLevel)->m_mapAnimations.find("AnimTower_Idle")->second;
		GetGameTower()->SetSize( m_pAnimTowerIdle->GetSize());
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_ArcherTower::OnUpdateState_Idle(real fDeltaTime)
	{
		CTDGameTowerImplement_TowerBase::OnUpdateState_Idle(fDeltaTime);

		//update tower
		m_pAnimTowerIdle->Update( fDeltaTime );

		//update archer state
		for( uint32 i=0; i<2; ++i )
		{
			EArcherDir eDir = m_aArcherData[i].m_eArcherDir;
			if( m_aArcherData[i].m_eArcherState == eArcherState_Attack )
			{
				real fLeftTime = m_aArcherData[i].m_pAnimArcher[eArcherState_Attack][eDir]->Update( fDeltaTime );
				if( fLeftTime > 0.0f )
				{
					//finished
					m_aArcherData[i].m_eArcherState = eArcherState_Idle;
					m_aArcherData[i].m_pAnimArcher[eArcherState_Idle][eDir]->Reset();
					m_aArcherData[i].m_pAnimArcher[eArcherState_Idle][eDir]->Update( fLeftTime );
				}
			}
			else
			{
				m_aArcherData[i].m_pAnimArcher[eArcherState_Idle][eDir]->Update( fDeltaTime );
			}
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_ArcherTower::OnRenderState_Idle( IGUIInterfaceRender* pRender )
	{
		CTDGameTowerImplement_TowerBase::OnRenderState_Idle(pRender);

		//render tower
		m_pAnimTowerIdle->Draw(pRender, GetRenderRect(), 0.0f, GetRenderColor(), 1.0f );

		//render archers
		for( uint32 i=0; i<2; ++i )
		{
			CGUIAnimation* pArcherAnim = m_aArcherData[i].m_pAnimArcher[m_aArcherData[i].m_eArcherState][m_aArcherData[i].m_eArcherDir];
			pArcherAnim->Draw( pRender, m_aArcherData[i].m_rectArcherRender, 0, GetRenderColor(), 1 );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_ArcherTower::OnLeaveState_Idle()
	{
		CTDGameTowerImplement_TowerBase::OnLeaveState_Idle();
		
		m_pAnimTowerIdle = NULL;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Mages
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Mages::CTDGameTowerImplement_Mages( CTDGameTower* pGameTower )
		:CTDGameTowerImplement_TowerBase( pGameTower, "tower_mages")
	{

	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Mages::OnEnterState_Idle()
	{
		CTDGameTowerImplement_TowerBase::OnEnterState_Idle();

		//set mage data
		m_aMageData.m_pAnimMage[eMageState_Idle][eMageDir_Up] = GetAnimation("AnimMage_Idle_Up");
		m_aMageData.m_pAnimMage[eMageState_Idle][eMageDir_Down] = GetAnimation("AnimMage_Idle_Down");
		m_aMageData.m_pAnimMage[eMageState_Attack][eMageDir_Up] = GetAnimation("AnimMage_Attack_Up");
		m_aMageData.m_pAnimMage[eMageState_Attack][eMageDir_Down] = GetAnimation("AnimMage_Attack_Down");
		m_aMageData.m_eMageDir = eMageDir_Down;
		m_aMageData.m_eMageState = eMageState_Idle;

		//set tower data
		m_aTowerData.m_eTowerState = eTowerState_Idle;

		STowerLevelInfo& rInfo = m_vecLevelInfos[m_uLevel];
		CGUIVector2 pos = rInfo.m_pLevelProperty->GetProperty("mage_pos")->GetSpecifiedValue<CGUIVector2>();
		const CGUISize& size = m_aMageData.m_pAnimMage[eMageState_Idle][eMageDir_Up]->GetSize();	
		m_aMageData.m_rectMageRender = CGUIRect( CGUIVector2( pos.x-size.m_fWidth/2, pos.y-size.m_fHeight/2 ), size );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Mages::OnUpdateState_Idle(real fDeltaTime)
	{
		CTDGameTowerImplement_TowerBase::OnUpdateState_Idle(fDeltaTime);

		//update mage state
		EMageDir eDir = m_aMageData.m_eMageDir;
		if( m_aMageData.m_eMageState == eMageState_Attack )
		{
			real fLeftTime = m_aMageData.m_pAnimMage[eMageState_Attack][eDir]->Update( fDeltaTime );
			if( fLeftTime > 0.0f )
			{
				//finished
				m_aMageData.m_eMageState = eMageState_Idle;
				m_aMageData.m_pAnimMage[m_aMageData.m_eMageState][eDir]->Reset();
				m_aMageData.m_pAnimMage[m_aMageData.m_eMageState][eDir]->Update( fLeftTime );
			}
		}
		else
		{
			m_aMageData.m_pAnimMage[eMageState_Idle][eDir]->Update( fDeltaTime );
		}

		//update tower
		if( m_aTowerData.m_eTowerState == eTowerState_Attack )
		{
			real fLeftTime = m_aTowerData.m_pAnimTower[eTowerState_Attack]->Update( fDeltaTime );
			if( fLeftTime > 0.0f )
			{
				//finished
				m_aTowerData.m_eTowerState = eTowerState_Idle;
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Reset();
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Update( fLeftTime );
			}
		}
		else
		{
			m_aTowerData.m_pAnimTower[eTowerState_Idle]->Update( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Mages::OnRenderState_Idle(IGUIInterfaceRender* pRender)
	{
		CTDGameTowerImplement_TowerBase::OnRenderState_Idle(pRender);

		//render tower
		m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Draw( pRender, GetRenderRect(), 0, GetRenderColor(), 1.0f );

		//render mage
		CGUIAnimation* pMageAnim = m_aMageData.m_pAnimMage[m_aMageData.m_eMageState][m_aMageData.m_eMageDir];
		pMageAnim->Draw( pRender, m_aMageData.m_rectMageRender, 0, GetRenderColor(), 1.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Mages::OnLeaveState_Idle()
	{
		CTDGameTowerImplement_TowerBase::OnLeaveState_Idle( );
		
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Mages::UpdateTowerDataByLevel( )
	{
		CTDGameTowerImplement_TowerBase::UpdateTowerDataByLevel( );

		//set tower data
		STowerLevelInfo& rInfo = m_vecLevelInfos[m_uLevel];
		m_aTowerData.m_pAnimTower[eTowerState_Idle] = rInfo.m_mapAnimations["AnimTower_Idle"];
		m_aTowerData.m_pAnimTower[eTowerState_Attack] = rInfo.m_mapAnimations["AnimTower_Attack"];

		GetGameTower()->SetSize( m_aTowerData.m_pAnimTower[eTowerState_Idle]->GetSize() );
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Barracks
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Barracks::CTDGameTowerImplement_Barracks( CTDGameTower* pGameTower )
		:CTDGameTowerImplement_TowerBase( pGameTower, "tower_barracks")
	{

	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Barracks::OnEnterState_Idle()
	{
		CTDGameTowerImplement_TowerBase::OnEnterState_Idle( );

		//set tower data
		m_aTowerData.m_eTowerState = eTowerState_Idle;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Barracks::OnUpdateState_Idle(real fDeltaTime)
	{
		CTDGameTowerImplement_TowerBase::OnUpdateState_Idle( fDeltaTime );

		//update tower
		if( m_aTowerData.m_eTowerState == eTowerState_Open )
		{
			real fLeftTime = m_aTowerData.m_pAnimTower[eTowerState_Open]->Update( fDeltaTime );
			if( fLeftTime > 0.0f )
			{
				//finished
				m_aTowerData.m_eTowerState = eTowerState_Close;
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Reset();
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Update( fLeftTime );
			}
		}
		else if( m_aTowerData.m_eTowerState == eTowerState_Close )
		{
			real fLeftTime = m_aTowerData.m_pAnimTower[eTowerState_Close]->Update( fDeltaTime );
			if( fLeftTime > 0.0f )
			{
				//finished
				m_aTowerData.m_eTowerState = eTowerState_Idle;
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Reset();
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Update( fLeftTime );
			}
		}
		else
		{
			m_aTowerData.m_pAnimTower[eTowerState_Idle]->Update( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Barracks::OnRenderState_Idle(IGUIInterfaceRender* pRender)
	{
		CTDGameTowerImplement_TowerBase::OnRenderState_Idle( pRender );

		//render tower
		m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Draw( pRender, GetRenderRect(), 0, GetRenderColor(), 1.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Barracks::OnLeaveState_Idle()
	{
		CTDGameTowerImplement_TowerBase::OnLeaveState_Idle( );

	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Barracks::UpdateTowerDataByLevel( )
	{
		CTDGameTowerImplement_TowerBase::UpdateTowerDataByLevel( );

		//set tower data
		STowerLevelInfo& rInfo = m_vecLevelInfos[m_uLevel];
		m_aTowerData.m_pAnimTower[eTowerState_Idle] = rInfo.m_mapAnimations["AnimTower_Idle"];
		m_aTowerData.m_pAnimTower[eTowerState_Open] = rInfo.m_mapAnimations["AnimTower_Open"];
		m_aTowerData.m_pAnimTower[eTowerState_Close] = rInfo.m_mapAnimations["AnimTower_Close"];

		GetGameTower()->SetSize( m_aTowerData.m_pAnimTower[eTowerState_Idle]->GetSize() );
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Bombard
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Bombard::CTDGameTowerImplement_Bombard( CTDGameTower* pGameTower )
		:CTDGameTowerImplement_TowerBase( pGameTower, "tower_bombard")
	{

	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Bombard::OnEnterState_Idle()
	{
		CTDGameTowerImplement_TowerBase::OnEnterState_Idle( );

		//set tower data
		m_aTowerData.m_eTowerState = eTowerState_Idle;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Bombard::OnUpdateState_Idle(real fDeltaTime)
	{
		CTDGameTowerImplement_TowerBase::OnUpdateState_Idle( fDeltaTime );

		//update tower
		if( m_aTowerData.m_eTowerState == eTowerState_Attack )
		{
			real fLeftTime = m_aTowerData.m_pAnimTower[eTowerState_Attack]->Update( fDeltaTime );
			if( fLeftTime > 0.0f )
			{
				//finished
				m_aTowerData.m_eTowerState = eTowerState_Idle;
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Reset();
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Update( fLeftTime );
			}
		}
		else
		{
			m_aTowerData.m_pAnimTower[eTowerState_Idle]->Update( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Bombard::OnRenderState_Idle(IGUIInterfaceRender* pRender)
	{
		CTDGameTowerImplement_TowerBase::OnRenderState_Idle( pRender );

		//render tower
		m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Draw( pRender, GetRenderRect(), 0, GetRenderColor(), 1.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Bombard::OnLeaveState_Idle()
	{
		CTDGameTowerImplement_TowerBase::OnLeaveState_Idle( );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Bombard::UpdateTowerDataByLevel( )
	{
		CTDGameTowerImplement_TowerBase::UpdateTowerDataByLevel( );

		//set tower data
		STowerLevelInfo& rInfo = m_vecLevelInfos[m_uLevel];
		m_aTowerData.m_pAnimTower[eTowerState_Idle] = rInfo.m_mapAnimations["AnimTower_Idle"];
		m_aTowerData.m_pAnimTower[eTowerState_Attack] = rInfo.m_mapAnimations["AnimTower_Attack"];

		GetGameTower()->SetSize( m_aTowerData.m_pAnimTower[eTowerState_Idle]->GetSize() );
	}
	//------------------------------------------------------------------------------
}
