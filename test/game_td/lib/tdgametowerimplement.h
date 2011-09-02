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
	eTowerState_Idle,
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
		struct STowerLevelInfo
		{
			uint32 m_uPrice;
			uint32 m_uSellPrice;
			real m_fReloadTime;
			std::map<CGUIString, CGUIAnimation*> m_mapAnimations;
			const class CGUIProperty* m_pLevelProperty;
		};

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

		virtual uint32 GetUpgradeCost() const{return 0;}
		virtual void Upgrade(){}

		virtual uint32 GetTotalLevel( ) const { return 0;}
		virtual uint32 GetCurrentLevel( ) const { return m_uLevel;}
		virtual const STowerLevelInfo* GetLevelInfo( uint32 uLevel) const {return 0;}

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
	// CTDGameTowerImplement_Base
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Base : public CTDGameTowerImplement
	{
	public:
		CTDGameTowerImplement_Base( CTDGameTower* pGameTower );
		virtual ~CTDGameTowerImplement_Base(){}

		virtual void OnUpdate( real fDeltaTime );
		virtual void OnRender( IGUIInterfaceRender* pRender );

		virtual void OnBuild();

	protected:
		CGUIAnimation* m_pAnimTowerBase;
	};


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_TowerBase
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_TowerBase : public CTDGameTowerImplement
	{
	public:
		CTDGameTowerImplement_TowerBase( CTDGameTower* pGameTower, const CGUIString& rTowerType );
		virtual ~CTDGameTowerImplement_TowerBase();

		virtual void OnUpdate( real fDeltaTime );
		virtual void OnRender( IGUIInterfaceRender* pRender );

		virtual void OnBuild();
		virtual void OnDestruct();

		void SetTowerState( ETowerState eState );

		virtual uint32 GetTotalLevel( ) const {return m_vecLevelInfos.size();}
		virtual const STowerLevelInfo* GetLevelInfo( uint32 uLevel) const {return &m_vecLevelInfos[uLevel];}
		virtual uint32 GetUpgradeCost() const;
		virtual void Upgrade();

	protected:
		virtual void OnEnterState_Construct();
		virtual void OnUpdateState_Construct(real fDeltaTime);
		virtual void OnRenderState_Construct(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Construct();

		virtual void OnEnterState_Idle();
		virtual void OnUpdateState_Idle(real fDeltaTime);
		virtual void OnRenderState_Idle(IGUIInterfaceRender* pRender){}
		virtual void OnLeaveState_Idle();

		virtual void UpdateTowerDataByLevel( );

		virtual void Attack();

	protected:
		CGUIAnimation* m_pAnimBuilding;

		ETowerState m_eTowerState;

		//data for state construct
		real m_fBuildingTime;
		real m_fBuildingTimeElapsed;

		real m_fWaitTime;

		std::vector<STowerLevelInfo> m_vecLevelInfos;
	};


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_ArcherTower
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_ArcherTower : public CTDGameTowerImplement_TowerBase
	{
	public:
		enum EArcherDir
		{
			eArcherDir_UpRight = 0,
			eArcherDir_UpLeft,
			eArcherDir_DownRight,
			eArcherDir_DownLeft,

			__eArcherDir_MAX__
		};

		enum EArcherState
		{
			eArcherState_Idle = 0,
			eArcherState_Attack,

			__eArcherState_MAX__
		};

	public:
		CTDGameTowerImplement_ArcherTower( CTDGameTower* pGameTower );

	protected:
		virtual void OnEnterState_Idle();
		virtual void OnUpdateState_Idle(real fDeltaTime);
		virtual void OnRenderState_Idle(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Idle();

		virtual void UpdateTowerDataByLevel( );

	protected:
		CGUIAnimation* m_pAnimTowerIdle;

		struct SArcherData
		{
			CGUIAnimation* m_pAnimArcher[__eArcherState_MAX__][__eArcherDir_MAX__];
			EArcherState m_eArcherState;
			EArcherDir m_eArcherDir;
			CGUIRect m_rectArcherRender;

			SArcherData()
				:m_eArcherState(eArcherState_Idle)
				,m_eArcherDir(eArcherDir_DownRight)
			{
				memset( m_pAnimArcher, 0, sizeof( m_pAnimArcher) );
			}
		};
		SArcherData m_aArcherData[2];
	};


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Mages
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Mages : public CTDGameTowerImplement_TowerBase
	{
	public:
		enum EMageDir
		{
			eMageDir_Up = 0,
			eMageDir_Down,

			__eMageDir_MAX__
		};

		enum EMageState
		{
			eMageState_Idle = 0,
			eMageState_Attack,

			__eMageState_MAX__
		};

		enum ETowerState
		{
			eTowerState_Idle = 0,
			eTowerState_Attack,

			__eTowerState_MAX__
		};

	public:
		CTDGameTowerImplement_Mages( CTDGameTower* pGameTower );

	protected:
		virtual void OnEnterState_Idle();
		virtual void OnUpdateState_Idle(real fDeltaTime);
		virtual void OnRenderState_Idle(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Idle();

		virtual void UpdateTowerDataByLevel( );

	protected:
		struct STowerData
		{
			CGUIAnimation* m_pAnimTower[__eTowerState_MAX__];
			ETowerState m_eTowerState;

			STowerData()
				:m_eTowerState(eTowerState_Idle)
			{
				memset( m_pAnimTower, 0, sizeof( m_pAnimTower ) );
			}
		};
		STowerData m_aTowerData;

		struct SMageData
		{
			CGUIAnimation* m_pAnimMage[__eMageState_MAX__][__eMageDir_MAX__];
			EMageState m_eMageState;
			EMageDir m_eMageDir;
			CGUIRect m_rectMageRender;

			SMageData()
				:m_eMageState(eMageState_Idle)
				,m_eMageDir(eMageDir_Down)
			{
				memset( m_pAnimMage, 0, sizeof( m_pAnimMage ) );
			}
		};
		SMageData m_aMageData;
	};


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Barracks
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Barracks : public CTDGameTowerImplement_TowerBase
	{
	public:
		enum ETowerState
		{
			eTowerState_Idle = 0,
			eTowerState_Open,
			eTowerState_Close,

			__eTowerState_MAX__
		};

	public:
		CTDGameTowerImplement_Barracks( CTDGameTower* pGameTower );

	protected:
		virtual void OnEnterState_Idle();
		virtual void OnUpdateState_Idle(real fDeltaTime);
		virtual void OnRenderState_Idle(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Idle();

		virtual void UpdateTowerDataByLevel( );

	protected:
		struct STowerData
		{
			CGUIAnimation* m_pAnimTower[__eTowerState_MAX__];
			ETowerState m_eTowerState;

			STowerData()
				:m_eTowerState(eTowerState_Idle)
			{
				memset( m_pAnimTower, 0, sizeof( m_pAnimTower ) );
			}
		};
		STowerData m_aTowerData;
	};


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Bombard
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Bombard : public CTDGameTowerImplement_TowerBase
	{
	public:
		enum ETowerState
		{
			eTowerState_Idle = 0,
			eTowerState_Attack,

			__eTowerState_MAX__
		};

	public:
		CTDGameTowerImplement_Bombard( CTDGameTower* pGameTower );

	protected:
		virtual void OnEnterState_Idle();
		virtual void OnUpdateState_Idle(real fDeltaTime);
		virtual void OnRenderState_Idle(IGUIInterfaceRender* pRender);
		virtual void OnLeaveState_Idle();

		virtual void UpdateTowerDataByLevel( );

	protected:
		struct STowerData
		{
			CGUIAnimation* m_pAnimTower[__eTowerState_MAX__];
			ETowerState m_eTowerState;

			STowerData()
				:m_eTowerState(eTowerState_Idle)
			{
				memset( m_pAnimTower, 0, sizeof( m_pAnimTower ) );
			}
		};
		STowerData m_aTowerData;
	};

}

#endif //__TD_GAMETOWER_IMPLEMENT_20110829_H__

