/** 
* @file guiwgtcombobox.cpp
* @brief combobox used in this guiex system
* @author ken
* @date 2006-09-22
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtcombobox.h>
#include <libguiex_widget\guiwgtlistbox.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guievent.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guipropertymanager.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CGUIWgtComboBoxDropList : public CGUIWgtListBox
	{
	public:
		/// constructor
		CGUIWgtComboBoxDropList(const CGUIString& rName, const CGUIString& rProjectName );

	protected:
		///initialize
		void	InitComboBoxDropList();

		//update dirty rect
		virtual void		PostUpdateDirtyRect();	

	protected:
		virtual uint32		OnLostFocus( CGUIEventNotification* pEvent );

		virtual void		ProcessMouseLeftUp(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);
		virtual void		ProcessMouseMove(CGUIListBoxItem* pItem, CGUIEventMouse* pEvent);

	private:
		static CGUIString	ms_strType;
	};
}

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUIString	CGUIWgtComboBoxDropList::ms_strType = "CGUIWgtComboBoxDropList";
	//------------------------------------------------------------------------------
	CGUIWgtComboBoxDropList::CGUIWgtComboBoxDropList(const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWgtListBox(ms_strType,rName, rProjectName )
	{
		InitComboBoxDropList();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBoxDropList::InitComboBoxDropList()
	{
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtComboBoxDropList::PostUpdateDirtyRect()
	{
		CGUIRect	aClientClipRect = m_aClientClipRect;

		CGUIWgtListBox::PostUpdateDirtyRect();

		m_aClipRect = CGUIWidgetSystem::Instance()->GetScreenRect().GetIntersection(m_aWidgetRect);
		m_aClientClipRect = CGUIWidgetSystem::Instance()->GetScreenRect().GetIntersection(aClientClipRect);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtComboBoxDropList::OnLostFocus( CGUIEventNotification* pEvent )
	{
		if( IsOpen() )
		{
			Close();
		}
		return CGUIWidget::OnLostFocus(pEvent);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtComboBoxDropList::ProcessMouseMove( CGUIListBoxItem* pItem,CGUIEventMouse* pEvent )
	{
		ClearAllSelections_impl();
		pItem->SetSelected(true);

	}
	//------------------------------------------------------------------------------
	void		CGUIWgtComboBoxDropList::ProcessMouseLeftUp( CGUIListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		static_cast<CGUIWgtComboBox*>(GetParent())->SetSelectedItem(pItem);
		if( IsOpen() )
		{
			Close();
		}
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtComboBox);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtComboBox::ms_strType = "CGUIWgtComboBox";
	//------------------------------------------------------------------------------
	CGUIWgtComboBox::CGUIWgtComboBox( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitComboBox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtComboBox::CGUIWgtComboBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
	{
		InitComboBox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtComboBox::~CGUIWgtComboBox( )
	{
		//delete drop list
		m_pDropList->SetParent(NULL);
		delete m_pDropList;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::InitComboBox()
	{
		m_pImageBG = NULL;
		m_pSelectedItem = NULL;

		//create drop list
		m_pDropList = new CGUIWgtComboBoxDropList(GetName()+"_DropList__auto__", GetProjectName());
		m_pDropList->SetParent( this);
		m_pDropList->SetOpenWithParent( false );
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtComboBox::Create()
	{
		if( !m_pImageBG )
		{
			throw CGUIException("[CGUIWgtComboBox::Create]: the image <BGIMAGE> hasn't been found!");
		}

		//drop list
		m_pDropList->SetLocalPosition( 0.0f, GetSize().GetHeight());
		m_pDropList->Create();

		return CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::OnSetImage( const CGUIString& rName, CGUIImage* pImage)
	{
		if( rName == "BGIMAGE")
		{
			m_pImageBG = pImage;
			SetSize(pImage->GetSize());
		}
		else if( rName == "DROPLIST_BGIMAGE")
		{
			m_pDropList->SetImage("BGIMAGE", pImage);
		}
		else if( rName == "DROPLIST_SELECTION_COLOR")
		{
			m_pDropList->SetImage("SELECTION_COLOR", pImage);
		}
		else
		{
			m_pDropList->SetImage(rName, pImage);
		}
	}	
	//------------------------------------------------------------------------------
	void			CGUIWgtComboBox::SetTextInfo( const CGUIStringInfo& rInfo)
	{
		m_pDropList->SetTextInfo(rInfo);
		CGUIWidget::SetTextInfo(rInfo);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIRect aClientArea = GetClientRect();

		/// draw bg
		DrawImage( pRender, m_pImageBG, GetRect() );

		/// draw item
		if( m_pSelectedItem )
		{
			DrawString( pRender, GetText(),GetClientRect(), GetTextAlignment(), &GetClipRect());
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::SetDropListSize( const CGUISize& rSize )
	{
		m_pDropList->SetSize( rSize );
	}
	//------------------------------------------------------------------------------
	const CGUISize&	CGUIWgtComboBox::GetDropListSize(  ) const
	{
		return m_pDropList->GetSize( );
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtComboBox::SetSelectedItem(CGUIListBoxItem* pItem)
	{
		uint32 nIdx = m_pDropList->GetItemIndex(pItem);
		m_pSelectedItem = pItem;
		m_pSelectedItem->SetSelected(false);
		if( !m_pSelectedItem )
		{
			throw CGUIException("[CGUIWgtComboBox::SetSelectedItem]: the item doesn't exist!");
		}
		SetTextContent(m_pSelectedItem->GetTextContent());
		SetTextInfo(m_pSelectedItem->GetTextInfo());

		//send event
		CGUIEventComboBox aEvent;
		aEvent.SetEventId(eEVENT_COMBOBOX_SELECTED);
		aEvent.SetReceiver(this);
		aEvent.SetSelectedItemIdx( nIdx );
		CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtComboBox::SetSelectedItem(uint32 nIdx )
	{
		CGUIListBoxItem* pItem = m_pDropList->GetItemFromIndex(nIdx);
		SetSelectedItem(pItem);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtComboBox::GetSelectedItemIndex() const
	{
		if( !m_pSelectedItem )
		{
			return GUI_INVALID;
		}

		return  m_pDropList->GetItemIndex(m_pSelectedItem);
	}
	//------------------------------------------------------------------------------
	const CGUIListBoxItem*	CGUIWgtComboBox::GetSelectedItem() const
	{
		return m_pSelectedItem;
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtComboBox::GetItemCount(void) const
	{
		return m_pDropList->GetItemCount();
	}	
	//------------------------------------------------------------------------------
	CGUIListBoxItem*	CGUIWgtComboBox::GetItemFromIndex(uint32 nIndex) const
	{
		return m_pDropList->GetItemFromIndex(nIndex);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtComboBox::GetItemIndex(const CGUIListBoxItem* pItem) const
	{
		return m_pDropList->GetItemIndex(pItem);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::AddItem(CGUIListBoxItem* pItem)
	{
		m_pDropList->AddItem(pItem);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::AddItem( const wchar_t* pText )
	{
		m_pDropList->AddItem(pText);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::InsertItem(CGUIListBoxItem* pItem, const CGUIListBoxItem* pPosition)
	{
		m_pDropList->InsertItem(pItem,pPosition);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::InsertItem(CGUIListBoxItem* pItem, uint32 nIndex )
	{
		m_pDropList->InsertItem(pItem,nIndex);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::InsertItem( const wchar_t* pText, const CGUIListBoxItem* pPosition)
	{
		m_pDropList->InsertItem(pText,pPosition);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::InsertItem( const wchar_t* pText, uint32 nIndex )
	{
		m_pDropList->InsertItem(pText,nIndex);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::RemoveItem( CGUIListBoxItem* pItem)
	{
		m_pDropList->RemoveItem(pItem);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::RemoveItem( uint32 nIndex)
	{
		m_pDropList->RemoveItem(nIndex);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::ClearList(void)
	{
		m_pDropList->ClearList();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtComboBox::IsVertScrollbarAlwaysShown(void) const
	{
		return m_pDropList->IsVertScrollbarAlwaysShown();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::ForceVertScrollbar(bool bSetting)
	{
		m_pDropList->ForceVertScrollbar(bSetting);
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtComboBox::IsHorzScrollbarAlwaysShown(void) const
	{
		return m_pDropList->IsHorzScrollbarAlwaysShown();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::ForceHorzScrollbar(bool bSetting)
	{
		m_pDropList->ForceHorzScrollbar(bSetting);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::SetSorting(bool bSorting)
	{
		m_pDropList->SetSorting(bSorting);
	}
	//------------------------------------------------------------------------------
	bool 	CGUIWgtComboBox::IsSorting() const
	{
		return m_pDropList->IsSorting();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::MakeItemVisible(uint32 nItemIndex)
	{
		m_pDropList->MakeItemVisible(nItemIndex);
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtComboBox::MakeItemVisible(const CGUIListBoxItem* pItem)
	{
		m_pDropList->MakeItemVisible(pItem);
	}
	//------------------------------------------------------------------------------
	CGUIProperty*	CGUIWgtComboBox::GenerateProperty(const CGUIString& rName, const CGUIString& rType )
	{
		CGUIProperty* pProperty = NULL;

		if( rName == "DROPLIST_SIZE" && rType == "SIZE" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::SizeToString(GetDropListSize( )));
		}
		else if( rName == "SORT" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsSorting( )));
		}
		else if( rName == "TEXT_ITEM" && rType == "CGUIString" )
		{
			GUI_ASSERT(0, "not support now");
			/*pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::SizeToString(IsSorting( )));*/
		}
		return pProperty ? pProperty : CGUIWidget::GenerateProperty(rName, rType);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtComboBox::ProcessProperty( const CGUIProperty* pProperty)
	{
		CGUIWidget::ProcessProperty(pProperty);


		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for DROPLIST_SIZE
		/*
		*<property name="DROPLIST_SIZE" type="SIZE" value="150,150" />
		*/
		if( pProperty->GetName() == "DROPLIST_SIZE" && pProperty->GetType()=="SIZE")
		{
			SetDropListSize(CGUIStringConvertor::StringToSize(pProperty->GetValue()));
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



	//------------------------------------------------------------------------------
	uint32		CGUIWgtComboBox::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		if( m_pDropList->IsOpen()==false )
		{
			m_pDropList->Open();
		}
		return CGUIWidget::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex
