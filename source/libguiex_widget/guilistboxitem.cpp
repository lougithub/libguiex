/** 
* @file guilistboxitem.cpp
* @brief list box item, base class
* @author ken
* @date 2006-09-18
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guilistboxitem.h>
#include <libguiex_widget\guiwgtlistbox.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIListBoxItem);
	//------------------------------------------------------------------------------
	CGUIString CGUIListBoxItem::ms_strType = "CGUIListBoxItem";
	//------------------------------------------------------------------------------
	CGUIListBoxItem::CGUIListBoxItem( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIItemBase(ms_strType, rName, rProjectName)
	{
		InitListboxItem();
	}
	//------------------------------------------------------------------------------
	CGUIListBoxItem::CGUIListBoxItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIItemBase(rType, rName, rProjectName)
	{
		InitListboxItem();
	}
	//------------------------------------------------------------------------------
	CGUIListBoxItem::~CGUIListBoxItem(void)
	{
	}
	//------------------------------------------------------------------------------
	CGUIWgtListBox*	CGUIListBoxItem::GetListBox()
	{
		GUI_ASSERT( GetParent(), "this item havn't parent");
		return static_cast<CGUIWgtListBox*>(GetParent());
	}
	//------------------------------------------------------------------------------
	void	CGUIListBoxItem::InitListboxItem()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIListBoxItem::RenderSelf(IGUIInterfaceRender* pRender)
	{
		///draw selection color if selected
		CGUIItemBase::RenderSelf(pRender);
	}
	//------------------------------------------------------------------------------





	//------------------------------------------------------------------------------
	uint32		CGUIListBoxItem::OnMouseLeftClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftClick(this, pEvent);
		return CGUIWidget::OnMouseLeftClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIListBoxItem::OnMouseRightClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightClick(this, pEvent);
		return CGUIWidget::OnMouseRightClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIListBoxItem::OnMouseLeftDbClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftDbClick(this, pEvent);
		return CGUIWidget::OnMouseLeftDbClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIListBoxItem::OnMouseRightDbClick( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightDbClick(this, pEvent);
		return CGUIWidget::OnMouseRightDbClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIListBoxItem::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftDown(this, pEvent);
		return CGUIWidget::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIListBoxItem::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseLeftUp(this, pEvent);
		return CGUIWidget::OnMouseLeftUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIListBoxItem::OnMouseRightDown( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightDown(this, pEvent);
		return CGUIWidget::OnMouseRightDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIListBoxItem::OnMouseRightUp( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseRightUp(this, pEvent);
		return CGUIWidget::OnMouseRightUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIListBoxItem::OnMouseMove( CGUIEventMouse* pEvent )
	{
		GetListBox()->ProcessMouseMove(this, pEvent);
		return CGUIWidget::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex


