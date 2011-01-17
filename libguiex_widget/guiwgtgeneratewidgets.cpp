/** 
* @file guiwgtgeneratewidgets.cpp
* @brief used to generate all widgets
* @author ken
* @date 2006-08-03
*/



//============================================================================//
// include
//============================================================================// 
#include <vector>
#include <libguiex_core/guiwidgetgenerator.h>
#include <libguiex_widget/guiwgt.h>

//============================================================================//
// implement
//============================================================================// 
namespace guiex
{
#if defined( GUIEX_PLATFORM_WIN32)
	extern "C"  GUIEXPORT CGUIWidgetGenerator** GetAllWidgetGenerators() 
	{ 
		static std::vector<CGUIWidgetGenerator*> aList; 
		aList.clear();

#define GUI_ADD_WIDGET_GENERATOR( widget ) \
	aList.push_back( widget::widget##_Generator::GenerateSelf());

		GUI_ADD_WIDGET_GENERATOR(CGUIWgtButton);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtCheckButton);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtRadioButton);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtStaticText);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtStaticImage);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtParticle2D);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtEditBox);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtMultiEditBox);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtPanel);
		//GUI_ADD_WIDGET_GENERATOR(CGUIWgtFrame);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtScrollbar);
		//GUI_ADD_WIDGET_GENERATOR(CGUIWgtScrollPanel);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtEmptyNode);
		//GUI_ADD_WIDGET_GENERATOR(CGUIWgtAnimation);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtDialog);
		//GUI_ADD_WIDGET_GENERATOR(CGUIWgtListBox);
		//GUI_ADD_WIDGET_GENERATOR(CGUIWgtComboBox);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtProgress);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtScrollbarContainer);
		//GUI_ADD_WIDGET_GENERATOR(CGUIListBoxTextItem);
		//GUI_ADD_WIDGET_GENERATOR(CGUIMenuTextItem);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtTabControl);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtTabButton);
		//GUI_ADD_WIDGET_GENERATOR(CGUIWgtPopupMenu);
		//GUI_ADD_WIDGET_GENERATOR(CGUIWgtColumnList);
		//GUI_ADD_WIDGET_GENERATOR(CGUIColListHeader);
		//GUI_ADD_WIDGET_GENERATOR(CGUIColListHeaderSegment);
		//GUI_ADD_WIDGET_GENERATOR(CGUIColListTextItem);

#undef GUI_ADD_WIDGET_GENERATOR

		aList.push_back(NULL);
		return (CGUIWidgetGenerator**)&aList[0];
	}

#elif defined(GUIEX_PLATFORM_MAC)
	extern "C"  GUIEXPORT CGUIWidgetGenerator** GetAllWidgetGenerators() 
	{ 
		static std::vector<CGUIWidgetGenerator*> aList; 
		aList.clear();

#define GUI_ADD_WIDGET_GENERATOR( widget ) \
	aList.push_back( widget::widget##_Generator::GenerateSelf());

		GUI_ADD_WIDGET_GENERATOR(CGUIWgtEmptyNode);		
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtScrollbar);	
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtScrollbarContainer);		
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtStaticImage);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtParticle2D);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtStaticText);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtPanel);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtButton);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtCheckButton);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtRadioButton);
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtEditBox);		
		GUI_ADD_WIDGET_GENERATOR(CGUIWgtMultiEditBox);		
#undef GUI_ADD_WIDGET_GENERATOR

		aList.push_back(NULL);
		return (CGUIWidgetGenerator**)&aList[0];
	}
#else
#	error "unknown platform"	
#endif

}//namespace guiex

