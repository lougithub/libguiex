/** 
* @file guicollistitem.h
* @brief column list item, base class
* @author ken
* @date 2007-08-02
*/

#ifndef __GUI_COLLISTITEM_20070802_H__
#define __GUI_COLLISTITEM_20070802_H__

//============================================================================//
// include
//============================================================================// 
#include "guiitembase.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWgtColumnList;
}//namespace guiex

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIColListItem
	* @brief base item for list box
	* used image name:
	*/
	class GUIEXPORT CGUIColListItem : public CGUIItemBase
	{
	public:
		/**
		* @brief base class constructor
		*/
		CGUIColListItem( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief base class destructor
		*/
		virtual ~CGUIColListItem(void);

		//get list box ptr
		CGUIWgtColumnList*	GetListBox();

		void		SetWidth(real fWidth);

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
		CGUIColListItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		//init item
		void	InitListboxItem();

		//render self
		virtual void RenderSelf(IGUIInterfaceRender* pRender);


	protected:


	private:
		static CGUIString	ms_strType;
	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIColListItem);

}//namespace guiex

#endif //__GUI_COLLISTITEM_20070802_H__
