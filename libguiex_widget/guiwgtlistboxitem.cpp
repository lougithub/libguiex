/** 
* @file guiwgtlistboxitem.cpp
* @brief list box item
* @author ken
* @date 2011-07-07
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtlistboxitem.h"
#include "guiwgtlistbox.h"
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guistringconvertor.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtListBoxItem);
	//------------------------------------------------------------------------------
	CGUIWgtListBoxItem::CGUIWgtListBoxItem( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitListboxItem();
	}
	//------------------------------------------------------------------------------
	CGUIWgtListBoxItem::~CGUIWgtListBoxItem(void)
	{
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBoxItem::InitListboxItem()
	{
		m_pImageNormal = NULL;
		m_pImageSelected = NULL;

		m_pUserData = NULL;

		m_bSelected = false;

		SetGenerateChildSizeChangeEvent(true);
		SetGenerateParentChildEvent( true );
		SetFocusAgency( true );
	}
	//------------------------------------------------------------------------------
	CGUIWgtListBox*	CGUIWgtListBoxItem::GetListBox()
	{
		return m_pListBox;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBoxItem::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "normal")
		{
			m_pImageNormal = pImage;
			if( pImage && GetSize().IsEqualZero())
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "selected")
		{
			m_pImageSelected = pImage;
			if( pImage && GetSize().IsEqualZero())
			{
				SetPixelSize(pImage->GetSize());
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBoxItem::RenderSelf(IGUIInterfaceRender* pRender)
	{
		///draw selection color if selected
		CGUIWidget::RenderSelf(pRender);

		if( m_pImageSelected && IsSelected() )
		{
			DrawImage( pRender, m_pImageSelected, GetBoundArea());
		}
		else if( m_pImageNormal )
		{
			DrawImage( pRender, m_pImageNormal, GetBoundArea());
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtListBoxItem::IsSelected( void ) const
	{
		return m_bSelected;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBoxItem::SetSelected( bool bSelect )
	{
		m_bSelected = bSelect;
	}
	//------------------------------------------------------------------------------
	void* CGUIWgtListBoxItem::GetUserData(void) const
	{
		return m_pUserData;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBoxItem::SetUserData(void* pUserData)
	{
		m_pUserData = pUserData;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBoxItem::SetItemContent(const CGUIStringW& rText)
	{
		m_strItemContent = rText;
	}
	//------------------------------------------------------------------------------
	const CGUIStringW& CGUIWgtListBoxItem::GetItemContent( ) const
	{
		return m_strItemContent;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtListBoxItem::SetItemContentUTF8( const CGUIString& rString)
	{
		AppMultiByteToWideChar( rString, m_strItemContent);
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtListBoxItem::GetItemContentUTF8( ) const
	{
		CGUIString aContentUTF8;
		AppWideByteToMultiChar( m_strItemContent, aContentUTF8 );
		return aContentUTF8;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnParentChanged( CGUIEventRelativeChange* pEvent )
	{
		if( GetParent() && GetParent()->GetType() == "CGUIWgtListBox")
		{
			m_pListBox = static_cast<CGUIWgtListBox*>(GetParent());
		}
		else
		{
			m_pListBox = NULL;
		}
		return CGUIWidget::OnParentChanged(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseLeftClick( CGUIEventMouse* pEvent )
	{
		if( GetListBox() )
		{
			GetListBox()->ProcessMouseLeftClick(this, pEvent);
		}
		return CGUIWidget::OnMouseLeftClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseRightClick( CGUIEventMouse* pEvent )
	{
		if( GetListBox() )
		{
			GetListBox()->ProcessMouseRightClick(this, pEvent);
		}
		return CGUIWidget::OnMouseRightClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseLeftDbClick( CGUIEventMouse* pEvent )
	{
		if( GetListBox() )
		{
			GetListBox()->ProcessMouseLeftDbClick(this, pEvent);
		}
		return CGUIWidget::OnMouseLeftDbClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseRightDbClick( CGUIEventMouse* pEvent )
	{
		if( GetListBox() )
		{
			GetListBox()->ProcessMouseRightDbClick(this, pEvent);
		}
		return CGUIWidget::OnMouseRightDbClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		if( GetListBox() )
		{
			GetListBox()->ProcessMouseLeftDown(this, pEvent);
		}
		return CGUIWidget::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		if( GetListBox() )
		{
			GetListBox()->ProcessMouseLeftUp(this, pEvent);
		}
		return CGUIWidget::OnMouseLeftUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseRightDown( CGUIEventMouse* pEvent )
	{
		if( GetListBox() )
		{
			GetListBox()->ProcessMouseRightDown(this, pEvent);
		}
		return CGUIWidget::OnMouseRightDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseRightUp( CGUIEventMouse* pEvent )
	{
		if( GetListBox() )
		{
			GetListBox()->ProcessMouseRightUp(this, pEvent);
		}
		return CGUIWidget::OnMouseRightUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseMove( CGUIEventMouse* pEvent )
	{
		if( GetListBox() )
		{
			GetListBox()->ProcessMouseMove(this, pEvent);
		}
		return CGUIWidget::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex


