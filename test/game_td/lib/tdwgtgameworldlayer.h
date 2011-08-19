/** 
* @file tdwgtgameworldlayer.h
* @brief used to render monster and other things.
* @author ken
* @date 2011-08-19
*/

#ifndef __TD_WGTGAMEWORLDLAYER_20110819_H__
#define __TD_WGTGAMEWORLDLAYER_20110819_H__

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
	* @class TDWgtGameWorldLayer
	* @brief used to render monster and other things.
	*/
	class GUIEXPORT TDWgtGameWorldLayer : public CGUIWidget
	{
	public:
		TDWgtGameWorldLayer( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		void InitGameLayer();

	protected:	//!< callback function

	protected:

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(TDWgtGameWorldLayer);
	};
}//namespace guiex

#endif //__TD_WGTGAMEWORLDLAYER_20110819_H__

