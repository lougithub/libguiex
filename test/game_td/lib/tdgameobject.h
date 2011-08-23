/** 
* @file tdgameobject.h
* @brief game object, could be monster, bullet or other dynamic actor in game.
* @author ken
* @date 2011-08-22
*/

#ifndef __TD_GAMEOBJECT_20110822_H__
#define __TD_GAMEOBJECT_20110822_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiex.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CTDGameObject
	{
	public:
		CTDGameObject( uint32 uObjectType, class CTDGameWorld* pGameWorld );
		virtual ~CTDGameObject();

		uint32 GetType() const;
		CTDGameWorld* GetGameWorld() const;
		bool IsAlive() const;

		void KillObject();

	protected:
		friend class CTDGameObjectManager;
		virtual void OnActive();
		virtual void OnDeactive();

		virtual void OnRender( IGUIInterfaceRender* pRender ) = 0;
		virtual void OnUpdate( real fDeltaTime ) = 0;

	private:
		uint32 m_uObjectType;
		CTDGameWorld* m_pGameWorld;

		bool m_bIsAlive;
	};
}

#endif //__TD_GAMEOBJECT_20110822_H__
