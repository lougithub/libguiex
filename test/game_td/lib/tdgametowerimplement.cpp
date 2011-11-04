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
		for( uint32 i=0; i<pTowerProp->GetPropertyCount(); ++i )
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
	// CTDGameTowerImplement_Basement
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Basement::CTDGameTowerImplement_Basement( CTDGameTower* pGameTower )
		:CTDGameTowerImplement( pGameTower, "tower_basement")
		,m_pAnimTowerBase(NULL)
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Basement::OnBuild()
	{
		m_pAnimTowerBase = GetAnimation( "AnimIdle" );
		GetGameTower()->SetSize( m_pAnimTowerBase->GetSize());
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Basement::OnUpdate( real fDeltaTime )
	{
		CTDGameTowerImplement::OnUpdate( fDeltaTime );

		m_pAnimTowerBase->Update( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Basement::OnRender( IGUIInterfaceRender* pRender )
	{
		CTDGameTowerImplement::OnRender( pRender );

		m_pAnimTowerBase->Draw( pRender, GetRenderRect(), 0.0f, GetRenderColor(), 1.0f );
	}
	//------------------------------------------------------------------------------




	//------------------------------------------------------------------------------
	// CTDGameTowerImplementBase
	//------------------------------------------------------------------------------
	CTDGameTowerImplementBase::CTDGameTowerImplementBase( CTDGameTower* pGameTower, const CGUIString& rTowerType )
		:CTDGameTowerImplement( pGameTower, rTowerType)
		,m_eTowerState(eTowerState_Unknown)
		,m_fBuildingTimeElapsed(0.0f)
	{
		const CGUIProperty* pAllTowerProperty = GetGameWorld()->GetDataProperty("tower.xml");
		const CGUIProperty* pTowerProp = pAllTowerProperty->GetProperty(GetTowerType());

		m_fBuildingTime = pTowerProp->GetProperty("building_time")->GetCommonValue<real>();

		char szLevelName[10];
		uint32 nLevel = 1;
		snprintf( szLevelName, 10, "level%03d", nLevel );
		const CGUIProperty* pLevelProp = pTowerProp->GetProperty( szLevelName );
		while( pLevelProp )
		{
			m_arrayLevelInfoCommon.push_back( STowerLevelInfoCommon() );
			STowerLevelInfoCommon& rInfo = m_arrayLevelInfoCommon.back();
			rInfo.m_uPrice = pLevelProp->GetProperty("price")->GetCommonValue<uint32>();
			rInfo.m_uSellPrice = pLevelProp->GetProperty("sell_price")->GetCommonValue<uint32>();
			rInfo.m_fRadius = pLevelProp->GetProperty("radius")->GetCommonValue<real>();
			rInfo.m_pLevelProperty = pLevelProp;

			for( uint32 i=0; i<pLevelProp->GetPropertyCount(); ++i )
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
	CTDGameTowerImplementBase::~CTDGameTowerImplementBase()
	{
		for( TArrayLevelInfoCommon::iterator itorLevel = m_arrayLevelInfoCommon.begin();
			itorLevel != m_arrayLevelInfoCommon.end();
			++itorLevel )
		{
			STowerLevelInfoCommon& rInfo = *itorLevel;

			for( std::map<CGUIString, CGUIAnimation*>::iterator itor = rInfo.m_mapAnimations.begin();
				itor != rInfo.m_mapAnimations.end();
				++itor )
			{
				itor->second->RefRelease();
			}
			rInfo.m_mapAnimations.clear();
		}
		m_arrayLevelInfoCommon.clear();
	}
	//------------------------------------------------------------------------------
	uint32 CTDGameTowerImplementBase::GetBuildCost() const
	{
		if( GetTotalLevel() == 0 )
		{
			GUI_THROW( "[CTDGameTowerImplementBase::GetBuildCost]: invalid tower level");
			return 0;
		}
		return m_arrayLevelInfoCommon[0].m_uPrice;
	}
	//------------------------------------------------------------------------------
	uint32 CTDGameTowerImplementBase::GetUpgradeCost() const
	{
		if( GetTotalLevel() == 0 || m_uLevel >= GetTotalLevel()-1 )
		{
			GUI_THROW( "[CTDGameTowerImplementBase::GetUpgradeCost]: invalid tower level");
			return 0;
		}
		return m_arrayLevelInfoCommon[m_uLevel+1].m_uPrice;
	}
	//------------------------------------------------------------------------------
	uint32 CTDGameTowerImplementBase::GetSellCost() const
	{
		if( GetTotalLevel() == 0 || m_uLevel >= GetTotalLevel() )
		{
			GUI_THROW( "[CTDGameTowerImplementBase::GetSellCost]: invalid tower level");
			return 0;
		}
		return m_arrayLevelInfoCommon[m_uLevel].m_uSellPrice;
	}
	//------------------------------------------------------------------------------
	real CTDGameTowerImplementBase::GetRadius() const
	{
		return m_arrayLevelInfoCommon[m_uLevel].m_fRadius;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::Upgrade()
	{
		if( GetTotalLevel() == 0 || m_uLevel >= GetTotalLevel()-1 )
		{
			GUI_THROW( "[CTDGameTowerImplementBase::Upgrade]: invalid tower level");
			return;
		}
		m_uLevel++;
		UpdateTowerDataByLevel();
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::SetTowerState( ETowerState eState )
	{
		if( m_eTowerState != eState )
		{
			switch( m_eTowerState )
			{
			case eTowerState_Construct:
				OnLeaveState_Construct();
				break;
			case eTowerState_Working:
				OnLeaveState_Working();
				break;
			}

			m_eTowerState = eState;
			switch( m_eTowerState )
			{
			case eTowerState_Construct:
				OnEnterState_Construct();
				break;
			case eTowerState_Working:
				OnEnterState_Working();
				break;
			}
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnBuild()
	{
		CTDGameTowerImplement::OnBuild();

		//set tower state
		SetTowerState( eTowerState_Construct );

		m_uLevel = 0;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnDestruct()
	{
		SetTowerState( eTowerState_Unknown );
		m_uLevel = 0;

		CTDGameTowerImplement::OnDestruct();
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::UpdateTowerDataByLevel( )
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnEnterState_Construct()
	{
		m_pAnimBuilding = GetAnimation("AnimTower_Building");
		GetGameTower()->SetSize( m_pAnimBuilding->GetSize());

		m_fBuildingTimeElapsed = 0.0f;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnLeaveState_Construct()
	{
		m_pAnimBuilding = NULL;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnEnterState_Working()
	{
		//set tower data
		UpdateTowerDataByLevel();
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnLeaveState_Working()
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnUpdateState_Construct(real fDeltaTime)
	{
		m_pAnimBuilding->Update( fDeltaTime );

		m_fBuildingTimeElapsed += fDeltaTime;
		if( m_fBuildingTimeElapsed >= m_fBuildingTime )
		{
			SetTowerState( eTowerState_Working );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnRenderState_Construct(IGUIInterfaceRender* pRender)
	{
		m_pAnimBuilding->Draw( pRender, GetRenderRect(), 0.0f, GetRenderColor(), 1.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnRenderState_Working(IGUIInterfaceRender* pRender)
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnUpdateState_Working(real fDeltaTime)
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnUpdate( real fDeltaTime )
	{
		switch( m_eTowerState )
		{
		case eTowerState_Construct:
			OnUpdateState_Construct( fDeltaTime );
			break;
		case eTowerState_Working:
			OnUpdateState_Working( fDeltaTime );
			break;
		}

		CTDGameTowerImplement::OnUpdate( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementBase::OnRender( IGUIInterfaceRender* pRender )
	{
		CTDGameTowerImplement::OnRender( pRender );

		switch( m_eTowerState )
		{
		case eTowerState_Construct:
			OnRenderState_Construct( pRender );
			break;
		case eTowerState_Working:
			OnRenderState_Working( pRender );
			break;
		}

		if( GetGameTower()->IsMouseOver() )
		{
			pRender->DrawCircle( CGUIVector2(0,0), GetRadius(), 2, false, 0, CGUIColor(1,0,0,1));
		}
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CTDGameTowerImplementAttackable
	//------------------------------------------------------------------------------
	CTDGameTowerImplementAttackable::CTDGameTowerImplementAttackable( CTDGameTower* pGameTower, const CGUIString& rTowerType )
		:CTDGameTowerImplementBase( pGameTower, rTowerType)
	{
		const CGUIProperty* pAllTowerProperty = GetGameWorld()->GetDataProperty("tower.xml");
		const CGUIProperty* pTowerProp = pAllTowerProperty->GetProperty(GetTowerType());

		char szLevelName[10];
		uint32 nLevel = 1;
		snprintf( szLevelName, 10, "level%03d", nLevel );
		const CGUIProperty* pLevelProp = pTowerProp->GetProperty( szLevelName );
		while( pLevelProp )
		{
			m_arrayLevelInfoAttackable.push_back( STowerLevelInfoAttackable() );
			STowerLevelInfoAttackable& rInfo = m_arrayLevelInfoAttackable.back();

			rInfo.m_fReloadTime = pLevelProp->GetProperty("reload")->GetCommonValue<real>();
			rInfo.m_fDamage = pLevelProp->GetProperty("damage")->GetCommonValue<real>();
			rInfo.m_fDamageRange = pLevelProp->GetProperty("damagerange")->GetCommonValue<real>();
			rInfo.m_eDamageType = String2DamageType(pLevelProp->GetProperty("damagetype")->GetValue());

			++nLevel;
			snprintf( szLevelName, 10, "level%03d", nLevel );
			pLevelProp = pTowerProp->GetProperty( szLevelName );
		}
	}
	//------------------------------------------------------------------------------
	CTDGameTowerImplementAttackable::~CTDGameTowerImplementAttackable()
	{
		m_arrayLevelInfoAttackable.clear();
	}
	//------------------------------------------------------------------------------
	EBulletDamageType CTDGameTowerImplementAttackable::String2DamageType( const CGUIString& rType ) const
	{
		if( rType == "physical" )
		{
			return eBulletDamageType_Physical;
		}
		else if( rType == "magical" )
		{
			return eBulletDamageType_Magical;
		}
		else
		{
			GUI_THROW( GUI_FORMAT("[CTDGameTowerImplementAttackable::String2DamageType]: unknown damate type: %s", rType.c_str( )));
			return eBulletDamageType_Physical;
		}
	}
	//------------------------------------------------------------------------------
	real CTDGameTowerImplementAttackable::GetReloadTime() const
	{
		return m_arrayLevelInfoAttackable[m_uLevel].m_fReloadTime;
	}
	//------------------------------------------------------------------------------
	real CTDGameTowerImplementAttackable::GetDamage() const
	{
		return m_arrayLevelInfoAttackable[m_uLevel].m_fDamage;
	}
	//------------------------------------------------------------------------------
	real CTDGameTowerImplementAttackable::GetDamageRange() const
	{
		return m_arrayLevelInfoAttackable[m_uLevel].m_fDamageRange;
	}
	//------------------------------------------------------------------------------
	EBulletDamageType CTDGameTowerImplementAttackable::GetDamageType() const
	{
		return m_arrayLevelInfoAttackable[m_uLevel].m_eDamageType;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementAttackable::OnEnterState_Working()
	{
		CTDGameTowerImplementBase::OnEnterState_Working();

		m_fAttackWaitTime = GetReloadTime();
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementAttackable::OnUpdateState_Working(real fDeltaTime)
	{
		if( m_fAttackWaitTime < GetReloadTime() )
		{
			m_fAttackWaitTime += fDeltaTime;
		}

		if( m_fAttackWaitTime >= GetReloadTime() )
		{
			CTDGameObjectMonster* pMonster = GetGameWorld()->FindMonster( GetPosition(), GetRadius() );
			if( pMonster )
			{
				Attack( pMonster );
				m_fAttackWaitTime = 0.0f;
			}
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplementAttackable::Attack( CTDGameObjectMonster* pMonster )
	{
		GetGameWorld()->AllocateBullet( "bomb1", GetPosition(), pMonster, GetDamageType(), GetDamage(), GetDamageRange());
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_ArcherTower
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_ArcherTower::CTDGameTowerImplement_ArcherTower( CTDGameTower* pGameTower )
		:CTDGameTowerImplementAttackable( pGameTower, "tower_archertower")
		,m_pAnimTowerIdle(NULL)
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_ArcherTower::UpdateTowerDataByLevel( )
	{
		CTDGameTowerImplementAttackable::UpdateTowerDataByLevel();

		//update archer data
		STowerLevelInfoCommon& rInfo = m_arrayLevelInfoCommon[m_uLevel];
		for( uint32 i=0; i<2; ++i )
		{
			m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Idle][eArcherGuardDir_UpRight] = rInfo.m_mapAnimations["AnimArcher_Idle_UpRight"];
			m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Idle][eArcherGuardDir_UpLeft] = rInfo.m_mapAnimations["AnimArcher_Idle_UpLeft"];
			m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Idle][eArcherGuardDir_DownRight] = rInfo.m_mapAnimations["AnimArcher_Idle_DownRight"];
			m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Idle][eArcherGuardDir_DownLeft] = rInfo.m_mapAnimations["AnimArcher_Idle_DownLeft"];
			m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Attack][eArcherGuardDir_UpRight] = rInfo.m_mapAnimations["AnimArcher_Attack_UpRight"];
			m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Attack][eArcherGuardDir_UpLeft] = rInfo.m_mapAnimations["AnimArcher_Attack_UpLeft"];
			m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Attack][eArcherGuardDir_DownRight] = rInfo.m_mapAnimations["AnimArcher_Attack_DownRight"];
			m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Attack][eArcherGuardDir_DownLeft] = rInfo.m_mapAnimations["AnimArcher_Attack_DownLeft"];

			m_aArcherData[i].m_eArcherState = eArcherGuardState_Idle;
			m_aArcherData[i].m_eArcherDir = eArcherGuardDir_DownRight;

			char posName[16];
			snprintf( posName, 16, "archer_pos_%d", i );
			CGUIVector2 pos = rInfo.m_pLevelProperty->GetProperty(posName)->GetCommonValue<CGUIVector2>();
			const CGUISize& size = m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Idle][eArcherGuardDir_UpRight]->GetSize();
			m_aArcherData[i].m_rectArcherRender = CGUIRect( CGUIVector2( pos.x-size.m_fWidth/2, pos.y-size.m_fHeight/2 ), size );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_ArcherTower::OnEnterState_Working()
	{
		CTDGameTowerImplementAttackable::OnEnterState_Working();

		//update tower data
		m_pAnimTowerIdle = m_arrayLevelInfoCommon[m_uLevel].m_mapAnimations["AnimTower_Idle"];
		GetGameTower()->SetSize( m_pAnimTowerIdle->GetSize());
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_ArcherTower::OnUpdateState_Working(real fDeltaTime)
	{
		CTDGameTowerImplementAttackable::OnUpdateState_Working(fDeltaTime);

		//update tower
		m_pAnimTowerIdle->Update( fDeltaTime );

		//update archer state
		for( uint32 i=0; i<2; ++i )
		{
			EArcherGuardDir eDir = m_aArcherData[i].m_eArcherDir;
			if( m_aArcherData[i].m_eArcherState == eArcherGuardState_Attack )
			{
				real fLeftTime = m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Attack][eDir]->Update( fDeltaTime );
				if( fLeftTime > 0.0f )
				{
					//finished
					m_aArcherData[i].m_eArcherState = eArcherGuardState_Idle;
					m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Idle][eDir]->Reset();
					m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Idle][eDir]->Update( fLeftTime );
				}
			}
			else
			{
				m_aArcherData[i].m_pAnimArcher[eArcherGuardState_Idle][eDir]->Update( fDeltaTime );
			}
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_ArcherTower::OnRenderState_Working( IGUIInterfaceRender* pRender )
	{
		CTDGameTowerImplementAttackable::OnRenderState_Working(pRender);

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
	void CTDGameTowerImplement_ArcherTower::OnLeaveState_Working()
	{
		CTDGameTowerImplementAttackable::OnLeaveState_Working();
		
		m_pAnimTowerIdle = NULL;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Mages
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Mages::CTDGameTowerImplement_Mages( CTDGameTower* pGameTower )
		:CTDGameTowerImplementAttackable( pGameTower, "tower_mages")
	{

	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Mages::OnEnterState_Working()
	{
		CTDGameTowerImplementAttackable::OnEnterState_Working();

		//set mage data
		m_aMageData.m_pAnimMage[eMageGuardState_Idle][eMageGuardDir_Up] = GetAnimation("AnimMage_Idle_Up");
		m_aMageData.m_pAnimMage[eMageGuardState_Idle][eMageGuardDir_Down] = GetAnimation("AnimMage_Idle_Down");
		m_aMageData.m_pAnimMage[eMageGuardState_Attack][eMageGuardDir_Up] = GetAnimation("AnimMage_Attack_Up");
		m_aMageData.m_pAnimMage[eMageGuardState_Attack][eMageGuardDir_Down] = GetAnimation("AnimMage_Attack_Down");
		m_aMageData.m_eMageDir = eMageGuardDir_Down;
		m_aMageData.m_eMageState = eMageGuardState_Idle;

		//set tower data
		m_aTowerData.m_eTowerState = eMageTowerState_Idle;

		STowerLevelInfoCommon& rInfo = m_arrayLevelInfoCommon[m_uLevel];
		CGUIVector2 pos = rInfo.m_pLevelProperty->GetProperty("mage_pos")->GetCommonValue<CGUIVector2>();
		const CGUISize& size = m_aMageData.m_pAnimMage[eMageGuardState_Idle][eMageGuardDir_Up]->GetSize();	
		m_aMageData.m_rectMageRender = CGUIRect( CGUIVector2( pos.x-size.m_fWidth/2, pos.y-size.m_fHeight/2 ), size );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Mages::OnUpdateState_Working(real fDeltaTime)
	{
		CTDGameTowerImplementAttackable::OnUpdateState_Working(fDeltaTime);

		//update mage state
		EMageGuardDir eDir = m_aMageData.m_eMageDir;
		if( m_aMageData.m_eMageState == eMageGuardState_Attack )
		{
			real fLeftTime = m_aMageData.m_pAnimMage[eMageGuardState_Attack][eDir]->Update( fDeltaTime );
			if( fLeftTime > 0.0f )
			{
				//finished
				m_aMageData.m_eMageState = eMageGuardState_Idle;
				m_aMageData.m_pAnimMage[m_aMageData.m_eMageState][eDir]->Reset();
				m_aMageData.m_pAnimMage[m_aMageData.m_eMageState][eDir]->Update( fLeftTime );
			}
		}
		else
		{
			m_aMageData.m_pAnimMage[eMageGuardState_Idle][eDir]->Update( fDeltaTime );
		}

		//update tower
		if( m_aTowerData.m_eTowerState == eMageTowerState_Attack )
		{
			real fLeftTime = m_aTowerData.m_pAnimTower[eMageTowerState_Attack]->Update( fDeltaTime );
			if( fLeftTime > 0.0f )
			{
				//finished
				m_aTowerData.m_eTowerState = eMageTowerState_Idle;
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Reset();
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Update( fLeftTime );
			}
		}
		else
		{
			m_aTowerData.m_pAnimTower[eMageTowerState_Idle]->Update( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Mages::OnRenderState_Working(IGUIInterfaceRender* pRender)
	{
		CTDGameTowerImplementAttackable::OnRenderState_Working(pRender);

		//render tower
		m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Draw( pRender, GetRenderRect(), 0, GetRenderColor(), 1.0f );

		//render mage
		CGUIAnimation* pMageAnim = m_aMageData.m_pAnimMage[m_aMageData.m_eMageState][m_aMageData.m_eMageDir];
		pMageAnim->Draw( pRender, m_aMageData.m_rectMageRender, 0, GetRenderColor(), 1.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Mages::OnLeaveState_Working()
	{
		CTDGameTowerImplementAttackable::OnLeaveState_Working( );
		
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Mages::UpdateTowerDataByLevel( )
	{
		CTDGameTowerImplementAttackable::UpdateTowerDataByLevel( );

		//set tower data
		STowerLevelInfoCommon& rInfo = m_arrayLevelInfoCommon[m_uLevel];
		m_aTowerData.m_pAnimTower[eMageTowerState_Idle] = rInfo.m_mapAnimations["AnimTower_Idle"];
		m_aTowerData.m_pAnimTower[eMageTowerState_Attack] = rInfo.m_mapAnimations["AnimTower_Attack"];

		GetGameTower()->SetSize( m_aTowerData.m_pAnimTower[eMageTowerState_Idle]->GetSize() );
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Bombard
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Bombard::CTDGameTowerImplement_Bombard( CTDGameTower* pGameTower )
		:CTDGameTowerImplementAttackable( pGameTower, "tower_bombard")
	{

	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Bombard::OnEnterState_Working()
	{
		CTDGameTowerImplementAttackable::OnEnterState_Working( );

		//set tower data
		m_aTowerData.m_eTowerState = eBombardTowerState_Idle;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Bombard::OnUpdateState_Working(real fDeltaTime)
	{
		CTDGameTowerImplementAttackable::OnUpdateState_Working( fDeltaTime );

		//update tower
		if( m_aTowerData.m_eTowerState == eBombardTowerState_Attack )
		{
			real fLeftTime = m_aTowerData.m_pAnimTower[eBombardTowerState_Attack]->Update( fDeltaTime );
			if( fLeftTime > 0.0f )
			{
				//finished
				m_aTowerData.m_eTowerState = eBombardTowerState_Idle;
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Reset();
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Update( fLeftTime );
			}
		}
		else
		{
			m_aTowerData.m_pAnimTower[eBombardTowerState_Idle]->Update( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Bombard::OnRenderState_Working(IGUIInterfaceRender* pRender)
	{
		CTDGameTowerImplementAttackable::OnRenderState_Working( pRender );

		//render tower
		m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Draw( pRender, GetRenderRect(), 0, GetRenderColor(), 1.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Bombard::OnLeaveState_Working()
	{
		CTDGameTowerImplementAttackable::OnLeaveState_Working( );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Bombard::UpdateTowerDataByLevel( )
	{
		CTDGameTowerImplementAttackable::UpdateTowerDataByLevel( );

		//set tower data
		STowerLevelInfoCommon& rInfo = m_arrayLevelInfoCommon[m_uLevel];
		m_aTowerData.m_pAnimTower[eBombardTowerState_Idle] = rInfo.m_mapAnimations["AnimTower_Idle"];
		m_aTowerData.m_pAnimTower[eBombardTowerState_Attack] = rInfo.m_mapAnimations["AnimTower_Attack"];

		GetGameTower()->SetSize( m_aTowerData.m_pAnimTower[eBombardTowerState_Idle]->GetSize() );
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Barracks
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Barracks::CTDGameTowerImplement_Barracks( CTDGameTower* pGameTower )
		:CTDGameTowerImplementBase( pGameTower, "tower_barracks")
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Barracks::OnEnterState_Working()
	{
		CTDGameTowerImplementBase::OnEnterState_Working( );

		//set tower data
		m_aTowerData.m_eTowerState = eBarrackTowerState_Idle;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Barracks::OnUpdateState_Working(real fDeltaTime)
	{
		CTDGameTowerImplementBase::OnUpdateState_Working( fDeltaTime );

		//update tower
		if( m_aTowerData.m_eTowerState == eBarrackTowerState_Open )
		{
			real fLeftTime = m_aTowerData.m_pAnimTower[eBarrackTowerState_Open]->Update( fDeltaTime );
			if( fLeftTime > 0.0f )
			{
				//finished
				m_aTowerData.m_eTowerState = eBarrackTowerState_Close;
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Reset();
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Update( fLeftTime );
			}
		}
		else if( m_aTowerData.m_eTowerState == eBarrackTowerState_Close )
		{
			real fLeftTime = m_aTowerData.m_pAnimTower[eBarrackTowerState_Close]->Update( fDeltaTime );
			if( fLeftTime > 0.0f )
			{
				//finished
				m_aTowerData.m_eTowerState = eBarrackTowerState_Idle;
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Reset();
				m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Update( fLeftTime );
			}
		}
		else
		{
			m_aTowerData.m_pAnimTower[eBarrackTowerState_Idle]->Update( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Barracks::OnRenderState_Working(IGUIInterfaceRender* pRender)
	{
		CTDGameTowerImplementBase::OnRenderState_Working( pRender );

		//render tower
		m_aTowerData.m_pAnimTower[m_aTowerData.m_eTowerState]->Draw( pRender, GetRenderRect(), 0, GetRenderColor(), 1.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Barracks::OnLeaveState_Working()
	{
		CTDGameTowerImplementBase::OnLeaveState_Working( );

	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Barracks::UpdateTowerDataByLevel( )
	{
		CTDGameTowerImplementBase::UpdateTowerDataByLevel( );

		//set tower data
		STowerLevelInfoCommon& rInfo = m_arrayLevelInfoCommon[m_uLevel];
		m_aTowerData.m_pAnimTower[eBarrackTowerState_Idle] = rInfo.m_mapAnimations["AnimTower_Idle"];
		m_aTowerData.m_pAnimTower[eBarrackTowerState_Open] = rInfo.m_mapAnimations["AnimTower_Open"];
		m_aTowerData.m_pAnimTower[eBarrackTowerState_Close] = rInfo.m_mapAnimations["AnimTower_Close"];

		GetGameTower()->SetSize( m_aTowerData.m_pAnimTower[eBarrackTowerState_Idle]->GetSize() );
	}
	//------------------------------------------------------------------------------

}
