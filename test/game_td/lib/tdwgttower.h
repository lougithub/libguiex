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
	* @class TDWgtTower
	* @brief tower of game
	* used image name:
	*		-tower_base
	*/
	class GUIEXPORT TDWgtTower : public CGUIWidget
	{
	public:
		TDWgtTower( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		void InitTower();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:
		CGUIImage* m_pImageTowerBase;

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(TDWgtTower);
	};
}//namespace guiex

#endif //__TD_WGTTOWER_20110817_H__

