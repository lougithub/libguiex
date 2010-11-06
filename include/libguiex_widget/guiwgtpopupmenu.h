/** 
* @file guiwgtpopupmenu.h
* @brief popup menu
* @author ken
* @date 2007-07-31
*/
#ifndef __GUI_WGTPOPUPMENU_20070731_H__
#define __GUI_WGTPOPUPMENU_20070731_H__

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
	class CGUIMenuItem;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtPopupMenu
	* @brief popup menu
	*
	* used image name:
	* - BGIMAGE
	* - SELECTION_COLOR
	*/
	class GUIEXPORT CGUIWgtPopupMenu : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtPopupMenu( const CGUIString& rName, const CGUIString& rProjectName );

		/**
		* @brief destructor
		*/
		virtual ~CGUIWgtPopupMenu( );

		/**
		* @brief create this widget
		*/
		virtual int32 Create();

		/**
		* @brief set value
		*/
		virtual void	SetValue(const CGUIString& rName, const CGUIString& rValue);

		/**
		* @brief get widget parameter's value
		* for example: GetValue("alpha_local") will return "0.5"
		*/
		virtual CGUIString	GetValue(const CGUIString& rName) const;

		/**
		* @brief process property
		*/
		virtual void		ProcessProperty( const CGUIProperty* pProperty);



		////////////////////////////////////////////////////////////////////////////////
		// popup menu related function
		////////////////////////////////////////////////////////////////////////////////

		/**¡¡
		* @brief set position of menu, this function should be called
		* after that the size of menu won't changed
		* @param bByMousePos if this is true, the position of menu 
		* will be set by mouse's position.the following parameter will be ignored
		*/
		void	SetMenuPos( bool bByMousePos, real x=0.0f, real y=0.0f );

		/**
		* @brief set line space
		*/
		void	SetLineSpace(uint32 nSpace);

		/**
		* @brief get line space
		*/
		uint32	GetLineSpace( ) const;

		/**
		* @brief Return number of items attached to the popup menu
		*/
		uint32	GetItemCount(void) const;

		/**
		*	@brief get the item at position by given index
		*/
		CGUIMenuItem*	GetItemFromIndex(uint32 nIndex) const;


		/**
		* @brief Return the index of CGUIMenuItem item
		*/
		uint32	GetItemIndex(const CGUIMenuItem* pItem) const;

		/**
		* @brief Add the given CGUIMenuItem to the list.
		*/
		void	AddItem(CGUIMenuItem* pItem);

		/**
		* @brief Add a default text item.
		*/
		void	AddItem( const wchar_t* pText );

		/** 
		* @brief Insert an item into the popup menu after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem(CGUIMenuItem* pItem, const CGUIMenuItem* pPosition);

		/** 
		* @brief Insert an item into the popup menu after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem(CGUIMenuItem* pItem, uint32 nIndex );

		/** 
		* @brief Insert an item into the popup menu after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem( const wchar_t* pText, const CGUIMenuItem* pPosition);

		/** 
		* @brief Insert an item into the popup menu after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem( const wchar_t* pText, uint32 nIndex );

		/** 
		* @brief Removes the given item from the popup menu. 
		* If the item is has the auto delete state set, the item will be deleted.
		*/
		void	RemoveItem( CGUIMenuItem* pItem);

		/** 
		* @brief Removes the given item from the popup menu. 
		* If the item is has the auto delete state set, the item will be deleted.
		*/
		void	RemoveItem( uint32 nIndex);

		/**
		* @brief Remove all items from the list.
		*/
		void	ClearList(void);

		/**
		* @brief return whether tooltips is enabled
		*/
		bool	IsItemTooltipsEnabled(void) const;

		/**
		* @brief Set whether the tooptips should be shown
		*/
		void	EnableItemTooltips(bool bEnable);

		/**
		* @brief Search the list for an item with the specified text
		* @param rText the text to be searched for.
		* @param pStartItem indicated where the search is to begin, the search
		*  will not include this item.  If it isNULL, the search will begin from the first item in the list.
		*/
		CGUIMenuItem*	FindItemWithText(
			const wchar_t* pText, const CGUIMenuItem* pStartItem = NULL);

		/**
		* @brief Return whether the specified CGUIMenuItem is in the List
		*/
		bool	IsMenuItemInList(const CGUIMenuItem* pItem) const;

		/**
		* @brief Set whether the list should be sorted.
		*/
		void	SetSorting(bool bSorting);

		/**
		* @brief Get whether the list should be sorted.
		*/
		bool 	IsSorting() const;

	protected:

		/**
		* @brief constructor as a base class
		*/
		CGUIWgtPopupMenu( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		/**
		* @brief update this widget only
		*/
		virtual void	UpdateSelf( real fDeltaTime );

		///initialize 
		void	InitPopupMenu();

		/// get item under point, return null if there isn't item
		CGUIMenuItem*	GetItemUnderPoint(const CGUIVector2& rPos);

		/**
		* @brief clear all selected item and don't send event for selected item change
		* @return true if selected item changes
		*/
		bool	ClearAllSelections_impl(void);

		/**
		* @brief remove all item without send event.
		* @return true if any item was removed
		*/
		bool	ClearList_impl(void);

		///update item size when it changed
		void	UpdateMenuItem();

		/**
		* @brief get total height of items
		*/
		real	GetTotalItemsHeight() const;

		/**
		* @brief get widest width of items
		*/
		real	GetWidestItemsWidth() const;

		//add a child
		void	AddChild_Imp(CGUIMenuItem* pItem);

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName,CGUIImage* pImage );

		//create a item
		CGUIMenuItem* CreateItem_Imp(const wchar_t* pText);


	protected:	//!< callback function
		virtual uint32		OnMouseMove( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );

		friend class CGUIMenuItem;
		virtual void		ProcessMouseLeftUp(CGUIMenuItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseMove(CGUIMenuItem* pItem, CGUIEventMouse* pEvent);

	protected:
		typedef std::vector<CGUIMenuItem*>	TListItem;
		TListItem			m_aListItems;				//!< list of items in the popup menu.

		bool				m_bItemTooltips;			//!< true if each item should have an individual tooltip

		CGUIImage*			m_pImageBG;					/// image for listbox's background
		CGUIImage*			m_pImageSelection;			/// image for listbox's selection color

		bool				m_bSorted;					//!< true if list is sorted

		uint32				m_nLineSpace;				//!< line space

		CGUIString			m_strItemType;				/// type of item, default is text item

	private:
		static CGUIString	ms_strType;
	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtPopupMenu);


}//namespace guiex

#endif //__GUI_WGTPOPUPMENU_20070731_H__
