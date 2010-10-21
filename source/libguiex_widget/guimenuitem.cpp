/** 
* @file guimenuitem.cpp
* @brief item for menu 
* @author ken
* @date 2007-07-31
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guimenuitem.h>
#include <libguiex_widget\guiwgtpopupmenu.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIMenuItem);
	//------------------------------------------------------------------------------
	CGUIString CGUIMenuItem::ms_strType = "CGUIMenuItem";
	//------------------------------------------------------------------------------
	CGUIMenuItem::CGUIMenuItem( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIItemBase(ms_strType, rName, rProjectName)
	{
		InitMenuItem();
	}
	//------------------------------------------------------------------------------
	CGUIMenuItem::CGUIMenuItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIItemBase(rType, rName, rProjectName)
	{
		InitMenuItem();
	}
	//------------------------------------------------------------------------------
	CGUIMenuItem::~CGUIMenuItem(void)
	{
	}
	//------------------------------------------------------------------------------
	CGUIWgtPopupMenu*	CGUIMenuItem::GetPopupMenu()
	{
		GUI_ASSERT( GetParent(), "this item havn't parent");
		return static_cast<CGUIWgtPopupMenu*>(GetParent());
	}
	//------------------------------------------------------------------------------
	void	CGUIMenuItem::InitMenuItem()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIMenuItem::RenderSelf(IGUIInterfaceRender* pRender)
	{
		///draw selection color if selected
		CGUIItemBase::RenderSelf(pRender);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIMenuItem::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		GetPopupMenu()->ProcessMouseLeftUp(this, pEvent);
		return CGUIWidget::OnMouseLeftUp(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIMenuItem::OnMouseMove( CGUIEventMouse* pEvent )
	{
		GetPopupMenu()->ProcessMouseMove(this, pEvent);
		return CGUIWidget::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
}//namespace guiex


