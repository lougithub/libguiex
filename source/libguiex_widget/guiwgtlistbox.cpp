/** 
* @file guiwgtlistbox.cpp
* @brief list box
* @author ken
* @date 2006-09-18
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtlistbox.h>
#include <libguiex_widget/guiwgtscrollbar.h>
#include <libguiex_widget/guilistboxitem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiinterfacekeyboard.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guistringconvertor.h>

#include <algorithm>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtListBox);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtListBox::ms_strType = "CGUIWgtListBox";
	//------------------------------------------------------------------------------
	CGUIWgtListBox::CGUIWgtListBox( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWgtScrollbarContainer(ms_strType, rName, rProjectName)
	{
		InitListBox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtListBox::CGUIWgtListBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWgtScrollbarContainer(rType, rName, rProjectName)
	{
		InitListBox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtListBox::~CGUIWgtListBox( )
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::InitListBox()
	{
		m_bSorted = false;
		m_bMultiselect = false;
		m_bItemTooltips = false;

		m_pImageBG = NULL;
		m_pImageSelection = NULL;
		m_pLastOperateItem = NULL;

		m_strItemType = "CGUIListBoxTextItem";

		//set flag
		SetFocusable(true);
		SetGenerateClickEvent(true);
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtListBox::Create()
	{
		if( !m_pImageBG )
		{
			throw CGUIException("[CGUIWgtCheckButton::Create]: the image <BGIMAGE> hasn't been found!");
		}
		if( !m_pImageSelection)
		{
			throw CGUIException("[CGUIWgtCheckButton::Create]: the image <SELECTION_COLOR> hasn't been found!");
		}

		return CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "BGIMAGE")
		{
			m_pImageBG = pImage;
			SetSize(pImage->GetSize());
		}
		else if( rName == "SELECTION_COLOR")
		{
			m_pImageSelection = pImage;
		}
		else 
		{
			CGUIWgtScrollbarContainer::OnSetImage(rName, pImage );
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::SetValue(const CGUIString& rName, const CGUIString& rValue)
	{
		CGUIWidget::SetValue(rName, rValue);
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWgtListBox::GetValue(const CGUIString& rName) const
	{
		return CGUIWidget::GetValue( rName );
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtListBox::GetItemCount(void) const	
	{
		return static_cast<uint32>(m_aListItems.size());
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtListBox::GetSelectedCount(void) const
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
	CGUIListBoxItem*	CGUIWgtListBox::GetFirstSelectedItem(void) const
	{
		return GetNextSelectedItem(NULL);
	}
	//------------------------------------------------------------------------------
	CGUIListBoxItem*	CGUIWgtListBox::GetNextSelectedItem(const CGUIListBoxItem* pStartItem) const
	{
		for(size_t index = (pStartItem == NULL) ? 0 : (GetItemIndex(pStartItem) + 1);
			index < m_aListItems.size();
			++index)
		{
			// return pointer to this item if it's selected.
			if (m_aListItems[index]->IsSelected())
			{
				return m_aListItems[index];
			}
		}

		// no more selected items.
		return NULL;
	}
	//------------------------------------------------------------------------------
	CGUIListBoxItem*	CGUIWgtListBox::GetSelectedItem( uint32 nIdx) const
	{
		uint32 count = 0;
		for(size_t index = 0; index < m_aListItems.size(); ++index)
		{
			// return pointer to this item if it's selected.
			if (m_aListItems[index]->IsSelected())
			{
				if( count == nIdx )
				{
					return m_aListItems[index];
				}
				else
				{
					++count;
				}
			}
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::SetItemSelected(CGUIListBoxItem* pItem, bool bSelected)
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
	void	CGUIWgtListBox::SetItemSelected(uint32 nItemIndex, bool bSelected)
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

				/// send event for select changed
				{
					CGUIEventListBox aEvent;
					aEvent.SetEventId(eEVENT_LISTBOX_SELECTED);
					aEvent.SetReceiver(this);
					aEvent.SetSelectedItemIdx( nItemIndex );
					CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
				}

				/// send event for selecte event at single select mode
				if (bSelected && !m_bMultiselect)
				{
					CGUIEventListBox aEvent;
					aEvent.SetEventId(eEVENT_LISTBOX_SELECTCHANGED);
					aEvent.SetReceiver(this);
					CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
				}
			}

		}
		else
		{
			throw CGUIException("[CGUIWgtListBox::SetItemSelected]: the index <%d> of Item is out of range.", nItemIndex);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::ClearAllSelections(void)
	{
		if (ClearAllSelections_impl())
		{
			CGUIEventListBox aEvent;
			aEvent.SetEventId(eEVENT_LISTBOX_SELECTCHANGED);
			aEvent.SetReceiver(this);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	CGUIListBoxItem*	CGUIWgtListBox::GetItemFromIndex(uint32 nIndex) const
	{
		if (nIndex < m_aListItems.size())
		{
			return m_aListItems[nIndex];
		}
		else
		{
			throw CGUIException("[CGUIWgtListBox::GetItemFromIndex]: the index <%d> of Item is out of range.", nIndex);
		}
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtListBox::GetItemIndex(const CGUIListBoxItem* pItem) const
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
	CGUIListBoxItem* CGUIWgtListBox::CreateItem_Imp(const wchar_t* pText)
	{
		CGUIListBoxItem * pItem = static_cast<CGUIListBoxItem*>(GUI_CREATE_WIDGET(m_strItemType.c_str(), CGUIString("listitem_")+CGUIWidgetSystem::Instance()->GenerateAnonymousName(), GetProjectName()));
		pItem->SetItemInfo(pText, GetTextInfo());
		return pItem;
	}
	//------------------------------------------------------------------------------
	bool listboxitem_less(const CGUIListBoxItem* a, const CGUIListBoxItem* b)
	{
		return *a < *b;
	}
	void	CGUIWgtListBox::AddItem(CGUIListBoxItem* pItem)
	{
		if (pItem != NULL)
		{
			if (IsSorting())
			{
				// if sorting is enabled, re-sort the list
				m_aListItems.insert(std::upper_bound(m_aListItems.begin(), m_aListItems.end(), pItem, listboxitem_less), pItem);
				AddChild_Imp(pItem);
				UpdateItemPos();
			}
			else
			{
				// not sorted, just stick it on the end.
				m_aListItems.push_back(pItem);
				AddChild_Imp(pItem);
				UpdateItemPos();

			}

//			SetRectDirty();
			UpdateScrollbars();
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::AddItem( const wchar_t* pText )
	{
		CGUIListBoxItem* pItem= CreateItem_Imp(pText);
		pItem->SetSelectionImage( m_pImageSelection );
		AddItem( pItem );
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::AddChild_Imp(CGUIListBoxItem* pItem)
	{
		AddChild(pItem);
		if( IsOpen())
		{
			pItem->Open();
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::InsertItem(CGUIListBoxItem* pItem, uint32 nIndex )
	{
		InsertItem( pItem, GetItemFromIndex(nIndex));
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::InsertItem( const wchar_t* pText, const CGUIListBoxItem* pPosition)
	{
		CGUIListBoxItem* pItem= CreateItem_Imp(pText);
		pItem->SetSelectionImage( m_pImageSelection );
		InsertItem( pItem, pPosition);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::InsertItem( const wchar_t* pText, uint32 nIndex )
	{
		CGUIListBoxItem* pItem= CreateItem_Imp(pText);
		pItem->SetSelectionImage( m_pImageSelection );
		InsertItem( pItem, GetItemFromIndex(nIndex));
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::InsertItem(CGUIListBoxItem* pItem, const CGUIListBoxItem* pPosition)
	{
		// if the list is sorted, it's the same as a normal add operation
		if (IsSorting())
		{
			AddItem(pItem);
		}
		else if (pItem != NULL)
		{
			// if position is NULL begin insert at begining, else insert after item 'position'
			TListItem::iterator itor;
			if (pPosition == NULL)
			{
				itor = m_aListItems.begin();
			}
			else
			{
				itor = std::find(m_aListItems.begin(), m_aListItems.end(), pPosition);
				if (itor == m_aListItems.end())
				{
					throw CGUIException("[CGUIWgtListBox::InsertItem]: the specified Item for parameter 'pPosition' is not attached to this Listbox.");
				}
			}

			m_aListItems.insert(itor, pItem);
			AddChild_Imp(pItem);
			UpdateItemPos();


//			SetRectDirty();
			UpdateScrollbars();
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::RemoveItem( CGUIListBoxItem* pItem)
	{
		if (pItem != NULL)
		{
			TListItem::iterator itor = std::find(m_aListItems.begin(), m_aListItems.end(), pItem);

			// if item is in the list
			if (itor != m_aListItems.end())
			{
				// remove item
				m_aListItems.erase(itor);

				RemoveChild(pItem);
				UpdateItemPos();

				//// if item was the last selected item, reset that to NULL
				//if (pItem == m_aLastSelected)
				//{
				//	m_aLastSelected = NULL;
				//}

				// if item is supposed to be deleted by us
				CGUIWidgetSystem::Instance()->DestroyWidget(pItem);

//				SetRectDirty();
				UpdateScrollbars();

				if( m_pLastOperateItem  == pItem )
				{
					m_pLastOperateItem = NULL;
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::RemoveItem( uint32 nIndex)
	{
		RemoveItem(GetItemFromIndex(nIndex) );
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::ClearList(void)
	{
		if (ClearList_impl())
		{

		}

//		SetRectDirty();
		UpdateScrollbars();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::EnableMultiselect(bool bEnable)
	{
		if (m_bMultiselect != bEnable)
		{
			m_bMultiselect = bEnable;

			// if we change to single-select, deselect all except the first selected item.
			bool bDeselected = false;
			if ((!m_bMultiselect) && (GetSelectedCount() > 1))
			{
				CGUIListBoxItem* itm = GetFirstSelectedItem();

				while ((itm = GetNextSelectedItem(itm)))
				{
					itm->SetSelected(false);
					bDeselected = true;
				}
			}

			// send event
			if( bDeselected )
			{
				CGUIEventListBox aEvent;
				aEvent.SetEventId(eEVENT_LISTBOX_SELECTCHANGED);
				aEvent.SetReceiver(this);
				CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
			}
		}

	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtListBox::IsMultiselectEnabled(void) const
	{
		return m_bMultiselect;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtListBox::IsItemTooltipsEnabled(void) const
	{
		return m_bItemTooltips;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::EnableItemTooltips(bool bEnable)
	{
		m_bItemTooltips = bEnable;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtListBox::IsItemSelected(uint32 nIndex) const
	{
		if (nIndex < m_aListItems.size())
		{
			return m_aListItems[nIndex]->IsSelected();
		}
		else
		{
			throw CGUIException("[CGUIWgtListBox::]: the specified index <%d> is out of range for this Listbox.", nIndex);
		}
	}
	//------------------------------------------------------------------------------
	CGUIListBoxItem*	CGUIWgtListBox::FindItemWithText(
		const wchar_t* pText, const CGUIListBoxItem* pStartItem)
	{
		size_t index = (pStartItem == NULL) ? 0 : (GetItemIndex(pStartItem) + 1);

		for( TListItem::iterator itor = m_aListItems.begin() + index;
			itor != m_aListItems.end();
			++itor)
		{
			if( wcscmp((*itor)->GetTextContent(), pText)==0)
			{
				return *itor;
			}
		}

		// no items matched.
		return NULL;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtListBox::IsListboxItemInList(const CGUIListBoxItem* pItem) const
	{
		return std::find(m_aListItems.begin(), m_aListItems.end(), pItem) != m_aListItems.end();
	}
	//------------------------------------------------------------------------------
	bool listboxitem_greater(const CGUIListBoxItem* a, const CGUIListBoxItem* b)
	{
		return *a > *b;
	}
	void	CGUIWgtListBox::SetSorting(bool bSorting)
	{
		// only react if the setting will change
		if (m_bSorted != bSorting)
		{
			m_bSorted = bSorting;

			// if we are enabling sorting, we need to sort the list
			if (m_bSorted)
			{
				std::sort(m_aListItems.begin(), m_aListItems.end(), listboxitem_greater);
			}
		}
	}
	//------------------------------------------------------------------------------
	bool 	CGUIWgtListBox::IsSorting() const
	{
		return m_bSorted;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::MakeItemVisible(uint32 nItemIndex)
	{
		//// handle simple "scroll to the bottom" case
		//if (nItemIndex >= GetItemCount())
		//{
		//	//d_vertScrollbar->setScrollPosition(d_vertScrollbar->getDocumentSize() - d_vertScrollbar->getPageSize());
		//}
		//else
		//{
		//	real bottom;
		//	real listHeight = GetClientRect.GetHeight();//getListRenderArea().getHeight();
		//	real top = 0;

		//	// get height to top of item
		//	size_t i;
		//	for (i = 0; i < nItemIndex; ++i)
		//	{
		//		top += m_aListItems[i]->GetSize().GetHeight();
		//	}

		//	// calculate height to bottom of item
		//	bottom = top + m_aListItems[i]->GetSize().GetHeight();

		//	// account for current scrollbar value
		//	real currPos = d_vertScrollbar->getScrollPosition();
		//	top		-= currPos;
		//	bottom	-= currPos;

		//	// if top is above the view area, or if item is too big to fit
		//	if ((top < 0.0f) || ((bottom - top) > listHeight))
		//	{
		//		// scroll top of item to top of box.
		//		d_vertScrollbar->setScrollPosition(currPos + top);
		//	}
		//	// if bottom is below the view area
		//	else if (bottom >= listHeight)
		//	{
		//		// position bottom of item at the bottom of the list
		//		d_vertScrollbar->setScrollPosition(currPos + bottom - listHeight);		
		//	}
		//	
		//	// Item is already fully visible - nothing more to do.
		//}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::MakeItemVisible(const CGUIListBoxItem* pItem)
	{
		MakeItemVisible(GetItemIndex(pItem));
	}
	//------------------------------------------------------------------------------
	//void	CGUIWgtListBox::UpdateDirtyRect()
	//{
	//	CGUIWgtScrollbarContainer::UpdateDirtyRect_SC_Begin();

	//	m_aClientRect.SetWidth(0.0f);
	//	m_aClientRect.SetHeight(0.0f);

	//	// loop through the items
	//	uint32 itemCount = static_cast<uint32>(m_aListItems.size());
	//	for (uint32 i = 0; i < itemCount; ++i)
	//	{
	//		if( m_aListItems[i]->GetWidth() > m_aClientRect.GetWidth())
	//		{
	//			m_aClientRect.SetWidth( m_aListItems[i]->GetWidth());
	//		}
	//		m_aClientRect.SetHeight(m_aClientRect.GetHeight() + m_aListItems[i]->GetHeight());
	//	}

	//	CGUIWgtScrollbarContainer::UpdateDirtyRect_SC_End();
	//}
	//------------------------------------------------------------------------------
	void CGUIWgtListBox::RenderSelf(IGUIInterfaceRender* pRender)
	{
		/// draw bg
		DrawImage( pRender, m_pImageBG, GetRect());
	}
	//------------------------------------------------------------------------------
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
	void	CGUIWgtListBox::UpdateItemPos()
	{
		CGUIVector2	m_posLeftTop(0.0f,0);
		for( TListItem::iterator itor = m_aListItems.begin(); itor != m_aListItems.end(); ++itor)
		{
			CGUIListBoxItem* pItem = *itor;
			pItem->SetLocalPosition(m_posLeftTop);
			m_posLeftTop.y += pItem->GetHeight();
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtListBox::ClearList_impl(void)
	{
		if (GetItemCount() == 0)
		{
			return false;
		}
		else
		{
			// delete any items we are supposed to
			for (size_t i = 0; i < m_aListItems.size(); ++i)
			{
				RemoveChild(m_aListItems[i]);
				CGUIWidgetSystem::Instance()->DestroyWidget(m_aListItems[i]);
			}

			// clear out the list.
			m_aListItems.clear();

			UpdateItemPos();

			//m_aLastSelected = NULL;

			return true;
		}

		m_pLastOperateItem = NULL;
	}
	//------------------------------------------------------------------------------
	real	CGUIWgtListBox::GetTotalItemsHeight() const
	{
		real fHeight = 0.0f;
		for( size_t i=0; i<m_aListItems.size(); ++i )
		{
			fHeight += m_aListItems[i]->GetHeight();
		}

		return fHeight;
	}
	//------------------------------------------------------------------------------
	real	CGUIWgtListBox::GetWidestItemsWidth() const
	{
		real fWidth = 0.0f;
		for( size_t i=0; i<m_aListItems.size(); ++i )
		{
			if( m_aListItems[i]->GetWidth() > fWidth )
			{
				fWidth = m_aListItems[i]->GetWidth();
			}
		}

		return fWidth;
	}
	//------------------------------------------------------------------------------
	CGUIListBoxItem*	CGUIWgtListBox::GetItemUnderPoint(const CGUIVector2& rPos)
	{
		for( TListItem::iterator itor = m_aListItems.begin();
			itor != m_aListItems.end();
			++itor)
		{
			CGUIListBoxItem* pItem = *itor;
			const CGUIRect& rItemRect = pItem->GetRect();
			if( rItemRect.m_fTop <= rPos.y && rItemRect.m_fBottom >= rPos.y)
			{
				return pItem;
			}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	uint32	CGUIWgtListBox::OnMouseMove( CGUIEventMouse* pEvent )
	{
		CGUIListBoxItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
		if( pItem )
		{
			CGUIEventMouse aEventMouse;
			aEventMouse.SetEventId(eEVENT_MOUSE_MOVE);
			aEventMouse.SetReceiver(pItem);
			aEventMouse.SetPosition(pEvent->GetPosition());
			aEventMouse.SetKeyboardInterface(pEvent->GetKeyboardInterface());
			CGUIWidgetSystem::Instance()->SendEvent(&aEventMouse);
		}

		return CGUIWidget::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtListBox::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		CGUIListBoxItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
		if( pItem )
		{
			CGUIEventMouse aEventMouse;
			aEventMouse.SetEventId(eEVENT_MOUSE_DOWN);
			aEventMouse.SetButton( pEvent->GetButton() );
			aEventMouse.SetReceiver(pItem);
			aEventMouse.SetPosition(pEvent->GetPosition());
			aEventMouse.SetKeyboardInterface(pEvent->GetKeyboardInterface());
			CGUIWidgetSystem::Instance()->SendEvent(&aEventMouse);
		}

		return CGUIWidget::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtListBox::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		CGUIListBoxItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
		if( pItem )
		{
			CGUIEventMouse aEventMouse;
			aEventMouse.SetEventId(eEVENT_MOUSE_UP);
			aEventMouse.SetButton( pEvent->GetButton() );
			aEventMouse.SetReceiver(pItem);
			aEventMouse.SetPosition(pEvent->GetPosition());
			aEventMouse.SetKeyboardInterface(pEvent->GetKeyboardInterface());
			CGUIWidgetSystem::Instance()->SendEvent(&aEventMouse);
		}

		return CGUIWidget::OnMouseLeftUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtListBox::OnMouseRightDown( CGUIEventMouse* pEvent )
	{
		CGUIListBoxItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
		if( pItem )
		{
			CGUIEventMouse aEventMouse;
			aEventMouse.SetEventId(eEVENT_MOUSE_DOWN);
			aEventMouse.SetButton( pEvent->GetButton() );
			aEventMouse.SetReceiver(pItem);
			aEventMouse.SetPosition(pEvent->GetPosition());
			aEventMouse.SetKeyboardInterface(pEvent->GetKeyboardInterface());
			CGUIWidgetSystem::Instance()->SendEvent(&aEventMouse);
		}

		return CGUIWidget::OnMouseRightDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtListBox::OnMouseRightUp( CGUIEventMouse* pEvent )
	{
		CGUIListBoxItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
		if( pItem )
		{
			CGUIEventMouse aEventMouse;
			aEventMouse.SetEventId(eEVENT_MOUSE_UP);
			aEventMouse.SetButton( pEvent->GetButton() );
			aEventMouse.SetReceiver(pItem);
			aEventMouse.SetPosition(pEvent->GetPosition());
			aEventMouse.SetKeyboardInterface(pEvent->GetKeyboardInterface());
			CGUIWidgetSystem::Instance()->SendEvent(&aEventMouse);
		}

		return CGUIWidget::OnMouseRightUp(pEvent);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::ProcessMouseLeftDown(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent)
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
	void		CGUIWgtListBox::ProcessMouseLeftUp(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent)
	{

	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::ProcessMouseRightDown(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		ClearAllSelections_impl();
		SetItemSelected(pItem, true);
		m_pLastOperateItem = pItem;
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtListBox::ProcessMouseRightUp(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent)
	{

	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::ProcessMouseLeftClick(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		if( pItem == m_pLastOperateItem )
		{
			CGUIEventListBox aEvent;
			aEvent.SetEventId(eEVENT_LISTBOX_CLICK_LEFT);
			aEvent.SetReceiver(this);
			aEvent.SetDbClickedItemIdx( GetItemIndex(pItem) );
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::ProcessMouseRightClick(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		if( pItem == m_pLastOperateItem )
		{
			CGUIEventListBox aEvent;
			aEvent.SetEventId(eEVENT_LISTBOX_CLICK_RIGHT);
			aEvent.SetReceiver(this);
			aEvent.SetDbClickedItemIdx( GetItemIndex(pItem) );
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::ProcessMouseLeftDbClick(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		if( pItem == m_pLastOperateItem )
		{
			CGUIEventListBox aEvent;
			aEvent.SetEventId(eEVENT_LISTBOX_DBCLICK_LEFT);
			aEvent.SetReceiver(this);
			aEvent.SetDbClickedItemIdx( GetItemIndex(pItem) );
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtListBox::ProcessMouseRightDbClick(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		if( pItem == m_pLastOperateItem )
		{
			CGUIEventListBox aEvent;
			aEvent.SetEventId(eEVENT_LISTBOX_DBCLICK_RIGHT);
			aEvent.SetReceiver(this);
			aEvent.SetDbClickedItemIdx( GetItemIndex(pItem) );
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtListBox::ProcessMouseMove(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent)
	{

	}
	//------------------------------------------------------------------------------
	void		CGUIWgtListBox::ProcessProperty( const CGUIProperty* pProperty)
	{
		CGUIWgtScrollbarContainer::ProcessProperty(pProperty);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for MULTI_SELECT
		/*
		*<property name="MULTI_SELECT" type="BOOL" value="false" />
		*/
		if( pProperty->GetName() == "MULTI_SELECT" && pProperty->GetType()=="BOOL")
		{
			EnableMultiselect(StringToValue(pProperty->GetValue()));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for SORT
		/*
		*<property name="SORT" type="BOOL" value="false" />
		*/
		else if( pProperty->GetName() == "SORT" && pProperty->GetType()=="BOOL")
		{
			SetSorting(StringToValue(pProperty->GetValue()));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for text item
		/*
		*<property name="TEXT_ITEM" type="STRING" value="abc" />
		*/
		else if( pProperty->GetName() == "TEXT_ITEM" && pProperty->GetType()=="STRING")
		{
			CGUIStringEx aString;
			CGUIStringConvertor::MultiByteToWideChar(pProperty->GetValue(), aString);
			AddItem(aString.GetContent());
		}
	}
	//------------------------------------------------------------------------------
}
