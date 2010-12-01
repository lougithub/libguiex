/** 
* @file guilistboxtextitem.cpp
* @brief list box item for text
* @author ken
* @date 2006-09-18
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guimenutextitem.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiimagemanager.h>
#include <libguiex_core/guiwidgetsystem.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIMenuTextItem);
	//------------------------------------------------------------------------------
	CGUIMenuTextItem::CGUIMenuTextItem( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIMenuItem(StaticGetType(), rName, rSceneName)
	{
		InitTextItem();
	}
	//------------------------------------------------------------------------------
	CGUIMenuTextItem::CGUIMenuTextItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIMenuItem(rType, rName, rSceneName)
	{
		InitTextItem();
	}
	//------------------------------------------------------------------------------
	CGUIMenuTextItem::~CGUIMenuTextItem(void)	
	{

	}
	//------------------------------------------------------------------------------
	void	CGUIMenuTextItem::InitTextItem()
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIMenuTextItem::UpdateTextSize()
	{
		IGUIInterfaceFont* pInterfaceFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		SetSize(pInterfaceFont->GetStringWidth( m_strText ), m_strText.GetStringInfo().m_nFontSize);
	}
	//------------------------------------------------------------------------------
	void	CGUIMenuTextItem::SetTextContent(const CGUIStringW& rText)
	{
		CGUIMenuItem::SetTextContent(pText);
		UpdateTextSize();
	}
	//------------------------------------------------------------------------------
	void		CGUIMenuTextItem::SetTextInfo( const CGUIStringInfo& rInfo)
	{
		CGUIMenuItem::SetTextInfo(rInfo);
		UpdateTextSize();
	}
	//------------------------------------------------------------------------------
	real		CGUIMenuTextItem::GetHeight(void)
	{
		return GetSize().GetHeight()*GetDerivedScale().GetHeight();
	}
	//------------------------------------------------------------------------------
	real	CGUIMenuTextItem::GetWidth(void)
	{
		return GetSize().GetWidth()*GetDerivedScale().GetWidth();
	}
	//------------------------------------------------------------------------------
	void CGUIMenuTextItem::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIMenuItem::RenderSelf(pRender);

		DrawString( pRender, GetText(), GetClientRect(), GetTextAlignment(), &GetClipRect());
	}
	//------------------------------------------------------------------------------
}
