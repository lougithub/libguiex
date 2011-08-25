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
		CTDGameTower( class CTDGameWorld * pGameWorld, const CGUIVector2& rAnchorPoint );
		~CTDGameTower();

		void OnUpdate( real fDeltaTime );
		void OnRender( class IGUIInterfaceRender* pRender );

		void OnMouseEnter();
		void OnMouseLeave();

		class CTDGameWorld* GetGameWorld() const;

		void SetSize( const CGUISize& rSize );
		
		const CGUIRect& GetRenderRect() const;
		const CGUIColor& GetRenderColor() const;

		void SetTowerType( ETowerType eType );
		ETowerType GetTowerType( ) const;

	protected:
		class CTDGameWorld * m_pGameWorld;
		ETowerType m_eTowerType;
		
		CGUIVector2 m_aAnchorPoint;
		CGUISize m_aSize;

		CGUIRect m_aRenderRect;
		CGUIColor m_aRenderColor;

		class CTDGameTowerImplement* m_pTowerImplement[__eTowerType_MAX__];
	};
}//namespace guiex

#endif //__TD_GAMETOWER_20110824_H__

