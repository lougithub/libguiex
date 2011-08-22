/** 
* @file tdwgttower.h
* @brief tower of td game
* @author ken
* @date 2011-08-17
*/

#ifndef __TD_WGTTOWER_20110817_H__
#define __TD_WGTTOWER_20110817_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CTDWgtTower
	* @brief tower of game
	* used image name:
	*		-tower_base
	*/
	class GUIEXPORT CTDWgtTower : public CGUIWidget
	{
	public:
		CTDWgtTower( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		void InitTower();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:	//!< callback function
		virtual uint32 OnMouseEnter( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeave( CGUIEventMouse* pEvent );

	protected:
		CGUIImage* m_pImageTowerBase;

		bool m_bHovering;

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CTDWgtTower);
	};
}//namespace guiex

#endif //__TD_WGTTOWER_20110817_H__

