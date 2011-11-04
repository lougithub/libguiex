/** 
* @file tdgametowerimplement.h
* @brief tower of td game
* @author ken
* @date 2011-08-29
*/

#ifndef __TD_GAMETOWER_IMPLEMENT_20110829_H__
#define __TD_GAMETOWER_IMPLEMENT_20110829_H__

//============================================================================//
// include
//============================================================================// 
#include "tdgametower.h"
#include "tdgameworld.h"
#include "tdwgttower.h"
#include <libguiex_core/guiex.h>

enum ETowerState
{
	eTowerState_Unknown,
	eTowerState_Construct,
	eTowerState_Working,
};


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	// CTDGameTowerImplement
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement
	{
	public:
		CTDGameTowerImplement( CTDGameTower* pGameTower, const CGUIString& rTowerType );
		virtual ~CTDGameTowerImplement();

		virtual void OnUpdate( real fDeltaTime );
		virtual void OnRender( IGUIInterfaceRender* pRender );
		virtual void OnBuild(){}
		virtual void OnDestruct(){}

		const CGUIString& GetTowerType() const;

		CTDGameTower* GetGameTower() const { return m_pGameTower; }
		CTDGameWorld* GetGameWorld() const { return m_pGameTower->GetGameWorld(); }

		const CGUIRect& GetRenderRect() const {return m_pGameTower->GetRenderRect();}
		const CGUIColor& GetRenderColor() const {return m_pGameTower->GetRenderColor();}
		const CGUIVector2& GetPosition() const {return m_pGameTower->GetPosition();}

		virtual uint32 GetBuildCost() const{return 0;}
		virtual uint32 GetUpgradeCost() const{return 0;}
		virtual uint32 GetSellCost() const{return 0;}
		virtual real GetRadius() const{return 0.0f;}
		virtual void Upgrade(){}

		virtual uint32 GetTotalLevel( ) const { return 0;}
		virtual uint32 GetCurrentLevel( ) const { return m_uLevel;}

	protected:
		CGUIAnimation* GetAnimation( const CGUIString& rAnimName );

	protected:
		uint32 m_uLevel;

	private:
		CTDGameTower* m_pGameTower;
		CGUIString m_strTowerType;
		std::map<CGUIString, CGUIAnimation*> m_mapAnimations;
	};


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Basement
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Basement : public CTDGameTowerImplement
	{
	public:
		CTDGameTowerImplement_Basement( CTDGameTower* pGameTower );
		virtual ~CTDGameTowerImplement_Basement(){}

		virtual void OnUpdate( real fDeltaTime );
		virtual void OnRender( IGUIInterfaceRender* pRender );

		virtual void OnBuild();

	protected:
		CGUIAnimation* m_pAnimTowerBase;
	};

	//------------------------------------------------------------------------------
	// CTDGameTowerImplementBase
	//------------------------------------------------------------------------------
	class CTDGameTowerImplementBase : public CTDGameTowerImplement
	{
	public:
		struct STowerLevelInfoCommon
		{
			uint32 m_uPrice;
			uint32 m_uSellPrice;
			real m_fRadius;
			std::map<CGUIString, CGUIAnimation*> m_mapAnimations;
			const class CGUIProperty* m_pLevelProperty;
		};

	public:
		CTDGameTowerImplementBase( CTDGameTower* pGameTower, const CGUIString& rTowerType );
		virtual ~CTDGameTowerImplementBase();

		virtual void OnUpdate( real fDeltaTime );
		virtual void OnRender( IGUIInterfaceRender* pRender );

		virtual void OnBuild();
		virtual void OnDestruct();

		void SetTowerState( ETowerState eState );

		virtual uint32 GetBuildCost() const;
		virtual uint32 GetUpgradeCost() const;
		virtual uint32 GetSellCost() const;
		virtual real GetRadius() const;
		virtual void Upgrade();

		virtual uint32 GetTotalLevel( ) const {return m_arrayLevelInfoCommon.size();}

	protected:
		virtual void OnEnterState_Construct();
		virtual void OnUpdateState_Construct(real fDeltaTime);
		virtual void OnRenderState_Construct(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Construct();

		virtual void OnEnterState_Working();
		virtual void OnUpdateState_Working(real fDeltaTime);
		virtual void OnRenderState_Working(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Working();

		virtual void UpdateTowerDataByLevel( );

	protected:
		CGUIAnimation* m_pAnimBuilding;

		ETowerState m_eTowerState;

		//data for state construct
		real m_fBuildingTime;
		real m_fBuildingTimeElapsed;

		//common level info
		typedef std::vector<STowerLevelInfoCommon> TArrayLevelInfoCommon;
		TArrayLevelInfoCommon m_arrayLevelInfoCommon;
	};

	//------------------------------------------------------------------------------
	// CTDGameTowerImplementAttackable
	//------------------------------------------------------------------------------
	class CTDGameTowerImplementAttackable : public CTDGameTowerImplementBase
	{
	public:
		struct STowerLevelInfoAttackable
		{
			real m_fReloadTime;
			real m_fDamage;
			real m_fDamageRange;
			EBulletDamageType m_eDamageType;
		};

	public:
		CTDGameTowerImplementAttackable( CTDGameTower* pGameTower, const CGUIString& rTowerType );
		virtual ~CTDGameTowerImplementAttackable();

	protected:
		virtual void OnEnterState_Working();
		virtual void OnUpdateState_Working(real fDeltaTime);

		virtual void Attack( class CTDGameObjectMonster* pMonster );

		real GetReloadTime() const;
		real GetDamage() const;
		real GetDamageRange() const;
		EBulletDamageType GetDamageType() const;

		EBulletDamageType String2DamageType( const CGUIString& rType ) const;

	protected:
		real m_fAttackWaitTime;

		typedef std::vector<STowerLevelInfoAttackable> TArrayLevelInfoAttackable;
		TArrayLevelInfoAttackable m_arrayLevelInfoAttackable;
	};

	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_ArcherTower
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_ArcherTower : public CTDGameTowerImplementAttackable
	{
	public:
		enum EArcherGuardDir
		{
			eArcherGuardDir_UpRight = 0,
			eArcherGuardDir_UpLeft,
			eArcherGuardDir_DownRight,
			eArcherGuardDir_DownLeft,

			__eArcherGuardDir_MAX__
		};

		enum EArcherGuardState
		{
			eArcherGuardState_Idle = 0,
			eArcherGuardState_Attack,

			__eArcherGuardState_MAX__
		};

	public:
		CTDGameTowerImplement_ArcherTower( CTDGameTower* pGameTower );

	protected:
		virtual void OnEnterState_Working();
		virtual void OnUpdateState_Working(real fDeltaTime);
		virtual void OnRenderState_Working(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Working();

		virtual void UpdateTowerDataByLevel( );

	protected:
		CGUIAnimation* m_pAnimTowerIdle;

		struct SArcherData
		{
			CGUIAnimation* m_pAnimArcher[__eArcherGuardState_MAX__][__eArcherGuardDir_MAX__];
			EArcherGuardState m_eArcherState;
			EArcherGuardDir m_eArcherDir;
			CGUIRect m_rectArcherRender;

			SArcherData()
				:m_eArcherState(eArcherGuardState_Idle)
				,m_eArcherDir(eArcherGuardDir_DownRight)
			{
				memset( m_pAnimArcher, 0, sizeof( m_pAnimArcher) );
			}
		};
		SArcherData m_aArcherData[2];
	};


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Mages
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Mages : public CTDGameTowerImplementAttackable
	{
	public:
		enum EMageGuardDir
		{
			eMageGuardDir_Up = 0,
			eMageGuardDir_Down,

			__eMageGuardDir_MAX__
		};

		enum EMageGuardState
		{
			eMageGuardState_Idle = 0,
			eMageGuardState_Attack,

			__eMageGuardState_MAX__
		};

		enum EMageTowerState
		{
			eMageTowerState_Idle = 0,
			eMageTowerState_Attack,

			__eMageTowerState_MAX__
		};

	public:
		CTDGameTowerImplement_Mages( CTDGameTower* pGameTower );

	protected:
		virtual void OnEnterState_Working();
		virtual void OnUpdateState_Working(real fDeltaTime);
		virtual void OnRenderState_Working(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Working();

		virtual void UpdateTowerDataByLevel( );

	protected:
		struct STowerData
		{
			CGUIAnimation* m_pAnimTower[__eMageTowerState_MAX__];
			EMageTowerState m_eTowerState;

			STowerData()
				:m_eTowerState(eMageTowerState_Idle)
			{
				memset( m_pAnimTower, 0, sizeof( m_pAnimTower ) );
			}
		};
		STowerData m_aTowerData;

		struct SMageData
		{
			CGUIAnimation* m_pAnimMage[__eMageGuardState_MAX__][__eMageGuardDir_MAX__];
			EMageGuardState m_eMageState;
			EMageGuardDir m_eMageDir;
			CGUIRect m_rectMageRender;

			SMageData()
				:m_eMageState(eMageGuardState_Idle)
				,m_eMageDir(eMageGuardDir_Down)
			{
				memset( m_pAnimMage, 0, sizeof( m_pAnimMage ) );
			}
		};
		SMageData m_aMageData;
	};


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Bombard
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Bombard : public CTDGameTowerImplementAttackable
	{
	public:
		enum EBombardTowerState
		{
			eBombardTowerState_Idle = 0,
			eBombardTowerState_Attack,

			__eBombardTowerState_MAX__
		};

	public:
		CTDGameTowerImplement_Bombard( CTDGameTower* pGameTower );

	protected:
		virtual void OnEnterState_Working();
		virtual void OnUpdateState_Working(real fDeltaTime);
		virtual void OnRenderState_Working(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Working();

		virtual void UpdateTowerDataByLevel( );

	protected:
		struct STowerData
		{
			CGUIAnimation* m_pAnimTower[__eBombardTowerState_MAX__];
			EBombardTowerState m_eTowerState;

			STowerData()
				:m_eTowerState(eBombardTowerState_Idle)
			{
				memset( m_pAnimTower, 0, sizeof( m_pAnimTower ) );
			}
		};
		STowerData m_aTowerData;
	};



	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Barracks
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Barracks : public CTDGameTowerImplementBase
	{
	public:
		enum EBarrackTowerState
		{
			eBarrackTowerState_Idle = 0,
			eBarrackTowerState_Open,
			eBarrackTowerState_Close,

			__eBarrackTowerState_MAX__
		};

	public:
		CTDGameTowerImplement_Barracks( CTDGameTower* pGameTower );

	protected:
		virtual void OnEnterState_Working();
		virtual void OnUpdateState_Working(real fDeltaTime);
		virtual void OnRenderState_Working(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Working();

		virtual void UpdateTowerDataByLevel( );

	protected:
		struct STowerData
		{
			CGUIAnimation* m_pAnimTower[__eBarrackTowerState_MAX__];
			EBarrackTowerState m_eTowerState;

			STowerData()
				:m_eTowerState(eBarrackTowerState_Idle)
			{
				memset( m_pAnimTower, 0, sizeof( m_pAnimTower ) );
			}
		};
		STowerData m_aTowerData;
	};


}

#endif //__TD_GAMETOWER_IMPLEMENT_20110829_H__

