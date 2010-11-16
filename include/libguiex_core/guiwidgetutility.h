/** 
* @file guiwidgetutility.h
* @brief utility function for widget.
* @author ken
* @date 2006-09-27
*/

#ifndef __GUI_WIDGET_UTILITY_H_20060927__
#define __GUI_WIDGET_UTILITY_H_20060927__


//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiwidget.h"



//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUIWidgetUtility
	* @brief some useful function for widget
	*/
	class GUIEXPORT CGUIWidgetUtility
	{
	public:
		/**
		* @brief get widget's information, the format of it is:
		* NAME<> TYPE<>	STATE<>
		*/
		static CGUIString	GetWidgetInfo( const CGUIWidget* pWidget );
		static CGUIString	GetWidgetInfo( const CGUIString& rWidgetName, const CGUIString& rSceneName );

		/**
		* @brief get widget's and it's child's information, the format of it is:
		* NAME<> TYPE<>	STATE<>
		*		NAME<> TYPE<>	STATE<>
		*			NAME<> TYPE<>	STATE<>
		*			NAME<> TYPE<>	STATE<>
		*		NAME<> TYPE<>	STATE<>
		*/
		static CGUIString	GetWidgetTreeInfo( const CGUIWidget* pWidget );
		static CGUIString	GetWidgetTreeInfo( const CGUIString& rWidgetName, const CGUIString& rSceneName );

	};

	

}//namespace guiex

#endif //__GUI_WIDGET_UTILITY_H_20060927__
