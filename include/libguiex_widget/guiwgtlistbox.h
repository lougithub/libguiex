/** 
* @file guiwgtlistbox.h
* @brief list box
* @author ken
* @date 2006-09-18
*/
#ifndef __GUI_WGTLISTBOX_20060918_H__
#define __GUI_WGTLISTBOX_20060918_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiwidget.h>
#include <libguiex_core\guiwidgetgenerator.h>
#include "guiwgtscrollbarcontainer.h"

#include <vector>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIListBoxItem;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtListBox
	* @brief list box
	*
	* used image name:
	* - BGIMAGE
	* - SELECTION_COLOR
	*
	* - SCROLLBAR_VERTIC_BG
	* - SCROLLBAR_VERTIC_ARROW_NORMAL_0
	* - SCROLLBAR_VERTIC_ARROW_HOVER_0
	* - SCROLLBAR_VERTIC_ARROW_DISABLE_0
	* - SCROLLBAR_VERTIC_ARROW_PUSH_0
	* - SCROLLBAR_VERTIC_ARROW_NORMAL_1
	* - SCROLLBAR_VERTIC_ARROW_HOVER_1
	* - SCROLLBAR_VERTIC_ARROW_DISABLE_1
	* - SCROLLBAR_VERTIC_ARROW_PUSH_1
	* - SCROLLBAR_VERTIC_SLIDE_NORMAL
	* - SCROLLBAR_VERTIC_SLIDE_HOVER
	* - SCROLLBAR_VERTIC_SLIDE_DISABLE
	* - SCROLLBAR_VERTIC_SLIDE_PUSH
	*
	* - SCROLLBAR_HORIZON_BG
	* - SCROLLBAR_HORIZON_ARROW_NORMAL_0
	* - SCROLLBAR_HORIZON_ARROW_HOVER_0
	* - SCROLLBAR_HORIZON_ARROW_DISABLE_0
	* - SCROLLBAR_HORIZON_ARROW_PUSH_0
	* - SCROLLBAR_HORIZON_ARROW_NORMAL_1
	* - SCROLLBAR_HORIZON_ARROW_HOVER_1
	* - SCROLLBAR_HORIZON_ARROW_DISABLE_1
	* - SCROLLBAR_HORIZON_ARROW_PUSH_1
	* - SCROLLBAR_HORIZON_SLIDE_NORMAL
	* - SCROLLBAR_HORIZON_SLIDE_HOVER
	* - SCROLLBAR_HORIZON_SLIDE_DISABLE
	* - SCROLLBAR_HORIZON_SLIDE_PUSH
	*
	*/
	class GUIEXPORT CGUIWgtListBox : public CGUIWgtScrollbarContainer
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtListBox( const CGUIString& rName, const CGUIString& rProjectName );

		/**
		* @brief destructor
		*/
		virtual ~CGUIWgtListBox( );

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
		// list box related function
		////////////////////////////////////////////////////////////////////////////////
		/**
		* @brief Return number of items attached to the list box
		*/
		uint32	GetItemCount(void) const;

		/**
		* @brief Return number of selected items attached to the list box
		*/
		uint32	GetSelectedCount(void) const;

		/**
		*brief Return a pointer to the first selected item.
		*/
		CGUIListBoxItem*	GetFirstSelectedItem(void) const;

		/**
		* @brief get the next selected item.
		*	@param pStartItem the pointer of item  where the next item of this item will 
		* be searched. if it's NULL, the first selected item will be return
		*/
		CGUIListBoxItem*	GetNextSelectedItem(const CGUIListBoxItem* pStartItem) const;

		/**
		* @brief get the next selected item by index
		*/
		CGUIListBoxItem*	GetSelectedItem( uint32 nIdx) const;

		/**
		* @brief Set the select state of an attached CGUIListBoxItem.
		* @param bSelected
		*		- true to select the item, 
		*		- false to de-select the item.
		*/
		void	SetItemSelected(CGUIListBoxItem* pItem, bool bSelected);

		/**
		* @brief Set the select state of an attached CGUIListBoxItem.
		*/
		void	SetItemSelected(uint32 nItemIndex, bool bSelected);

		/**
		* @brief Clear the selected state for all items.
		*/
		void	ClearAllSelections(void);

		/**
		*	@brief get the item at position by given index
		*/
		CGUIListBoxItem*	GetItemFromIndex(uint32 nIndex) const;


		/**
		* @brief Return the index of CGUIListBoxItem item
		*/
		uint32	GetItemIndex(const CGUIListBoxItem* pItem) const;

		/**
		* @brief Add the given CGUIListBoxItem to the list.
		*/
		void	AddItem(CGUIListBoxItem* pItem);

		/**
		* @brief Add a default text item.
		*/
		void	AddItem( const wchar_t* pText );

		/** 
		* @brief Insert an item into the list box after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem(CGUIListBoxItem* pItem, const CGUIListBoxItem* pPosition);

		/** 
		* @brief Insert an item into the list box after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem(CGUIListBoxItem* pItem, uint32 nIndex );

		/** 
		* @brief Insert an item into the list box after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem( const wchar_t* pText, const CGUIListBoxItem* pPosition);

		/** 
		* @brief Insert an item into the list box after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem( const wchar_t* pText, uint32 nIndex );

		/** 
		* @brief Removes the given item from the list box. 
		* If the item is has the auto delete state set, the item will be deleted.
		*/
		void	RemoveItem( CGUIListBoxItem* pItem);

		/** 
		* @brief Removes the given item from the list box. 
		* If the item is has the auto delete state set, the item will be deleted.
		*/
		void	RemoveItem( uint32 nIndex);

		/**
		* @brief Remove all items from the list.
		*/
		void	ClearList(void);


		/**
		* @brief Set whether the list should allow multiple selections or just a single selection
		*/
		void	EnableMultiselect(bool bEnable);

		/**
		* @brief return whether multi-select is enabled
		*/
		bool	IsMultiselectEnabled(void) const;

		/**
		* @brief return whether tooltips is enabled
		*/
		bool	IsItemTooltipsEnabled(void) const;

		/**
		* @brief Set whether the tooptips should be shown
		*/
		void	EnableItemTooltips(bool bEnable);

		/**
		* @brief return whether the string at index position index is selected
		*/
		bool	IsItemSelected(uint32 nIndex) const;

		/**
		* @brief Search the list for an item with the specified text
		* @param rText the text to be searched for.
		* @param pStartItem indicated where the search is to begin, the search
		*  will not include this item.  If it isNULL, the search will begin from the first item in the list.
		*/
		CGUIListBoxItem*	FindItemWithText(
			const wchar_t* pText, const CGUIListBoxItem* pStartItem = NULL);

		/**
		* @brief Return whether the specified CGUIListBoxItem is in the List
		*/
		bool	IsListboxItemInList(const CGUIListBoxItem* pItem) const;

		/**
		* @brief Set whether the list should be sorted.
		*/
		void	SetSorting(bool bSorting);

		/**
		* @brief Get whether the list should be sorted.
		*/
		bool 	IsSorting() const;

		/**
		* @brief make the specified item visible
		*/
		void	MakeItemVisible(uint32 nItemIndex);

		/**
		* @brief make the specified item visible
		*/
		void	MakeItemVisible(const CGUIListBoxItem* pItem);

	protected:

		/**
		* @brief constructor as a base class
		*/
		CGUIWgtListBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		//update rect, which may different for different widget
		virtual void		UpdateDirtyRect();		

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName,CGUIImage* pImage );

		///initialize check button
		void	InitListBox();

		/// get item under point, return null if there isn't item
		CGUIListBoxItem*	GetItemUnderPoint(const CGUIVector2& rPos);

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
		void	UpdateItemPos();

		/**
		* @brief get total height of items
		*/
		real	GetTotalItemsHeight() const;

		/**
		* @brief get widest width of items
		*/
		real	GetWidestItemsWidth() const;

		//add a child
		void	AddChild_Imp(CGUIListBoxItem* pItem);

		//create a item
		CGUIListBoxItem* CreateItem_Imp(const wchar_t* pText);

	protected:	//!< callback function

		virtual uint32		OnMouseMove( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightUp( CGUIEventMouse* pEvent );

		friend class CGUIListBoxItem;
		virtual void		ProcessMouseLeftDown(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseLeftUp(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseRightDown(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseRightUp(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseLeftClick(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseRightClick(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseLeftDbClick(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseRightDbClick(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseMove(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);

	protected:
		typedef std::vector<CGUIListBoxItem*>	TListItem;
		TListItem	m_aListItems;			//!< list of items in the list box.
		//TListboxItem*	m_aLastSelected;	//!< holds pointer to the last selected item (used in range selections)

		bool	m_bSorted;						//!< true if list is sorted
		bool	m_bMultiselect;					//!< true if multi-select is enabled
		bool	m_bItemTooltips;				//!< true if each item should have an individual tooltip

		CGUISize			m_aWorkAreaSize;			///size of work area

		CGUIImage*			m_pImageBG;					/// image for listbox's background
		CGUIImage*			m_pImageSelection;			/// image for listbox's selection color

		CGUIListBoxItem*	m_pLastOperateItem;			/// the last item which is been operated.for multiselect

		CGUIString			m_strItemType;				/// type of item, default is text item

	private:
		static CGUIString	ms_strType;
	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtListBox);


}//namespace guiex

#endif //__GUI_WGTFLISTBOX_20060918_H__
