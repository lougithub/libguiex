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
#include <libguiex_core/guiex.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CTDGameMonsterState
	{
	public:
		CTDGameMonsterState( CTDGameObjectMonster* pMonster )
			:m_pMonster( pMonster )	{}
		virtual ~CTDGameMonsterState(){}

		virtual void EnterState(){}
		virtual void LeaveState(){}
		virtual void UpdateState( real fDeltaTime){}

	protected:
		CTDGameObjectMonster* GetMonster() { return m_pMonster;}
		void SetPosition( const CGUIVector2& rPos){ return m_pMonster->SetPosition(rPos); }
		const CGUIVector2& GetPosition() const{ return m_pMonster->GetPosition(); }
		void GetSize( const CGUISize& rSize){ return m_pMonster->SetSize(rSize); }
		const CGUISize& GetSize() const{ return m_pMonster->GetSize(); }
		void SetAlpha( real fAlpha ){ m_pMonster->SetAlpha(fAlpha); }
		real GetAlpha( ) const{ return m_pMonster->GetAlpha(); }
		void SetAnimState( CTDGameObjectMonster::EAnimState eAnimState ) { m_pMonster->SetAnimState( eAnimState );}
		const CGUIWgtSimplePathNode* GetTargetPathNode() const { return m_pMonster->GetTargetPathNode(); }
		void SetStartPathNode( const CGUIWgtSimplePathNode* pNode ){m_pMonster->SetStartPathNode( pNode );}
		void SetMonsterState( CTDGameObjectMonster::EMonsterState eMonsterState ){ m_pMonster->SetMonsterState( eMonsterState ) ;}
		real GetSpeed() const{return m_pMonster->GetSpeed();}

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
	//CTDGameMonsterState_MoveToTarget
	//------------------------------------------------------------------------------
	class CTDGameMonsterState_MoveToTarget : public CTDGameMonsterState
	{
	public:
		CTDGameMonsterState_MoveToTarget( CTDGameObjectMonster* pMonster )
			:CTDGameMonsterState(pMonster){}
		virtual ~CTDGameMonsterState_MoveToTarget(){}
		virtual void EnterState();
		virtual void LeaveState();
		virtual void UpdateState( real fDeltaTime);
	};
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
}

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
		m_eMonsterState = eMonsterState_None_Max;
		m_fAlpha = 1.0f;

		//init state
		m_arrayMonsterState[eMonsterState_EnterMap] = new CTDGameMonsterState_EnterMap( this );
		m_arrayMonsterState[eMonsterState_LeaveMap] = new CTDGameMonsterState_LeaveMap( this );
		m_arrayMonsterState[eMonsterState_MoveToTarget] = new CTDGameMonsterState_MoveToTarget( this );
	}
	//------------------------------------------------------------------------------
	CTDGameObjectMonster::~CTDGameObjectMonster()
	{
		//clear state
		for( uint32 i=0; i<eMonsterState_None_Max; ++i )
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
		m_fSpeed = rMonsterProp["Speed"]->GetSpecifiedValue<real>();

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

		m_eMonsterState = eMonsterState_None_Max;
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

		SetMonsterState( eMonsterState_None_Max );

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
			if( m_eMonsterState != eMonsterState_None_Max )
			{
				m_arrayMonsterState[m_eMonsterState]->LeaveState();
			}

			m_eMonsterState = eMonsterState;

			if( m_eMonsterState != eMonsterState_None_Max )
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
		m_eAnimState = eAnimState;
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
}

