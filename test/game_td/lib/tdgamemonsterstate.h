/** 
* @file tdgamemonsterstate.h
* @brief monster state
* @author ken
* @date 2011-09-20
*/
#ifndef __TD_GAMEMONSTERSTATE_20110920_H__
#define __TD_GAMEMONSTERSTATE_20110920_H__

//============================================================================//
// include
//============================================================================// 
#include "tdgamestate.h"
#include "tdgameobjectmonster.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	// CTDGameMonsterState
	//------------------------------------------------------------------------------
	class CTDGameMonsterState : public CTDGameState
	{
	public:
		CTDGameMonsterState( CTDGameObjectMonster* pMonster );

	protected:
		CTDGameObjectMonster* GetMonster();
		void SetPosition( const CGUIVector2& rPos);
		const CGUIVector2& GetPosition() const;
		void GetSize( const CGUISize& rSize);
		const CGUISize& GetSize() const;
		void SetAlpha( real fAlpha );
		real GetAlpha( ) const;
		void SetAnimState( CTDGameObjectMonster::EAnimState eAnimState ) ;
		const CGUIWgtSimplePathNode* GetTargetPathNode() const ;
		void SetStartPathNode( const CGUIWgtSimplePathNode* pNode );
		void SetMonsterState( CTDGameObjectMonster::EMonsterState eMonsterState );
		real GetSpeed() const;
		CGUIAnimation* GetCurrentAnimation() const ;

	private:
		CTDGameObjectMonster* m_pMonster;
	};

	//------------------------------------------------------------------------------
	// CTDGameMonsterState_EnterMap
	//------------------------------------------------------------------------------
	class CTDGameMonsterState_EnterMap : public CTDGameMonsterState
	{
	public:
		CTDGameMonsterState_EnterMap( CTDGameObjectMonster* pMonster );
		virtual ~CTDGameMonsterState_EnterMap();
		virtual void EnterState();
		virtual void LeaveState();
		virtual void UpdateState( real fDeltaTime);

	protected:
		CGUIAsInterpolation<real>* m_pAlphaAs;
	};

	//------------------------------------------------------------------------------
	//CTDGameMonsterState_MoveToTarget
	//------------------------------------------------------------------------------
	class CTDGameMonsterState_MoveToTarget : public CTDGameMonsterState
	{
	public:
		CTDGameMonsterState_MoveToTarget( CTDGameObjectMonster* pMonster )
			:CTDGameMonsterState(pMonster){}
		virtual void EnterState();
		virtual void LeaveState();
		virtual void UpdateState( real fDeltaTime);
	};


	//------------------------------------------------------------------------------
	//CTDGameMonsterState_LeaveMap
	//------------------------------------------------------------------------------
	class CTDGameMonsterState_LeaveMap : public CTDGameMonsterState
	{
	public:
		CTDGameMonsterState_LeaveMap( CTDGameObjectMonster* pMonster );
		virtual ~CTDGameMonsterState_LeaveMap();
		virtual void EnterState();
		virtual void LeaveState();
		virtual void UpdateState( real fDeltaTime);

	protected:
		CGUIAsInterpolation<real>* m_pAlphaAs;
	};



	//------------------------------------------------------------------------------
	//CTDGameMonsterState_Die
	//------------------------------------------------------------------------------
	class CTDGameMonsterState_Die : public CTDGameMonsterState
	{
	public:
		CTDGameMonsterState_Die( CTDGameObjectMonster* pMonster );
		virtual ~CTDGameMonsterState_Die();
		virtual void EnterState();
		virtual void LeaveState();
		virtual void UpdateState( real fDeltaTime);

	protected:
	};
}

#endif //__TD_GAMEMONSTERSTATE_20110920_H__
