/** 
* @file guimenuitem.h
* @brief item for menu 
* @author ken
* @date 2007-07-31
*/

#ifndef __GUI_MENUITEM_20070731_H__
#define __GUI_MENUITEM_20070731_H__

//============================================================================//
// include
//============================================================================// 
#include "guiitembase.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWgtPopupMenu;
}//namespace guiex

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIMenuItem
	* @brief base item for list box
	* used image name:
	*/
	class GUIEXPORT CGUIMenuItem : public CGUIItemBase
	{
	public:
		/**
		* @brief base class constructor
		*/
		CGUIMenuItem( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief base class destructor
		*/
		virtual ~CGUIMenuItem(void);

		//get list box ptr
		CGUIWgtPopupMenu*	GetPopupMenu();

	protected:	//!< callback function
		virtual uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseMove( CGUIEventMouse* pEvent );

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIMenuItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		//init item
		void	InitMenuItem();

		//render self
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIMenuItem);
	};


}//namespace guiex

#endif //__GUI_MENUITEM_20070731_H__
