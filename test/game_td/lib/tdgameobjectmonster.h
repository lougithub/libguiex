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
		enum EMonsterState
		{
			eMonsterState_EnterMap = 0,
			eMonsterState_MoveToTarget,
			eMonsterState_LeaveMap,

			__eMonsterState_Max__,
		};
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

	public:
		CTDGameObjectMonster( class CTDGameWorld* pGameWorld );
		virtual ~CTDGameObjectMonster();

		static void GlobalInit();

		void InitMonster( const CGUIString& rMonsterType, const CGUIString& rStartPathNode );

		const CGUIRect& GetRect() const;
		void SetPosition(const CGUIVector2& rPos );
		const CGUIVector2& GetPosition() const;
		void SetSize(const CGUISize& rSize);
		const CGUISize& GetSize() const;
		void SetAlpha( real fAlpha );
		real GetAlpha( ) const;

		void SetAnimState( EAnimState eAnimState );

		const CGUIWgtSimplePathNode* GetTargetPathNode() const;
		void SetStartPathNode( const CGUIWgtSimplePathNode* pNode );

		void SetMonsterState( EMonsterState eMonsterState );

		real GetSpeed() const;

	protected:
		friend class CTDGameWorld;

		virtual void OnActive();
		virtual void OnDeactive();

		virtual void OnRender( IGUIInterfaceRender* pRender );
		virtual void OnUpdate( real fDeltaTime );

	protected:
		EAnimState m_eAnimState;
		CGUIAnimation* m_arrayAnimations[__eAnimState_MAX__];

		EMonsterState m_eMonsterState;
		class CTDGameMonsterState* m_arrayMonsterState[__eMonsterState_Max__];


		real m_fAlpha;
		real m_fSpeed;

		const CGUIWgtSimplePathNode* m_pStartNode;
		const CGUIWgtSimplePathNode* m_pTargetNode;

	private:
		CGUISize m_aSize;
		CGUIVector2 m_aPosition;
		CGUIRect m_aMonsterRect;
	};
}

#endif //__TD_GAMEOBJECTMONSTER_20110822_H__
