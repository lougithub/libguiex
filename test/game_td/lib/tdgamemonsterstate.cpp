/** 
* @file tdgamemonsterstate.cpp
* @brief monster state
* @author ken
* @date 2011-09-20
*/


//============================================================================//
// include
//============================================================================// 
#include "tdgamemonsterstate.h"
#include "tdgameobjectmonster.h"
#include "tdgameobjectmanager.h"
#include "tdgameworld.h"
#include "tdgamestate.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	// CTDGameMonsterState
	//------------------------------------------------------------------------------
	CTDGameMonsterState::CTDGameMonsterState( CTDGameObjectMonster* pMonster )
		:m_pMonster( pMonster )
	{
	}
	//------------------------------------------------------------------------------
	CTDGameObjectMonster* CTDGameMonsterState::GetMonster() 
	{
		return m_pMonster;
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState::SetPosition( const CGUIVector2& rPos)
	{
		return m_pMonster->SetPosition(rPos);
	}
	//------------------------------------------------------------------------------
	const CGUIVector2& CTDGameMonsterState::GetPosition() const
	{
		return m_pMonster->GetPosition(); 
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState::GetSize( const CGUISize& rSize)
	{
		return m_pMonster->SetSize(rSize); 
	}
	//------------------------------------------------------------------------------
	const CGUISize& CTDGameMonsterState::GetSize() const
	{
		return m_pMonster->GetSize();
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState::SetAlpha( real fAlpha )
	{
		m_pMonster->SetAlpha(fAlpha); 
	}
	//------------------------------------------------------------------------------
	real CTDGameMonsterState::GetAlpha( ) const
	{
		return m_pMonster->GetAlpha(); 
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState::SetAnimState( CTDGameObjectMonster::EAnimState eAnimState )
	{
		m_pMonster->SetAnimState( eAnimState );
	}
	//------------------------------------------------------------------------------
	const CGUIWgtSimplePathNode* CTDGameMonsterState::GetTargetPathNode() const
	{
		return m_pMonster->GetTargetPathNode(); 
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState::SetStartPathNode( const CGUIWgtSimplePathNode* pNode )
	{
		m_pMonster->SetStartPathNode( pNode );
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState::SetMonsterState( CTDGameObjectMonster::EMonsterState eMonsterState )
	{
		m_pMonster->SetMonsterState( eMonsterState ) ;
	}
	//------------------------------------------------------------------------------
	real CTDGameMonsterState::GetSpeed() const
	{
		return m_pMonster->GetSpeed();
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CTDGameMonsterState::GetCurrentAnimation() const 
	{
		return m_pMonster->GetCurrentAnimation();
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	CTDGameMonsterState_EnterMap::CTDGameMonsterState_EnterMap( CTDGameObjectMonster* pMonster )
		:CTDGameMonsterState(pMonster)
	{
		m_pAlphaAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<real> >();
	}
	//------------------------------------------------------------------------------
	CTDGameMonsterState_EnterMap::~CTDGameMonsterState_EnterMap()
	{
		m_pAlphaAs->RefRelease();
		m_pAlphaAs = NULL;
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_EnterMap::EnterState()
	{
		CTDGameMonsterState::EnterState();

		//animation
		SetAnimState( CTDGameObjectMonster::eAnimState_Idle);

		//alpha
		m_pAlphaAs->SetInterpolationValue( 0.0f, 1.0f, 1.0f );
		m_pAlphaAs->Reset();
		SetAlpha( 0.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_EnterMap::LeaveState()
	{
		CTDGameMonsterState::LeaveState();

		//alpha
		SetAlpha( 1.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_EnterMap::UpdateState( real fDeltaTime)
	{
		CTDGameMonsterState::UpdateState(fDeltaTime);

		m_pAlphaAs->Update( fDeltaTime );
		SetAlpha( m_pAlphaAs->GetCurrentValue() );

		if( m_pAlphaAs->IsRetired() )
		{
			SetMonsterState( CTDGameObjectMonster::eMonsterState_MoveToTarget );
			return;
		}
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	// CTDGameMonsterState_MoveToTarget
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_MoveToTarget::EnterState()
	{
		CTDGameMonsterState::EnterState();
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_MoveToTarget::LeaveState()
	{
		CTDGameMonsterState::LeaveState();
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_MoveToTarget::UpdateState( real fDeltaTime)
	{
		CTDGameMonsterState::UpdateState(fDeltaTime);
		if( !GetTargetPathNode() )
		{
			SetMonsterState( CTDGameObjectMonster::eMonsterState_LeaveMap );
			return;
		}

		while( fDeltaTime > 0.0f )
		{
			const CGUIVector2& rTargetPos = GetTargetPathNode()->GetPixelPosition();
			//has target, move to it
			real fCurMoveLen = GetSpeed() * fDeltaTime;
			if( GUI_REAL_EQUAL( fCurMoveLen, 0.0f ) )
			{
				return;
			}

			CGUIVector2 aDelta = rTargetPos - GetPosition();
			real fDeltaLen = aDelta.Normalise();

			//update animation
			if( abs( aDelta.x ) > abs( aDelta.y ) )
			{
				if( aDelta.x > 0 )
				{
					SetAnimState( CTDGameObjectMonster::eAnimState_MoveRight);
				}
				else
				{
					SetAnimState( CTDGameObjectMonster::eAnimState_MoveRight );
				}
			}
			else
			{
				if( aDelta.y > 0 )
				{
					SetAnimState( CTDGameObjectMonster::eAnimState_MoveDown);
				}
				else
				{
					SetAnimState( CTDGameObjectMonster::eAnimState_MoveUp);
				}
			}

			//update position
			if( fDeltaLen <= fCurMoveLen )
			{
				//reached
				SetPosition( rTargetPos );
				fDeltaTime = fDeltaTime * ( 1.0f - fDeltaLen / fCurMoveLen );

				//get next node path
				SetStartPathNode( GetTargetPathNode());
				if( !GetTargetPathNode() )
				{
					return;
				}
			}
			else
			{
				SetPosition( GetPosition() + aDelta * fCurMoveLen );
				return;
			}
		}
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	CTDGameMonsterState_LeaveMap::CTDGameMonsterState_LeaveMap( CTDGameObjectMonster* pMonster )
		:CTDGameMonsterState(pMonster)
	{
		m_pAlphaAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsInterpolation<real> >();
	}
	//------------------------------------------------------------------------------
	CTDGameMonsterState_LeaveMap::~CTDGameMonsterState_LeaveMap()
	{
		m_pAlphaAs->RefRelease();
		m_pAlphaAs = NULL;
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_LeaveMap::EnterState()
	{
		CTDGameMonsterState::EnterState();

		SetAnimState( CTDGameObjectMonster::eAnimState_Idle);
		SetAlpha( 1.0f );
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_LeaveMap::LeaveState()
	{
		CTDGameMonsterState::LeaveState();
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_LeaveMap::UpdateState( real fDeltaTime)
	{
		CTDGameMonsterState::UpdateState(fDeltaTime);

		m_pAlphaAs->Update( fDeltaTime );
		SetAlpha( m_pAlphaAs->GetCurrentValue() );

		if( m_pAlphaAs->IsRetired() )
		{
			GetMonster()->KillObject();
			return;
		}
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	CTDGameMonsterState_Die::CTDGameMonsterState_Die( CTDGameObjectMonster* pMonster )
		:CTDGameMonsterState(pMonster)
	{
	}
	//------------------------------------------------------------------------------
	CTDGameMonsterState_Die::~CTDGameMonsterState_Die()
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_Die::EnterState()
	{
		CTDGameMonsterState::EnterState();

		SetAnimState( CTDGameObjectMonster::eAnimState_Die);
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_Die::LeaveState()
	{
		CTDGameMonsterState::LeaveState();
	}
	//------------------------------------------------------------------------------
	void CTDGameMonsterState_Die::UpdateState( real fDeltaTime)
	{
		CTDGameMonsterState::UpdateState(fDeltaTime);

		if( GetCurrentAnimation()->IsFinished() )
		{
			GetMonster()->KillObject();
			return;
		}
	}
	//------------------------------------------------------------------------------
}

