/** 
* @file guiwgttabcontrol.h
* @brief tab control panel
* @author ken
* @date 2007-07-09
*/



#ifndef __GUI_WGTTABCONTROL_20070709_H__
#define __GUI_WGTTABCONTROL_20070709_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>
#include <vector>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWgtTabButton;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtTabControl
	* @brief controller for tab panel
	*/
	class GUIEXPORT CGUIWgtTabControl : public CGUIWidget
	{
	public:
		CGUIWgtTabControl( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief Set the selected tab by the name of the tab button.
		*/
		void SetSelectedTab(const CGUIString &name);

		/**
		 * @brief Set the selected tab by the index position in the tab control.
		 */
		void SetSelectedTab(uint32 index);
		
	protected:
		CGUIWgtTabControl( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		void InitTabControl();

	protected:	//!< callback function
		virtual uint32 OnAddChild( CGUIEventRelativeChange* pEvent );
		virtual uint32 OnRemoveChild( CGUIEventRelativeChange* pEvent );

	protected:
		typedef	std::vector<CGUIWgtTabButton*>	TVecBtn;
		TVecBtn	m_vecButtons;			//only tab button could become control's child

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtTabControl);
	};
}//namespace libguiex

#endif //__GUI_WGTTABCONTROL_20070709_H__
