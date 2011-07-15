/** 
* @file guiwgtcombobox.cpp
* @brief combobox used in this guiex system
* @author ken
* @date 2006-09-22
*/


//============================================================================//
// include 
//============================================================================// 
#include "guiwgtcombobox.h"
#include "guiwgtlistbox.h"
#include "guiwgtlistboxitem.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guievent.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiwidgetmanager.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiuicanvaslayer.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CGUIWgtComboBoxDropList : public CGUIWgtListBox
	{
	public:
		/// constructor
		CGUIWgtComboBoxDropList(const CGUIString& rName, const CGUIString& rSceneName );

		void ShowDropList();
		void HideDropList();

	protected:
		///initialize
		void InitComboBoxDropList();

	protected:
		virtual uint32 OnLostFocus( CGUIEventNotification* pEvent );
		virtual void ProcessMouseLeftUp(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent);
		virtual void ProcessMouseMove(CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent);

	private:
		static CGUIString ms_strType;
	};
}

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtComboBoxDropList::ms_strType = "CGUIWgtComboBoxDropList";
	//------------------------------------------------------------------------------
	CGUIWgtComboBoxDropList::CGUIWgtComboBoxDropList(const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtListBox( ms_strType,rName, rSceneName )
	{
		InitComboBoxDropList();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBoxDropList::InitComboBoxDropList()
	{
		SetVisible( false );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBoxDropList::ShowDropList()
	{
		SetVisible(true);
		GSystem->GetUICanvas()->SetPopupWidget( this );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBoxDropList::HideDropList()
	{
		SetVisible(false);
		if( GSystem->GetUICanvas()->GetPopupWidget( ) == this )
		{
			GSystem->GetUICanvas()->SetPopupWidget( NULL );
		}
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtComboBoxDropList::OnLostFocus( CGUIEventNotification* pEvent )
	{
		HideDropList( );
		ClearAllSelections_impl();
		return CGUIWidget::OnLostFocus(pEvent);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBoxDropList::ProcessMouseMove( CGUIWgtListBoxItem* pItem,CGUIEventMouse* pEvent )
	{
		ClearAllSelections_impl();
		pItem->SetSelected(true);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBoxDropList::ProcessMouseLeftUp( CGUIWgtListBoxItem* pItem, CGUIEventMouse* pEvent)
	{
		static_cast<CGUIWgtComboBox*>(GetParent())->SetSelectedItem(pItem);
		HideDropList();
		ClearAllSelections_impl();
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtComboBox);
	//------------------------------------------------------------------------------
	CGUIWgtComboBox::CGUIWgtComboBox( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitComboBox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtComboBox::CGUIWgtComboBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitComboBox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtComboBox::~CGUIWgtComboBox( )
	{
		//delete drop list
		m_pDropList->SetParent(NULL);
		delete m_pDropList;
		m_pDropList = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::InitComboBox()
	{
		m_pImageBG = NULL;
		m_pSelectedItem = NULL;

		//create drop list
		m_pDropList = new CGUIWgtComboBoxDropList(CGUIWidgetManager::MakeInternalName(GetName()+"_DropList"), GetSceneName());
		m_pDropList->SetParent( this);

		SetForceHitTest(true);
		SetFocusable(true);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::OnCreate()
	{
		CGUIWidget::OnCreate();

		//drop list
		m_pDropList->SetPosition( 0.0f, GetPixelSize().GetHeight());
		m_pDropList->Create();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::OnSetImage( const CGUIString& rName, CGUIImage* pImage)
	{
		if( rName == "background")
		{
			m_pImageBG = pImage;
			if( pImage && GetSize().IsEqualZero())
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName.find( "droplist_" ) == 0 )
		{
			m_pDropList->SetImage( rName.substr(strlen("droplist_")), pImage );
		}
	}	
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIWidget::RenderSelf( pRender );

		// draw bg
		DrawImage( pRender, m_pImageBG, GetBoundArea() );

		// draw item
		if( m_pSelectedItem )
		{
			CGUIVector2 rOldPos = m_pSelectedItem->GetPixelPosition();
			m_pSelectedItem->SetPixelPosition( 0.0f, 0.0f );
			m_pSelectedItem->Refresh();

			m_pSelectedItem->Render( pRender );

			m_pSelectedItem->SetPixelPosition( rOldPos );
			m_pSelectedItem->Refresh();
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set the size of drop list
	*/
	void CGUIWgtComboBox::SetDropListSize( const CGUISize& rSize )
	{
		m_pDropList->SetPixelSize( rSize );
	}
	//------------------------------------------------------------------------------
	const CGUISize&	CGUIWgtComboBox::GetDropListSize(  ) const
	{
		return m_pDropList->GetPixelSize( );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::SetSelectedItem(CGUIWgtListBoxItem* pItem)
	{
		if( !pItem )
		{
			GUI_THROW(  "[CGUIWgtComboBox::SetSelectedItem]: the item doesn't exist!");
		}

		uint32 nIdx = m_pDropList->GetItemIndex(pItem);
		m_pSelectedItem = pItem;

		//send event
		CGUIEventComboBox aEvent;
		aEvent.SetEventId(eEVENT_COMBOBOX_SELECTED);
		aEvent.SetReceiver(this);
		aEvent.SetSelectedItemIdx( nIdx );
		GSystem->SendEvent( &aEvent);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::SetSelectedItem(uint32 nIdx )
	{
		CGUIWgtListBoxItem* pItem = m_pDropList->GetItemByIndex(nIdx);
		SetSelectedItem(pItem);
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtComboBox::HasSelectedItem() const
	{
		return m_pSelectedItem!= NULL;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtComboBox::GetSelectedItemIndex() const
	{
		if( !m_pSelectedItem )
		{
			GUI_THROW("[CGUIWgtComboBox::GetSelectedItemIndex]: no item selected!");
		}

		return m_pDropList->GetItemIndex(m_pSelectedItem);
	}
	//------------------------------------------------------------------------------
	const CGUIWgtListBoxItem* CGUIWgtComboBox::GetSelectedItem() const
	{
		return m_pSelectedItem;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtComboBox::GetItemCount(void) const
	{
		return m_pDropList->GetItemCount();
	}	
	//------------------------------------------------------------------------------
	CGUIWgtListBoxItem* CGUIWgtComboBox::GetItemByIndex(uint32 nIndex) const
	{
		return m_pDropList->GetItemByIndex(nIndex);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtComboBox::GetItemIndex(const CGUIWgtListBoxItem* pItem) const
	{
		return m_pDropList->GetItemIndex(pItem);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::AddItem(CGUIWgtListBoxItem* pItem)
	{
		m_pDropList->AddItem(pItem);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::RemoveItem( CGUIWgtListBoxItem* pItem)
	{
		m_pDropList->RemoveItem(pItem);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::RemoveItem( uint32 nIndex)
	{
		m_pDropList->RemoveItem(nIndex);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtComboBox::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		m_pDropList->SetFocus( true );		
		m_pDropList->ShowDropList( );
		return CGUIWidget::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex
