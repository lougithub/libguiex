/** 
* @file guicollistitem.cpp
* @brief column list item, base class
* @author ken
* @date 2007-08-02
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guicollistitem.h>
#include <libguiex_widget/guiwgtcolumnlist.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIColListItem);
	//------------------------------------------------------------------------------
	CGUIColListItem::CGUIColListItem( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIItemBase(StaticGetType(), rName, rSceneName)
	{
		InitListboxItem();
	}
	//------------------------------------------------------------------------------
	CGUIColListItem::CGUIColListItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIItemBase(rType, rName, rSceneName)
	{
		InitListboxItem();
	}
	//------------------------------------------------------------------------------
	CGUIColListItem::~CGUIColListItem(void)
	{
	}
	//------------------------------------------------------------------------------
	CGUIWgtColumnList*	CGUIColListItem::GetListBox()
	{
		GUI_ASSERT( GetParent(), "this item havn't parent");
		return static_cast<CGUIWgtColumnList*>(GetParent());
	}
	//------------------------------------------------------------------------------
	void	CGUIColListItem::InitListboxItem()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIColListItem::RenderSelf(IGUIInterfaceRender* pRender)
	{
		///draw selection color if selected
		CGUIItemBase::RenderSelf(pRender);
	}
	//------------------------------------------------------------------------------
	void		CGUIColListItem::SetWidth(real fWidth)
	{
		SetSize(fWidth, GetSize().m_fHeight);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	uint32		CGUIColListItem::OnMouseLeftClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftClick(this, pEvent);
		return CGUIWidget::OnMouseLeftClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListItem::OnMouseRightClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightClick(this, pEvent);
		return CGUIWidget::OnMouseRightClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListItem::OnMouseLeftDbClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftDbClick(this, pEvent);
		return CGUIWidget::OnMouseLeftDbClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListItem::OnMouseRightDbClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightDbClick(this, pEvent);
		return CGUIWidget::OnMouseRightDbClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListItem::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftDown(this, pEvent);
		return CGUIWidget::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListItem::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftUp(this, pEvent);
		return CGUIWidget::OnMouseLeftUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListItem::OnMouseRightDown( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightDown(this, pEvent);
		return CGUIWidget::OnMouseRightDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListItem::OnMouseRightUp( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightUp(this, pEvent);
		return CGUIWidget::OnMouseRightUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIColListItem::OnMouseMove( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseMove(this, pEvent);
		return CGUIWidget::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex


