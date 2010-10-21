/** 
* @file guiwgtradiobutton.h
* @brief radiobutton used in the system
* @author ken
* @date 2006-08-03
*/



#ifndef __GUI_WGTRADIOBUTTON_20060803_H__
#define __GUI_WGTRADIOBUTTON_20060803_H__

//============================================================================//
// include
//============================================================================// 
#include "guiwgtcheckbutton.h"
#include <libguiex_core\guiwidgetgenerator.h>



//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtRadioButton
	* @brief button class.
	* used image name:
	*		-BTN_NORMAL
	*		-BTN_NORMAL_CHECKED
	*		-BTN_HOVER
	*		-BTN_HOVER_CHECKED
	*		-BTN_DISABLE
	*		-BTN_DISABLE_CHECKED
	*		-BTN_PUSH
	*		-BTN_PUSH_CHECKED
	*/
	class GUIEXPORT CGUIWgtRadioButton : public CGUIWgtCheckButton
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtRadioButton( const CGUIString& rName, const CGUIString& rProjectName );

		static CGUIWgtRadioButton*	FromWidget( CGUIWidget* pWidget );

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtRadioButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		/// initialize radio button
		void InitRadioButton();


	protected:	//!< callback function
		virtual uint32		OnMouseLeftClick( CGUIEventMouse* pEvent );

	private:
		static CGUIString	ms_strType;
	};



	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtRadioButton);

}//namespace libguiex



#endif //__GUI_WGTRADIOBUTTON_20060803_H__

