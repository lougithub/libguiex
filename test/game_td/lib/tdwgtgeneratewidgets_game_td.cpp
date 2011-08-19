/** 
* @file tdwgtgeneratewidgets_game_td.cpp
* @brief used to generate all widgets
* @author ken
* @date 2011-08-17
*/

//============================================================================//
// include
//============================================================================// 
#include <vector>
#include <libguiex_core/guiwidgetgenerator.h>
#include "tdwgt_game_td.h"


//============================================================================//
// implement
//============================================================================// 
namespace guiex
{
	GUIEXPORT CGUIWidgetGenerator** GetAllWidgetGenerators_Game_TD() 
	{ 
		static std::vector<CGUIWidgetGenerator*> aList; 
		aList.clear();

#define GUI_ADD_WIDGET_GENERATOR( widget ) \
	aList.push_back( widget::widget##_Generator::GenerateSelf());

		GUI_ADD_WIDGET_GENERATOR(TDWgtTower);
		GUI_ADD_WIDGET_GENERATOR(TDWgtGameWorldLayer);

#undef GUI_ADD_WIDGET_GENERATOR

		aList.push_back(NULL);
		return (CGUIWidgetGenerator**)&aList[0];
	}

}//namespace guiex

