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
#include <libguiex_core/guiwidgetgenerator.h>



//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtRadioButton
	* @brief button class.
	* used image name:
	*		-normal
	*		-normal_checked
	*		-hover
	*		-hover_checked
	*		-disable
	*		-disable_checked
	*		-push
	*		-push_checked
	*/
	class GUIEXPORT CGUIWgtRadioButton : public CGUIWgtCheckButton
	{
	public:
		CGUIWgtRadioButton( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		CGUIWgtRadioButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		/// initialize radio button
		void InitRadioButton();

	protected:	//!< callback function
		virtual uint32 OnMouseLeftClick( CGUIEventMouse* pEvent );

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtRadioButton);
	};

}//namespace libguiex



#endif //__GUI_WGTRADIOBUTTON_20060803_H__

