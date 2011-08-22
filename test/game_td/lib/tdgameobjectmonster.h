/** 
* @file tdgameobjectmonster.h
* @brief monster
* @author ken
* @date 2011-08-22
*/
#ifndef __TD_GAMEOBJECTMONSTER_20110822_H__
#define __TD_GAMEOBJECTMONSTER_20110822_H__

//============================================================================//
// include
//============================================================================// 
#include "tdgameobject.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CTDGameObjectMonster : public CTDGameObject
	{
	public:
		CTDGameObjectMonster( class CTDGameWorld* pGameWorld );
		virtual ~CTDGameObjectMonster();

		void InitMonster();

	protected:
		friend class CTDGameWorld;

		virtual void OnActive();
		virtual void OnDeactive();

		virtual void OnRender( IGUIInterfaceRender* pRender );
		virtual void OnUpdate( real fDeltaTime );

		void InitAnimation();
		void ReleaseAnimation();

	protected:
		enum EAnimState
		{
			eAnimState_Idle = 0,

			__eAnimState_MAX__
		};
		CGUIAnimation* m_arrayAnimations[__eAnimState_MAX__];
	};
}

#endif //__TD_GAMEOBJECTMONSTER_20110822_H__
