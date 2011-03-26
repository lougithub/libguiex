/** 
* @file guiwgtgeneratewidgets_game.cpp
* @brief used to generate all widgets
* @author ken
* @date 2011-01-26
*/

//============================================================================//
// include
//============================================================================// 
#include <vector>
#include <libguiex_core/guiwidgetgenerator.h>
#include "guiwgt_game.h"


//============================================================================//
// implement
//============================================================================// 
namespace guiex
{
	extern "C"  GUIEXPORT CGUIWidgetGenerator** GetAllWidgetGenerators_Game() 
	{ 
		static std::vector<CGUIWidgetGenerator*> aList; 
		aList.clear();

#define GUI_ADD_WIDGET_GENERATOR( widget ) \
	aList.push_back( widget::widget##_Generator::GenerateSelf());

		GUI_ADD_WIDGET_GENERATOR(CGUIWgtParticle2D);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtTiledMap);

#undef GUI_ADD_WIDGET_GENERATOR

		aList.push_back(NULL);
		return (CGUIWidgetGenerator**)&aList[0];
	}

}//namespace guiex

