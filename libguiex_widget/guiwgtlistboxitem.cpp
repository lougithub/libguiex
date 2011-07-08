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
	}
	//------------------------------------------------------------------------------
	CGUIWgtListBox*	CGUIWgtListBoxItem::GetListBox()
	{
		GUI_ASSERT( GetParent(), "CGUIWgtListBoxItem: this item havn't parent");
		GUI_ASSERT( GetParent()->GetType() == "CGUIWgtListBox", "CGUIWgtListBoxItem: parent isn't a CGUIWgtListBox");
		
		return static_cast<CGUIWgtListBox*>(GetParent());
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
	void CGUIWgtListBoxItem::SetParentImpl( CGUIWidget* pParent )
	{
		CGUIWidget* pOldParent = GetParent();
		CGUIWidget::SetParentImpl( pParent );
		if( pOldParent != GetParent() )
		{
			if( pOldParent && pOldParent->GetType() == "CGUIWgtListBox")
			{
				static_cast<CGUIWgtListBox*>(pOldParent)->DoRemoveItem(this);
			}
			if( GetParent() && GetParent()->GetType() == "CGUIWgtListBox")
			{
				static_cast<CGUIWgtListBox*>(GetParent())->DoAddItem(this);
			}
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



	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseLeftClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftClick(this, pEvent);
		return CGUIWidget::OnMouseLeftClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseRightClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightClick(this, pEvent);
		return CGUIWidget::OnMouseRightClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseLeftDbClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftDbClick(this, pEvent);
		return CGUIWidget::OnMouseLeftDbClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseRightDbClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightDbClick(this, pEvent);
		return CGUIWidget::OnMouseRightDbClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftDown(this, pEvent);
		return CGUIWidget::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftUp(this, pEvent);
		return CGUIWidget::OnMouseLeftUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseRightDown( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightDown(this, pEvent);
		return CGUIWidget::OnMouseRightDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseRightUp( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightUp(this, pEvent);
		return CGUIWidget::OnMouseRightUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtListBoxItem::OnMouseMove( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseMove(this, pEvent);
		return CGUIWidget::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex


