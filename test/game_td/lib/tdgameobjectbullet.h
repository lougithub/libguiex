/** 
* @file tdgameobjectbullet.h
* @brief bullet
* @author ken
* @date 2011-08-22
*/
#ifndef __TD_GAMEOBJECTBULLET_20110822_H__
#define __TD_GAMEOBJECTBULLET_20110822_H__

//============================================================================//
// include
//============================================================================// 
#include "tdgameobject.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CTDGameObjectBullet : public CTDGameObject
	{
	public:
		enum EBulletState
		{
			eBulletState_Fly = 0,
			eBulletState_Explosion,
			__eBulletState_MAX__,
		};

	public:
		CTDGameObjectBullet( class CTDGameWorld* pGameWorld );
		virtual ~CTDGameObjectBullet();

		void InitBullet( const CGUIString& rBulletType, const CGUIVector2& rStartPos, class CTDGameObjectMonster* pTarget );

	protected:
		friend class CTDGameWorld;

		virtual void OnActive();
		virtual void OnDeactive();

		virtual void OnRender( IGUIInterfaceRender* pRender );
		virtual void OnUpdate( real fDeltaTime );

	protected:
		EBulletState m_eBulletState;
		CGUIAnimation* m_arrayAnimations[__eBulletState_MAX__];

		real m_fSpeed;

		CGUISize m_aSize;
		CGUIVector2 m_vPosition;
		real m_fYaw;

		bool m_bSelfRotation;
		real m_fSelfRotation;
		CGUIRotator m_rRotator;

		//test
		CGUIVector2 m_vTestTargetPos;
		//test


		class CTDGameObjectMonster* m_pTarget;
	};
}

#endif //__TD_GAMEOBJECTBULLET_20110822_H__
