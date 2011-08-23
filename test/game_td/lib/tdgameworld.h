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
#include <libguiex_widget_game/guiwgt_game.h>

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

		const CGUIProperty* GetDataProperty( const CGUIString& rFilename ) const;

		CGUIWgtSimplePathNode* GetSimplePathNode( const CGUIString& rPathNodeName ) const;

	protected:
		virtual void OnRender( IGUIInterfaceRender* pRender );
		virtual void OnUpdate( real fDeltaTime );

		void UpdateAllObjects( real fDeltaTime );
		void RenderAllObjects( IGUIInterfaceRender* pRender );

		class CTDGameObjectMonster* AllocateMonster( const CGUIString& rMonsterType, const CGUIString& rStartPathNode );

	protected:
		CTDGameObjectManager m_aObjectManager;
		CTDGameConfigDataManager m_aConfigDataManager;

		//game objects
		typedef std::vector<class CTDGameObjectMonster*> TMonsterObjects;
		TMonsterObjects m_arrayObjectMonster;
		typedef std::vector<class CTDGameObjectBullet*> TBulletObjects;
		TBulletObjects m_arrayObjectBullet;

		//path node
		CGUIWgtSimplePathNodeMgr* m_pSimplePathNodeMgr;
	};
}

#endif //__TD_GAMEWORLD_20110822_H__
