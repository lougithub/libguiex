/** 
* @file guiwgttabbutton.h
* @brief tabbutton used in the system
* @author ken
* @date 2007-07-06
*/



#ifndef __GUI_WGTRADIOBUTTON_20070706_H__
#define __GUI_WGTRADIOBUTTON_20070706_H__

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
	* @class CGUIWgtTabButton
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
	class GUIEXPORT CGUIWgtTabButton : public CGUIWgtCheckButton
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtTabButton( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief
		* Return whether this tab button is selected or not
		*/
		bool IsSelected(void) const;


	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtTabButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		/// initialize radio button
		void InitTabButton();

		virtual void		RefreshSelf();

	protected:	//!< callback function
		virtual uint32		OnMouseLeftClick( CGUIEventMouse* pEvent );

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtTabButton);
	};

}//namespace libguiex



#endif //__GUI_WGTRADIOBUTTON_20070706_H__

