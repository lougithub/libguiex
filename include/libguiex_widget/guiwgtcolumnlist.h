/** 
* @file guiwgtcolumnlist.h
* @brief column list box
* @author ken
* @date 2007-08-02
*/
#ifndef __GUI_WGTCOLUMNLIST_20070802_H__
#define __GUI_WGTCOLUMNLIST_20070802_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>
#include "guiwgtscrollbarcontainer.h"

#include <vector>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIColListHeader;
	class CGUIColListItem;
	class CGUIColListHeaderSegment;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	struct GUIEXPORT SGridRef
	{
		SGridRef(uint32 nRow, uint32 nColumn);

		uint32	m_nRow;			//!< Zero based row index.
		uint32	m_nColumn;		//!< Zero based column index.

		// operators
		SGridRef& operator=(const SGridRef& rhs);
		bool operator<(const SGridRef& rhs) const;
		bool operator<=(const SGridRef& rhs) const;
		bool operator>(const SGridRef& rhs) const;
		bool operator>=(const SGridRef& rhs) const;
		bool operator==(const SGridRef& rhs) const;
		bool operator!=(const SGridRef& rhs) const;
	};


	/**
	* @class CGUIWgtColumnList
	* @brief list box
	*
	* used image name:
	* - BGIMAGE
	* - SELECTION_COLOR
	*
	* - COLUMN_HEADER_NORMAL
	* - COLUMN_HEADER_HOVER
	* - COLUMN_HEADER_PUSH
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
	class GUIEXPORT CGUIWgtColumnList : public CGUIWgtScrollbarContainer
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtColumnList( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief destructor
		*/
		virtual ~CGUIWgtColumnList( );

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
		* @brief load widget config from property
		*/
		virtual void			ProcessProperty( const CGUIProperty* pProperty);


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
		CGUIColListItem*	GetFirstSelectedItem(void) const;

		/**
		* @brief get the next selected item.
		*	@param pStartItem the pointer of item  where the next item of this item will 
		* be searched. if it's NULL, the first selected item will be return
		*/
		CGUIColListItem*	GetNextSelectedItem(const CGUIColListItem* pStartItem) const;

		/**
		* @brief get the next selected item by index
		*/
		CGUIColListItem*	GetSelectedItem( uint32 nIdx) const;

		/**
		* @brief Set the select state of an attached CGUIColListItem.
		* @param bSelected
		*		- true to select the item, 
		*		- false to de-select the item.
		*/
		void	SetItemSelected(CGUIColListItem* pItem, bool bSelected);

		/**
		* @brief Set the select state of an attached CGUIColListItem.
		*/
		void	SetItemSelected(uint32 nItemIndex, bool bSelected);

		/**
		* @brief Clear the selected state for all items.
		*/
		void	ClearAllSelections(void);

		/**
		*	@brief get the item at position by given index
		*/
		CGUIColListItem*	GetItemFromIndex(uint32 nIndex) const;


		/**
		* @brief Return the index of CGUIColListItem item
		*/
		uint32	GetItemIndex(const CGUIColListItem* pItem) const;

		/**
		* @brief Add the given CGUIColListItem to the list.
		*/
		void	AddItem(CGUIColListItem* pItem);

		/**
		* @brief Add a default text item.
		*/
		void	AddItem( const CGUIStringW& rText );

		/** 
		* @brief Insert an item into the list box after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem(CGUIColListItem* pItem, const CGUIColListItem* pPosition);

		/** 
		* @brief Insert an item into the list box after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem(CGUIColListItem* pItem, uint32 nIndex );

		/** 
		* @brief Insert an item into the list box after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem( const CGUIStringW& rText, const CGUIColListItem* pPosition);

		/** 
		* @brief Insert an item into the list box after a specified item already in the list.
		* Note that if the list is sorted, the item may not end up in the requested position.
		*/
		void	InsertItem( const CGUIStringW& rText, uint32 nIndex );

		/** 
		* @brief Removes the given item from the list box. 
		* If the item is has the auto delete state set, the item will be deleted.
		*/
		void	RemoveItem( CGUIColListItem* pItem);

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
		CGUIColListItem*	FindItemWithText(
			const CGUIStringW& rText, const CGUIColListItem* pStartItem = NULL);

		/**
		* @brief Return whether the specified CGUIColListItem is in the List
		*/
		bool	IsListboxItemInList(const CGUIColListItem* pItem) const;

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
		void	MakeItemVisible(const CGUIColListItem* pItem);




		/**
		* @brief Return the number of segment 
		*/
		uint32	GetSegmentCount(void) const;


		/**
		* @brief Return the number of rows.
		*/
		uint32	GetRowCount(void) const;

		/**
		* @brief Set the column to be used as the sort key.
		*/
		void	SetSortSegment(uint32 nIdx);

		/**
		* @brief return the current sort segment pointer
		*/
		CGUIColListHeaderSegment*	GetSortSegment(void) const;

		/**
		* @brief Return the zero based index of the current sort column.  
		There must be at least one column to successfully call this method.
		*/
		uint32	GetSortSegmentIndex(void) const;

		/** 
		* @brief set segment text info
		*/
		void	SetSegmentTextInfo(const CGUIStringInfo& rInfo);

		/** 
		* @brief get segment text info
		*/
		const CGUIStringInfo&	GetSegmentTextInfo() const;


		/**
		* @brief add a segment
		*/
		void	AddSegment( const CGUIStringW& rText, uint32 nId );

		/**
		* @brief set default size of segment
		*/
		void	SetDefaultSegmentSize( const CGUISize& rSize);

		/**
		* @brief get default size of segment
		*/
		const CGUISize& GetDefaultSegmentSize( ) const;

		/**
		* @brief get segment width by specified size
		*/
		real	GetSegmentWidthByIndex( uint32 nIdx) const;

		/**
		* @brief get the width of all segments
		*/
		real	GetSegmentTotalWidth( ) const;

		/**
		* @brief get segment position by specified size
		*/
		CGUIVector2	GetSegmentPosByIndex( uint32 nIdx) const;

		/**
		* @brief Add an empty row to the list box.
		*/
		uint32	AddRow(uint32 nRowId = 0);

		/**
		* @brief set the item for column list
		*/
		void	SetItem(CGUIColListItem* pItem, const SGridRef& rGrid);

		/**
		* @brief set the item for column list
		*/
		void	SetItem(const CGUIStringW& rText, const SGridRef& rGrid);


	protected:
		/** 
		* @brief struct used internally to represent a row in the list and 
		* also to ease sorting of the rows.
		*/
		struct SListRow
		{
			typedef	std::vector<CGUIColListItem*>	TRowItems;
			TRowItems	m_vecItems;
			uint32		m_nSortColumn;			///< index of current sort column
			uint32		m_nRowID;				///< id of this row

			// operators
			CGUIColListItem* const& operator[](uint32 idx) const;
			CGUIColListItem*&	operator[](uint32 idx);
			bool	operator<(const SListRow& rhs) const;
			bool	operator>(const SListRow& rhs) const;
		};


		/**
		* @brief constructor as a base class
		*/
		CGUIWgtColumnList( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		/// render
		virtual void		RenderSelf(IGUIInterfaceRender* pRender);

		/**
		* @brief override the OnSetImage function
		*/
		virtual void		OnSetImage( const CGUIString& rName, CGUIImage* pImage );

		///initialize check button
		void				InitListBox();

		/// get item under point, return null if there isn't item
		CGUIColListItem*	GetItemUnderPoint(const CGUIVector2& rPos);

		/**
		* @brief clear all selected item and don't send event for selected item change
		* @return true if selected item changes
		*/
		bool				ClearAllSelections_impl(void);

		/**
		* @brief remove all item without send event.
		* @return true if any item was removed
		*/
		bool				ClearList_impl(void);

		///update item size when it changed
		void				UpdateItemPos();

		/**
		* @brief get total height of items
		*/
		real				GetTotalItemsHeight() const;

		/**
		* @brief get widest width of items
		*/
		real				GetWidestItemsWidth() const;

		//add a child
		void				AddChild_Imp(CGUIColListItem* pItem);

		//create a item
		CGUIColListItem*	CreateItem_Imp(const CGUIStringW& rText);

		//get row height
		real				GetRowHeight( const SListRow& rRow ) const;


	protected:	//!< callback function
		virtual uint32		OnMouseMove( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightUp( CGUIEventMouse* pEvent );

		friend class CGUIColListItem;
		virtual void		ProcessMouseLeftDown(CGUIColListItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseLeftUp(CGUIColListItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseRightDown(CGUIColListItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseRightUp(CGUIColListItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseLeftClick(CGUIColListItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseRightClick(CGUIColListItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseLeftDbClick(CGUIColListItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseRightDbClick(CGUIColListItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseMove(CGUIColListItem* pItem, CGUIEventMouse* pEvent);

	protected:
		typedef std::vector<CGUIColListItem*>	TListItem;
		TListItem	m_aListItems;			//!< list of items in the list box.
		//TListboxItem*	m_aLastSelected;	//!< holds pointer to the last selected item (used in range selections)

		bool	m_bSorted;						//!< true if list is sorted
		bool	m_bMultiselect;					//!< true if multi-select is enabled
		bool	m_bItemTooltips;				//!< true if each item should have an individual tooltip

		CGUISize			m_aWorkAreaSize;			///size of work area

		CGUIImage*			m_pImageBG;					/// image for listbox's background
		CGUIImage*			m_pImageSelection;			/// image for listbox's selection color

		CGUIColListItem*	m_pLastOperateItem;			/// the last item which is been operated.for multiselect
		

		typedef std::vector<SListRow>		TListRow;
		TListRow		m_aListRows;			//!< list of rows in the list box.
		


		CGUIColListHeader*	m_pHeader;			/// header of column list

		CGUIString			m_strItemType;		/// type of item

	private:
		static CGUIString	ms_strType;
	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtColumnList);


}//namespace guiex

#endif //__GUI_WGTCOLUMNLIST_20070802_H__
