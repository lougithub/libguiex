/** 
* @file guiwgtgeneratewidgets_box2d.cpp
* @brief used to generate all widgets
* @author ken
* @date 2010-12-21
*/



//============================================================================//
// include
//============================================================================// 
#include <vector>
#include <libguiex_core/guiwidgetgenerator.h>
#include "guiwgt_box2d.h"





//============================================================================//
// implement
//============================================================================// 
namespace guiex
{
	extern "C"  GUIEXPORT CGUIWidgetGenerator** GetAllWidgetGenerators_Box2d() 
	{ 
		static std::vector<CGUIWidgetGenerator*> aList; 
		aList.clear();

#define GUI_ADD_WIDGET_GENERATOR( widget ) \
	aList.push_back( widget::widget##_Generator::GenerateSelf());

		GUI_ADD_WIDGET_GENERATOR(CGUIWgtBox2DDynamicBody);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtBox2DStaticBody);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtBox2DJoint);

#undef GUI_ADD_WIDGET_GENERATOR

		aList.push_back(NULL);
		return (CGUIWidgetGenerator**)&aList[0];
	}

}//namespace guiex

