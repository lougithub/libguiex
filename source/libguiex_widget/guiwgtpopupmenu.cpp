/** 
* @file guiwgtpopupmenu.cpp
* @brief popup menu
* @author ken
* @date 2007-07-31
*/
//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtpopupmenu.h>
#include <libguiex_widget\guimenuitem.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guiinterfacemanager.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiinterfacemouse.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guistringconvertor.h>

#include <algorithm>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtPopupMenu);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtPopupMenu::ms_strType = "CGUIWgtPopupMenu";
	//------------------------------------------------------------------------------
	CGUIWgtPopupMenu::CGUIWgtPopupMenu( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitPopupMenu();
	}
	//------------------------------------------------------------------------------
	CGUIWgtPopupMenu::CGUIWgtPopupMenu( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
	{
		InitPopupMenu();
	}
	//------------------------------------------------------------------------------
	CGUIWgtPopupMenu::~CGUIWgtPopupMenu( )
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::InitPopupMenu()
	{
		m_bSorted = false;
		m_bItemTooltips = false;

		m_pImageBG = NULL;
		m_pImageSelection = NULL;

		m_nLineSpace = 4;

		m_strItemType = "CGUIMenuTextItem";

		//set flag
		SetFocusable(true);
		SetFlag(eFLAG_EVENT_DBCLICK, true);
		SetFlag(eFLAG_MOVABLE, false);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::SetMenuPos( bool bByMousePos, real x, real y )
	{
		if( bByMousePos )
		{
			IGUIInterfaceMouse* pMouse = CGUIInterfaceManager::Instance()->GetInterfaceMouse();
			CGUIVector2 aMousePos = pMouse->GetPosition();
			const CGUISize& rMenuSize = GetSize();
			const CGUISize& rScreenSize = CGUIWidgetSystem::Instance()->GetScreenSize();

			if( aMousePos.x+rMenuSize.m_fWidth > rScreenSize.m_fWidth )
			{
				aMousePos.x = rScreenSize.m_fWidth - rMenuSize.m_fWidth;
			}
			if( aMousePos.y+rMenuSize.m_fHeight > rScreenSize.m_fHeight )
			{
				aMousePos.y = rScreenSize.m_fHeight  - rMenuSize.m_fHeight;
			}
			SetGlobalPosition(aMousePos);
		}
		else
		{
			SetGlobalPosition(CGUIVector2( x, y));
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtPopupMenu::Create()
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
	void CGUIWgtPopupMenu::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
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
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::SetValue(const CGUIString& rName, const CGUIString& rValue)
	{
		CGUIWidget::SetValue(rName, rValue);
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWgtPopupMenu::GetValue(const CGUIString& rName) const
	{
		return CGUIWidget::GetValue( rName );
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::SetLineSpace(uint32 nSpace)
	{
		m_nLineSpace = nSpace;
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtPopupMenu::GetLineSpace( ) const
	{
		return m_nLineSpace;
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtPopupMenu::GetItemCount(void) const	
	{
		return static_cast<uint32>(m_aListItems.size());
	}
	//------------------------------------------------------------------------------
	CGUIMenuItem*	CGUIWgtPopupMenu::GetItemFromIndex(uint32 nIndex) const
	{
		if (nIndex < m_aListItems.size())
		{
			return m_aListItems[nIndex];
		}
		else
		{
			throw CGUIException("[CGUIWgtPopupMenu::GetItemFromIndex]: the index <%d> of Item is out of range.", nIndex);
		}
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtPopupMenu::GetItemIndex(const CGUIMenuItem* pItem) const
	{
		TListItem::const_iterator itor = std::find(m_aListItems.begin(), m_aListItems.end(), pItem);

		if (itor != m_aListItems.end())
		{
			return static_cast<uint32>(std::distance(m_aListItems.begin(), itor));
		}
		else
		{
			throw CGUIException("[CGUIWgtPopupMenu::GetItemIndex]: the specified Item is not attached to this listbox.");
		}
	}
	//------------------------------------------------------------------------------
	bool menuitem_less(const CGUIMenuItem* a, const CGUIMenuItem* b)
	{
		return *a < *b;
	}
	void	CGUIWgtPopupMenu::AddItem(CGUIMenuItem* pItem)
	{
		if (pItem != NULL)
		{
			if (IsSorting())
			{
				// if sorting is enabled, re-sort the list
				m_aListItems.insert(std::upper_bound(m_aListItems.begin(), m_aListItems.end(), pItem, menuitem_less), pItem);
				AddChild_Imp(pItem);
			}
			else
			{
				// not sorted, just stick it on the end.
				m_aListItems.push_back(pItem);
				AddChild_Imp(pItem);
			}
			UpdateMenuItem();
		}
	}
	//------------------------------------------------------------------------------
	CGUIMenuItem* CGUIWgtPopupMenu::CreateItem_Imp(const wchar_t* pText)
	{
		CGUIMenuItem * pItem = static_cast<CGUIMenuItem*>(GUI_CREATE_WIDGET(m_strItemType.c_str(), CGUIString("menuitem_")+CGUIWidgetSystem::Instance()->GenerateAnonymousName(), GetProjectName()));
		pItem->SetItemInfo(pText, GetTextInfo());
		return pItem;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::AddItem( const wchar_t* pText )
	{
		CGUIMenuItem* pItem= CreateItem_Imp(pText);
		pItem->SetSelectionImage( m_pImageSelection );
		AddItem( pItem );
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::AddChild_Imp(CGUIMenuItem* pItem)
	{
		AddChild(pItem);
		if( IsOpen())
		{
			pItem->Open();
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::InsertItem(CGUIMenuItem* pItem, uint32 nIndex )
	{
		InsertItem( pItem, GetItemFromIndex(nIndex));
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::InsertItem( const wchar_t* pText, const CGUIMenuItem* pPosition)
	{
		CGUIMenuItem* pItem= CreateItem_Imp(pText);
		pItem->SetSelectionImage( m_pImageSelection );
		InsertItem( pItem, pPosition);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::InsertItem( const wchar_t* pText, uint32 nIndex )
	{
		CGUIMenuItem* pItem= CreateItem_Imp(pText);
		pItem->SetSelectionImage( m_pImageSelection );
		InsertItem( pItem, GetItemFromIndex(nIndex));
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::InsertItem(CGUIMenuItem* pItem, const CGUIMenuItem* pPosition)
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
					throw CGUIException("[CGUIWgtPopupMenu::InsertItem]: the specified Item for parameter 'pPosition' is not attached to this Listbox.");
				}
			}

			m_aListItems.insert(itor, pItem);
			AddChild_Imp(pItem);
			UpdateMenuItem();
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::RemoveItem( CGUIMenuItem* pItem)
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
				UpdateMenuItem();

				//// if item was the last selected item, reset that to NULL
				//if (pItem == m_aLastSelected)
				//{
				//	m_aLastSelected = NULL;
				//}

				// if item is supposed to be deleted by us
				CGUIWidgetSystem::Instance()->DestroyWidget(pItem);
			}
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::RemoveItem( uint32 nIndex)
	{
		RemoveItem(GetItemFromIndex(nIndex) );
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::ClearList(void)
	{
		if (ClearList_impl())
		{

		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtPopupMenu::IsItemTooltipsEnabled(void) const
	{
		return m_bItemTooltips;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPopupMenu::EnableItemTooltips(bool bEnable)
	{
		m_bItemTooltips = bEnable;
	}
	//------------------------------------------------------------------------------
	CGUIMenuItem*	CGUIWgtPopupMenu::FindItemWithText(
		const wchar_t* pText, const CGUIMenuItem* pStartItem)
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
	bool	CGUIWgtPopupMenu::IsMenuItemInList(const CGUIMenuItem* pItem) const
	{
		return std::find(m_aListItems.begin(), m_aListItems.end(), pItem) != m_aListItems.end();
	}
	//------------------------------------------------------------------------------
	bool listboxitem_greater(const CGUIMenuItem* a, const CGUIMenuItem* b)
	{
		return *a > *b;
	}
	void	CGUIWgtPopupMenu::SetSorting(bool bSorting)
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
	bool 	CGUIWgtPopupMenu::IsSorting() const
	{
		return m_bSorted;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtPopupMenu::RenderSelf(IGUIInterfaceRender* pRender)
	{
		/// draw bg
		DrawImage( pRender, m_pImageBG, GetRect(), pRender->GetAndIncZ(),&GetClipRect());
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtPopupMenu::ClearAllSelections_impl(void)
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
	void	CGUIWgtPopupMenu::UpdateMenuItem()
	{
		CGUISize	aMenuSize(0.0f, 0.0f);
		CGUIVector2	aPosLeftTop(0.0f,0.0f);
		for( TListItem::iterator itor = m_aListItems.begin(); itor != m_aListItems.end(); ++itor)
		{
			//update item pos
			CGUIMenuItem* pItem = *itor;
			aPosLeftTop.y += m_nLineSpace;
			pItem->SetLocalPosition(aPosLeftTop);
			aPosLeftTop.y += pItem->GetHeight();
			aPosLeftTop.y += m_nLineSpace;

			//calculate menu size
			if( pItem->GetWidth() > aMenuSize.m_fWidth)
			{
				aMenuSize.m_fWidth =  pItem->GetWidth();
			}
		}
		aMenuSize.m_fHeight = aPosLeftTop.y;

		SetSize( aMenuSize);
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtPopupMenu::ClearList_impl(void)
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

			UpdateMenuItem();

			return true;
		}
	}
	//------------------------------------------------------------------------------
	real	CGUIWgtPopupMenu::GetTotalItemsHeight() const
	{
		real fHeight = 0.0f;
		for( size_t i=0; i<m_aListItems.size(); ++i )
		{
			fHeight += m_aListItems[i]->GetHeight();
		}

		return fHeight;
	}
	//------------------------------------------------------------------------------
	real	CGUIWgtPopupMenu::GetWidestItemsWidth() const
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
	void	CGUIWgtPopupMenu::UpdateSelf()
	{
		CGUIWidget::UpdateSelf();

		if( !IsFocus())
		{
			//lost focus
			CGUIWidgetSystem::Instance()->ClosePopupWidget(this);
		}
	}	
	//------------------------------------------------------------------------------
	CGUIMenuItem*	CGUIWgtPopupMenu::GetItemUnderPoint(const CGUIVector2& rPos)
	{
		for( TListItem::iterator itor = m_aListItems.begin();
			itor != m_aListItems.end();
			++itor)
		{
			CGUIMenuItem* pItem = *itor;
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
	uint32	CGUIWgtPopupMenu::OnMouseMove( CGUIEventMouse* pEvent )
	{
		CGUIMenuItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
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
	uint32	CGUIWgtPopupMenu::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		CGUIMenuItem* pItem = GetItemUnderPoint( pEvent->GetPosition());
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
	void		CGUIWgtPopupMenu::ProcessMouseLeftUp(CGUIMenuItem* pItem, CGUIEventMouse* pEvent)
	{
		ClearAllSelections_impl();
		CGUIWidgetSystem::Instance()->ClosePopupWidget(this);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtPopupMenu::ProcessMouseMove(CGUIMenuItem* pItem, CGUIEventMouse* pEvent)
	{
		ClearAllSelections_impl();
		pItem->SetSelected(true);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtPopupMenu::ProcessProperty( const CGUIProperty* pProperty)
	{
		CGUIWidget::ProcessProperty(pProperty);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for SORT
		/*
		*<property name="SORT" type="BOOL" value="false" />
		*/
		if( pProperty->GetName() == "SORT" && pProperty->GetType()=="BOOL")
		{
			SetSorting(CGUIStringConvertor::StringToBool(pProperty->GetValue()));
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
