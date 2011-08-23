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
#include <libguiex_widget_game/guiwgt_game.h>

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

		static void GlobalInit();

		void InitMonster( const CGUIString& rMonsterType, const CGUIString& rStartPathNode );


	protected:
		friend class CTDGameWorld;

		virtual void OnActive();
		virtual void OnDeactive();

		virtual void OnRender( IGUIInterfaceRender* pRender );
		virtual void OnUpdate( real fDeltaTime );

		enum EMonsterState
		{
			eMonsterState_EnterMap = 0,
			eMonsterState_MoveToTarget,
			eMonsterState_LeaveMap,

			eMonsterState_None_Max,
		};

		void SetMonsterState( EMonsterState eMonsterState );

	protected:
		enum EAnimState
		{
			eAnimState_Idle = 0,
			eAnimState_Die,
			eAnimState_Attack,
			eAnimState_MoveLeft,
			eAnimState_MoveRight,
			eAnimState_MoveUp,
			eAnimState_MoveDown,
			__eAnimState_MAX__
		};
		EAnimState m_eAnimState;
		CGUIAnimation* m_arrayAnimations[__eAnimState_MAX__];

		EMonsterState m_eMonsterState;
		class CTDGameMonsterState* m_arrayMonsterState[eMonsterState_None_Max];

		CGUISize m_aSize;
		CGUIVector2 m_aPosition;
		real m_fAlpha;
		real m_fSpeed;

		CGUIWgtSimplePathNode* m_pStartNode;
		CGUIWgtSimplePathNode* m_pTargetNode;

		friend class CTDGameMonsterState_EnterMap;
		friend class CTDGameMonsterState_LeaveMap;
		friend class CTDGameMonsterState_MoveToTarget;
	};
}

#endif //__TD_GAMEOBJECTMONSTER_20110822_H__
