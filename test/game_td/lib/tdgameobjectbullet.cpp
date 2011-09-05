/** 
* @file tdgameobjectbullet.cpp
* @brief bullet
* @author ken
* @date 2011-08-22
*/


//============================================================================//
// include
//============================================================================// 
#include "tdgameobjectbullet.h"
#include "tdgameobjectmanager.h"
#include "tdgameobjectmonster.h"
#include "tdgameworld.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CTDGameObjectBullet::CTDGameObjectBullet( CTDGameWorld* pGameWorld )
		:CTDGameObject( eGameObject_Bullet, pGameWorld )
		,m_pTarget(NULL)
		,m_fYaw(0)
		,m_eBulletState( eBulletState_Fly )
		,m_uRotation(0)
		,m_uRotationSpeed(0)
		,m_fGravity(0)
		,m_fDistance(0)
	{
		memset( m_arrayAnimations, 0, sizeof( m_arrayAnimations ));
	}
	//------------------------------------------------------------------------------
	CTDGameObjectBullet::~CTDGameObjectBullet()
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::InitBullet( const CGUIString& rBulletType, const CGUIVector2& rStartPos, CTDGameObjectMonster* pTarget )
	{
		//get data property
		const CGUIProperty* pAllBulletProperty = GetGameWorld()->GetDataProperty("bullet.xml");
		const CGUIProperty& rBulletProp = *pAllBulletProperty->GetProperty(rBulletType);

		//init data
		m_arrayAnimations[eBulletState_Fly] = CGUIAnimationManager::Instance()->AllocateResource( rBulletProp["AnimFly"]->GetValue() );
		m_arrayAnimations[eBulletState_Explosion] = CGUIAnimationManager::Instance()->AllocateResource( rBulletProp["AnimExplosion"]->GetValue() );
		m_fDistance = rBulletProp["distance"]->GetSpecifiedValue<real>();
		m_fGravity = rBulletProp["gravity"]->GetSpecifiedValue<real>();
		
		m_uRotationSpeed = rBulletProp["rotation_speed"]->GetSpecifiedValue<uint32>();
		
		m_aSize = m_arrayAnimations[eBulletState_Fly]->GetSize();
		m_vPosition = rStartPos;

		m_eBulletState = eBulletState_Fly;

		m_pTarget = pTarget;

		//init velocity
		CGUIVector2 vTargetPos = m_pTarget->GetPosition();

		real fTop = vTargetPos.y > m_vPosition.y ? m_vPosition.y - m_fDistance : vTargetPos.y - m_fDistance;
		real S1 = m_vPosition.y - fTop;
		real t1 = sqrt(S1 * 2 / m_fGravity);
		real S2 = vTargetPos.y - fTop;
		real t2 = sqrt(S2 * 2 / m_fGravity);
		real t = t1 + t2;
		m_vCurrentVelocity.y = -m_fGravity * t1;
		m_vCurrentVelocity.x = (vTargetPos.x - m_vPosition.x) / t;
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::OnActive()
	{
		CTDGameObject::OnActive();
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::OnDeactive()
	{
		//release animation
		for( uint32 i=0; i<__eBulletState_MAX__; ++i )
		{
			if( m_arrayAnimations[i] )
			{
				m_arrayAnimations[i]->RefRelease();
				m_arrayAnimations[i] = NULL;
			}
		}

		m_fYaw = 0;
		m_pTarget = NULL;
		m_eBulletState = eBulletState_Fly;

		CTDGameObject::OnDeactive();
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::OnRender( IGUIInterfaceRender* pRender )
	{
		if( !IsAlive() )
		{
			return;
		}

		pRender->PushMatrix();

		CGUIMatrix4 matrix;
		matrix.makeTransform( CGUIVector3(m_vPosition), CGUIVector3(1.0f,1.0f,1.0f), CGUIQuaternion( m_rRotator));
		pRender->MultMatrix( matrix );

		CGUIRect aRenderRect( 
			- m_aSize.m_fWidth/2, 
			- m_aSize.m_fHeight/2,
			+ m_aSize.m_fHeight/2, 
			+ m_aSize.m_fHeight/2);

		m_arrayAnimations[m_eBulletState]->Draw( pRender, aRenderRect, 0, 1.0f );

		pRender->PopMatrix();
	}
	//------------------------------------------------------------------------------
	real CTDGameObjectBullet::UpdateBulletFlying( real fDeltaTime )
	{
		real fLeftTime = 0.0f;
		//update position
		if( m_vCurrentVelocity.y < 0.0f )
		{
			real fNewVerticalSpeed = m_vCurrentVelocity.y + ( fDeltaTime * m_fGravity );
			if( m_vCurrentVelocity.y > -fDeltaTime * m_fGravity )
			{
				fLeftTime = fDeltaTime + m_vCurrentVelocity.y / m_fGravity; 
				fDeltaTime = fDeltaTime - fLeftTime;
				fNewVerticalSpeed = 0.0f;
			}

			real s = m_vCurrentVelocity.y * fDeltaTime + 0.5f * m_fGravity * fDeltaTime * fDeltaTime;
			m_vCurrentVelocity.y = fNewVerticalSpeed;
			m_vPosition.y += s;
			m_vPosition.x += m_vCurrentVelocity.x * fDeltaTime;
		}
		else
		{
			CGUIVector2 vTargetPos = m_pTarget->GetPosition();
			CGUIVector2 vDelta = vTargetPos - m_vPosition;
			real fDeltaLen = vDelta.Normalise();
			m_vCurrentVelocity.y += ( 0.5f * m_fGravity * fDeltaTime );
			real fCurMoveLen = m_vCurrentVelocity.Length() * fDeltaTime;
			m_vCurrentVelocity.y += ( 0.5f * m_fGravity * fDeltaTime );
			if( GUI_REAL_EQUAL( fCurMoveLen, 0.0f ) )
			{
				return 0.0f;
			}
			if( fDeltaLen <= fCurMoveLen )
			{
				//reached
				m_vPosition = vTargetPos;

				HitEnemy();
				return 0.0f;
			}
			else
			{
				m_vPosition = m_vPosition + vDelta * fCurMoveLen;
			}
		}

		//update self rotation
		if( m_uRotationSpeed != 0 )
		{
			m_uRotation += uint32(fDeltaTime * m_uRotationSpeed);
			m_uRotation %= 360;
			m_rRotator.Yaw = real(m_uRotation);
		}
		else
		{
			m_rRotator = CGUIVector3(m_vCurrentVelocity).Rotation();
		}

		return fLeftTime;
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::OnUpdate( real fDeltaTime )
	{
		if( !IsAlive() )
		{
			return;
		}

		switch( m_eBulletState )
		{
			case eBulletState_Fly:
				OnUpdateFly( fDeltaTime );
				break;
			case eBulletState_Explosion:
				OnUpdateExplosion( fDeltaTime );
				break;
		}

	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::OnUpdateFly( real fDeltaTime )
	{
		if( !m_pTarget->IsAlive() )
		{
			KillObject();
			return;
		}

		fDeltaTime = UpdateBulletFlying(fDeltaTime);
		while( fDeltaTime > 0.0f )
		{
			fDeltaTime = UpdateBulletFlying(fDeltaTime);
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::OnUpdateExplosion( real fDeltaTime )
	{
		m_arrayAnimations[m_eBulletState]->Update( fDeltaTime );
		if( m_arrayAnimations[m_eBulletState]->IsFinished() )
		{
			KillObject();
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameObjectBullet::HitEnemy( )
	{
		m_eBulletState = eBulletState_Explosion;
		m_arrayAnimations[m_eBulletState]->Reset();
	}
	//------------------------------------------------------------------------------
}

