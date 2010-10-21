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
#include <libguiex_core\guiwidgetgenerator.h>
#include <libguiex_widget\guiwgt.h>





//============================================================================//
// implement
//============================================================================// 
namespace guiex
{
	GUI_WIDGET_ALLGENERATOR_IMPLEMENT_BEGIN( )
	{
		GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtButton)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtCheckButton)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtRadioButton)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtStaticText)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtStaticImage)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtEditBox)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtMultiEditBox)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtPanel)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtFrame)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtScrollbar)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtScrollPanel)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtEmptyNode)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtAnimation)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtDialog)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtListBox)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtComboBox)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtProgress)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtScrollbarContainer)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIListBoxTextItem)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIMenuTextItem)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtTabControl)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtTabButton)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtPopupMenu)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIWgtColumnList)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIColListHeader)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIColListHeaderSegment)
			GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD(CGUIColListTextItem)
	}
	GUI_WIDGET_ALLGENERATOR_IMPLEMENT_END( )

}//namespace guiex

