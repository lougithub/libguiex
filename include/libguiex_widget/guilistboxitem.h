/** 
* @file guilistboxitem.h
* @brief list box item, base class
* @author ken
* @date 2006-09-18
*/

#ifndef __GUI_LISTBOXITEM_20060918_H__
#define __GUI_LISTBOXITEM_20060918_H__

//============================================================================//
// include
//============================================================================// 
#include "guiitembase.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWgtListBox;
}//namespace guiex

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIListBoxItem
	* @brief base item for list box
	* used image name:
	*/
	class GUIEXPORT CGUIListBoxItem : public CGUIItemBase
	{
	public:
		/**
		* @brief base class constructor
		*/
		CGUIListBoxItem( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief base class destructor
		*/
		virtual ~CGUIListBoxItem(void);

		//get list box ptr
		CGUIWgtListBox*	GetListBox();


	protected:	//!< callback function
		virtual uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightUp( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightClick( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftClick( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightDbClick( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftDbClick( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseMove( CGUIEventMouse* pEvent );


	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIListBoxItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		//init item
		void	InitListboxItem();

		//render self
		virtual void RenderSelf(IGUIInterfaceRender* pRender);


	protected:


	private:
		static CGUIString	ms_strType;
	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIListBoxItem);

}//namespace guiex

#endif //__GUI_LISTBOXITEM_20060918_H__
