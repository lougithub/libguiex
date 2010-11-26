/** 
* @file guiwgtcolumnlist.cpp
* @brief column list box
* @author ken
* @date 2007-08-02
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtcolumnlist.h>
#include <libguiex_widget/guiwgtscrollbar.h>
#include <libguiex_widget/guicollisttextitem.h>
#include <libguiex_widget/guicollistheader.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiinterfacekeyboard.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiwidgetmanager.h>

#include <algorithm>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	SGridRef::SGridRef(uint32 nRow, uint32 nColumn)
		:m_nRow(nRow)
		,m_nColumn(nColumn)
	{

	}
	//------------------------------------------------------------------------------
	SGridRef& SGridRef::operator=(const SGridRef& rhs)
	{
		m_nRow = rhs.m_nRow;
		m_nColumn = rhs.m_nColumn;
		return *this;
	}
	//------------------------------------------------------------------------------
	bool SGridRef::operator<(const SGridRef& rhs) const
	{
		if ((m_nRow < rhs.m_nRow) ||
			((m_nRow == rhs.m_nRow) && (m_nColumn < rhs.m_nColumn)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	bool SGridRef::operator<=(const SGridRef& rhs) const
	{
		return !operator>(rhs);
	}
	//------------------------------------------------------------------------------
	bool SGridRef::operator>(const SGridRef& rhs) const
	{
		return (operator<(rhs) || operator==(rhs)) ? false : true;
	}
	//------------------------------------------------------------------------------
	bool SGridRef::operator>=(const SGridRef& rhs) const
	{
		return !operator<(rhs);
	}
	//------------------------------------------------------------------------------
	bool SGridRef::operator==(const SGridRef& rhs) const
	{
		return ((m_nColumn == rhs.m_nColumn) && (m_nRow == rhs.m_nRow)) ? true : false;
	}
	//------------------------------------------------------------------------------
	bool SGridRef::operator!=(const SGridRef& rhs) const
	{
		return !operator==(rhs);
	}
	//------------------------------------------------------------------------------
	CGUIColListItem* const& CGUIWgtColumnList::SListRow::operator[](uint32 idx) const
	{
		if( idx >= m_vecItems.size())
		{
			throw CGUIException("the index is outof range of row item list");
		}
		return m_vecItems[idx];
	}
	//------------------------------------------------------------------------------
	CGUIColListItem*&	CGUIWgtColumnList::SListRow::operator[](uint32 idx)
	{
		if( idx >= m_vecItems.size())
		{
			throw CGUIException("the index is outof range of row item list");
		}
		return m_vecItems[idx];
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtColumnList::SListRow::operator<(const SListRow& rhs) const
	{
		CGUIColListItem* a = m_vecItems[m_nSortColumn];
		CGUIColListItem* b = rhs.m_vecItems[m_nSortColumn];

		// handle cases with empty slots
		if (!b)
		{
			return false;
		}
		else if (!a)
		{
			return true;
		}
		else
		{
			return *a < *b;
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtColumnList::SListRow::operator>(const SListRow& rhs) const
	{
		CGUIColListItem* a = m_vecItems[m_nSortColumn];
		CGUIColListItem* b = rhs.m_vecItems[m_nSortColumn];

		// handle cases with empty slots
		if (!a)
		{
			return false;
		}
		else if (!b)
		{
			return true;
		}
		else
		{
			return *a > *b;
		}
	}
	//------------------------------------------------------------------------------




	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtColumnList);
	//------------------------------------------------------------------------------
	CGUIWgtColumnList::CGUIWgtColumnList( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtScrollbarContainer(StaticGetType(), rName, rSceneName)
	{
		InitListBox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtColumnList::CGUIWgtColumnList( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtScrollbarContainer(rType, rName, rSceneName)
	{
		InitListBox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtColumnList::~CGUIWgtColumnList( )
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::InitListBox()
	{
		m_bSorted = false;
		m_bMultiselect = false;
		m_bItemTooltips = false;

		m_pImageBG = NULL;
		m_pImageSelection = NULL;
		m_pLastOperateItem = NULL;

		m_strItemType = "CGUIColListTextItem";

		//header
		m_pHeader = static_cast<CGUIColListHeader*>(GUI_CREATE_WIDGET( "CGUIColListHeader", GetName()+"_header",GetSceneName()) );
		m_pHeader->SetParent(this);
		m_pHeader->SetAnchorPoint(0.0f, 0.0f);
		m_pHeader->SetLocalPosition(0.0f,0.0f);

		//set flag
		SetFocusable(true);
		SetGenerateDBClickEvent(true);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtColumnList::Create()
	{
		if( !m_pImageBG )
		{
			throw CGUIException("[CGUIWgtCheckButton::Create]: the image <BGIMAGE> hasn't been found!");
		}
		if( !m_pImageSelection)
		{
			throw CGUIException("[CGUIWgtCheckButton::Create]: the image <SELECTION_COLOR> hasn't been found!");
		}

		m_pHeader->Create();

		CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtColumnList::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
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
		else if( rName == "COLUMN_HEADER_NORMAL" || 
			rName == "COLUMN_HEADER_HOVER" ||
			rName == "COLUMN_HEADER_PUSH" )
		{
			m_pHeader->SetImage(rName, pImage);
		}
		else 
		{
			CGUIWgtScrollbarContainer::OnSetImage(rName, pImage );
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::SetValue(const CGUIString& rName, const CGUIString& rValue)
	{
		CGUIWidget::SetValue(rName, rValue);
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWgtColumnList::GetValue(const CGUIString& rName) const
	{
		return CGUIWidget::GetValue( rName );
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::GetItemCount(void) const	
	{
		return static_cast<uint32>(m_aListItems.size());
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::GetSelectedCount(void) const
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
	CGUIColListItem*	CGUIWgtColumnList::GetFirstSelectedItem(void) const
	{
		return GetNextSelectedItem(NULL);
	}
	//------------------------------------------------------------------------------
	CGUIColListItem*	CGUIWgtColumnList::GetNextSelectedItem(const CGUIColListItem* pStartItem) const
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
	CGUIColListItem*	CGUIWgtColumnList::GetSelectedItem( uint32 nIdx) const
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
	void	CGUIWgtColumnList::SetItemSelected(CGUIColListItem* pItem, bool bSelected)
	{
		TListItem::iterator itor = std::find(m_aListItems.begin(), m_aListItems.end(), pItem);

		if (itor != m_aListItems.end())
		{
			SetItemSelected(static_cast<uint32>(std::distance(m_aListItems.begin(), itor)), bSelected);
		}
		else
		{
			throw CGUIException("[CGUIWgtColumnList::SetItemSelected]: the specified Item is not attached to this Listbox.");
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::SetItemSelected(uint32 nItemIndex, bool bSelected)
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
			throw CGUIException("[CGUIWgtColumnList::SetItemSelected]: the index <%d> of Item is out of range.", nItemIndex);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::ClearAllSelections(void)
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
	CGUIColListItem*	CGUIWgtColumnList::GetItemFromIndex(uint32 nIndex) const
	{
		if (nIndex < m_aListItems.size())
		{
			return m_aListItems[nIndex];
		}
		else
		{
			throw CGUIException("[CGUIWgtColumnList::GetItemFromIndex]: the index <%d> of Item is out of range.", nIndex);
		}
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::GetItemIndex(const CGUIColListItem* pItem) const
	{
		TListItem::const_iterator itor = std::find(m_aListItems.begin(), m_aListItems.end(), pItem);

		if (itor != m_aListItems.end())
		{
			return static_cast<uint32>(std::distance(m_aListItems.begin(), itor));
		}
		else
		{
			throw CGUIException("[CGUIWgtColumnList::GetItemIndex]: the specified Item is not attached to this listbox.");
		}
	}
	//------------------------------------------------------------------------------
	bool listboxitem_less(const CGUIColListItem* a, const CGUIColListItem* b)
	{
		return *a < *b;
	}
	void	CGUIWgtColumnList::AddItem(CGUIColListItem* pItem)
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

			//SetRectDirty();
			UpdateScrollbars();
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::AddItem( const CGUIStringW& rText )
	{
		CGUIColListTextItem* pItem= CGUIColListTextItem::CreateTextItem(pText, GetTextInfo(), GetSceneName());
		pItem->SetSelectionImage( m_pImageSelection );
		AddItem( pItem );
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::AddChild_Imp(CGUIColListItem* pItem)
	{
		AddChild(pItem);
		if( IsOpen())
		{
			pItem->Open();
		}
	}
	//------------------------------------------------------------------------------
	CGUIColListItem*	CGUIWgtColumnList::CreateItem_Imp(const CGUIStringW& rText)
	{
		CGUIString strItemName = GSystem->GenerateAnonymousName();
		CGUIColListItem * pItem = static_cast<CGUIColListItem*>(GUI_CREATE_WIDGET(m_strItemType.c_str(), strItemName, GetSceneName()));
		pItem->SetItemInfo(rText, GetSegmentTextInfo());
		return pItem;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::InsertItem(CGUIColListItem* pItem, uint32 nIndex )
	{
		InsertItem( pItem, GetItemFromIndex(nIndex));
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::InsertItem( const CGUIStringW& rText, const CGUIColListItem* pPosition)
	{
		CGUIColListTextItem* pItem= CGUIColListTextItem::CreateTextItem(pText,GetTextInfo(), GetSceneName());
		pItem->SetSelectionImage( m_pImageSelection );
		InsertItem( pItem, pPosition);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::InsertItem( const CGUIStringW& rText, uint32 nIndex )
	{
		CGUIColListTextItem* pItem= CGUIColListTextItem::CreateTextItem(pText,GetTextInfo(), GetSceneName());
		pItem->SetSelectionImage( m_pImageSelection );
		InsertItem( pItem, GetItemFromIndex(nIndex));
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::InsertItem(CGUIColListItem* pItem, const CGUIColListItem* pPosition)
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
					throw CGUIException("[CGUIWgtColumnList::InsertItem]: the specified Item for parameter 'pPosition' is not attached to this Listbox.");
				}
			}

			m_aListItems.insert(itor, pItem);
			AddChild_Imp(pItem);
			UpdateItemPos();


			//SetRectDirty();
			UpdateScrollbars();
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::RemoveItem( CGUIColListItem* pItem)
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
				GSystem->DestroyWidget(pItem);

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
	void	CGUIWgtColumnList::RemoveItem( uint32 nIndex)
	{
		RemoveItem(GetItemFromIndex(nIndex) );
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::ClearList(void)
	{
		if (ClearList_impl())
		{

		}

//		SetRectDirty();
		UpdateScrollbars();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::EnableMultiselect(bool bEnable)
	{
		if (m_bMultiselect != bEnable)
		{
			m_bMultiselect = bEnable;

			// if we change to single-select, deselect all except the first selected item.
			bool bDeselected = false;
			if ((!m_bMultiselect) && (GetSelectedCount() > 1))
			{
				CGUIColListItem* itm = GetFirstSelectedItem();

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
				GSystem->SendEvent( &aEvent);
			}
		}

	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtColumnList::IsMultiselectEnabled(void) const
	{
		return m_bMultiselect;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtColumnList::IsItemTooltipsEnabled(void) const
	{
		return m_bItemTooltips;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::EnableItemTooltips(bool bEnable)
	{
		m_bItemTooltips = bEnable;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtColumnList::IsItemSelected(uint32 nIndex) const
	{
		if (nIndex < m_aListItems.size())
		{
			return m_aListItems[nIndex]->IsSelected();
		}
		else
		{
			throw CGUIException("[CGUIWgtColumnList::]: the specified index <%d> is out of range for this Listbox.", nIndex);
		}
	}
	//------------------------------------------------------------------------------
	CGUIColListItem*	CGUIWgtColumnList::FindItemWithText(
		const CGUIStringW& rText, const CGUIColListItem* pStartItem)
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
	bool	CGUIWgtColumnList::IsListboxItemInList(const CGUIColListItem* pItem) const
	{
		return std::find(m_aListItems.begin(), m_aListItems.end(), pItem) != m_aListItems.end();
	}
	//------------------------------------------------------------------------------
	bool listboxitem_greater(const CGUIColListItem* a, const CGUIColListItem* b)
	{
		return *a > *b;
	}
	void	CGUIWgtColumnList::SetSorting(bool bSorting)
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
	bool 	CGUIWgtColumnList::IsSorting() const
	{
		return m_bSorted;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::MakeItemVisible(uint32 nItemIndex)
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
	void	CGUIWgtColumnList::MakeItemVisible(const CGUIColListItem* pItem)
	{
		MakeItemVisible(GetItemIndex(pItem));
	}
	//------------------------------------------------------------------------------
	void CGUIWgtColumnList::RenderSelf(IGUIInterfaceRender* pRender)
	{
		/// draw bg
		DrawImage( pRender, m_pImageBG, GetRect());
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtColumnList::ClearAllSelections_impl(void)
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
	void	CGUIWgtColumnList::UpdateItemPos()
	{
		//CGUIVector2	aPos(0.0f,0);
		//for( TListItem::iterator itor = m_aListItems.begin(); itor != m_aListItems.end(); ++itor)
		//{
		//	CGUIColListItem* pItem = *itor;
		//	pItem->SetLocalPosition(aPos);
		//	aPos.y += pItem->GetHeight();
		//}

		CGUIVector2	aPos(0.0f,0.0f);
		for( TListRow::iterator itor = m_aListRows.begin();
			itor != m_aListRows.end();
			++itor)
		{
			SListRow& aRow = *itor;
			for(uint32 i=0; i<aRow.m_vecItems.size(); ++i)
			{
				if( aRow.m_vecItems[i] )
				{
					aRow.m_vecItems[i]->SetLocalPosition(aPos);
				}
				aPos.x += GetSegmentWidthByIndex(i);
			}

			aPos.x = 0.0f;
			aPos.y += GetRowHeight(aRow);
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtColumnList::ClearList_impl(void)
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
				GSystem->DestroyWidget(m_aListItems[i]);
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
	real	CGUIWgtColumnList::GetTotalItemsHeight() const
	{
		real fHeight = 0.0f;
		for( size_t i=0; i<m_aListItems.size(); ++i )
		{
			fHeight += m_aListItems[i]->GetHeight();
		}

		return fHeight;
	}
	//------------------------------------------------------------------------------
	real	CGUIWgtColumnList::GetWidestItemsWidth() const
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
	CGUIColListItem*	CGUIWgtColumnList::GetItemUnderPoint(const CGUIVector2& rPos)
	{
		for( TListItem::iterator itor = m_aListItems.begin();
			itor != m_aListItems.end();
			++itor)
		{
			CGUIColListItem* pItem = *itor;
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
	uint32	CGUIWgtColumnList::GetSegmentCount(void) const
	{
		return m_pHeader->GetSegmentCount();
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::GetRowCount(void) const
	{
		return static_cast<uint32>(m_aListRows.size());
	}
	//------------------------------------------------------------------------------
	real	CGUIWgtColumnList::GetRowHeight( const SListRow& rRow ) const
	{
		real fHeight = 0.0f;
		for( SListRow::TRowItems::const_iterator itor = rRow.m_vecItems.begin();
			itor != rRow.m_vecItems.end();
			++itor)
		{
			if( *itor )
			{
				fHeight = (*itor)->GetHeight();
				break;
			}
		}
		return fHeight;
	}
	//------------------------------------------------------------------------------
	//void	CGUIWgtColumnList::UpdateDirtyRect()
	//{
	//	UpdateDirtyRect_SC_Begin();

	//	real fHeaderHeight = m_pHeader->GetSize().m_fHeight * m_pHeader->GetDerivedScale().m_fHeight;
	//	m_aClientRect.m_fTop += fHeaderHeight;
	//	m_aClientClipRect.m_fTop += fHeaderHeight;


	//	m_aClientRect.SetWidth(0.0f);
	//	m_aClientRect.SetHeight(0.0f);

	//	// loop through the items
	//	uint32 itemCount = static_cast<uint32>(m_aListItems.size());
	//	for (TListRow::iterator itor = m_aListRows.begin();
	//		itor != m_aListRows.end();
	//		++itor )
	//	{
	//		m_aClientRect.SetHeight(m_aClientRect.GetHeight() + GetRowHeight(*itor));
	//	}
	//	m_aClientRect.SetWidth(GetSegmentTotalWidth());

	//	UpdateDirtyRect_SC_End();

	//}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::UpdateDirtyRect_SC_End()
	{
		real aOldX = m_aClientRect.m_fLeft;
		CGUIWgtScrollbarContainer::UpdateDirtyRect_SC_End();
		m_pHeader->SetLocalPosition(m_aClientRect.m_fLeft - aOldX, 0.0f);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::SetSegmentTextInfo(const CGUIStringInfo& rInfo)
	{
		m_pHeader->SetSegmentTextInfo(rInfo);
	}
	//------------------------------------------------------------------------------
	const CGUIStringInfo&	CGUIWgtColumnList::GetSegmentTextInfo() const
	{
		return m_pHeader->GetSegmentTextInfo();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::AddSegment( const CGUIStringW& rText, uint32 nId )
	{
		m_pHeader->AddSegment(pText, nId);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::SetDefaultSegmentSize( const CGUISize& rSize)
	{
		m_pHeader->SetDefaultSegmentSize(rSize);
	}
	//------------------------------------------------------------------------------
	const CGUISize& CGUIWgtColumnList::GetDefaultSegmentSize( ) const
	{
		return m_pHeader->GetDefaultSegmentSize();
	}
	//------------------------------------------------------------------------------
	real	CGUIWgtColumnList::GetSegmentWidthByIndex( uint32 nIdx) const 
	{
		return m_pHeader->GetSegmentWidthByIndex( nIdx );
	}
	//------------------------------------------------------------------------------
	real	CGUIWgtColumnList::GetSegmentTotalWidth( ) const
	{
		return m_pHeader->GetSegmentTotalWidth();
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIWgtColumnList::GetSegmentPosByIndex( uint32 nIdx) const
	{
		return m_pHeader->GetSegmentPosByIndex( nIdx );
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::SetSortSegment(uint32 nIdx)
	{
		m_pHeader->SetSortSegment(nIdx);
	}
	//------------------------------------------------------------------------------
	CGUIColListHeaderSegment*	CGUIWgtColumnList::GetSortSegment(void) const
	{
		return m_pHeader->GetSortSegment();
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::GetSortSegmentIndex(void) const
	{
		return m_pHeader->GetSortSegmentIndex();
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::AddRow(uint32 nRowId /*= 0*/)
	{
		uint32 nColIdx = 0;

		// Build the new row
		SListRow aRow;
		aRow.m_nSortColumn = GetSortSegmentIndex();
		aRow.m_vecItems.resize(GetSegmentCount(), NULL);
		aRow.m_nRowID = nRowId;

		uint32 nPos;

		//// if sorting is enabled, insert at an appropriate position
		//if (GetSortDirection() != ListHeaderSegment::None)
		//{
		//	// calculate where the row should be inserted
		//	ListItemGrid::iterator ins_pos = std::upper_bound(d_grid.begin(), d_grid.end(), row);
		//	// insert item and get final inserted position.
		//	ListItemGrid::iterator final_pos = d_grid.insert(ins_pos, row);
		//	// get final inserted position as an uint.
		//	pos = (uint)std::distance(d_grid.begin(), final_pos);
		//}
		//// not sorted, just stick it on the end.
		//else
		{
			nPos = GetRowCount();
			m_aListRows.push_back(aRow);
		}

		//// signal a change to the list contents
		//WindowEventArgs args(this);
		//onListContentsChanged(args);

		return nPos;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::SetItem(CGUIColListItem* pItem, const SGridRef& rGrid)
	{
		// validate grid ref
		if (rGrid.m_nColumn >= GetSegmentCount())
		{
			throw CGUIException("CGUIWgtColumnList::SetItem - the specified column index is invalid.");
		}
		else if (rGrid.m_nRow >= GetRowCount())
		{
			throw CGUIException("CGUIWgtColumnList::SetItem - the specified row index is invalid.");
		}

		// delete old item as required
		CGUIColListItem* oldItem = m_aListRows[rGrid.m_nRow][rGrid.m_nColumn];
		if (oldItem)
		{
			oldItem->SetParent(NULL);
			CGUIWidgetManager::Instance->DestroyWidget(oldItem);
		}

		// set new item.
		if (pItem)
		{
			pItem->SetParent(this);
		}

		m_aListRows[rGrid.m_nRow][rGrid.m_nColumn] = pItem;
		pItem->SetWidth(GetSegmentWidthByIndex(rGrid.m_nColumn));
		UpdateItemPos();

//		SetRectDirty();
		UpdateScrollbars();

		//// signal a change to the list contents
		//WindowEventArgs args(this);
		//onListContentsChanged(args);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::SetItem(const CGUIStringW& rText, const SGridRef& rGrid)
	{
		CGUIColListTextItem* pItem= CGUIColListTextItem::CreateTextItem(pText, GetTextInfo(), GetSceneName());
		pItem->SetSelectionImage( m_pImageSelection );
		SetItem( pItem, rGrid );
	}
	//------------------------------------------------------------------------------






	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::OnMouseMove( CGUIEventMouse* pEvent )
	{
		CGUIColListItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
		if( pItem )
		{
			CGUIEventMouse aEventMouse;
			aEventMouse.SetEventId(eEVENT_MOUSE_MOVE);
			aEventMouse.SetReceiver(pItem);
			aEventMouse.SetPosition(pEvent->GetPosition());
			aEventMouse.SetKeyboardInterface(pEvent->GetKeyboardInterface());
			GSystem->SendEvent(&aEventMouse);
		}

		return CGUIWidget::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		CGUIColListItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
		if( pItem )
		{
			CGUIEventMouse aEventMouse;
			aEventMouse.SetEventId(eEVENT_MOUSE_DOWN);
			aEventMouse.SetButton( pEvent->GetButton() );
			aEventMouse.SetReceiver(pItem);
			aEventMouse.SetPosition(pEvent->GetPosition());
			aEventMouse.SetKeyboardInterface(pEvent->GetKeyboardInterface());
			GSystem->SendEvent(&aEventMouse);
		}

		return CGUIWidget::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		CGUIColListItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
		if( pItem )
		{
			CGUIEventMouse aEventMouse;
			aEventMouse.SetEventId(eEVENT_MOUSE_UP);
			aEventMouse.SetButton( pEvent->GetButton() );
			aEventMouse.SetReceiver(pItem);
			aEventMouse.SetPosition(pEvent->GetPosition());
			aEventMouse.SetKeyboardInterface(pEvent->GetKeyboardInterface());
			GSystem->SendEvent(&aEventMouse);
		}

		return CGUIWidget::OnMouseLeftUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::OnMouseRightDown( CGUIEventMouse* pEvent )
	{
		CGUIColListItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
		if( pItem )
		{
			CGUIEventMouse aEventMouse;
			aEventMouse.SetEventId(eEVENT_MOUSE_DOWN);
			aEventMouse.SetButton( pEvent->GetButton() );
			aEventMouse.SetReceiver(pItem);
			aEventMouse.SetPosition(pEvent->GetPosition());
			aEventMouse.SetKeyboardInterface(pEvent->GetKeyboardInterface());
			GSystem->SendEvent(&aEventMouse);
		}

		return CGUIWidget::OnMouseRightDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtColumnList::OnMouseRightUp( CGUIEventMouse* pEvent )
	{
		CGUIColListItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
		if( pItem )
		{
			CGUIEventMouse aEventMouse;
			aEventMouse.SetEventId(eEVENT_MOUSE_UP);
			aEventMouse.SetButton( pEvent->GetButton() );
			aEventMouse.SetReceiver(pItem);
			aEventMouse.SetPosition(pEvent->GetPosition());
			aEventMouse.SetKeyboardInterface(pEvent->GetKeyboardInterface());
			GSystem->SendEvent(&aEventMouse);
		}

		return CGUIWidget::OnMouseRightUp(pEvent);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::ProcessMouseLeftDown(CGUIColListItem* pItem, CGUIEventMouse* pEvent)
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
	void		CGUIWgtColumnList::ProcessMouseLeftUp(CGUIColListItem* pItem, CGUIEventMouse* pEvent)
	{

	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::ProcessMouseRightDown(CGUIColListItem* pItem, CGUIEventMouse* pEvent)
	{
		ClearAllSelections_impl();
		SetItemSelected(pItem, true);
		m_pLastOperateItem = pItem;
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtColumnList::ProcessMouseRightUp(CGUIColListItem* pItem, CGUIEventMouse* pEvent)
	{

	}
	//------------------------------------------------------------------------------
	void	CGUIWgtColumnList::ProcessMouseLeftClick(CGUIColListItem* pItem, CGUIEventMouse* pEvent)
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
	void	CGUIWgtColumnList::ProcessMouseRightClick(CGUIColListItem* pItem, CGUIEventMouse* pEvent)
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
	void	CGUIWgtColumnList::ProcessMouseLeftDbClick(CGUIColListItem* pItem, CGUIEventMouse* pEvent)
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
	void	CGUIWgtColumnList::ProcessMouseRightDbClick(CGUIColListItem* pItem, CGUIEventMouse* pEvent)
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
	void		CGUIWgtColumnList::ProcessMouseMove(CGUIColListItem* pItem, CGUIEventMouse* pEvent)
	{

	}
	//------------------------------------------------------------------------------
	void			CGUIWgtColumnList::ProcessProperty( const CGUIProperty* pProperty)
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
		*<property name="TEXT_ITEM" type="CGUIString" value="abc" />
		*/
		else if( pProperty->GetName() == "TEXT_ITEM" && pProperty->GetType()=="CGUIString")
		{
			CGUIStringEx aString;
			CGUIStringConvertor::MultiByteToWideChar(pProperty->GetValue(), aString);
			AddItem(aString.GetContent());
		}

	}
	//------------------------------------------------------------------------------

}
