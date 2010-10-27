/** 
* @file guilistboxtextitem.cpp
* @brief list box item for text
* @author ken
* @date 2006-09-18
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guimenutextitem.h>
#include <libguiex_core\guiinterfacemanager.h>
#include <libguiex_core\guiinterfacefont.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiimagemanager.h>
#include <libguiex_core\guiwidgetsystem.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIMenuTextItem);
	//------------------------------------------------------------------------------
	CGUIString CGUIMenuTextItem::ms_strType = "CGUIMenuTextItem";
	//------------------------------------------------------------------------------
	CGUIMenuTextItem::CGUIMenuTextItem( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIMenuItem(ms_strType, rName, rProjectName)
	{
		InitTextItem();
	}
	//------------------------------------------------------------------------------
	CGUIMenuTextItem::CGUIMenuTextItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIMenuItem(rType, rName, rProjectName)
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

		SetSize(pInterfaceFont->GetStringWidth( m_strText ), m_strText.GetDefaultInfo().m_nFontSize);
	}
	//------------------------------------------------------------------------------
	void	CGUIMenuTextItem::SetTextContent(const wchar_t* pText)
	{
		CGUIMenuItem::SetTextContent(pText);
		UpdateTextSize();
	}
	//------------------------------------------------------------------------------
	void		CGUIMenuTextItem::SetTextInfo( const CGUIStringExInfo& rInfo)
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
