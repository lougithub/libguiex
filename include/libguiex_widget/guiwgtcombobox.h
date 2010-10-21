/** 
* @file guiwgtcombobox.h
* @brief combobox used in guiex system
* @author ken
* @date 2006-09-22
*/

#ifndef __GUI_WGTCOMBOBOX_20060918_H__
#define __GUI_WGTCOMBOBOX_20060918_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiwidget.h>
#include <libguiex_core\guiwidgetgenerator.h>
#include "guilistboxitem.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUIWgtComboBox
	* @brief ComboBox
	* used image name:
	*	- BGIMAGE
	*
	* - DROPLIST_BGIMAGE
	* - DROPLIST_SELECTION_COLOR
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
	*/
	class GUIEXPORT CGUIWgtComboBox : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtComboBox( const CGUIString& rName, const CGUIString& rProjectName );

		/**
		* @brief destructor
		*/
		~CGUIWgtComboBox( );

		/**
		* @brief create this widget
		*/
		virtual int32 Create();

		/**
		* @brief set size of text
		*/
		virtual void		SetTextInfo( const CGUIStringExInfo& rInfo);

		/**
		* @brief load widget config from property
		*/
		virtual CGUIProperty*	GenerateProperty(const CGUIString& rName, const CGUIString& rType );


		/** 
		* @brief process property
		*/
		virtual void		ProcessProperty( const CGUIProperty* pProperty);

		/**
		* @brief set the size of drop list
		*/
		void	SetDropListSize( const CGUISize& rSize );

		/**
		* @brief get the size of drop list
		*/
		const CGUISize&	GetDropListSize(  ) const;


		////////////////////////////////////////////////////////////////////////////////
		// combo box related function
		////////////////////////////////////////////////////////////////////////////////
		/**
		* @brief select item and show it in the combobox.
		*/
		void			SetSelectedItem(uint32 nIdx );

		/**
		* @brief select item and show it in the combobox.
		*/
		void			SetSelectedItem( CGUIListBoxItem* pItem );

		/**
		* @brief get index of selected item
		*/
		uint32	GetSelectedItemIndex() const;

		/**
		* @brief get Selected item
		*/
		const CGUIListBoxItem*	GetSelectedItem() const;

		/**
		* @brief Return number of items attached to the combo box
		*/
		uint32	GetItemCount(void) const	;

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
		* @brief Return whether the vertical scroll bar is always shown.
		*/
		bool	IsVertScrollbarAlwaysShown(void) const;

		/**
		* @brief Set whether the vertical scroll bar should always be shown.
		*/
		void	ForceVertScrollbar(bool bSetting);

		/**
		* @brief Return whether the horizontal scroll bar is always shown.
		*/
		bool	IsHorzScrollbarAlwaysShown(void) const;

		/**
		* @brief Set whether the horizontal scroll bar should always be shown.
		*/
		void	ForceHorzScrollbar(bool bSetting);

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
		CGUIWgtComboBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		///initialize Combo box
		void	InitComboBox();

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName,CGUIImage* pImage );

	protected:	//!< callback function
		virtual uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );

	protected:

		friend class CGUIWgtComboBoxDropList;
		CGUIWgtComboBoxDropList*		m_pDropList;

		CGUIListBoxItem*		m_pSelectedItem;		/// selected item

		CGUIImage* m_pImageBG;					/// image for listbox's backbround

	private:
		static CGUIString	ms_strType;	/// type of this widget
	};


	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtComboBox);

}//namespace guiex

#endif	//__GUI_WGTCOMBOBOX_20060918_H__
