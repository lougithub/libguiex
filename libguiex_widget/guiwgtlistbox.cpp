/** 
* @file guiwgtlistbox.cpp
* @brief list box
* @author ken
* @date 2006-09-18
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtlistbox.h"
#include "guiwgtlistboxitem.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiinterfacekeyboard.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guiwidgetmanager.h>

#include <algorithm>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtListBox);
	//------------------------------------------------------------------------------
	CGUIWgtListBox::CGUIWgtListBox( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtScrollbarContainer(StaticGetType(), rName, rSceneName)
	{
		InitListBox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtListBox::~CGUIWgtListBox( )
	{
		//delete child
		CGUIWidget* pChild = GetChild();
		CGUIWidget* pChildTmp = NULL;
		while( pChild && pChild->GetType() == "CGUIWgtListBoxItem" )
		{
			pChildTmp = pChild;
			pChild = pChild->GetNextSibling();

			//destroy it
			CGUIWidgetManager::Instance()->DestroyWidget(pChildTmp);		
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::InitListBox()
	{
		m_bMultiselect = false;

		m_pImageBG = NULL;
		m_pLastOperateItem = NULL;

		SetGenerateParentChildEvent( true );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "background")
		{
			m_pImageBG = pImage;
			if( pImage && GetSize().IsEqualZero())
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else
		{
			CGUIWgtScrollbarContainer::OnSetImage( rName, pImage );
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return number of items attached to the list box
	*/
	uint32 CGUIWgtListBox::GetItemCount(void) const	
	{
		return static_cast<uint32>(m_aListItems.size());
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return number of selected items attached to the list box
	*/
	uint32	CGUIWgtListBox::GetSelectedItemCount(void) const
	{
		uint32 count = 0;
		for (size_t index = 0; index < m_aListItems.size(); ++index)
		{
			if (m_aListItems[index]->IsSelected())
			{
				++count;
			}
		}
		return count;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get the selected item by index
	*/
	void CGUIWgtListBox::GetSelectedItems( std::vector<CGUIWgtListBoxItem*>& rItems) const
	{
		rItems.clear();
		for(size_t index = 0; index < m_aListItems.size(); ++index)
		{
			// return pointer to this item if it's selected.
			if (m_aListItems[index]->IsSelected())
			{
				rItems.push_back(m_aListItems[index]);
			}
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Set the select state of an attached CGUIWgtListBoxItem.
	* @param bSelected
	*		- true to select the item, 
	*		- false to de-select the item.
	*/
	void CGUIWgtListBox::SetItemSelected(CGUIWgtListBoxItem* pItem, bool bSelected)
	{
		TListItem::iterator itor = std::find(m_aListItems.begin(), m_aListItems.end(), pItem);

		if (itor != m_aListItems.end())
		{
			SetItemSelected(static_cast<uint32>(std::distance(m_aListItems.begin(), itor)), bSelected);
		}
		else
		{
			throw CGUIException("[CGUIWgtListBox::SetItemSelected]: the specified Item is not attached to this Listbox.");
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Set the select state of an attached CGUIWgtListBoxItem.
	*/
	void CGUIWgtListBox::SetItemSelected(uint32 nItemIndex, bool bSelected)
	{	
		if (nItemIndex < m_aListItems.size())
		{
			// only do this if the setting is changing
			if (m_aListItems[nItemIndex]->IsSelected() != bSelected)
			{
				// clear selected item for single select mode
				if (bSelected && !m_bMultiselect)
				{
					ClearAllSelections_impl();
				}

				m_aListItems[nItemIndex]->SetSelected(bSelected);

				// send event for select changed
				{
					CGUIEventListBox aEvent;
					aEvent.SetEventId(eEVENT_LISTBOX_SELECTED);
					aEvent.SetReceiver(this);
					aEvent.SetSelectedItemIdx( nItemIndex );
					GSystem->SendEvent( &aEvent);
				}

				/// send event for selecte event at single select mode
				if (bSelected && !m_bMultiselect)
				{
					CGUIEventListBox aEvent;
					aEvent.SetEventId(eEVENT_LISTBOX_SELECTCHANGED);
					aEvent.SetReceiver(this);
					GSystem->SendEvent( &aEvent);
				}
			}
		}
		else
		{
			throw CGUIException("[CGUIWgtListBox::SetItemSelected]: the index <%d> of Item is out of range.", nItemIndex);
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Clear the selected state for all items.
	*/
	void CGUIWgtListBox::ClearAllSelections(void)
	{
		if (ClearAllSelections_impl())
		{
			CGUIEventListBox aEvent;
			aEvent.SetEventId(eEVENT_LISTBOX_SELECTCHANGED);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	/**
	*	@brief get the item at position by given index
	*/
	CGUIWgtListBoxItem*	CGUIWgtListBox::GetItemByIndex(uint32 nIndex) const
	{
		if (nIndex < m_aListItems.size())
		{
			return m_aListItems[nIndex];
		}
		else
		{
			throw CGUIException("[CGUIWgtListBox::GetItemByIndex]: the index <%d> of Item is out of range.", nIndex);
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the index of CGUIWgtListBoxItem item
	*/
	uint32	CGUIWgtListBox::GetItemIndex(const CGUIWgtListBoxItem* pItem) const
	{
		TListItem::const_iterator itor = std::find(m_aListItems.begin(), m_aListItems.end(), pItem);

		if (itor != m_aListItems.end())
		{
			return static_cast<uint32>(std::distance(m_aListItems.begin(), itor));
		}
		else
		{
			throw CGUIException("[CGUIWgtListBox::GetItemIndex]: the specified Item is not attached to this listbox.");
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::DoAddItem(CGUIWgtListBoxItem* pItem)
	{
		GUI_ASSERT( pItem, "[CGUIWgtListBox::DoAddItem] invalid pointer" );

		m_aListItems.push_back(pItem);
		if( IsOpen())
		{
			pItem->Open();
		}

		UpdateTotalItems();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::DoRemoveItem( CGUIWgtListBoxItem* pItem)
	{
		GUI_ASSERT( pItem, "[CGUIWgtListBox::DoRemoveItem] invalid pointer" );

		TListItem::iterator itor = std::find(m_aListItems.begin(), m_aListItems.end(), pItem);
		if (itor != m_aListItems.end())
		{
			m_aListItems.erase(itor);
		}
		else
		{
			GUI_THROW( GUI_FORMAT("[CGUIWgtListBox::DoRemoveItem]: failed to find item, type <%s>, name<%s>!", 
				pItem->GetType().c_str(), pItem->GetName().c_str()));
		}

		if( m_pLastOperateItem == pItem )
		{
			m_pLastOperateItem = NULL;
		}

		UpdateTotalItems();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::UpdateTotalItems( )
	{
		m_aTotalItemSize.SetValue( 0, 0 );

		//update item position
		CGUIVector2	posLeftTop(0.0f,0);
		for( TListItem::iterator itor = m_aListItems.begin(); itor != m_aListItems.end(); ++itor)
		{
			CGUIWgtListBoxItem* pItem = *itor;
			pItem->SetPixelPosition(posLeftTop );
			posLeftTop.y += pItem->GetPixelSize().GetHeight();

			if( pItem->GetPixelSize().GetWidth() > m_aTotalItemSize.m_fWidth )
			{
				m_aTotalItemSize.m_fWidth = pItem->GetPixelSize().GetWidth();
			}
		}
		m_aTotalItemSize.m_fHeight = posLeftTop.y;

		Refresh();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Add the given CGUIWgtListBoxItem to the list.
	*/
	void CGUIWgtListBox::AddItem(CGUIWidget* pItem)
	{
		GUI_ASSERT( pItem, "[CGUIWgtListBox::AddItem] invalid pointer" );
		GUI_ASSERT( pItem->GetType() == "CGUIWgtListBoxItem", "[CGUIWgtListBox::AddItem] invalid pointer" );

		AddChild( pItem );
		pItem->Open();
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Removes the given item from the list box. 
	* If the item is has the auto delete state set, the item will be deleted.
	*/
	void CGUIWgtListBox::RemoveItem( CGUIWidget* pItem)
	{
		GUI_ASSERT( pItem, "[CGUIWgtListBox::RemoveItem] invalid pointer" );
		GUI_ASSERT( pItem->GetType() == "CGUIWgtListBoxItem", "[CGUIWgtListBox::RemoveItem] invalid pointer" );

		RemoveChild( pItem );
		pItem->Close();
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Removes the given item from the list box. 
	* If the item is has the auto delete state set, the item will be deleted.
	*/
	void CGUIWgtListBox::RemoveItem( uint32 nIndex)
	{
		RemoveItem(GetItemByIndex(nIndex) );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Remove all items from the list.
	*/
	void CGUIWgtListBox::RemoveAllItems(void)
	{
		while( !m_aListItems.empty() )
		{
			RemoveChild(m_aListItems[0]);
		}
		m_pLastOperateItem = NULL;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Set whether the list should allow multiple selections or just a single selection
	*/
	void CGUIWgtListBox::EnableMultiselect(bool bEnable)
	{
		if (m_bMultiselect != bEnable)
		{
			m_bMultiselect = bEnable;

			// if we change to single-select, deselect all except the first selected item.
			if( m_bMultiselect )
			{
				bool bFirstSelectedItem = true;
				bool bDeselected = false;
				for (size_t index = 0; index < m_aListItems.size(); ++index)
				{
					if (m_aListItems[index]->IsSelected())
					{
						if( bFirstSelectedItem )
						{
							bFirstSelectedItem = false;
						}
						else
						{
							m_aListItems[index]->SetSelected(false);
							bDeselected = true;
						}
					}
				}

				// send event
				if( bDeselected )
				{
					CGUIEventListBox aEvent;
					aEvent.SetEventId(eEVENT_LISTBOX_SELECTCHANGED);
					aEvent.SetReceiver(this);
					GSystem->SendEvent( &aEvent);
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief return whether multi-select is enabled
	*/
	bool CGUIWgtListBox::IsMultiselectEnabled(void) const
	{
		return m_bMultiselect;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIWgtScrollbarContainer::RenderSelf(pRender);
		/// draw bg
		DrawImage( pRender, m_pImageBG, GetBoundArea());
	}
	//------------------------------------------------------------------------------
	/**
	* @brief clear all selected item and don't send event for selected item change
	* @return true if selected item changes
	*/
	bool CGUIWgtListBox::ClearAllSelections_impl(void)
	{
		// flag used so we can track if we did anything.
		bool bModified = false;

		for (size_t index = 0; index < m_aListItems.size(); ++index)
		{
			if (m_aListItems[index]->IsSelected())
			{
				m_aListItems[index]->SetSelected(false);
				bModified = true;
			}
		}

		return bModified;
	}
	//------------------------------------------------------------------------------
	CGUISize CGUIWgtListBox::GetDesiredVirtualClientSize( )
	{
		return m_aTotalItemSize;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::RefreshSelf()
	{
		CGUIWgtScrollbarContainer::RefreshSelf();
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	void CGUIWgtListBox::ProcessMouseLeftDown(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		if( IsMultiselectEnabled())
		{
			if( pEvent->GetKeyboardInterface()->IsKeyPressed( KC_CTRL ))
			{
				if( pItem->IsSelected())
				{
					///control pressed
					SetItemSelected(pItem, false);
				}
				else
				{
					SetItemSelected(pItem, true);
				}
				m_pLastOperateItem = pItem;
			}
			else if( pEvent->GetKeyboardInterface()->IsKeyPressed( KC_SHIFT ))
			{
				if( m_pLastOperateItem )
				{
					ClearAllSelections_impl();
					uint32 start = GetItemIndex(m_pLastOperateItem);
					uint32 end = GetItemIndex(pItem);
					if( start > end )
					{
						std::swap(start, end);
					}
					for( uint32 i= start; i<=end; ++i )
					{
						SetItemSelected(i, true);
					}
				}
			}
			else
			{
				ClearAllSelections_impl();
				SetItemSelected(pItem, true);
				m_pLastOperateItem = pItem;
			}
		}
		else
		{
			ClearAllSelections_impl();
			SetItemSelected(pItem, true);
			m_pLastOperateItem = pItem;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::ProcessMouseLeftUp(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent)
	{

	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::ProcessMouseRightDown(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		ClearAllSelections_impl();
		SetItemSelected(pItem, true);
		m_pLastOperateItem = pItem;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::ProcessMouseRightUp(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent)
	{

	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::ProcessMouseLeftClick(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		if( pItem == m_pLastOperateItem )
		{
			CGUIEventListBox aEvent;
			aEvent.SetEventId(eEVENT_LISTBOX_CLICK_LEFT);
			aEvent.SetReceiver(this);
			aEvent.SetDbClickedItemIdx( GetItemIndex(pItem) );
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::ProcessMouseRightClick(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		if( pItem == m_pLastOperateItem )
		{
			CGUIEventListBox aEvent;
			aEvent.SetEventId(eEVENT_LISTBOX_CLICK_RIGHT);
			aEvent.SetReceiver(this);
			aEvent.SetDbClickedItemIdx( GetItemIndex(pItem) );
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::ProcessMouseLeftDbClick(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		if( pItem == m_pLastOperateItem )
		{
			CGUIEventListBox aEvent;
			aEvent.SetEventId(eEVENT_LISTBOX_DBCLICK_LEFT);
			aEvent.SetReceiver(this);
			aEvent.SetDbClickedItemIdx( GetItemIndex(pItem) );
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::ProcessMouseRightDbClick(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		if( pItem == m_pLastOperateItem )
		{
			CGUIEventListBox aEvent;
			aEvent.SetEventId(eEVENT_LISTBOX_DBCLICK_RIGHT);
			aEvent.SetReceiver(this);
			aEvent.SetDbClickedItemIdx( GetItemIndex(pItem) );
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::ProcessMouseMove(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent)
	{

	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBox::OnAddChild( CGUIEventRelativeChange* pEvent )
	{
		if( pEvent->GetRelative()->GetType() == "CGUIWgtListBoxItem")
		{
			DoAddItem(static_cast<CGUIWgtListBoxItem*>(pEvent->GetRelative()));
		}

		return CGUIWgtScrollbarContainer::OnAddChild(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBox::OnRemoveChild( CGUIEventRelativeChange* pEvent )
	{
		if( pEvent->GetRelative()->GetType() == "CGUIWgtListBoxItem")
		{
			DoRemoveItem(static_cast<CGUIWgtListBoxItem*>(pEvent->GetRelative()));
		}

		return CGUIWgtScrollbarContainer::OnRemoveChild(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBox::OnChildSizeChange( CGUIEventSize* pEvent )
	{
		UpdateTotalItems();
		return CGUIWgtScrollbarContainer::OnChildSizeChange(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBox::OnOpen( CGUIEventNotification* pEvent )
	{
		UpdateTotalItems();
		return CGUIWgtScrollbarContainer::OnOpen(pEvent);
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtListBox::GenerateProperty( CGUIProperty& rProperty )
	{
		//else
		{
			return CGUIWgtScrollbarContainer::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::ProcessProperty( const CGUIProperty& rProperty)
	{
		//else
		{
			CGUIWgtScrollbarContainer::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
}
