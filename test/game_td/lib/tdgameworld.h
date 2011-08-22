/** 
* @file tdgameworld.h
* @brief class of game world, contain all monster, bullet and thing in fighting game
* @author ken
* @date 2011-08-22
*/

#ifndef __TD_GAMEWORLD_20110822_H__
#define __TD_GAMEWORLD_20110822_H__

//============================================================================//
// include
//============================================================================// 
#include "tdgameworldbase.h"
#include "tdgameobjectmanager.h"
#include "tdgameconfigdatamanager.h"

#include <libguiex_core/guiex.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CTDGameWorld : public CTDGameWorldBase
	{
	public:
		CTDGameWorld();
		virtual ~CTDGameWorld();

		virtual void InitGameWorld( CGUIWidget* pGameRoot );
		virtual void DestroyGameWorld();

	protected:
		virtual void OnRender( IGUIInterfaceRender* pRender );
		virtual void OnUpdate( real fDeltaTime );

		void UpdateAllObjects( real fDeltaTime );
		void RenderAllObjects( IGUIInterfaceRender* pRender );

		class CTDGameObjectMonster* AllocateMonster();

	protected:
		CTDGameObjectManager m_aObjectManager;
		CTDGameConfigDataManager m_aConfigDataManager;

		//game objects
		typedef std::vector<class CTDGameObjectMonster*> TMonsterObjects;
		TMonsterObjects m_arrayObjectMonster;
		typedef std::vector<class CTDGameObjectBullet*> TBulletObjects;
		TBulletObjects m_arrayObjectBullet;
	};
}

#endif //__TD_GAMEWORLD_20110822_H__
