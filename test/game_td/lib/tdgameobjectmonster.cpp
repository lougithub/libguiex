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
#include "tdgamestate.h"
#include "tdgamemonsterstate.h"
#include <libguiex_core/guiex.h>

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
		m_fSpeed = 0.0f;
		m_pStartNode = NULL;
		m_pTargetNode = NULL;
		m_eAnimState = eAnimState_Idle;
		m_eMonsterState = __eMonsterState_Max__;
		m_fAlpha = 1.0f;
		m_fTotalHp = 0.0f;
		m_fCurrentHp = 0.0f;

		//init state
		m_arrayMonsterState[eMonsterState_EnterMap] = new CTDGameMonsterState_EnterMap( this );
		m_arrayMonsterState[eMonsterState_LeaveMap] = new CTDGameMonsterState_LeaveMap( this );
		m_arrayMonsterState[eMonsterState_MoveToTarget] = new CTDGameMonsterState_MoveToTarget( this );
		m_arrayMonsterState[eMonsterState_Die] = new CTDGameMonsterState_Die( this );
	}
	//------------------------------------------------------------------------------
	CTDGameObjectMonster::~CTDGameObjectMonster()
	{
		//clear state
		for( uint32 i=0; i<__eMonsterState_Max__; ++i )
		{
			delete m_arrayMonsterState[i];
			m_arrayMonsterState[i] = NULL;
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::InitMonster( const CGUIString& rMonsterType, const CGUIString& rStartPathNode )
	{
		//get data property
		const CGUIProperty* pAllMonsterProperty = GetGameWorld()->GetDataProperty("monster.xml");
		const CGUIProperty& rMonsterProp = *pAllMonsterProperty->GetProperty(rMonsterType);

		//init animation
		m_arrayAnimations[eAnimState_Idle] = CGUIAnimationManager::Instance()->AllocateResource( rMonsterProp["AnimIdle"]->GetValue() );
		m_arrayAnimations[eAnimState_Die] = CGUIAnimationManager::Instance()->AllocateResource( rMonsterProp["AnimDie"]->GetValue() );
		m_arrayAnimations[eAnimState_Attack] = CGUIAnimationManager::Instance()->AllocateResource( rMonsterProp["AnimAttack"]->GetValue() );
		m_arrayAnimations[eAnimState_MoveLeft] = CGUIAnimationManager::Instance()->AllocateResource( rMonsterProp["AnimMoveLeft"]->GetValue() );
		m_arrayAnimations[eAnimState_MoveRight] = CGUIAnimationManager::Instance()->AllocateResource( rMonsterProp["AnimMoveRight"]->GetValue() );
		m_arrayAnimations[eAnimState_MoveUp] = CGUIAnimationManager::Instance()->AllocateResource( rMonsterProp["AnimMoveUp"]->GetValue() );
		m_arrayAnimations[eAnimState_MoveDown] = CGUIAnimationManager::Instance()->AllocateResource( rMonsterProp["AnimMoveDown"]->GetValue() );
		m_aSize = m_arrayAnimations[eAnimState_Idle]->GetSize();

		//init values
		m_fSpeed = rMonsterProp["Speed"]->GetCommonValue<real>();
		m_fTotalHp = rMonsterProp["Hp"]->GetCommonValue<real>();
		m_fCurrentHp = m_fTotalHp;

		//init path node
		m_pStartNode = GetGameWorld()->GetSimplePathNode( rStartPathNode );
		m_pTargetNode = m_pStartNode->GetNextNode();
		m_aPosition = m_pStartNode->GetPixelPosition();

		//init state
		SetMonsterState( eMonsterState_EnterMap );
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::OnActive()
	{
		CTDGameObject::OnActive();

		m_eMonsterState = __eMonsterState_Max__;
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::OnDeactive()
	{
		//release animation
		for( uint32 i=0; i<__eAnimState_MAX__; ++i )
		{
			if( m_arrayAnimations[i] )
			{
				m_arrayAnimations[i]->RefRelease();
				m_arrayAnimations[i] = NULL;
			}
		}

		//reset values
		m_eAnimState = eAnimState_Idle;
		m_fSpeed = 0.0f;
		m_pStartNode = NULL;
		m_pTargetNode = NULL;
		m_aPosition.x = m_aPosition.y = 0.0f;
		m_aSize.m_fWidth = m_aSize.m_fHeight = 0.0f;
		m_fAlpha = 1.0f;

		SetMonsterState( __eMonsterState_Max__ );

		CTDGameObject::OnDeactive();
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::OnRender( IGUIInterfaceRender* pRender )
	{
		if( !IsAlive() )
		{
			return;
		}

		m_arrayAnimations[m_eAnimState]->Draw( pRender, m_aMonsterRect, 0, m_fAlpha );
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::OnUpdate( real fDeltaTime )
	{
		if( !IsAlive() )
		{
			return;
		}

		//update monster
		m_arrayMonsterState[m_eMonsterState]->UpdateState(fDeltaTime);

		//update animation
		m_arrayAnimations[m_eAnimState]->Update( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::SetMonsterState( EMonsterState eMonsterState )
	{
		if( m_eMonsterState != eMonsterState )
		{
			if( m_eMonsterState != __eMonsterState_Max__ )
			{
				m_arrayMonsterState[m_eMonsterState]->LeaveState();
			}

			m_eMonsterState = eMonsterState;

			if( m_eMonsterState != __eMonsterState_Max__ )
			{
				m_arrayMonsterState[m_eMonsterState]->EnterState();
			}
		}
	}
	//------------------------------------------------------------------------------
	const CGUIRect& CTDGameObjectMonster::GetRect() const
	{
		return m_aMonsterRect;
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::SetPosition(const CGUIVector2& rPos )
	{
		m_aPosition = rPos;
		m_aMonsterRect = CGUIRect( m_aPosition - CGUIVector2(m_aSize.m_fWidth, m_aSize.m_fHeight) / 2.0f, m_aSize );
	}
	//------------------------------------------------------------------------------
	const CGUIVector2& CTDGameObjectMonster::GetPosition() const
	{
		return m_aPosition;
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::SetSize(const CGUISize& rSize)
	{
		m_aSize = rSize;
		m_aMonsterRect = CGUIRect( m_aPosition - CGUIVector2(m_aSize.m_fWidth, m_aSize.m_fHeight) / 2.0f, m_aSize );
	}
	//------------------------------------------------------------------------------
	const CGUISize& CTDGameObjectMonster::GetSize() const
	{
		return m_aSize;
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::SetAnimState( EAnimState eAnimState )
	{
		if( m_eAnimState != eAnimState )
		{
			m_eAnimState = eAnimState;

			m_arrayAnimations[m_eAnimState]->Reset();
		}
	}
	//------------------------------------------------------------------------------
	const CGUIWgtSimplePathNode* CTDGameObjectMonster::GetTargetPathNode() const
	{
		return m_pTargetNode;
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::SetStartPathNode( const CGUIWgtSimplePathNode* pNode )
	{
		m_pStartNode = pNode;
		m_pTargetNode = m_pStartNode->GetNextNode();
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::SetAlpha( real fAlpha )
	{
		m_fAlpha = fAlpha;
	}
	//------------------------------------------------------------------------------
	real CTDGameObjectMonster::GetAlpha( ) const
	{
		return m_fAlpha;
	}
	//------------------------------------------------------------------------------
	real CTDGameObjectMonster::GetSpeed() const
	{
		return m_fSpeed;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CTDGameObjectMonster::GetCurrentAnimation() const
	{
		return m_arrayAnimations[m_eAnimState];
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectMonster::TakeDamage( const CGUIVector2& rDamageLocation, EBulletDamageType eDamageType, real fDamage )
	{
		m_fCurrentHp -= fDamage;
		if( m_fCurrentHp <= 0.0f )
		{
			//dead
			m_fCurrentHp = 0.0f;
			SetMonsterState( CTDGameObjectMonster::eMonsterState_Die );
		}
	}
	//------------------------------------------------------------------------------
}

