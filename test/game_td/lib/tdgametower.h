/** 
* @file tdgametower.h
* @brief tower of td game
* @author ken
* @date 2011-08-24
*/

#ifndef __TD_GAMETOWER_20110824_H__
#define __TD_GAMETOWER_20110824_H__

//============================================================================//
// include
//============================================================================// 
#include "tdgametype.h"
#include <libguiex_core/guiex.h>
//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CTDGameTower
	*/
	class CTDGameTower
	{
	public:
		CTDGameTower( class CTDWgtTower* pWidgetTower, class CTDGameWorld * pGameWorld, const CGUIVector2& rAnchorPoint );
		~CTDGameTower();

		void OnUpdate( real fDeltaTime );
		void OnRender( class IGUIInterfaceRender* pRender );

		void OnMouseEnter();
		void OnMouseLeave();
		void OnMouseLeftClick();

		bool IsMouseOver() const;

		class CTDGameWorld* GetGameWorld() const;

		void SetSize( const CGUISize& rSize );
		
		const CGUIRect& GetRenderRect() const;
		const CGUIColor& GetRenderColor() const;

		ETowerType GetTowerType( ) const;

		bool CouldBuild( ETowerType eType ) const;
		void BuildTower(ETowerType eType);
		bool CouldUpgrade( );
		void UpgradeTower( );
		void SellTower( );

		uint32 GetBuildCost( ETowerType eType ) const;
		uint32 GetUpgradeCost() const;
		uint32 GetSellCost() const;

		void CloseTowerSelectPanel();
		void CloseTowerUpgradePanel();

		const CGUIVector2& GetPosition() const;

	protected:
		void SetTowerType( ETowerType eType );

	protected:
		class CTDWgtTower *m_pWidgetTower;
		class CTDGameWorld * m_pGameWorld;
		ETowerType m_eTowerType;
		
		CGUIVector2 m_aAnchorPoint;
		CGUISize m_aSize;

		CGUIVector2 m_aPosition;
		CGUIRect m_aRenderRect;
		CGUIColor m_aRenderColor;

		bool m_bMouseOver;

		class CTDGameTowerImplement* m_pTowerImplement[__eTowerType_MAX__];
	};
}//namespace guiex

#endif //__TD_GAMETOWER_20110824_H__

