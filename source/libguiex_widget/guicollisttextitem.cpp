/** 
* @file guicollisttextitem.cpp
* @brief column list box item for text
* @author ken
* @date 2007-08-02
*/
//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guicollisttextitem.h>
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
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIColListTextItem);
	//------------------------------------------------------------------------------
	CGUIColListTextItem::CGUIColListTextItem( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIColListItem(StaticGetType(), rName, rSceneName)
	{
		InitTextItem();
	}
	//------------------------------------------------------------------------------
	CGUIColListTextItem::CGUIColListTextItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIColListItem(rType, rName, rSceneName)
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

		//SetSize(pInterfaceFont->GetStringWidth( m_strText ), m_strText.GetStringInfo()->m_nFontSize);
		SetSize(GetSize().m_fWidth, m_strText.GetStringInfo().m_nFontSize);
	}
	//------------------------------------------------------------------------------
	void	CGUIColListTextItem::SetTextContent(const CGUIStringW& rText)
	{
		CGUIColListItem::SetTextContent(pText);
		UpdateTextSize();
	}
	//------------------------------------------------------------------------------
	void		CGUIColListTextItem::SetTextInfo( const CGUIStringInfo& rInfo)
	{
		CGUIColListItem::SetTextInfo(rInfo);
		UpdateTextSize();
	}
	//------------------------------------------------------------------------------
	real		CGUIColListTextItem::GetHeight(void)
	{
		return GetSize().GetHeight()*GetDerivedScale().GetHeight();
	}
	//------------------------------------------------------------------------------
	real	CGUIColListTextItem::GetWidth(void)
	{
		return GetSize().GetWidth()*GetDerivedScale().GetWidth();
	}
	//------------------------------------------------------------------------------
	CGUIColListTextItem*	CGUIColListTextItem::CreateTextItem(
		const CGUIStringW& rText, 
		const CGUIStringInfo& rInfo,
		const CGUIString& rSceneName,
		uint32 nItemID,
		void* pItemData ,
		bool bDisable)	
	{
		CGUIString strItemName = CGUIString("collistitem_")+GSystem->GenerateAnonymousName();
		CGUIColListTextItem * pItem = 
			static_cast<CGUIColListTextItem *>(GUI_CREATE_WIDGET("CGUIColListTextItem", strItemName, rSceneName));
		pItem->SetDisable(bDisable);
		pItem->SetID(nItemID);
		pItem->SetUserData(pItemData);
		pItem->SetTextInfo(rInfo);
		pItem->SetTextContent(rText);
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
