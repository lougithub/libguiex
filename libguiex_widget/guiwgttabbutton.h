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
	*		-normal
	*		-normal_checked
	*		-hover
	*		-hover_checked
	*		-disable
	*		-disable_checked
	*		-push
	*		-push_checked
	*/
	class GUIEXPORT CGUIWgtTabButton : public CGUIWgtCheckButton
	{
	public:
		CGUIWgtTabButton( const CGUIString& rName, const CGUIString& rSceneName );

		bool IsSelected(void) const;

	protected:
		CGUIWgtTabButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		/// initialize radio button
		void InitTabButton();

		virtual void RefreshSelf();

	protected:	//!< callback function
		virtual uint32 OnMouseLeftClick( CGUIEventMouse* pEvent );

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtTabButton);
	};

}//namespace libguiex

#endif //__GUI_WGTRADIOBUTTON_20070706_H__

