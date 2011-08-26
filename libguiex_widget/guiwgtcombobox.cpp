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
#include <libguiex_core/guipropertyconvertor.h>


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
		virtual uint32 OnPageLostFocus( CGUIEventNotification* pEvent );
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
	CGUIString CGUIWgtComboBoxDropList::ms_strType = "CGUIWgtListBox";
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
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBoxDropList::HideDropList()
	{
		SetVisible(false);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtComboBoxDropList::OnPageLostFocus( CGUIEventNotification* pEvent )
	{
		HideDropList( );
		ClearAllSelections_impl();
		return CGUIWidget::OnPageLostFocus(pEvent);
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
		:CGUIWgtTextBase(StaticGetType(), rName, rSceneName)
	{
		InitComboBox();
	}
	//------------------------------------------------------------------------------
	CGUIWgtComboBox::CGUIWgtComboBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtTextBase(rType, rName, rSceneName)
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
		m_nSelectedItemIdx = -1;

		//create drop list
		m_pDropList = new CGUIWgtComboBoxDropList(CGUIWidgetManager::MakeInternalName(GetName()+"_DropList"), GetSceneName());
		m_pDropList->SetParent( this);
		m_pDropList->SetPage( m_pDropList );

		SetDropListSize( CGUISize( 100.f, 100.f ));
		SetForceHitTest(true);
		SetFocusable(true);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::OnCreate()
	{
		CGUIWgtTextBase::OnCreate();

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
		CGUIWgtTextBase::RenderSelf( pRender );

		// draw bg
		DrawImage( pRender, m_pImageBG, GetBoundArea() );
		
		//draw item content
		DrawString( pRender, m_strText, GetClientArea(), m_eTextAlignmentHorz, m_eTextAlignmentVert);
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
		SetSelectedItem( nIdx );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::SetSelectedItem(uint32 nIdx )
	{
		m_nSelectedItemIdx = nIdx;

		const CGUIWgtListBoxItem* pItem = GetSelectedItem();
		if( pItem )
		{
			SetTextContent( pItem->GetItemContent() );
		}
		else
		{
			SetTextContent(CGUIStringW());
		}

		//send event
		CGUIEventComboBox aEvent;
		aEvent.SetEventId(eEVENT_COMBOBOX_SELECTED);
		aEvent.SetReceiver(this);
		aEvent.SetSelectedItemIdx( m_nSelectedItemIdx );
		GSystem->SendEvent( &aEvent);
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtComboBox::HasSelectedItem() const
	{
		return m_nSelectedItemIdx != static_cast<uint32>(-1);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtComboBox::GetSelectedItemIndex() const
	{
		return m_nSelectedItemIdx;
	}
	//------------------------------------------------------------------------------
	const CGUIWgtListBoxItem* CGUIWgtComboBox::GetSelectedItem() const
	{
		return m_pDropList->GetItemByIndex(m_nSelectedItemIdx);
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
	void CGUIWgtComboBox::AddItem(CGUIWidget* pItem)
	{
		GUI_ASSERT( pItem, "[CGUIWgtComboBox::AddItem] invalid pointer" );
		GUI_ASSERT( pItem->GetType() == "CGUIWgtListBoxItem", "[CGUIWgtComboBox::AddItem] invalid pointer" );
		m_pDropList->AddItem(pItem);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::RemoveItem( CGUIWidget* pItem)
	{
		GUI_ASSERT( pItem, "[CGUIWgtComboBox::RemoveItem] invalid pointer" );
		GUI_ASSERT( pItem->GetType() == "CGUIWgtListBoxItem", "[CGUIWgtComboBox::RemoveItem] invalid pointer" );
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
		m_pDropList->SetFocus( );		
		m_pDropList->ShowDropList( );
		return CGUIWgtTextBase::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	int32 CGUIWgtComboBox::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Size && rProperty.GetName() == "droplist_size" )
		{
			ValueToProperty( GetDropListSize(), rProperty);
		}
		else
		{
			return CGUIWgtTextBase::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtComboBox::ProcessProperty( const CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Size && rProperty.GetName() == "droplist_size" )
		{
			CGUISize aSize;
			PropertyToValue( rProperty, aSize );
			SetDropListSize( aSize );
		}
		else
		{
			CGUIWgtTextBase::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
