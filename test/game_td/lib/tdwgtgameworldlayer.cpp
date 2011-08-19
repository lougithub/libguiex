/** 
* @file tdwgtgameworldlayer.cpp
* @brief used to render monster and other things.
* @author ken
* @date 2011-08-19
*/



//============================================================================//
// include 
//============================================================================// 
#include "tdwgtgameworldlayer.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(TDWgtGameWorldLayer);
	//------------------------------------------------------------------------------
	TDWgtGameWorldLayer::TDWgtGameWorldLayer( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitGameLayer();
	}
	//------------------------------------------------------------------------------
	void TDWgtGameWorldLayer::InitGameLayer()
	{
	}
	//------------------------------------------------------------------------------
}//namespace guiex

