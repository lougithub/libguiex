/** 
* @file guiwgtroot.cpp
* @brief base class, for root widget
* @author ken
* @date 2009-09-29
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core\guiwidgetroot.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiinterfacerender.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWidgetRoot);
	//------------------------------------------------------------------------------
	CGUIString CGUIWidgetRoot::ms_strType = "CGUIWidgetRoot";
	//------------------------------------------------------------------------------
	CGUIWidgetRoot::CGUIWidgetRoot(const CGUIString& rName, const CGUIString& rProjectName)
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitRoot();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetRoot::InitRoot()
	{
		SetFocusable(false);
		SetSizeRelative(true);
		SetSelfActivable(false);
		m_aBitFlag.reset(eFLAG_MOUSE_CONSUMED);			//!< should this mouse consume mouse event
		m_aBitFlag.reset(eFLAG_HITABLE);			
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetRoot::Open()
	{
		SetRectSize(CGUIWidgetSystem::Instance()->GetScreenSize());

		CGUIWidget::Open();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetRoot::RenderSelf(IGUIInterfaceRender* pRender)
	{
		pRender->AddScissor(CGUIWidgetSystem::Instance()->GetScreenRect());
	}
	////------------------------------------------------------------------------------
	//void	CGUIWidgetRoot::PreUpdateDirtyRect()
	//{
	//	GUI_ASSERT( GetParent() == NULL, "Root Widget shouldn't has parent" );
	//	SetRectSize(CGUIWidgetSystem::Instance()->GetScreenSize());
	//	
	//	CGUIWidget::PreUpdateDirtyRect();
	//}
	//------------------------------------------------------------------------------

}//namespace guiex
