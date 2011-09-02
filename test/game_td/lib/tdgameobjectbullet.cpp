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
		,m_fSelfRotation(0)
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

		//init animation
		m_arrayAnimations[eBulletState_Fly] = CGUIAnimationManager::Instance()->AllocateResource( rBulletProp["AnimFly"]->GetValue() );
		m_arrayAnimations[eBulletState_Explosion] = CGUIAnimationManager::Instance()->AllocateResource( rBulletProp["AnimExplosion"]->GetValue() );
		
		m_fSpeed = rBulletProp["speed"]->GetSpecifiedValue<real>();
		
		m_aSize = m_arrayAnimations[eBulletState_Fly]->GetSize();
		m_vPosition = rStartPos;

		m_eBulletState = eBulletState_Fly;

		m_pTarget = pTarget;

		//test
		m_vTestTargetPos = m_vPosition+CGUIVector2(50,0);
		//test
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
	void CTDGameObjectBullet::OnUpdate( real fDeltaTime )
	{
		if( !IsAlive() )
		{
			return;
		}

		CGUIVector2 vDelta = m_vTestTargetPos - m_vPosition;
		real fDeltaLen = vDelta.Normalise();
		real fCurMoveLen = m_fSpeed * fDeltaTime;
		if( GUI_REAL_EQUAL( fCurMoveLen, 0.0f ) )
		{
			return;
		}

		//update position
		if( fDeltaLen <= fCurMoveLen )
		{
			//reached
			m_vPosition = m_vTestTargetPos;
		}
		else
		{
			m_vPosition = m_vPosition + vDelta * fCurMoveLen;
			return;
		}
	}
	//------------------------------------------------------------------------------
}

