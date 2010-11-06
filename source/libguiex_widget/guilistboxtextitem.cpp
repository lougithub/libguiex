/** 
* @file guilistboxtextitem.cpp
* @brief list box item for text
* @author ken
* @date 2006-09-18
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guilistboxtextitem.h>
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
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIListBoxTextItem);
	//------------------------------------------------------------------------------
	CGUIString CGUIListBoxTextItem::ms_strType = "CGUIListBoxTextItem";
	//------------------------------------------------------------------------------
	CGUIListBoxTextItem::CGUIListBoxTextItem( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIListBoxItem(ms_strType, rName, rProjectName)
	{
		InitTextItem();
	}
	//------------------------------------------------------------------------------
	CGUIListBoxTextItem::CGUIListBoxTextItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIListBoxItem(rType, rName, rProjectName)
	{
		InitTextItem();
	}
	//------------------------------------------------------------------------------
	CGUIListBoxTextItem::~CGUIListBoxTextItem(void)	
	{

	}
	//------------------------------------------------------------------------------
	void	CGUIListBoxTextItem::InitTextItem()
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIListBoxTextItem::UpdateTextSize()
	{
		IGUIInterfaceFont* pInterfaceFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		SetSize(pInterfaceFont->GetStringWidth( m_strText ), m_strText.GetDefaultInfo().m_nFontSize);
	}
	//------------------------------------------------------------------------------
	void	CGUIListBoxTextItem::SetTextContent(const wchar_t* pText)
	{
		CGUIListBoxItem::SetTextContent(pText);
		UpdateTextSize();
	}
	//------------------------------------------------------------------------------
	void		CGUIListBoxTextItem::SetTextInfo( const CGUIStringInfo& rInfo)
	{
		CGUIListBoxItem::SetTextInfo(rInfo);
		UpdateTextSize();
	}
	//------------------------------------------------------------------------------
	real		CGUIListBoxTextItem::GetHeight(void)
	{
		return GetSize().GetHeight()*GetDerivedScale().GetHeight();
	}
	//------------------------------------------------------------------------------
	real	CGUIListBoxTextItem::GetWidth(void)
	{
		return GetSize().GetWidth()*GetDerivedScale().GetWidth();
	}
	//------------------------------------------------------------------------------
	void CGUIListBoxTextItem::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIListBoxItem::RenderSelf(pRender);

		DrawString( pRender, GetText(), GetClientRect(), GetTextAlignment(), &GetClipRect());
	}
	//------------------------------------------------------------------------------
}
