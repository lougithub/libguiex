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
#include <libguiex_widget/guiwgt.h>
#include <libguiex_widget_game/guiwgt_game.h>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	extern class CTDGameWorld* GGameWorld;

	extern class CTDGameWorld* GetGameWorld();
}

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

		virtual void InitGameWorld( const CGUIString& rRound, CGUIWidget* pGameRoot, CGUIWidget* pTowerSelectPanel, CGUIWidget* pTowerUpgradePanel );
		virtual void DestroyGameWorld();

		const CGUIProperty* GetDataProperty( const CGUIString& rFilename ) const;

		CGUIWgtSimplePathNode* GetSimplePathNode( const CGUIString& rPathNodeName ) const;

		//towe relative functions
		class CTDGameTower* GetCurrentFocusTower() const;
		void SetCurrentFocusTower( class CTDGameTower* pTower );
		CGUIWidget* GetTowerSelectPanel() const;
		CGUIWidget* GetTowerUpgradePanel() const;

		uint32 GetGold() const;
		void CostGold( uint32 uGold );
		uint32 GetHP() const;

		void AllocateBullet(const CGUIString& rBulletType, const CGUIVector2& rStartPos, class CTDGameObjectMonster* pTarget);
		void AllocateMonster( const CGUIString& rMonsterType, const CGUIString& rStartPathNode );

	protected:
		virtual void OnRender( IGUIInterfaceRender* pRender );
		virtual void OnUpdate( real fDeltaTime );

		void UpdateAllObjects( real fDeltaTime );
		void RenderAllObjects( IGUIInterfaceRender* pRender );
		void RemoveDeadObject();

		void InitRound( const CGUIString& rRound );

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

		//tower
		class CTDGameTower* m_pCurrentFocusTower;
		CGUIWidget* m_pTowerSelectPanel;
		CGUIWidget* m_pTowerUpgradePanel;

		//money
		uint32 m_uGold;
		uint32 m_uHP;
	};
}

#endif //__TD_GAMEWORLD_20110822_H__
