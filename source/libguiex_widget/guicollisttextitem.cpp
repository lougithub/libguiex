/** 
* @file guicollisttextitem.cpp
* @brief column list box item for text
* @author ken
* @date 2007-08-02
*/
//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guicollisttextitem.h>
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
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIColListTextItem);
	//------------------------------------------------------------------------------
	CGUIString CGUIColListTextItem::ms_strType = "CGUIColListTextItem";
	//------------------------------------------------------------------------------
	CGUIColListTextItem::CGUIColListTextItem( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIColListItem(ms_strType, rName, rProjectName)
	{
		InitTextItem();
	}
	//------------------------------------------------------------------------------
	CGUIColListTextItem::CGUIColListTextItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIColListItem(rType, rName, rProjectName)
	{
		InitTextItem();
	}
	//------------------------------------------------------------------------------
	CGUIColListTextItem::~CGUIColListTextItem(void)	
	{

	}
	//------------------------------------------------------------------------------
	void	CGUIColListTextItem::InitTextItem()
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIColListTextItem::UpdateTextSize()
	{
		IGUIInterfaceFont* pInterfaceFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();

		//SetSize(pInterfaceFont->GetStringWidth( m_strText ), m_strText.GetDefaultInfo()->m_nFontSize);
		SetSize(GetSize().m_fWidth, m_strText.GetDefaultInfo().m_nFontSize);
	}
	//------------------------------------------------------------------------------
	void	CGUIColListTextItem::SetTextContent(const wchar_t* pText)
	{
		CGUIColListItem::SetTextContent(pText);
		UpdateTextSize();
	}
	//------------------------------------------------------------------------------
	void		CGUIColListTextItem::SetTextInfo( const CGUIStringExInfo& rInfo)
	{
		CGUIColListItem::SetTextInfo(rInfo);
		UpdateTextSize();
	}
	//------------------------------------------------------------------------------
	real		CGUIColListTextItem::GetHeight(void)
	{
		return GetSize().GetHeight()*GetScale().GetHeight();
	}
	//------------------------------------------------------------------------------
	real	CGUIColListTextItem::GetWidth(void)
	{
		return GetSize().GetWidth()*GetScale().GetWidth();
	}
	//------------------------------------------------------------------------------
	CGUIColListTextItem*	CGUIColListTextItem::CreateTextItem(
		const wchar_t* pText, 
		const CGUIStringExInfo& rInfo,
		const CGUIString& rProjectName,
		uint32 nItemID,
		void* pItemData ,
		bool bDisable)	
	{
		CGUIColListTextItem * pItem = static_cast<CGUIColListTextItem *>(GUI_CREATE_WIDGET("CGUIColListTextItem", CGUIString("collistitem_")+CGUIWidgetSystem::Instance()->GenerateAnonymousName(), rProjectName));
		pItem->SetSelfDisable(bDisable);
		pItem->SetID(nItemID);
		pItem->SetUserData(pItemData);
		pItem->SetTextInfo(rInfo);
		pItem->SetTextContent(pText);
		return pItem;
	}
	//------------------------------------------------------------------------------
	void CGUIColListTextItem::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIColListItem::RenderSelf(pRender);

		DrawString( pRender, GetText(), GetClientRect(), GetTextAlignment(), &GetClipRect());
	}
	//------------------------------------------------------------------------------
}
